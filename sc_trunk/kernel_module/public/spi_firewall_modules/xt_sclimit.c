/* (C) 1999 Jérôme de Vivie <devivie@info.enserb.u-bordeaux.fr>
 * (C) 1999 Hervé Eychenne <eychenne@info.enserb.u-bordeaux.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_sclimit.h>
#include <linux/kernel.h>
#include <linux/ip.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <linux/netfilter.h>
#include <sc/sc_spi.h>
#include <linux/version.h>
#include "common.h"

#define SRC_HASH_SHIFT		9
#define SRC_HASH_SIZE		(1 << SRC_HASH_SHIFT)

struct xt_limit_priv_entry {
	unsigned long prev;
	uint32_t credit;
	uint32_t saddr;
	struct hlist_node hlist;
};

struct xt_limit_priv_sc {
	//global limit
	unsigned long prev;
	uint32_t credit;
	// per-source limit
	struct hlist_head source[SRC_HASH_SIZE];

	int hash_table_lock;
};

//MODULE_LICENSE("GPL");
//MODULE_AUTHOR("Herve Eychenne <rv@wallfire.org>");
MODULE_DESCRIPTION("Xtables: SC rate-limit match");
MODULE_ALIAS("ipt_sclimit");
MODULE_ALIAS("ip6t_sclimit");

/* The algorithm used is the Simple Token Bucket Filter (TBF)
 * see net/sched/sch_tbf.c in the linux source tree
 */

static DEFINE_SPINLOCK(limit_lock);

/* Rusty: This is my (non-mathematically-inclined) understanding of
   this algorithm.  The `average rate' in jiffies becomes your initial
   amount of credit `credit' and the most credit you can ever have
   `credit_cap'.  The `peak rate' becomes the cost of passing the
   test, `cost'.

   `prev' tracks the last packet hit: you gain one credit per jiffy.
   If you get credit balance more than this, the extra credit is
   discarded.  Every time the match passes, you lose `cost' credits;
   if you don't have that many, the test fails.

   See Alexey's formal explanation in net/sched/sch_tbf.c.

   To get the maxmum range, we multiply by this factor (ie. you get N
   credits per jiffy).  We want to allow a rate as low as 1 per day
   (slowest userspace tool allows), which means
   CREDITS_PER_JIFFY*HZ*60*60*24 < 2^32. ie. */
#define MAX_CPJ (0xFFFFFFFF / (HZ*60*60*24))

/* Repeated shift and or gives us all 1s, final shift and add 1 gives
 * us the power of 2 below the theoretical max, so GCC simply does a
 * shift. */
#define _POW2_BELOW2(x) ((x)|((x)>>1))
#define _POW2_BELOW4(x) (_POW2_BELOW2(x)|_POW2_BELOW2((x)>>2))
#define _POW2_BELOW8(x) (_POW2_BELOW4(x)|_POW2_BELOW4((x)>>4))
#define _POW2_BELOW16(x) (_POW2_BELOW8(x)|_POW2_BELOW8((x)>>8))
#define _POW2_BELOW32(x) (_POW2_BELOW16(x)|_POW2_BELOW16((x)>>16))
#define POW2_BELOW32(x) ((_POW2_BELOW32(x)>>1) + 1)

#define CREDITS_PER_JIFFY POW2_BELOW32(MAX_CPJ)

/*
 * Convert an IP address into a hash table index.
 */
static inline int hashfunc(__u32 addr)
{
	unsigned int value;
	int hash;

	value = addr;
	hash = 0;
	do {
		hash ^= value;
	} while ((value >>= SRC_HASH_SHIFT));

	return hash & (SRC_HASH_SIZE - 1);
}

struct xt_limit_priv_entry *find_xt_limit_entry(const struct sk_buff *skb, struct xt_limit_priv_sc *priv)
{
	const struct iphdr *iph = ip_hdr(skb);
	struct xt_limit_priv_entry *entry;
	struct hlist_head *head = &priv->source[hashfunc(iph->saddr)];

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
	hlist_for_each_entry(entry, head, hlist)
#else
	struct hlist_node *p;
	hlist_for_each_entry(entry, p, head, hlist)
#endif
		if (entry->saddr == iph->saddr)
			return entry;

	return NULL;
}

struct xt_limit_priv_entry *add_xt_limit_entry(const struct sk_buff *skb, struct xt_limit_priv_sc *priv, 
		const struct xt_rateinfo_sc *r, unsigned long now)
{
	const struct iphdr *iph = ip_hdr(skb);
	struct hlist_head *head = &priv->source[hashfunc(iph->saddr)];
	struct xt_limit_priv_entry *entry = kmalloc(sizeof(*entry), GFP_KERNEL);

	if (!entry) 
		return NULL;

	entry->saddr = iph->saddr;
	entry->credit = r->credit_cap_source;
	entry->prev = now;

	hlist_add_head_rcu(&entry->hlist, head);

	return entry;
}

static int xt_limit_flush_hash_table(struct xt_limit_priv_sc *priv)
{
        struct hlist_head *lih;
        struct hlist_node *tmp;
        struct xt_limit_priv_entry *entry = NULL;
	int i;

	for (i = 0; i < SRC_HASH_SIZE; i++)
	{
	    lih = &priv->source[i];

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
	    hlist_for_each_entry_safe(entry, tmp, lih, hlist)
#else
        struct hlist_node *node; 
	    hlist_for_each_entry_safe(entry, node, tmp, lih, hlist)
#endif
        {
		if (entry)
		{
		    hlist_del_rcu(&entry->hlist);
		    kfree(entry);
		}
	    }
	}
        return 0;
}

//#define BLOCK_IN_LIMIT
#ifdef BLOCK_IN_LIMIT
static unsigned short get_sport(const struct sk_buff *skb,const struct iphdr *iph)
{
	unsigned short sport=0, dport=0;

        switch (iph->protocol) {
        case IPPROTO_TCP: {
            struct tcphdr *tcph = (struct tcphdr *)(skb_network_header(skb) + ip_hdrlen(skb));

            if (ntohs(iph->frag_off) & IP_OFFSET) {
                break;
            }
            sport = ntohs(tcph->source);
            dport = ntohs(tcph->dest);
            break;
        }
        case IPPROTO_UDP: {
            struct udphdr *udph = (struct udphdr *)(skb_network_header(skb) + ip_hdrlen(skb));

            if (ntohs(iph->frag_off) & IP_OFFSET) {
                break;
            }
            sport = ntohs(udph->source);
            dport = ntohs(udph->dest);
            break;
        }
        default:
            break;
        }
	dport = dport;
	return sport;
}
#endif

static bool
limit_mt(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_rateinfo_sc *r = (struct xt_rateinfo_sc *) par->matchinfo;
	struct xt_limit_priv_sc *priv = r->master;
	struct xt_limit_priv_entry *entry;
	unsigned long now = jiffies;
#ifdef BLOCK_IN_LIMIT
	const struct iphdr *iph = ip_hdr(skb);
	unsigned short sport;
#endif

	bool res_global = true;
	bool res_source = true;

	spin_lock_bh(&limit_lock);

	if (r->credit_cap) {

	    priv->credit += (now - xchg(&priv->prev, now)) * CREDITS_PER_JIFFY;
	    if (priv->credit > r->credit_cap)
		priv->credit = r->credit_cap;

	    if (priv->credit >= r->cost) {
		/* We're not limited. */
		priv->credit -= r->cost;
	    } else
		res_global = false;
	}

	if (r->credit_cap_source) {

	    entry = find_xt_limit_entry(skb, priv);

	    if (entry) {
		entry->credit += (now - xchg(&entry->prev, now)) * CREDITS_PER_JIFFY;
		if (entry->credit > r->credit_cap_source)
		    entry->credit = r->credit_cap_source;

		if (entry->credit >= r->cost_source) {
		    /* We're not limited. */
		    entry->credit -= r->cost_source;
		} else
		    res_source = false;
	    }
	    else 
		add_xt_limit_entry(skb, priv, r, now);
	}

	spin_unlock_bh(&limit_lock);

    /* block it now ? */
#ifdef BLOCK_IN_LIMIT
	/* block the source ip if neccessary */
	if (/*!res_source && */sc_block_source_ip_hook && g_source_block_time)
	{
		sport = get_sport(skb, iph);
		sc_block_source_ip_hook(iph->saddr, sport, g_source_block_time);
		if (net_ratelimit())
			NETGEAR_SYSLOG("19 00[DoS attack] Block Source: %u.%u.%u.%u:%hu due to Limit\n", NIPQUAD((iph->saddr)), ntohs(sport));
	}
#endif

	return res_global && res_source;
}

/* Precision saver. */
static u_int32_t
user2credits(u_int32_t user)
{
	/* If multiplying would overflow... */
	if (user > 0xFFFFFFFF / (HZ*CREDITS_PER_JIFFY))
		/* Divide first. */
		return (user / XT_LIMIT_SCALE) * HZ * CREDITS_PER_JIFFY;

	return (user * HZ * CREDITS_PER_JIFFY) / XT_LIMIT_SCALE;
}

static int limit_mt_check(const struct xt_mtchk_param *par)
{
	struct xt_rateinfo_sc *r = (struct xt_rateinfo_sc *) par->matchinfo;
	struct xt_limit_priv_sc *priv;

	int i;

	/* Check for overflow. */
	if (r->burst == 0
	    || (r->avg && user2credits(r->avg * r->burst) < user2credits(r->avg))) {
		pr_info("Overflow, try lower: %u/%u\n",
			r->avg, r->burst);
		return -ERANGE;
	}
	if (r->burst_source == 0
	    || (r->avg_source && user2credits(r->avg_source * r->burst_source) < user2credits(r->avg_source))) {
		pr_info("Overflow, try lower: %u/%u\n",
			r->avg, r->burst);
		return -ERANGE;
	}

	priv = kmalloc(sizeof(*priv), GFP_KERNEL);
	if (priv == NULL)
		return -ENOMEM;

	for (i = 0; i < SRC_HASH_SIZE; i++)
		INIT_HLIST_HEAD(&priv->source[i]);

	/* For SMP, we only want to use one set of state. */
	r->master = priv;
	if (r->cost == 0) {
		/* User avg in seconds * XT_LIMIT_SCALE: convert to jiffies *
		   128. */
		priv->prev = jiffies;
		priv->credit = user2credits(r->avg * r->burst); /* Credits full. */
		r->credit_cap = user2credits(r->avg * r->burst); /* Credits full. */
		r->cost = user2credits(r->avg);
	}
	if (r->cost_source == 0) {
		r->credit_cap_source = user2credits(r->avg_source * r->burst_source); /* Credits full. */
		r->cost_source = user2credits(r->avg_source);
	}
	return 0;
}

static void limit_mt_destroy(const struct xt_mtdtor_param *par)
{
	const struct xt_rateinfo_sc *info = (struct xt_rateinfo_sc *) par->matchinfo;

	xt_limit_flush_hash_table((struct xt_limit_priv_sc *)info->master);

	kfree(info->master);
}

#ifdef CONFIG_COMPAT
struct compat_xt_rateinfo {
	u_int32_t avg;
	u_int32_t burst;

	compat_ulong_t prev;
	u_int32_t credit;
	u_int32_t credit_cap, cost;

	u_int32_t master;
};

/* To keep the full "prev" timestamp, the upper 32 bits are stored in the
 * master pointer, which does not need to be preserved. */
static void limit_mt_compat_from_user(void *dst, const void *src)
{
	const struct compat_xt_rateinfo *cm = src;
	struct xt_rateinfo_sc m = {
		.avg		= cm->avg,
		.burst		= cm->burst,
		.prev		= cm->prev | (unsigned long)cm->master << 32,
		.credit		= cm->credit,
		.credit_cap	= cm->credit_cap,
		.cost		= cm->cost,
	};
	memcpy(dst, &m, sizeof(m));
}

static int limit_mt_compat_to_user(void __user *dst, const void *src)
{
	const struct xt_rateinfo_sc *m = src;
	struct compat_xt_rateinfo cm = {
		.avg		= m->avg,
		.burst		= m->burst,
		.prev		= m->prev,
		.credit		= m->credit,
		.credit_cap	= m->credit_cap,
		.cost		= m->cost,
		.master		= m->prev >> 32,
	};
	return copy_to_user(dst, &cm, sizeof(cm)) ? -EFAULT : 0;
}
#endif /* CONFIG_COMPAT */

static struct xt_match limit_mt_reg __read_mostly = {
	.name             = "sclimit",
	.revision         = 0,
	.family           = NFPROTO_UNSPEC,
	.match            = limit_mt,
	.checkentry       = limit_mt_check,
	.destroy          = limit_mt_destroy,
	.matchsize        = sizeof(struct xt_rateinfo_sc),
#ifdef CONFIG_COMPAT
	.compatsize       = sizeof(struct compat_xt_rateinfo),
	.compat_from_user = limit_mt_compat_from_user,
	.compat_to_user   = limit_mt_compat_to_user,
#endif
	.me               = THIS_MODULE,
};

static int __init limit_mt_init(void)
{
	return xt_register_match(&limit_mt_reg);
}

static void __exit limit_mt_exit(void)
{
	xt_unregister_match(&limit_mt_reg);
}

module_init(limit_mt_init);
module_exit(limit_mt_exit);

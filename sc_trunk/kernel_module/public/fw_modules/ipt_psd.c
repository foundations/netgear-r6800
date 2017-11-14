/*
  This is a module which is used for PSD (portscan detection)
  Derived from scanlogd v2.1 written by Solar Designer <solar at false.com>
  and LOG target module.

  Copyright (C) 2000,2001 astaro AG

  This file is distributed under the terms of the GNU General Public
  License (GPL). Copies of the GPL can be obtained from:
     ftp://prep.ai.mit.edu/pub/gnu/GPL

  2000-05-04 Markus Hennig <hennig at astaro.de> : initial
  2000-08-18 Dennis Koslowski <koslowski at astaro.de> : first release
  2000-12-01 Dennis Koslowski <koslowski at astaro.de> : UDP scans detection added
  2001-01-02 Dennis Koslowski <koslowski at astaro.de> : output modified
  2001-02-04 Jan Rekorajski <baggins at pld.org.pl> : converted from target to match
  2004-05-05 Martijn Lievaart <m at rtij.nl> : ported to 2.6
*/

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/tcp.h>
#include <linux/spinlock.h>
#include <linux/netfilter/x_tables.h>
#include "ipt_psd.h"
#include <linux/version.h>
#include <linux/types.h>

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dennis Koslowski <koslowski at astaro.com>");

#define HF_DADDR_CHANGING   0x01
#define HF_SPORT_CHANGING   0x02
#define HF_TOS_CHANGING	    0x04
#define HF_TTL_CHANGING	    0x08

/*
 * Information we keep per each target port
 */
struct port {
	u_int16_t number;      /* port number */
	u_int8_t proto;        /* protocol number */
	u_int8_t and_flags;    /* tcp ANDed flags */
	u_int8_t or_flags;     /* tcp ORed flags */
};

/*
 * Information we keep per each source address.
 */
struct host {
	struct host *next;		/* Next entry with the same hash */
	unsigned long timestamp;		/* Last update time */
	struct in_addr src_addr;	/* Source address */
	struct in_addr dest_addr;	/* Destination address */
	unsigned short src_port;	/* Source port */
	int count;			/* Number of ports in the list */
	int weight;			/* Total weight of ports in the list */
	struct port ports[SCAN_MAX_COUNT - 1];	/* List of ports */
	unsigned char tos;		/* TOS */
	unsigned char ttl;		/* TTL */
	unsigned char flags;		/* HF_ flags bitmask */
};

/*
 * State information.
 */
static struct {
	spinlock_t lock;
	struct host list[LIST_SIZE];	/* List of source addresses */
	struct host *hash[HASH_SIZE];	/* Hash: pointers into the list */
	int index;			/* Oldest entry to be replaced */
} state;

/*
 * Convert an IP address into a hash table index.
 */
static inline int hashfunc(struct in_addr addr)
{
	unsigned int value;
	int hash;

	value = addr.s_addr;
	hash = 0;
	do {
		hash ^= value;
	} while ((value >>= HASH_LOG));

	return hash & (HASH_SIZE - 1);
}
static bool
ipt_psd_match(const struct sk_buff *skb,
		      struct xt_match_param *par)
{
	struct iphdr *piphdr;
	struct tcphdr *tcp_hdr;
	struct in_addr addr;
	u_int16_t src_port,dest_port;
  	u_int8_t tcp_flags, proto;
	unsigned long now;
	struct host *curr, *last, **head;
	int hash, index, count;

	/* Parameters from userspace */
	const struct ipt_psd_info *psdinfo = par->matchinfo;

	/* IP header */
	piphdr = ip_hdr(skb);

	/* Sanity check */
	if (ntohs(piphdr->frag_off) & IP_OFFSET) {
		DEBUGP("PSD: sanity check failed\n");
		return 0;
	}

	/* TCP or UDP ? */
	proto = piphdr->protocol;

	if (proto != IPPROTO_TCP && proto != IPPROTO_UDP) {
		DEBUGP("PSD: protocol not supported\n");
		return 0;
	}

	/* Get the source address, source & destination ports, and TCP flags */

	addr.s_addr = piphdr->saddr;

	tcp_hdr = (struct tcphdr*)((u_int32_t *)piphdr + piphdr->ihl);

	/* Yep, it's dirty */
	src_port = tcp_hdr->source;
	dest_port = tcp_hdr->dest;

	if (proto == IPPROTO_TCP) {
		tcp_flags = *((u_int8_t*)tcp_hdr + 13);
	}
	else {
		tcp_flags = 0x00;
	}

	/* We're using IP address 0.0.0.0 for a special purpose here, so don't let
	 * them spoof us. [DHCP needs this feature - HW] */
	if (!addr.s_addr) {
		DEBUGP("PSD: spoofed source address (0.0.0.0)\n");
		return 0;
	}

	/* Use jiffies here not to depend on someone setting the time while we're
	 * running; we need to be careful with possible return value overflows. */
	now = jiffies;

	spin_lock(&state.lock);

	/* Do we know this source address already? */
	count = 0;
	last = NULL;
	if ((curr = *(head = &state.hash[hash = hashfunc(addr)])))
		do {
			if (curr->src_addr.s_addr == addr.s_addr) break;
			count++;
			if (curr->next) last = curr;
		} while ((curr = curr->next));

	if (curr) {

		/* We know this address, and the entry isn't too old. Update it. */
		if (now - curr->timestamp <= (psdinfo->delay_threshold*HZ)/100 &&
		    time_after_eq(now, curr->timestamp)) {

			/* Just update the appropriate list entry if we've seen this port already */
			for (index = 0; index < curr->count; index++) {
				if (curr->ports[index].number == dest_port) {
					curr->ports[index].proto = proto;
					curr->ports[index].and_flags &= tcp_flags;
					curr->ports[index].or_flags |= tcp_flags;
					goto out_no_match;
				}
			}

			/* TCP/ACK and/or TCP/RST to a new port? This could be an outgoing connection. */
			if (proto == IPPROTO_TCP && (tcp_hdr->ack || tcp_hdr->rst))
				goto out_no_match;

			/* Packet to a new port, and not TCP/ACK: update the timestamp */
			curr->timestamp = now;

			/* Logged this scan already? Then drop the packet. */
			if (curr->weight >= psdinfo->weight_threshold)
				goto out_match;

			/* Specify if destination address, source port, TOS or TTL are not fixed */
			if (curr->dest_addr.s_addr != piphdr->daddr)
				curr->flags |= HF_DADDR_CHANGING;
			if (curr->src_port != src_port)
				curr->flags |= HF_SPORT_CHANGING;
			if (curr->tos != piphdr->tos)
				curr->flags |= HF_TOS_CHANGING;
			if (curr->ttl != piphdr->ttl)
				curr->flags |= HF_TTL_CHANGING;

			/* Update the total weight */
			curr->weight += (ntohs(dest_port) < 1024) ?
				psdinfo->lo_ports_weight : psdinfo->hi_ports_weight;

			/* Got enough destination ports to decide that this is a scan? */
			/* Then log it and drop the packet. */
			if (curr->weight >= psdinfo->weight_threshold)
				goto out_match;

			/* Remember the new port */
			if (curr->count < SCAN_MAX_COUNT) {
				curr->ports[curr->count].number = dest_port;
				curr->ports[curr->count].proto = proto;
				curr->ports[curr->count].and_flags = tcp_flags;
				curr->ports[curr->count].or_flags = tcp_flags;
				curr->count++;
			}

			goto out_no_match;
		}

		/* We know this address, but the entry is outdated. Mark it unused, and
		 * remove from the hash table. We'll allocate a new entry instead since
		 * this one might get re-used too soon. */
		curr->src_addr.s_addr = 0;
		if (last)
			last->next = last->next->next;
		else if (*head)
			*head = (*head)->next;
		last = NULL;
	}

	/* We don't need an ACK from a new source address */
	if (proto == IPPROTO_TCP && tcp_hdr->ack)
		goto out_no_match;

	/* Got too many source addresses with the same hash value? Then remove the
	 * oldest one from the hash table, so that they can't take too much of our
	 * CPU time even with carefully chosen spoofed IP addresses. */
	if (count >= HASH_MAX && last) last->next = NULL;

	/* We're going to re-use the oldest list entry, so remove it from the hash
	 * table first (if it is really already in use, and isn't removed from the
	 * hash table already because of the HASH_MAX check above). */

	/* First, find it */
	if (state.list[state.index].src_addr.s_addr)
		head = &state.hash[hashfunc(state.list[state.index].src_addr)];
	else
		head = &last;
	last = NULL;
	if ((curr = *head))
	do {
		if (curr == &state.list[state.index]) break;
		last = curr;
	} while ((curr = curr->next));

	/* Then, remove it */
	if (curr) {
		if (last)
			last->next = last->next->next;
		else if (*head)
			*head = (*head)->next;
	}

	/* Get our list entry */
	curr = &state.list[state.index++];
	if (state.index >= LIST_SIZE) state.index = 0;

	/* Link it into the hash table */
	head = &state.hash[hash];
	curr->next = *head;
	*head = curr;

	/* And fill in the fields */
	curr->timestamp = now;
	curr->src_addr = addr;
	curr->dest_addr.s_addr = piphdr->daddr;
	curr->src_port = src_port;
	curr->count = 1;
	curr->weight = (ntohs(dest_port) < 1024) ?
		psdinfo->lo_ports_weight : psdinfo->hi_ports_weight;
	curr->ports[0].number = dest_port;
	curr->ports[0].proto = proto;
	curr->ports[0].and_flags = tcp_flags;
	curr->ports[0].or_flags = tcp_flags;
	curr->tos = piphdr->tos;
	curr->ttl = piphdr->ttl;

out_no_match:
	spin_unlock(&state.lock);
	return 0;

out_match:
	spin_unlock(&state.lock);
	return 1;
}

int ipt_psd_checkentry(const struct xt_mtchk_param *par)
{
/*	const struct ipt_psd_info *psdinfo = targinfo;*/

	/* we accept TCP only */
/*  	if (e->ip.proto != IPPROTO_TCP) { */
/*  		DEBUGP("PSD: specified protocol may be TCP only\n"); */
/*  		return 0; */
/*  	} */

/*	if (matchsize != IPT_ALIGN(sizeof(struct ipt_psd_info))) {
		DEBUGP("PSD: matchsize %u != %u\n",
		       matchsize,
		       IPT_ALIGN(sizeof(struct ipt_psd_info)));
		return 0;
	}*/

	return 1;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
static struct xt_match ipt_psd_reg = {
#else
static struct ipt_match ipt_psd_reg = {
#endif
	.name = "psd",
	.match = ipt_psd_match,
	.checkentry = ipt_psd_checkentry,
	.me = THIS_MODULE,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	.family         = AF_INET,
	.matchsize      = sizeof(struct ipt_psd_info),
#endif
};

static int __init init(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	if (xt_register_match(&ipt_psd_reg))
		return -EINVAL;
#else
	if (ipt_register_match(&ipt_psd_reg))
		return -EINVAL;
#endif

	memset(&state, 0, sizeof(state));

	spin_lock_init(&(state.lock));

	printk("netfilter PSD loaded - (c) astaro AG\n");
	return 0;
}

static void __exit fini(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	xt_unregister_match(&ipt_psd_reg);
#else
	ipt_unregister_match(&ipt_psd_reg);
#endif
	printk("netfilter PSD unloaded - (c) astaro AG\n");
}

module_init(init);
module_exit(fini);

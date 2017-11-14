/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2006 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* Everything about the rules for NAT. */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/types.h>
#include <linux/ip.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <net/checksum.h>
#include <net/route.h>
#include <linux/bitops.h>

#include <linux/netfilter_ipv4/ip_tables.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_core.h>
#include <net/netfilter/nf_nat_rule.h>
#ifdef CONFIG_SCM_SUPPORT
#ifdef CONFIG_CNAPT
#include <sc/cnapt/nf_cnapt.h>
#endif /* CONFIG_CNAPT */
#include <sc/sc_spi.h>
#endif /* CONFIG_SCM_SUPPORT */
#define NAT_VALID_HOOKS ((1 << NF_INET_PRE_ROUTING) | \
			 (1 << NF_INET_POST_ROUTING) | \
			 (1 << NF_INET_LOCAL_OUT) | \
			 (1 << NF_INET_LOCAL_IN))

static const struct xt_table nat_table = {
	.name		= "nat",
	.valid_hooks	= NAT_VALID_HOOKS,
	.me		= THIS_MODULE,
	.af		= NFPROTO_IPV4,
};

/* Source NAT */
static unsigned int
ipt_snat_target(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	const struct nf_nat_multi_range_compat *mr = par->targinfo;

	NF_CT_ASSERT(par->hooknum == NF_INET_POST_ROUTING ||
		     par->hooknum == NF_INET_LOCAL_IN);

	ct = nf_ct_get(skb, &ctinfo);

	/* Connection must be valid and new. */
	NF_CT_ASSERT(ct && (ctinfo == IP_CT_NEW || ctinfo == IP_CT_RELATED ||
			    ctinfo == IP_CT_RELATED + IP_CT_IS_REPLY));
	NF_CT_ASSERT(par->out != NULL);

	return nf_nat_setup_info(ct, &mr->range[0], IP_NAT_MANIP_SRC);
}

static unsigned int
ipt_dnat_target(struct sk_buff *skb, const struct xt_action_param *par)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	const struct nf_nat_multi_range_compat *mr = par->targinfo;
#ifdef CONFIG_SCM_SUPPORT
#ifdef CONFIG_CNAPT
	struct nf_nat_multi_range_compat new_mr;
	int cnapt_act = 0;
	__be32 privip, pubip;
	__u16 privport, pubport;
#endif
#endif

	NF_CT_ASSERT(par->hooknum == NF_INET_PRE_ROUTING ||
		     par->hooknum == NF_INET_LOCAL_OUT);

	ct = nf_ct_get(skb, &ctinfo);

	/* Connection must be valid and new. */
	NF_CT_ASSERT(ct && (ctinfo == IP_CT_NEW || ctinfo == IP_CT_RELATED));
#ifdef CONFIG_SCM_SUPPORT
#ifdef CONFIG_CNAPT
	if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP ||
		ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP) {
		typeof(cnapt_get_mapping_info_hook) get_info;

		rcu_read_lock();
		get_info = rcu_dereference(cnapt_get_mapping_info_hook);
		if (get_info && 
			(*get_info)(ct, par->hooknum, 
					&privip, &privport, &pubip, &pubport) == 0) {	

			/* This must be fake. */
			if (pubip == 0)
				cnapt_act = 1;

			if (!cnapt_act) {
				if (mr->range[0].flags & IP_NAT_RANGE_MAP_IPS)
					cnapt_act = (privip != mr->range[0].min_ip);
				else
					cnapt_act = (privip != pubip);
			}

			if (!cnapt_act) {
					if (mr->range[0].flags & IP_NAT_RANGE_PROTO_SPECIFIED) {
						if (ntohs(mr->range[0].min.tcp.port) != ntohs(mr->range[0].max.tcp.port))  {
							if (ntohs(mr->range[0].min.tcp.port) > privport || ntohs(mr->range[0].max.tcp.port) < privport)
								cnapt_act = 1;
							else
								cnapt_act = 2;
				 		} 
				 		else if (privport != ntohs(mr->range[0].min.tcp.port))
							cnapt_act = 1;
				 		else
							cnapt_act = 2;
					}
					else {
						if (privport != pubport)
							cnapt_act = 1;
						else
							cnapt_act = 2;
					}
			}
		}

		rcu_read_unlock();
	/* illegal  rule, skip it. */
		if (cnapt_act == 1) {
			printk("DNAT rule is illegal, ignore it! \n");		
			return XT_CONTINUE;
	}
	}
#endif
#endif
#if defined(CONFIG_SCM_SUPPORT) && defined(CONFIG_SPI_FIRWALL)
    if(g_spi_fun_switch && g_spi_enable && ct->from_wan && sc_detect_total_session_for_one_host_hook)
    {
        if(sc_detect_total_session_for_one_host_hook(skb, ct) == NF_DROP)
        {
            if(printk_ratelimit())
                printk("DNAT total session number from one host is too much\n");
            return  NF_DROP;
        }
    }
#endif /* CONFIG_SCM_SUPPORT && CONFIG_SPI_FIREWALL */

#if defined(CONFIG_SCM_SUPPORT) && defined(CONFIG_CNAPT)
	if (cnapt_act == 2) {
		memcpy(&new_mr, mr, sizeof(new_mr));
		/* force using this mapping. */
		new_mr.range[0].min.tcp.port = new_mr.range[0].max.tcp.port = htons(privport);
		mr = &new_mr;
	}
#endif
	return nf_nat_setup_info(ct, &mr->range[0], IP_NAT_MANIP_DST);
}

static int ipt_snat_checkentry(const struct xt_tgchk_param *par)
{
	const struct nf_nat_multi_range_compat *mr = par->targinfo;

	/* Must be a valid range */
	if (mr->rangesize != 1) {
		pr_info("SNAT: multiple ranges no longer supported\n");
		return -EINVAL;
	}
	return 0;
}

static int ipt_dnat_checkentry(const struct xt_tgchk_param *par)
{
	const struct nf_nat_multi_range_compat *mr = par->targinfo;

	/* Must be a valid range */
	if (mr->rangesize != 1) {
		pr_info("DNAT: multiple ranges no longer supported\n");
		return -EINVAL;
	}
	return 0;
}

static unsigned int
alloc_null_binding(struct nf_conn *ct, unsigned int hooknum)
{
	/* Force range to this IP; let proto decide mapping for
	   per-proto parts (hence not IP_NAT_RANGE_PROTO_SPECIFIED).
	   Use reply in case it's already been mangled (eg local packet).
	*/
	__be32 ip
		= (HOOK2MANIP(hooknum) == IP_NAT_MANIP_SRC
		   ? ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip
		   : ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip);
	struct nf_nat_range range
		= { IP_NAT_RANGE_MAP_IPS, ip, ip, { 0 }, { 0 } };

	pr_debug("Allocating NULL binding for %p (%pI4)\n", ct, &ip);
	return nf_nat_setup_info(ct, &range, HOOK2MANIP(hooknum));
}

int nf_nat_rule_find(struct sk_buff *skb,
		     unsigned int hooknum,
		     const struct net_device *in,
		     const struct net_device *out,
		     struct nf_conn *ct)
{
	struct net *net = nf_ct_net(ct);
	int ret;

	ret = ipt_do_table(skb, hooknum, in, out, net->ipv4.nat_table);

	if (ret == NF_ACCEPT) {
		if (!nf_nat_initialized(ct, HOOK2MANIP(hooknum)))
			/* NUL mapping */
			ret = alloc_null_binding(ct, hooknum);
	}
	return ret;
}

static struct xt_target ipt_snat_reg __read_mostly = {
	.name		= "SNAT",
	.target		= ipt_snat_target,
	.targetsize	= sizeof(struct nf_nat_multi_range_compat),
	.table		= "nat",
	.hooks		= (1 << NF_INET_POST_ROUTING) | (1 << NF_INET_LOCAL_IN),
	.checkentry	= ipt_snat_checkentry,
	.family		= AF_INET,
};

static struct xt_target ipt_dnat_reg __read_mostly = {
	.name		= "DNAT",
	.target		= ipt_dnat_target,
	.targetsize	= sizeof(struct nf_nat_multi_range_compat),
	.table		= "nat",
	.hooks		= (1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_LOCAL_OUT),
	.checkentry	= ipt_dnat_checkentry,
	.family		= AF_INET,
};

static int __net_init nf_nat_rule_net_init(struct net *net)
{
	struct ipt_replace *repl;

	repl = ipt_alloc_initial_table(&nat_table);
	if (repl == NULL)
		return -ENOMEM;
	net->ipv4.nat_table = ipt_register_table(net, &nat_table, repl);
	kfree(repl);
	if (IS_ERR(net->ipv4.nat_table))
		return PTR_ERR(net->ipv4.nat_table);
	return 0;
}

static void __net_exit nf_nat_rule_net_exit(struct net *net)
{
	ipt_unregister_table(net, net->ipv4.nat_table);
}

static struct pernet_operations nf_nat_rule_net_ops = {
	.init = nf_nat_rule_net_init,
	.exit = nf_nat_rule_net_exit,
};

int __init nf_nat_rule_init(void)
{
	int ret;

	ret = register_pernet_subsys(&nf_nat_rule_net_ops);
	if (ret != 0)
		goto out;
	ret = xt_register_target(&ipt_snat_reg);
	if (ret != 0)
		goto unregister_table;

	ret = xt_register_target(&ipt_dnat_reg);
	if (ret != 0)
		goto unregister_snat;

	return ret;

 unregister_snat:
	xt_unregister_target(&ipt_snat_reg);
 unregister_table:
	unregister_pernet_subsys(&nf_nat_rule_net_ops);
 out:
	return ret;
}

void nf_nat_rule_cleanup(void)
{
	xt_unregister_target(&ipt_dnat_reg);
	xt_unregister_target(&ipt_snat_reg);
	unregister_pernet_subsys(&nf_nat_rule_net_ops);
}

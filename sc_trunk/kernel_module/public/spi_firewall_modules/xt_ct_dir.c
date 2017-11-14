#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ipv6.h>
#include <net/ip.h>

#include <net/netfilter/nf_conntrack.h>

#include <sc/cnapt/xt_ct_dir.h>
#include <linux/netfilter/x_tables.h>

MODULE_DESCRIPTION("IP tables packet ct matching module");
MODULE_LICENSE("GPL");
/*
bool (*match)(const struct sk_buff *skb,
          const struct xt_match_param *);
static int
match(const struct sk_buff *skb,
      const struct net_device *in,
      const struct net_device *out,
      const struct xt_match *match,
      const void *matchinfo,
      int offset,
      unsigned int protoff,
      int *hotdrop)
*/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
static bool
match(const struct sk_buff *skb,
		      struct xt_action_param *param)
#else
bool match(const struct sk_buff *skb,
        struct xt_match_param *param)
#endif
{
	const struct xt_ct_dir_info *info = (struct xt_ct_dir_info *)param->matchinfo;
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;

	ct = nf_ct_get(skb, &ctinfo);
	
	if(ct)
		return (ct->from_wan == info->dir) ^ info->invert;
	else
		return 0;
}


static struct xt_match xt_ct_dir_match[] = {
	{
		.name		= "ct_dir",
		.family		= AF_INET,
		.match		= match,
		.matchsize	= sizeof(struct xt_ct_dir_info),
		.me		= THIS_MODULE,
	},
	{
		.name		= "ct_dir",
		.family		= AF_INET6,
		.match		= match,
		.matchsize	= sizeof(struct xt_ct_dir_info),
		.me		= THIS_MODULE,
	},
};

static int __init xt_ct_dir_init(void)
{
	return xt_register_matches(xt_ct_dir_match,
				   ARRAY_SIZE(xt_ct_dir_match));
}

static void __exit xt_ct_dir_fini(void)
{
	xt_unregister_matches(xt_ct_dir_match, ARRAY_SIZE(xt_ct_dir_match));
}

module_init(xt_ct_dir_init);
module_exit(xt_ct_dir_fini);

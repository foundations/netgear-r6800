/*
  This is a module which is used for a "random" match support.
  This file is distributed under the terms of the GNU General Public
  License (GPL). Copies of the GPL can be obtained from:
     ftp://prep.ai.mit.edu/pub/gnu/GPL

  2001-10-14 Fabrice MARIE <fabrice@netfilter.org> : initial implementation.
*/

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/random.h>
#include <net/tcp.h>
#include <linux/spinlock.h>
#include <linux/netfilter/x_tables.h>
#include <linux/version.h>
#include "ipt_random.h"

MODULE_LICENSE("GPL");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
static bool
ipt_rand_match(const struct sk_buff *skb,
		      struct xt_action_param *par)
#else   
static bool
ipt_rand_match(const struct sk_buff *skb, 
				struct xt_match_param *par)
#endif
{
	/* Parameters from userspace */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	const struct ipt_rand_info *info = par->matchinfo;
#else
    const struct ipt_rand_info *info = matchinfo;
#endif
	u_int8_t random_number;

	/* get 1 random number from the kernel random number generation routine */
	get_random_bytes((void *)(&random_number), 1);

	/* Do we match ? */
	if (random_number <= info->average)
		return 1;
	else
		return 0;
}

static int
ipt_rand_checkentry(const struct xt_mtchk_param *par)
{
	/* Parameters from userspace */
	const struct ipt_rand_info *info = par->matchinfo;
    /*
	if (match->matchsize != IPT_ALIGN(sizeof(struct ipt_rand_info))) {
		printk("ipt_random: matchsize %u != %u\n", match->matchsize,
		       IPT_ALIGN(sizeof(struct ipt_rand_info)));
		return 0;
	}
            */
	/* must be  1 <= average % <= 99 */
	/* 1  x 2.55 = 2   */
	/* 99 x 2.55 = 252 */
	if ((info->average < 2) || (info->average > 252)) {
		printk("ipt_random:  invalid average %u\n", info->average);
		return 0;
	}

	return 1;
}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
static struct xt_match ipt_rand_reg = {
#else
static struct ipt_match ipt_rand_reg = { 
#endif
    .name = "random",
	.match = ipt_rand_match,
	.checkentry = ipt_rand_checkentry,
	.me = THIS_MODULE,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	.family         = AF_INET,
	.matchsize      = sizeof(struct ipt_rand_info),
#endif
};

static int __init init(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	if (xt_register_match(&ipt_rand_reg))
		return -EINVAL;
#else
	if (ipt_register_match(&ipt_rand_reg))
		return -EINVAL;
#endif
	printk("ipt_random match loaded\n");
	return 0;
}

static void __exit fini(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	xt_unregister_match(&ipt_rand_reg);
#else
	ipt_unregister_match(&ipt_rand_reg);
#endif
	printk("ipt_random match unloaded\n");
}

module_init(init);
module_exit(fini);

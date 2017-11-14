#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/string.h>
#include <linux/kmod.h>
#include <linux/slab.h>
#include <linux/ip.h>
#include <linux/netfilter/x_tables.h>
#include <linux/version.h>

#define MAX_VPN_NAME_LENGTH 12
struct xt_vpntrigger_info {
	char name[MAX_VPN_NAME_LENGTH+1];
};
#define IPSEC_INIT_SH "/etc/ipsec_init_conn.sh"

/* make a copy of argv. */
static char **argv_dup(char **argv, int argc, gfp_t gfp_flags)
{
	char **cp;
	int i;

	cp = kzalloc(sizeof(char *) * argc, gfp_flags);
	if (!cp)
		return NULL;
	for (i = 0; argv[i]; i++) {
		cp[i] = kstrdup(argv[i], gfp_flags);
		if (!cp[i]) {
			argv_free(cp);
			return NULL;
		}
	}
	return cp;
}

/* callback function when usermodehelper done. */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
static void argv_cleanup(struct subprocess_info *info)
{
	argv_free(info->argv);
}
#else
static void argv_cleanup(char **argv, char **envp)
{
	argv_free(argv);
}
#endif

/*
 * when a packet sent to the vpn way was catch, a vpn connection will be
 * triggerred here. a user space script will be lauched asynchronously 
 * to do this.
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
static unsigned int
vpn_target(struct sk_buff *skb,
			const struct xt_action_param *tg_parm)
#else
static unsigned int
vpn_target(struct sk_buff *skb,
			const struct xt_target_param *tg_parm)
#endif
{
	struct xt_vpntrigger_info *info = (struct xt_vpntrigger_info *)tg_parm->targinfo;
	struct subprocess_info *sp_info = NULL;
	char **argv = NULL;
	char *user_argv[3];

	user_argv[0] = IPSEC_INIT_SH;
	user_argv[1] = info->name;
	user_argv[2] = NULL;
	/* it's necessary to make a copy of argv to avoid memory problem. */
	argv = argv_dup(user_argv, sizeof(user_argv)/sizeof(user_argv[0]), GFP_ATOMIC);

	if (!argv)		
		return NF_DROP;

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
	sp_info = call_usermodehelper_setup(argv[0], argv, NULL, GFP_ATOMIC,
		NULL, argv_cleanup, NULL);
#else
	sp_info = call_usermodehelper_setup(argv[0], argv, NULL, GFP_ATOMIC);
#endif
	if (!sp_info) {
		argv_free(argv);
		return NF_DROP;
	}

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
	call_usermodehelper_setfns(sp_info, NULL, argv_cleanup, NULL);
#else
	call_usermodehelper_setcleanup(sp_info, argv_cleanup);
#endif

	call_usermodehelper_exec(sp_info, UMH_NO_WAIT);

	/* eat this packet. */
	return NF_DROP;	
}

static struct xt_target vpn_trigger_reg = {
	.name		= "VPN_TRIGGER",
	.family		= AF_INET,
	.target		= vpn_target,
	.targetsize	= XT_ALIGN(sizeof(struct xt_vpntrigger_info)),
	.hooks		= (1 << NF_INET_POST_ROUTING),
	.me			= THIS_MODULE,
};
static int __init init(void)
{
	return xt_register_target(&vpn_trigger_reg);
}

static void __exit fini(void)
{
	xt_unregister_target(&vpn_trigger_reg);
}

module_init(init);
module_exit(fini);

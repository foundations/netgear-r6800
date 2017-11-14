#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <linux/types.h>
#include <xtables.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_VPN_NAME_LENGTH 12
struct xt_vpntrigger_info {
	char name[MAX_VPN_NAME_LENGTH+1];
};

/* Function which prints out usage message. */
static void
help(void)
{
	printf("--names 123 123 is the vpn connetion name\n");
}


/* Initialize the target. */
static void
init(struct xt_entry_target *t)
{
}

static struct option opts[] = {
	{ "name", 1, 0, '1' },
	{ 0 }
};
/* Function which parses command options; returns true if it
   ate an option */
static int parse(int c, char **argv, int invert, unsigned int *flags,
		     const void *entry,
		     struct xt_entry_target **target)
{
	struct xt_vpntrigger_info *info = (struct xt_vpntrigger_info *)(*target)->data;
	
	switch (c) {
	case '1':
		if(*optarg == '\0' || strlen(optarg) > MAX_VPN_NAME_LENGTH)
			return 0;
		strcpy(info->name, optarg);
		printf("vpn connection name(%s)\n", info->name);
		return 1;
	default:
		return 0;
	}
	return 0;
}

/* Final check; don't care. */
static void final_check(unsigned int flags)
{
	
}

/* Prints out the targinfo. */
static void
print(const void *ip,
		      const struct xt_entry_target *target, int numeric)
{
	struct xt_vpntrigger_info *info = (struct xt_vpntrigger_info *)target->data;
	
	printf("name:%s", info->name);
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save(const void *ip,
		 const struct xt_entry_target *target)
{
	struct xt_vpntrigger_info *info = (struct xt_vpntrigger_info *)target->data;
	
	printf("name:%s", info->name);
}

static struct xtables_target ipt_vpn_trigger = {
	.family		= NFPROTO_IPV4,
	.name		= "VPN_TRIGGER",
	.version	= XTABLES_VERSION,
	.size		= XT_ALIGN(sizeof(struct xt_vpntrigger_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct xt_vpntrigger_info)),
	.help		= &help,
	.init		= &init,
	.parse		= &parse,
	.final_check	= &final_check,
	.print		= &print,
	.save		= &save,
	.extra_opts	= opts
};

void _init(void)
{
	xtables_register_target(&ipt_vpn_trigger);
}

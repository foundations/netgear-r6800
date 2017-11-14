#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <xtables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/version.h>
#include "libipt_macblock_dnshj.h"
#ifndef IPADDR_LEN
#define IPADDR_LEN 16
#endif

static void macblock_dnshj_help(void)
{
}

static void macblock_dnshj_init(struct xt_entry_target *t)
{
	struct ipt_macblock_dnshj_info *dnshj = (struct ipt_macblock_dnshj_info *)t->data;
	strcpy(dnshj->srv_ip, "0.0.0.0");
}
static const struct option macblock_dnshj_opts[] = {
	{.name = "srvip", .has_arg = true, .val = '1'},
	XT_GETOPT_TABLEEND,
};
static int macblock_dnshj_parse(int c, char **argv, int invert, unsigned int *flags,
				const struct ipt_entry *entry,
				struct xt_entry_target **target)
{
	struct ipt_macblock_dnshj_info *dnshj = (struct ipt_macblock_dnshj_info *)(*target)->data;
	struct in_addr *addrs;
	switch(c) {
		case '1':
			if(xtables_check_inverse(optarg, &invert, NULL, 0, argv))
				xtables_error(PARAMETER_PROBLEM, "Unexpected '!' after --srvip");
			addrs = xtables_numeric_to_ipaddr(optarg);
			if(addrs == NULL)
				xtables_error(PARAMETER_PROBLEM, "xt_ipaddr: Parse error at %s", optarg);
			memcpy(dnshj->srv_ip, optarg, strlen(optarg) + 1);
			return true;
	}
	return false;
}

static void macblock_dnshj_check(unsigned int flags)
{
}

static void macblock_dnshj_print(const struct ipt_ip *ip, const struct ipt_entry_match *match, int numeric)
{
}
static void macblock_dnshj_save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
}


static struct xtables_target macblock_dnshj_tg_reg = {
	.name = "macblock_dnshj",
	.version	= XTABLES_VERSION,
	.size		= XT_ALIGN(sizeof(struct ipt_macblock_dnshj_info)),
	.userspacesize	= XT_ALIGN(sizeof(struct ipt_macblock_dnshj_info)),
	.family		= NFPROTO_IPV4,
	.help 		= macblock_dnshj_help,
	.init 		= macblock_dnshj_init,
	.parse		= macblock_dnshj_parse,
	.final_check	= macblock_dnshj_check,
	.print		= macblock_dnshj_print,
	.save		= macblock_dnshj_save,
	.extra_opts = macblock_dnshj_opts,
};

void _init(void)
{
	xtables_register_target(&macblock_dnshj_tg_reg);
}



/*
 * Copyright (C) 2012  Sercomm Corporation
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


/* Shared library add-on to iptables to add masquerade support. */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/types.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>

#include <sc/cnapt/ipt_NATMGR.h>

/* Function which prints out usage message. */
static void
help(void)
{
	printf(
"NATMGR v%s options:\n"
,
IPTABLES_VERSION);
}

static struct option opts[] = {
	{"dir", 1, 0, '1'},
	{ 0 }
};

/* Initialize the target. */
static void
init(struct ipt_entry_target *t, unsigned int *nfcache)
{
}


/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      struct ipt_entry_target **target)
{
	struct ipt_natmgr_info_t *info = (struct ipt_natmgr_info_t *)(*target)->data;

	switch (c) {
	case '1':
		if (strcmp(optarg, "out") == 0)
			info->dir = IP_CT_DIR_ORIGINAL;
		else if (strcmp(optarg, "in") == 0)
			info->dir = IP_CT_DIR_REPLY;
		else
			return 0;
		return 1;
	default:
		return 0;
	}
	return 1;
}

/* Final check; don't care. */
static void final_check(unsigned int flags)
{
}

/* Prints out the targinfo. */
static void
print(const struct ipt_ip *ip,
      const struct ipt_entry_target *target,
      int numeric)
{
	struct ipt_natmgr_info_t *info = (struct ipt_natmgr_info_t *)target->data;

	printf("direction:%s", info->dir == IP_CT_DIR_ORIGINAL ? "out" :"in");
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save(const struct ipt_ip *ip, const struct ipt_entry_target *target)
{
	struct ipt_natmgr_info_t *info = (struct ipt_natmgr_info_t *)target->data;

	printf("--dir %s", info->dir == IP_CT_DIR_ORIGINAL ? "out" :"in");
}

static struct iptables_target ipt_cdmz = { NULL,
	.name		= "NATMGR",
	.version	= IPTABLES_VERSION,
	.size		= IPT_ALIGN(sizeof(struct ipt_natmgr_info_t)),
	.userspacesize	= IPT_ALIGN(sizeof(struct ipt_natmgr_info_t)),
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
	register_target(&ipt_cdmz);
}

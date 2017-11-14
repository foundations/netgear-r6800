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
#include <linux/types.h>
#include <xtables.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sc/cnapt/ipt_NATLIMIT.h>

static int get_net_prefix(unsigned long ulmask);

static int get_net_prefix(unsigned long ulmask)
{
	int prefix_len;

	ulmask = ntohl(ulmask);

	for( prefix_len =0; prefix_len < 32 ; prefix_len ++)
	{
		if( ! (ulmask << prefix_len) )
		        return prefix_len;
	}

	return prefix_len;
}

/* Function which prints out usage message. */
static void
help(void)
{
	printf("NATLIMIT options:\n");
}

static struct option opts[] = {
	{"lan", 1, 0, '1'},
	{ 0 }
};

/* Initialize the target. */
static void
init(struct xt_entry_target *t)
{
}

/* Function which parses command options; returns true if it
   ate an option */
static int parse(int c, char **argv, int invert, unsigned int *flags,
		     const void *entry,
		     struct xt_entry_target **target)
{
	struct ipt_natlimit_info_t *info = (struct ipt_natlimit_info_t *)(*target)->data;
	char *prefix;
	int  len;
	u_int32_t mask = 0xffffffff;
	char buffer[64];
	
	switch (c) {
	case '1':
		snprintf(buffer, sizeof(buffer), optarg);

		prefix = strchr(buffer, '/');
		if (!prefix) {
			printf("error prefix\n");
			return 0;
		}
		*prefix++ = '\0';
/* We shouldn't use "inet_network" here as its return value is "Host byte order".     --- Joe */
//		info->lan_addr = inet_network(buffer);
		info->lan_addr = inet_addr(buffer);
		if (info->lan_addr == -1) {
			printf("error lan_addr\n");
			return 0;
		}
		len = atoi(prefix);
		if (len > 32){
			return 0;
		}
		mask <<= (32-len);
		info->lan_mask = htonl(mask);
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
static void print(const void *ip,
		      const struct xt_entry_target *target, int numeric)
{
	struct ipt_natlimit_info_t *info = (struct ipt_natlimit_info_t *)target->data;
	struct in_addr addr;

	addr.s_addr = info->lan_addr;

	printf("lan:%s/%d", inet_ntoa(addr), get_net_prefix(info->lan_mask));
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void save(const void *ip,
		     const struct xt_entry_target *target)
{
	struct ipt_natlimit_info_t *info = (struct ipt_natlimit_info_t *)target->data;
	struct in_addr addr;

	addr.s_addr = info->lan_addr;

	printf("--lan %s/%d", inet_ntoa(addr), get_net_prefix(info->lan_mask));
}

static struct xtables_target ipt_NATLIMIT = { NULL,
	.name		= "NATLIMIT",
	.version	= XTABLES_VERSION,
	.size		= XT_ALIGN(sizeof(struct ipt_natlimit_info_t)),
	.userspacesize	= XT_ALIGN(sizeof(struct ipt_natlimit_info_t)),
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
	xtables_register_target(&ipt_NATLIMIT);
}

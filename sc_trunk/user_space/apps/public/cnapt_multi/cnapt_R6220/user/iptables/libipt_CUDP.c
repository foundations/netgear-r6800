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
#include <sc/cnapt/ipt_CUDP.h>

/* Function which prints out usage message. */
static void
help(void)
{
	printf(
"CUDP options \n"
" --ctmark value\n"
"				set the conntrack mark value.\n");
}

static struct option opts[] = {
	{ "ctmark", 1, 0, '1' },
	{ "mode", 1, 0, '2'},
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
	struct ipt_cudp_info_t *info = (struct ipt_cudp_info_t *)(*target)->data;
	switch (c) {
		case '1':
			info->ctmark = strtoul(optarg, NULL, 0);
			return 1;
		case '2':
			if (strcmp(optarg, "open") ==0)
				info->mode = UDP_OPEN;
			else if (strcmp(optarg, "security") == 0)
				info->mode = UDP_SEC;
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
print(const void *ip,
		      const struct xt_entry_target *target, int numeric)
{
	struct ipt_cudp_info_t *info = (struct ipt_cudp_info_t *)target->data;

	switch (info->mode) {
	case UDP_OPEN:
		printf("mode:open ");
		break;
	case UDP_SEC:
		printf("mode:security ");
		break;
	default:
		break;
	}
	if (info->ctmark)
		printf(" ctmark:0x%x", info->ctmark);

}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save(const void *ip,
		 const struct xt_entry_target *target)
{
	struct ipt_cudp_info_t *info = (struct ipt_cudp_info_t *)target->data;

	switch (info->mode) {
	case UDP_OPEN:
		printf("--mode open ");
		break;
	case UDP_SEC:
		printf("--mode security ");
		break;
	default:
		break;
	}
	if (info->ctmark)
		printf(" --ctmark 0x%x", info->ctmark);
}

static struct xtables_target ipt_CUDP = { NULL,
	.name		= "CUDP",
	.version	= XTABLES_VERSION,
	.size		= XT_ALIGN(sizeof(struct ipt_cudp_info_t)),
	.userspacesize	= XT_ALIGN(sizeof(struct ipt_cudp_info_t)),
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
	xtables_register_target(&ipt_CUDP);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <iptables.h>

#include<xtables.h>
#include "ipt_condition.h"


static void
help(void)
{
	printf("condition match v%s options:\n"
	       "--condition [!] filename       "
	       "Match on boolean value stored in /proc file\n",
	       IPTABLES_VERSION);
}


static struct option opts[] = {
	{ .name = "condition", .has_arg = 1, .flag = 0, .val = 'X' },
	{ .name = 0 }
};



static int
parse(int c, char **argv, int invert, unsigned int *flags,
                       const void *entry, struct xt_entry_match **match)
{
	struct condition_info *info =
	    (struct condition_info *) (*match)->data;

	xtables_check_inverse(optarg, &invert, &optind, 0, argv);

	if (c == 'X') {
		if (*flags)
			xtables_error(PARAMETER_PROBLEM,
				   "Can't specify multiple conditions");

		if (strlen(argv[optind - 1]) < CONDITION_NAME_LEN)
			strcpy(info->name, argv[optind - 1]);
		else
			xtables_error(PARAMETER_PROBLEM,
				   "File name too long");

		info->invert = invert;
		*flags = 1;
		return 1;
	}

	return 0;
}


static void
final_check(unsigned int flags)
{
	if (!flags)
		xtables_error(PARAMETER_PROBLEM,
			   "Condition match: must specify --condition");
}


static void
print(const void *ip, const struct xt_entry_match *match,
                        int numeric)
{
	const struct condition_info *info =
	    (const struct condition_info *) match->data;

	printf("condition %s%s ", (info->invert) ? "!" : "", info->name);
}


static void
save(const void *ip, const struct xt_entry_match *match)
{
	const struct condition_info *info =
	    (const struct condition_info *) match->data;

	printf("--condition %s%s ", (info->invert) ? "! " : "", info->name);
}


static struct xtables_match condition = {
	.name = "condition",
	.version = XTABLES_VERSION,
	.family	= NFPROTO_IPV4,
	.size = IPT_ALIGN(sizeof(struct condition_info)),
	.userspacesize = IPT_ALIGN(sizeof(struct condition_info)),
	.help = help,
	.parse = parse,
	.final_check = final_check,
	.print = print,
	.save = save,
	.extra_opts = opts
};


void
_init(void)
{
	xtables_register_match(&condition);
}
/* Shared library add-on to iptables to add packet conntrack direction matching support. */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <xtables.h>
#include <sc/cnapt/xt_ct_dir.h>

#define FROM_WAN "from_wan"
/* Function which prints out usage message. */
static void
help(void)
{
        printf(
"conntrack direction v%s options:\n"
"[!] --ct_dir from_wan    Match packet conntrack dirctio against value\n",
XTABLES_VERSION);

}
  
static struct option opts[] = {
        { "ct_dir", 1, 0, '1' },
        {0}
};

/* from wan  */
static void
parse_ct_dir(const char *s, struct xt_ct_dir_info *info)
{
        char *buffer;

        buffer = strdup(s);
        if(strcmp(buffer, FROM_WAN) == 0){
                info->dir = 1;
        }
        free(buffer);
        
        if (!info->dir)
                xtables_error(PARAMETER_PROBLEM,
                           "conntrack direction parameter(%s) is wrong. "
                           "info->dir `%u'", s, info->dir);
        
}

/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const void *entry,
      struct xt_entry_match **match)
{
        struct xt_ct_dir_info *info = (struct xt_ct_dir_info *)(*match)->data;

        switch (c) {
                case '1':
                        if (*flags)
                                xtables_error(PARAMETER_PROBLEM,
                                           "ct direction: `--ct_dir' may only be "
                                           "specified once");
                        xtables_check_inverse(optarg, &invert, &optind, 0,argv);
                        parse_ct_dir(argv[optind-1], info);
                        if (invert)
                                info->invert = 1;
                        *flags = 1;
                        break;
                        
                default:
                        return 0;
        }
        return 1;
}

/* Final check; must have specified --ct_dir. */
static void
final_check(unsigned int flags)
{
        if (!flags)
                xtables_error(PARAMETER_PROBLEM,
                           "ct direction: You must specify `--ct_dir'");
}

/* Common match printing code. */
static void
print_ct_dir(struct xt_ct_dir_info *info)
{
        if (info->invert)
                printf("! ");
        
        if (info->dir)
        {
                printf("From WAN  ");
        
                printf("%d ", info->dir);
        }
}

/* Prints out the matchinfo. */
static void
print(const void *ip,
      const struct xt_entry_match *match,
      int numeric)
{
        printf("ct direction ");
        print_ct_dir((struct xt_ct_dir_info *)match->data);
}

/* Saves the union ipt_matchinfo in parsable form to stdout. */
static void
save(const void *ip, const struct xt_entry_match *match)
{
        printf("--ct_dir ");
        print_ct_dir((struct xt_ct_dir_info *)match->data);
}

static struct xtables_match ct_dir = { 
        .next           = NULL,
        .name           = "ct_dir",
        .version        = XTABLES_VERSION,
        .size           = XT_ALIGN(sizeof(struct xt_ct_dir_info)),
        .userspacesize  = XT_ALIGN(sizeof(struct xt_ct_dir_info)),
        .help           = &help,
        .parse          = &parse,
        .final_check    = &final_check,
        .print          = &print,
        .save           = &save,
        .extra_opts     = opts
};

void _init(void)
{
        xtables_register_match(&ct_dir);
}

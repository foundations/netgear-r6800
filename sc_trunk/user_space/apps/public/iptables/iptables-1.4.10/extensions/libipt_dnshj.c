#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>

#define MAX_DOMAIN_NAME_LEN     255

/* Function which prints out usage message. */
static void
help(void)
{
	printf("No option", IPTABLES_VERSION);
}

/* Initialize the target. */
static void
init(struct ipt_entry_match *m, unsigned int *nfcache)
{
    return;
}

/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      unsigned int *nfcache,
      struct ipt_entry_match **match)
{
}

/* Final check; nothing. */
static void final_check(unsigned int flags)
{
}

/* Prints out the targinfo. */
static void
print(const struct ipt_ip *ip,
      const struct ipt_entry_match *match,
      int numeric)
{
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
}

static struct xtables_match dnshj = {
	.name		= "dnshj",
	.version    = XTABLES_VERSION,
	.size		= XT_ALIGN(MAX_DOMAIN_NAME_LEN),
	.userspacesize	= XT_ALIGN(MAX_DOMAIN_NAME_LEN),
	.help		= &help,
	.init		= &init,
	.parse		= &parse,
	.final_check	= &final_check,
	.print		= &print,
	.save		= &save
};

void _init(void)
{
	xtables_register_match(&dnshj);
}

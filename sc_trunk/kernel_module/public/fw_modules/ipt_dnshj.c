/* Netgear Spec defines:
 *  When the router performs DNS hijack, it intercepts all the interested DNS queries
 *  going through it either destined to the router itself or to other DNS servers on
 *  the Internet. That is, no matter a PC on the LAN configures its DNS server
 *  to be setup dynamically or to IPs other than the router, the router MUST hijack the
 *  interested DNS queries from the PC.
 * PA's test case:
 *  1.Fix DNS of LAN host to IPs other than the router, eg:61.177.7.1
 *  2.LAN host access
 *       www.routerlogin.com/ routerlogin.com/ www.routerlogin.net/ routerlogin.net.
 * Expected result:
 *  DUT should hijack these dns.
 *
 * Write a macth module, to choose these interested DNS queries.
 */
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/in.h>
#include <net/ip.h>
#include <linux/netfilter/x_tables.h>
#include <linux/version.h>
#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif

#define MAX_DOMAIN_NAME_LEN     255

//#define DEBUG

static char interested_query1[] = {0x03, 'w', 'w', 'w', 0x0b, 'r', 'o', 'u', 't', 'e', 'r', 'l', 'o', 'g', 'i', 'n', 0x03, 'c', 'o', 'm', 0};
static char interested_query2[] = {0x0b, 'r', 'o', 'u', 't', 'e', 'r', 'l', 'o', 'g', 'i', 'n', 0x03, 'c', 'o', 'm', 0};
static char interested_query3[] = {0x03, 'w', 'w', 'w', 0x0b, 'r', 'o', 'u', 't', 'e', 'r', 'l', 'o', 'g', 'i', 'n', 0x03, 'n', 'e', 't', 0};
static char interested_query4[] = {0x0b, 'r', 'o', 'u', 't', 'e', 'r', 'l', 'o', 'g', 'i', 'n', 0x03, 'n', 'e', 't', 0};
static char interested_query5[] = {0x0a, 'r', 'e', 'a', 'd', 'y', 's', 'h', 'a', 'r', 'e', 0x0b, 'r', 'o', 'u','t', 'e', 'r', 'l', 'o', 'g', 'i', 'n', 0x03, 'c', 'o', 'm', 0};
static char interested_query6[] = {0x0a, 'r', 'e', 'a', 'd', 'y', 's', 'h', 'a', 'r', 'e', 0x0b, 'r', 'o', 'u','t', 'e', 'r', 'l', 'o', 'g', 'i', 'n', 0x03, 'n', 'e', 't', 0};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
static bool ipt_dnshj_match(const struct sk_buff *skb, struct xt_action_param *par)
#else
static bool ipt_dnshj_match(const struct sk_buff *skb, struct xt_match_param *par)
#endif
{
    struct iphdr  *iphdr = ip_hdr(skb);
    struct udphdr *udphdr = (struct udphdr *)(skb_network_header(skb) + ip_hdrlen(skb));
    char          *query_name = NULL;

#ifdef DEBUG
    printk("INTO ===ipt_dnshj_match====, protocol:<%d>, dest port:<%d>******\n", iphdr->protocol, ntohs(udphdr->dest));
#endif
    /* only check DNS query packet */
    if (iphdr->protocol != IPPROTO_UDP || ntohs(udphdr->dest) != 53) {
        return 0;
    }

    if (udphdr->len < 38) {
        printk("Bad dns query!\n");
        return 0;
    }
    /* skip UDP header */
    query_name = (char *)(struct udphdr *)(skb_network_header(skb) + ip_hdrlen(skb) + sizeof(struct udphdr) + 12);

#ifdef DEBUG
    printk("it's dns query packet, query name:<%s>******\n", query_name);
#endif
    if (!strcmp(query_name, interested_query1) || !strcmp(query_name, interested_query2)
     || !strcmp(query_name, interested_query3) || !strcmp(query_name, interested_query4)
     || !strcmp(query_name, interested_query5) || !strcmp(query_name, interested_query6)) {
        printk("[DNS QUERY Mach]\n");
        return 1;
    }
#ifdef DEBUG
    printk("[DNS QUERY not Mach]\n");
#endif
    return 0;
}

static int checkentry(const struct xt_mtchk_param *par)
{
    return 0;
}

static struct xt_match dnshj_match = {
	.name = "dnshj",
	.match = ipt_dnshj_match,
	.checkentry = checkentry,
	.me = THIS_MODULE,
	.family         = AF_INET,
	.matchsize      = MAX_DOMAIN_NAME_LEN,
};

static int __init init(void)
{
    if (xt_register_match(&dnshj_match))
		return -EINVAL;

	printk("netfilter dnshj choose interested dns queries module loaded !\n");
	return 0;
}

static void __exit fini(void)
{
	xt_unregister_match(&dnshj_match);
	printk("netfilter dnshj choose interested dns queries module unloaded !\n");
}

module_init(init);
module_exit(fini);

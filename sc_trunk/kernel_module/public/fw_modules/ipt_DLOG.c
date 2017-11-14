/*
 * This target implement log NO.15 and NO.20 of netgear spec v1.9.
 *
 * We use --log-level to distinguish which type of log.
 *
 * Caller should use service name as --log-prefix.
 * 15. If "Block Services" is enabled
 *     Should log an entry when LAN PC tries to access a blocked service
 *     [service blocked: HTTP(or user-defined service name)] from source x.x.x.x, Monday, February 20,2006 04:56:01
 *
 * 20. Any time an external IP accesses the internal IP, either through Port Forwarding/Port
 *     Triggering, or through DMZ, one log entry(the initial connection) should be logged:
 *     [LAN access from remote] from externalIP:port to internalIP:port, Monday, February 20,2006 04:56:01
 *                                                              Argon Cheng
 *                                                              2010-02-22
 */
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/icmp.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <net/route.h>
#include <linux/version.h>
#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ipt_LOG.h>
#include <sc/sc_spi.h>

#define DEBUGP(format, args...)


//static unsigned int
//ipt_log_target(struct sk_buff **pskb,
//	       const struct net_device *in,
//	       const struct net_device *out,
//	       unsigned int hooknum,
//	       const struct xt_target *target,
//	       const void *targinfo)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
static unsigned int
ipt_log_target(struct sk_buff *skb, const struct xt_action_param *par)
#else
static unsigned int
ipt_log_target(struct sk_buff *skb, const struct xt_target_param *par)
#endif
{
    struct iphdr *iph = ip_hdr(skb);
	const struct ipt_log_info *loginfo = par->targinfo;
//    struct sk_buff *skb = *pskb;
    unsigned short sport=0, dport=0;
	char service_name[64] = "";
	char *locat;

	if(loginfo->level != 15)
	{
        /* Get port number. Show 0 if not TCP or UDP */
        sport = dport = 0;
        switch (iph->protocol) {
        case IPPROTO_TCP: {
            struct tcphdr *tcph = (struct tcphdr *)(skb_network_header(skb) + ip_hdrlen(skb));

            if (ntohs(iph->frag_off) & IP_OFFSET) {
                break;
            }
            sport = ntohs(tcph->source);
            dport = ntohs(tcph->dest);
            break;
        }
        case IPPROTO_UDP: {
            struct udphdr *udph = (struct udphdr *)(skb_network_header(skb) + ip_hdrlen(skb));

            if (ntohs(iph->frag_off) & IP_OFFSET) {
                break;
            }
            sport = ntohs(udph->source);
            dport = ntohs(udph->dest);
            break;
        }
        default:
            break;
        }
	}
    switch(loginfo->level) {
    case 15:
		strcpy(service_name, loginfo->prefix);
		if((locat = strchr(service_name, '\x03')) != NULL)
		{
			*locat = '\0';
		}
        NETGEAR_SYSLOG("%02d 00[service blocked: %s] from source %u.%u.%u.%u\n", loginfo->level, service_name, NIPQUAD(iph->saddr));
	    return NF_DROP;
        break;
    case 20:
        /* We should add even number as prefix. See syslogd.c */
        NETGEAR_SYSLOG("%02d 00[LAN access from remote] from %u.%u.%u.%u:%u to %u.%u.%u.%u:%u\n",
            loginfo->level, NIPQUAD(iph->saddr), sport, NIPQUAD(iph->daddr), dport);
        break;
    case 19:
        NETGEAR_SYSLOG("%02d 00[DoS attack: %s] from source: %u.%u.%u.%u:%hu\n",
            loginfo->level, loginfo->prefix, NIPQUAD(iph->saddr), sport);
    	break;
#ifdef CONFIG_AMPED_DOS
    case 21: /* log it and then block it. */
        NETGEAR_SYSLOG("19 00[DoS attack: %s] from source: %u.%u.%u.%u:%hu\n",
            loginfo->prefix, NIPQUAD(iph->saddr), sport);
	if (sc_block_source_ip_hook && g_source_block_time)
		sc_block_source_ip_hook(iph->saddr, sport, g_source_block_time);
    	break;
#endif
    case 31:
        /* We should add even number as prefix. See syslogd.c */
        NETGEAR_SYSLOG("%02d 00%s from %u.%u.%u.%u:%u to %u.%u.%u.%u:%u\n",
            loginfo->level, loginfo->prefix, NIPQUAD(iph->saddr), sport, NIPQUAD(iph->daddr), dport);
        break;

    default:
        break;
    }
    return XT_CONTINUE;
}

//static int ipt_log_checkentry(const char *tablename,
//			      const void *e,
//			      const struct xt_target *target,
//			      void *targinfo,
//			      unsigned int hook_mask)
static int ipt_log_checkentry(const struct xt_tgchk_param *par)
{
	const struct ipt_log_info *loginfo = par->targinfo;

	if (loginfo->prefix[sizeof(loginfo->prefix)-1] != '\0') {
		DEBUGP("LOG: prefix term %i\n",
		       loginfo->prefix[sizeof(loginfo->prefix)-1]);
		return 1;
	}
	return 0;
}

static struct xt_target ipt_log_reg = {
	.name		= "DLOG",
	.family		= AF_INET,
	.target		= ipt_log_target,
	.targetsize	= XT_ALIGN(sizeof(struct ipt_log_info)),
	.checkentry	= ipt_log_checkentry,
	.me		= THIS_MODULE,
};

static int __init ipt_log_init(void)
{
	int ret;

	ret = xt_register_target(&ipt_log_reg);
	return ret;
}

static void __exit ipt_log_fini(void)
{
	xt_unregister_target(&ipt_log_reg);
}

module_init(ipt_log_init);
module_exit(ipt_log_fini);

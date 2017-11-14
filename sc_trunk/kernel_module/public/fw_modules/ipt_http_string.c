/**
 * @file   ipt_http_string.c
 * @author Oliver.Hao  <oliver_hao@sdc.sercomm.com>
 * @date   Fri Dec  4 15:41:59 2009
 *
 * @brief  Kernel module to match http string into a packet.
 *
 * This module provide functions for http packet filter. These
 * functions include:
 *
 * - URI/HOST/CONTENT/JAVA/ACTIVEX/COOKIE/PROXY
 *
 * NOTES:
 *
 * - If you want check host field in following packets, please enable
 *   the macro HOST_FLWPKT_DETECT and make sure use the function with
 *   http-block reject patch.
 *
 * - All the patten match are case insensitive.
 *
 * Changelist:
 *
 * - 2009/12/04, fog : reformat and add usage
 *
 */
#include <linux/version.h>
#include <linux/smp.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ctype.h>
#include <linux/file.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <net/ip.h>
#include "ipt_http_string.h"

#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif

//#define HTTP_STRING_DEBUG

#define HOST_FLWPKT_DETECT

#define smp_num_cpus			   1

#define IPT_STRING_HAYSTACK_THRESH 100
#define IPT_STRING_NEEDLE_THRESH   20

#define BM_MAX_NLEN                256
#define BM_MAX_HLEN                1024

#define HTTP_FIRST_LINE_MAX_LEN		1024

#define REQUEST_HEADER_HOST       "HOST: "

#define HTTP_HDR_COOKIE_PREFIX    "Cookie"
#define HTTP_HDR_SETCOOKIE_PREFIX "Set-Cookie"
#define HTTP_HDR_PROXY_PREFIX     "Proxy-Connection"

struct string_per_cpu {
	int *skip;
	int *shift;
	int *len;
};

typedef char *(*proc_ipt_search) (char *, char *, int, int);

static char *http_method_str[] = {
    "GET",
    "HEAD",
    "POST",
    "GROUP",
    "LISTGROUP",
    NULL
};

static int http_method_str_len[]={3/*GET*/,4/*HEAD*/,4/*POST*/,5/*GROUP*/,9/*LISTGROUP*/,0};

static char *http_java_exts[] = {
    ".CLASS",
    ".JAR",
    NULL
};

static char *http_activex_exts[] = {
    ".OCX",
    ".CAB",
    NULL
};

static struct string_per_cpu *bm_http_string_data = NULL;

#ifdef HTTP_STRING_DEBUG

#define ASCII_LINELENGTH           300
#define HEXDUMP_BYTES_PER_LINE     16
#define HEXDUMP_SHORTS_PER_LINE    (HEXDUMP_BYTES_PER_LINE / 2)
#define HEXDUMP_HEXSTUFF_PER_SHORT 5 /* 4 hex digits and a space */
#define HEXDUMP_HEXSTUFF_PER_LINE               \
		(HEXDUMP_HEXSTUFF_PER_SHORT * HEXDUMP_SHORTS_PER_LINE)

void
hex_and_ascii_print_with_offset(register const char *ident,
    register const u_char *cp, register u_int length, register u_int oset)
{
	register u_int i;
	register int s1, s2;
	register int nshorts;
	char hexstuff[HEXDUMP_SHORTS_PER_LINE*HEXDUMP_HEXSTUFF_PER_SHORT+1], *hsp;
	char asciistuff[ASCII_LINELENGTH+1], *asp;

	nshorts = length / sizeof(u_short);
	i = 0;
	hsp = hexstuff; asp = asciistuff;
	while (--nshorts >= 0) {
		s1 = *cp++;
		s2 = *cp++;
		(void)snprintf(hsp, sizeof(hexstuff) - (hsp - hexstuff),
		    " %02x%02x", s1, s2);
		hsp += HEXDUMP_HEXSTUFF_PER_SHORT;
		*(asp++) = (isgraph(s1) ? s1 : '.');
		*(asp++) = (isgraph(s2) ? s2 : '.');
		i++;
		if (i >= HEXDUMP_SHORTS_PER_LINE) {
			*hsp = *asp = '\0';
			(void)printk("%s0x%04x: %-*s  %s",
			    ident, oset, HEXDUMP_HEXSTUFF_PER_LINE,
			    hexstuff, asciistuff);
			i = 0; hsp = hexstuff; asp = asciistuff;
			oset += HEXDUMP_BYTES_PER_LINE;
		}
	}
	if (length & 1) {
		s1 = *cp++;
		(void)snprintf(hsp, sizeof(hexstuff) - (hsp - hexstuff),
		    " %02x", s1);
		hsp += 3;
		*(asp++) = (isgraph(s1) ? s1 : '.');
		++i;
	}
	if (i > 0) {
		*hsp = *asp = '\0';
		(void)printk("%s0x%04x: %-*s  %s",
		     ident, oset, HEXDUMP_HEXSTUFF_PER_LINE,
		     hexstuff, asciistuff);
	}

    printk("\n");
}

void print_packet(unsigned char *data, int len)
{
    hex_and_ascii_print_with_offset("\n", data, len, 8);
}

#endif

/**
 * This function first locate "tag" in "haystack", then match "needle"
 * from the character after "tag" until reach http header field's end
 * indicator('\r\n').
 *
 * The compare is case insensitive.
 *
 * @param tag
 * @param needle
 * @param haystack
 * @param needle_len
 * @param haystack_len
 *
 * @return
 * 0  match (done)
 * 1  find HOST but not match, and the http header parser not finish (next)
 * -1 not find HOST, and the http header parser not finish (record/break)
 * 2  not find HOST but http header reach the end (break)
 */
static int search_http_header_field (char *tag, char *needle, char *haystack,
                                     int needle_len, int haystack_len)
{
	char *k = haystack + (haystack_len-needle_len);
	char *t = haystack;

	while ( t < k )
    {
        /* check if we have reached the end of the http header */
		if(*t == '\r' && *(t+1) == '\n' && *(t+2) == '\r' && *(t+3) == '\n')
			return 2;

		if(strnicmp(t, tag, strlen(tag)) == 0)
        {
			t = t + strlen(tag);
			for(;(t < k) && (*t != '\r');t++)
            {
                if ( strnicmp(t, needle, needle_len) == 0 )
                    return 0;
            }

			return 1;
		}

        t++;
	}

	return -1;
}

/**
 * Search a needle in http header at each field's beginning, and if
 * need, do word swap to destroy the field.
 *
 * The compare is case insensitive.
 *
 * @param needle
 * @param disorder should we disorder the needle matched in haystack
 * @param loop should we loop search in the header for all matched needles
 * @param haystack stream from http header
 * @param haystack_len
 *
 * @return 0 is match
 */
static int parse_http_header_filed(char *needle, int needle_len,
                                   char *haystack, int haystack_len,
                                   int   disorder, int loop)
{
	char *k = haystack + (haystack_len-needle_len);
	char *t = haystack;

	while ( t < k )
    {
        /* check if we have reached the end of the http header */
		if(*t == '\r' && *(t+1) == '\n' && *(t+2) == '\r' && *(t+3) == '\n')
			return 2;

        for(;(*t!='\r') && (t < k);t++);

        if(t == k) return -1;

        t += 2; /* skip \r\n */

        if ( strnicmp(t, needle, needle_len) == 0 )
        {
            if(disorder)
            {
                char tmpc1, tmpc2;

                /* find an even address so that checksum not
                 * changed when we do word swap
                 */
                if((unsigned long)t&1) t++;

                /* do word swap */
                tmpc1 = *t; tmpc2 = *(t+1);
                *t = *(t+2); *(t+1) = *(t+3);
                *(t+2) = tmpc1; *(t+3) = tmpc2;
            }

            if(!loop) return 0;
        }

        t++;
	}

	return -1;
}

/**
 * Boyer Moore Sublinear string search, it is better to use when
 * needle is long, more longer more faster.
 * The compare is case sensitive.
 *
 * @param needle
 * @param haystack
 * @param needle_len
 * @param haystack_len
 *
 * @return
 */
static char *search_sublinear (char *needle, char *haystack,
                               int needle_len, int haystack_len)
{
	int M1, right_end, sk, sh;
	int ended, j, i;

	int *skip, *shift, *len;

	/* use data suitable for this CPU */
	shift=bm_http_string_data[smp_processor_id()].shift;
	skip=bm_http_string_data[smp_processor_id()].skip;
	len=bm_http_string_data[smp_processor_id()].len;

	/* setup skip/shift tables */
	M1 = right_end = needle_len-1;
	for (i = 0; i < BM_MAX_HLEN; i++) skip[i] = needle_len;
	for (i = 0; needle[i]; i++) skip[(int)needle[i]] = M1 - i;

	for (i = 1; i < needle_len; i++)
    {
		for (j = 0; j < needle_len && needle[M1 - j] == needle[M1 - i - j]; j++);
		len[i] = j;
	}

	shift[0] = 1;
	for (i = 1; i < needle_len; i++) shift[i] = needle_len;
	for (i = M1; i > 0; i--) shift[len[i]] = i;
	ended = 0;

	for (i = 0; i < needle_len; i++)
    {
		if (len[i] == M1 - i) ended = i;
		if (ended) shift[i] = ended;
	}

	/* do the search */
	while (right_end < haystack_len)
    {
#if 0
		for (i = 0;
             i < needle_len && haystack[right_end - i] == needle[M1 - i];
             i++);
#else
        for (i = 0;
             i < needle_len &&
                 tolower(haystack[right_end - i]) == tolower(needle[M1 - i]);
             i++);
#endif
		if (i == needle_len)
        {
			return haystack+(right_end - M1);
		}

		sk = skip[(int)haystack[right_end - i]];
		sh = shift[i];
		right_end = max(right_end - i + sk, right_end + sh);
	}

	return NULL;
}

/**
 * Simple linear string search, used when needle length is short.
 * The compare is case insensitive.
 *
 * @param needle
 * @param haystack
 * @param needle_len
 * @param haystack_len
 *
 * @return
 */
static char *search_linear (char *needle, char *haystack,
                            int needle_len, int haystack_len)
{
	char *k = haystack + (haystack_len-needle_len);
	char *t = haystack;

	while ( t <= k )
    {
#if 0
		if ( memcmp(t, needle, needle_len) == 0 ) return t;
#else
		if ( strnicmp(t, needle, needle_len) == 0 ) return t;

#endif
        t++;
	}

	return NULL;
}

#ifdef HOST_FLWPKT_DETECT
#include <linux/random.h>
#include <linux/jhash.h>

#define HS_HASH_SHIFT		6
#define HS_HASH_ENTRIES	    (1 << HS_HASH_SHIFT)
#define HS_HASH_FN(p)	    (jhash_3words((u32)(*(u32 *)(p)),           \
                                          (u32)(*(u32 *)((p)+4)),       \
                                          (u32)(*(u16 *)((p)+8)),       \
                                          hash_host_session_rnd) % HS_HASH_ENTRIES)

#define L3_SELECTOR_LEN     10 /* source ip(4) + dest ip(4) + source port(2) */

struct hsession {
    struct hlist_node list;
    char selector[L3_SELECTOR_LEN];
};

static int hash_host_session_rnd;
static struct hlist_head hash_hslist[HS_HASH_ENTRIES];

static struct hsession *host_session_add(char *ip_src)
{
    struct hsession *hs;
    int hash_idx;

    hs = kmalloc(sizeof(struct hsession), GFP_KERNEL);
    if(hs == NULL) return NULL;

    memcpy(hs->selector, ip_src, sizeof(hs->selector));
    hash_idx = HS_HASH_FN(ip_src);
    INIT_HLIST_NODE(&hs->list);
    hlist_add_head(&hs->list, &hash_hslist[hash_idx]);

    return hs;
}

static void host_session_del(struct hsession *hs)
{
    hlist_del(&hs->list);
    kfree(hs);
}

static struct hsession *host_session_search(char *ip_src)
{
    struct hlist_node *hn;
    int hash_idx;

    hash_idx = HS_HASH_FN(ip_src);
    hlist_for_each(hn, &hash_hslist[hash_idx])
    {
        struct hsession *hs = hlist_entry(hn, struct hsession, list);
        if(!memcmp(hs->selector, ip_src, sizeof(hs->selector)))
            return hs;
    }

    return NULL;
}
#endif

static int consider_not_http_packet(char *haystack, int haystack_len)
{
	int i;
	char *tmp;
	
	tmp = haystack;

	if(haystack_len <= HTTP_FIRST_LINE_MAX_LEN)
		return -1;
	
	for(i=0;(*tmp!='\r')&&(i<haystack_len);i++,tmp++);
	
	if(i > (HTTP_FIRST_LINE_MAX_LEN-1))
		return 1;
	else
		return -1;
}

//static int match(const struct sk_buff *skb,
//                 const struct net_device *in,
//                 const struct net_device *out,
//                 const struct xt_match *match,
//                 const void *matchinfo,
//                 int offset,
//                 unsigned int protoff,
//                 int *hotdrop)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
static bool
match(const struct sk_buff *skb,
		      struct xt_action_param *par)
#else
static bool
match(const struct sk_buff *skb,
		      struct xt_match_param *par)
#endif
{
	const struct ipt_http_string_info *info = par->matchinfo;
	struct iphdr *iph = ip_hdr(skb);
    struct tcphdr *tcph = (struct tcphdr *)(skb_network_header(skb) + ip_hdrlen(skb));
	int hlen;
	char *haystack;
	int find = 0;
	int i;
	proc_ipt_search search = search_linear;
    struct ipt_http_pattern *iptpat;
    char *ip_src, *uri = NULL;
    int rc = 0;
    char hostname[128]="";
    unsigned long srcAddr = 0;
    char *tmp;

#ifdef HTTP_STRING_DEBUG
//	printk("call http_string match,info->trust = %d\n", info->trust);
#endif

	if (!iph || !tcph) return 0;

	/* skip ip header and tcp header, get lenghts, and validate them */
    hlen = ntohs(iph->tot_len) - (iph->ihl*4);
    haystack = (char *)iph + (iph->ihl*4);

    tcph = (struct tcphdr *)haystack;

    if((hlen < sizeof(*tcph)) || (hlen <= (tcph->doff << 2)))
    {
        return 0;
	}
	
    ip_src = (char *)tcph - 8;
	hlen -= (tcph->doff << 2);
	haystack += (tcph->doff << 2);

#ifdef HTTP_STRING_DEBUG
	printk("hlen = %d, haystack = 0x%x\n", hlen, (unsigned int)haystack);
	print_packet(haystack, hlen);
#endif

	/* Rev10 : If the first line in HTTP packet is more than 1024 characters,
	 * we won't treat it as HTTP session. */
	if(consider_not_http_packet(haystack, hlen) == 1){
#ifdef HTTP_STRING_DEBUG
		printk("[HTTP block]First line is more than 1024 characters!\n");
#endif		
		return 0;
	}

	/*
     * TODO: remove the port check and use ip_conntrack table to
     * indicate a reply packet
     */
    if((ntohs(tcph->source) == 80) && (info->flags & IPT_HTTP_FLAG_COOKIE))
    {
        /* We should do cookie filter in http response packet */
        parse_http_header_filed(HTTP_HDR_SETCOOKIE_PREFIX,
                                strlen(HTTP_HDR_SETCOOKIE_PREFIX),
                                haystack, hlen,
                                1, 1);

#ifdef HTTP_STRING_DEBUG
        printk("HTTP header match - (%s)\n", HTTP_HDR_SETCOOKIE_PREFIX);

#endif
        goto find_match;
    }

	i = -1;
	
	while(http_method_str[++i] != NULL)
    {
        if(memcmp(haystack, http_method_str[i],
             http_method_str_len[i]) ==0)
        {
    #ifdef HTTP_STRING_DEBUG
			printk("http method is [%s]\n",http_method_str[i]);
    #endif
            uri = haystack + http_method_str_len[i] + 1;
			break;
		}
#if 0 /* no define ALL_TCP_CHECK */
		if((uri = search_linear(http_method_str[i],
                                haystack,
                                http_method_str_len[i],
                                hlen)) != NULL)
        {
    #ifdef HTTP_STRING_DEBUG
			printk("http method is [%s]\n",http_method_str[i]);
    #endif
            uri += http_method_str_len[i] + 1;
			break;
		}
#endif
	}

	/* if not find http method strings, return */
	if(uri == NULL){
		return 0;
	}

	/* Netgear SPEC define:
	 *   For other types of Request-URI (i.e. * or authority), an implementation
	 *   just lets the packet pass through the router and forward it as normal.
	 * So Let all request URI: / * pass.
	 */
	if (strncmp(uri, "/* ", 3) == 0) {
	    printk("URI is \"/*\", let it pass!\n");
	    return 0;
	}

	/* if HOST found, save it */
    if((tmp=strstr(haystack, "Host:")))
    {
    	int i = 0;

		/* skip the item name */
		tmp += strlen("Host:");
    	while(*(tmp+i)) {
    		if(*(tmp+i) == '\r' && *(tmp+i+1) == '\n')
    		{
    			strncpy(hostname, tmp, i);
    			hostname[i] = '\0';
    			break;
    		}
    		i++;
    	}
    	memcpy(&srcAddr, ip_src, 4);
    }

    iptpat = (struct ipt_http_pattern *)&info->iptpat[0];

    if(uri != NULL)
    {
        int urilen = 0, len;
#ifdef CONFIG_ARCH_MT7623
	/* Bypass MT7623 gcc stack size warning */
        char *ori_uri = (char *)kmalloc(1500, GFP_KERNEL);
	if (!ori_uri)
		return 0;
#else
        char ori_uri[1500]="";
#endif

        while((urilen < hlen) && (uri[urilen++] != 0x20));

		/* Netgear SPEC. construct original request-URI to match the keyword */
       	len = sprintf(ori_uri, "%s", hostname);
        strncpy(ori_uri+len, uri, urilen);
        urilen += len;
        ori_uri[urilen-1] = '\0';

        if(*http_method_str[i] == 'G')
        {
            if(info->flags & IPT_HTTP_FLAG_JAVA)
            {
                i = -1;
                while(http_java_exts[++i] != NULL)
                {
                    if(search_linear((char *)http_java_exts[i],
                                     uri,
                                     strlen(http_java_exts[i]),
                                     urilen) != NULL)
                    {
                        find = 1;
#ifdef HTTP_STRING_DEBUG
                        printk("JAVA(%s) is match\n", http_java_exts[i]);
#endif
                        goto find_match;
                    }
                }
            }

            if(info->flags & IPT_HTTP_FLAG_ACTIVEX)
            {
                i = -1;
                while(http_java_exts[++i] != NULL)
                {
                    if(search_linear((char *)http_activex_exts[i],
                                     uri,
                                     strlen(http_activex_exts[i]),
                                     urilen) != NULL)
                    {
                        find = 1;
#ifdef HTTP_STRING_DEBUG
                        printk("ACTIVEX(%s) is match\n", http_activex_exts[i]);
#endif
                        goto find_match;
                    }
                }
            }
        }

        /* We hit the packet which include URI, filter with the URI
         * patterns
         */
        for(;
            iptpat && iptpat->type == IPT_PATTERN_TYPE_URI;
            iptpat++)
        {
            if(iptpat->patlen > urilen)
                continue;

            if(search_linear((char *)iptpat->pattern,
                             ori_uri,
                             iptpat->patlen,
                             urilen) != NULL)
            {
                find = 1;
                if(hostname[0] != '\0' && !(info->flags&IPT_HTTP_FLAG_TRUST) ) {/*not trust ip*/
                	NETGEAR_SYSLOG("14 02[site blocked:%s] from source %u.%u.%u.%u\n", hostname, NIPQUAD(srcAddr));
                }
                else {
			if(hostname[0] != '\0') {
                		NETGEAR_SYSLOG("14 01[site allowed:%s] from source %u.%u.%u.%u\n", hostname, NIPQUAD(srcAddr));
			}
                }
#ifdef HTTP_STRING_DEBUG
                printk("URI(%s) is match,flag = %x\n", iptpat->pattern, info->flags);
#endif
                goto find_match;
            }
        }
#ifdef CONFIG_ARCH_MT7623
	if (ori_uri) {
		kfree(ori_uri);
		ori_uri = NULL;
	}
#endif

#ifdef HTTP_STRING_DEBUG
        printk("URI is not match,flag = %x\n", info->flags);
#endif

        /* Do cookie and proxy filter */
        if(info->flags & IPT_HTTP_FLAG_COOKIE)
        {
            if(parse_http_header_filed(HTTP_HDR_COOKIE_PREFIX,
                                       strlen(HTTP_HDR_COOKIE_PREFIX),
                                       haystack, hlen,
                                       1, 0) == 0)
            {
#ifdef HTTP_STRING_DEBUG
                printk("HTTP header match - (%s)\n", HTTP_HDR_COOKIE_PREFIX);
#endif
                goto find_match;
            }
        }

        if(info->flags & IPT_HTTP_FLAG_PROXY)
        {
            if(parse_http_header_filed(HTTP_HDR_PROXY_PREFIX,
                                       strlen(HTTP_HDR_PROXY_PREFIX),
                                       haystack, hlen,
                                       0, 0) == 0)
            {
                find = 1;
#ifdef HTTP_STRING_DEBUG
                printk("HTTP header match - (%s)\n", HTTP_HDR_PROXY_PREFIX);
#endif
                goto find_match;
            }
        }

        /* Try to find the HOST field in header, if fail then record
         * the session for later packets
         */
        for(;
            iptpat && iptpat->type == IPT_PATTERN_TYPE_HOST;
            iptpat++)
        {

            int loop_stop = 0;

            if(iptpat->patlen > hlen)
                continue;

            rc = search_http_header_field(REQUEST_HEADER_HOST,
                                          (char *)iptpat->pattern,
                                          haystack,
                                          iptpat->patlen,
                                          hlen);
            switch(rc)
            {
                case 0:
                    find = 1;
#ifdef HTTP_STRING_DEBUG
                    printk("HOST(%s) is match\n", iptpat->pattern);
#endif
                    goto find_match;
                case -1:
#ifdef HOST_FLWPKT_DETECT
#ifdef HTTP_STRING_DEBUG
                    printk("HOST field not found, add in hash table\n");
#endif
                    host_session_add(ip_src);
#endif
                case 2:
                    loop_stop = 1;
                    break;

            }

            if(loop_stop) break;
        }
    }

#ifdef HOST_FLWPKT_DETECT
    else
    {
        struct hsession *hs;

        while(iptpat && iptpat->type == IPT_PATTERN_TYPE_URI)
            iptpat++;

        /*
         * Now, do HOST filter with all the recorded sessions
         */
        if((hs = host_session_search(ip_src)) != NULL)
        {
            for(;
                iptpat && iptpat->type == IPT_PATTERN_TYPE_HOST;
                iptpat++)
            {

                if(iptpat->patlen > hlen)
                    continue;

                rc = search_http_header_field(REQUEST_HEADER_HOST,
                                              (char *)iptpat->pattern,
                                              haystack,
                                              iptpat->patlen,
                                              hlen);
                if(rc == 0)
                {
#ifdef HTTP_STRING_DEBUG
                    printk("HOST(%s) is match in following packet\n",
                           iptpat->pattern);
#endif
                    find = 1;
                    /*
                     * Delete the session here is not good enough
                     * because it limit the http_string module need
                     * work with http-block reject patch.
                     *
                     * After the session is deleted, we can't handle
                     * the resend packets.
                     */
                    host_session_del(hs);
                    goto find_match;

                }
                else if(rc == 2)
                {
                    break;
                }
            }

            /*
             * In following status we should delete the monitor session:
             *   - HOST found but not match
             *   - HOST not found but http header end
             */
            if((rc == 1)||(rc == 2))
            {
                host_session_del(hs);
            }

            /* TODO: use ref count to prevent from HOST packet loss?
             */
        }
    }
#endif
	if(strcmp(hostname, "") != 0) {
	   	NETGEAR_SYSLOG("14 01[site allowed:%s] from source %u.%u.%u.%u\n", hostname, NIPQUAD(srcAddr));
        }

#ifdef HTTP_STRING_DEBUG
    printk("HOST is not match\n");
#endif

    while(iptpat && iptpat->type == IPT_PATTERN_TYPE_HOST)
        iptpat++;

    for(;
        iptpat && iptpat->type == IPT_PATTERN_TYPE_CONTENT;
        iptpat++)
    {
		if(iptpat->patlen != 0)
        {
            /*
             * The sublinear search comes in to its own on the larger
             * packets
             */
			if ( (hlen > IPT_STRING_HAYSTACK_THRESH) &&
                 (iptpat->patlen > IPT_STRING_NEEDLE_THRESH) )
            {
                if ( hlen < BM_MAX_HLEN )
                {
                    search = search_sublinear;
                }
                else
                {
                    if (net_ratelimit());
				}
			}

			if(iptpat->patlen > hlen)
				continue;

			if(search((char *)iptpat->pattern, haystack,
                      iptpat->patlen, hlen) != NULL)
            {
				find = 1;
#ifdef HTTP_STRING_DEBUG
                printk("CONTENT(%s) is match\n", iptpat->pattern);
#endif
				goto find_match;
			}
		}
	}

  find_match :
    return (find ^ info->invert);
}

static int checkentry(const struct xt_mtchk_param *par)
{
    if (par->match->matchsize != XT_ALIGN(sizeof(struct ipt_http_string_info)))
        return 1;

    return 0;
}

static void string_freeup_data(void)
{
	int c;

	if ( bm_http_string_data )
    {
		for(c = 0; c < smp_num_cpus; c++)
        {
			if ( bm_http_string_data[c].shift ) kfree(bm_http_string_data[c].shift);
			if ( bm_http_string_data[c].skip ) kfree(bm_http_string_data[c].skip);
			if ( bm_http_string_data[c].len ) kfree(bm_http_string_data[c].len);
		}
		kfree(bm_http_string_data);
	}
}

static struct xt_match http_string_match = {
    .name = "http_string",
    .match = &match,
    .checkentry = &checkentry,
    .destroy = NULL,
    .me = THIS_MODULE,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
	.family         = AF_INET,
	.matchsize      = sizeof(struct ipt_http_string_info),
#endif
};

static int __init init(void)
{
	int c;
	size_t tlen;
	size_t alen;

	tlen = sizeof(struct string_per_cpu)*smp_num_cpus;
	alen = sizeof(int)*BM_MAX_HLEN;

	/* allocate array of structures */
	if ( !(bm_http_string_data = kmalloc(tlen,GFP_KERNEL)) )
    {
		return 0;
	}

	memset(bm_http_string_data, 0, tlen);

	/* allocate our skip/shift tables */
	for(c = 0; c < smp_num_cpus; c++)
    {
		if ( !(bm_http_string_data[c].shift = kmalloc(alen, GFP_KERNEL)) )
			goto alloc_fail;
		if ( !(bm_http_string_data[c].skip = kmalloc(alen, GFP_KERNEL)) )
			goto alloc_fail;
		if ( !(bm_http_string_data[c].len = kmalloc(alen, GFP_KERNEL)) )
			goto alloc_fail;
	}

#ifdef HOST_FLWPKT_DETECT
    get_random_bytes(&hash_host_session_rnd, 4);
    /* initialize host session hash list */
    for(c = 0; c < HS_HASH_ENTRIES; c++)
    {
        INIT_HLIST_HEAD(&hash_hslist[c]);
    }
#endif
	return xt_register_match(&http_string_match);

  alloc_fail:
	string_freeup_data();
	return 0;
}

static void __exit fini(void)
{
	xt_unregister_match(&http_string_match);
	string_freeup_data();
}

module_init(init);
module_exit(fini);

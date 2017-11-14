/*
 * This is a module which is used for rejecting packets.
 * Added support for customized reject packets (Jozsef Kadlecsik).
 * Added support for ICMP type-3-code-13 (Maciej Soltysiak). [RFC 1812]
 */

/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/fs.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/if_ether.h>
#include <linux/netdevice.h>
#include <net/icmp.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <net/route.h>
#include <net/dst.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4.h>
#include "ipt_REJECT.h"
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/file.h>
#include <linux/version.h>
#ifdef CONFIG_BRIDGE_NETFILTER
#include <linux/netfilter_bridge.h>
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
#include <net/arp.h>
/* linux 3.10.20 does not define RTO_CONN. define it here for compatibility */
#ifndef RTO_CONN
#define RTO_CONN	0
#endif
#ifndef NF_IP_FORWARD
#define	NF_IP_FORWARD		NF_INET_FORWARD
#endif
#ifndef NF_IP_LOCAL_IN
#define	NF_IP_LOCAL_IN		NF_INET_LOCAL_IN
#endif
#ifndef NF_IP_LOCAL_OUT
#define	NF_IP_LOCAL_OUT		NF_INET_LOCAL_OUT
#endif
#ifndef NF_IP_PRE_ROUTING
#define	NF_IP_PRE_ROUTING	NF_INET_PRE_ROUTING
#endif
#ifndef NF_IP_POST_ROUTING
#define	NF_IP_POST_ROUTING	NF_INET_POST_ROUTING
#endif
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Netfilter Core Team <coreteam@netfilter.org>");
MODULE_DESCRIPTION("iptables REJECT target module");

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif
#define BLKPAGE_PATH "/tmp/etc/block.htm"

#define BLKPAGE "HTTP/1.1 503 Service Unavailable\r\nConnection: Close\r\nConten-Type:text/html\r\nContent-Length: %d\r\n\r\n"

#ifndef SEND_SYNACK
#define SEND_SYNACK 0
#endif

#ifndef SEND_BLOCK
#define SEND_BLOCK 1
#endif

#define NNTP_NO_NEWSGROUP "411 no such newsgroup\r\n"

#define BLKPAGE_DEF "HTTP/1.0 503 Service Unavailable\r\nConnection: Close\r\nConten-Type:text/html\r\n\r\n<html><head><meta http-equiv=\"content-type\" content=\"text/html;charset=ISO-8859-1\"><title>Web Site Blocked</title><style type=\"text/css\">A{text-decoration:none}</style><body bgcolor=black text=white><br><br><br><table border=0 width=100%><tr height=25><td bgcolor=red></td></tr><tr><td><br><center><H2>Web Site Blocked By NETGEAR Firewall</H2><br></td></tr><tr height=25><td bgcolor=red></td></tr></table><p align=right><a href=http://www.netgear.com><font color=white size=5><b>NETGEAR</head></html>"
#define MACBLK_DEF "<html><head><meta http-equiv=\"content-type\" content=\"text/html;charset=ISO-8859-1\"><title>Web Site Blocked</title><style type=\"text/css\">A{text-decoration:none}</style><body bgcolor=black text=white><br><br><br><table border=0 width=100%><tr height=25><td bgcolor=red></td></tr><tr><td><br><center><H2>This device is blocked by Access Control in the router</H2><br></td></tr><tr height=25><td bgcolor=red></td></tr></table><p align=right><a href=http://www.netgear.com><font color=white size=5><b>NETGEAR</head></html>"

#if 0
#define HH_DATA_MOD     16
#define HH_DATA_OFF(__len) \
        (HH_DATA_MOD - ((__len) & (HH_DATA_MOD - 1)))
#endif
#define HH_DATA_ALIGN(__len) \
	        (((__len)+(HH_DATA_MOD-1))&~(HH_DATA_MOD - 1))

static char blockpage[65535];
static char macblkpage[65535];
static int http_blen=0;
static int macblock_blen;
extern long sys_open(const char __user *filename, int flags, int mode);
extern long sys_close(unsigned int fd);
extern ssize_t sys_read(unsigned int fd, char __user * buf, size_t count);

#if LINUX_VERSION_CODE <= KERNEL_VERSION(3,2,0)
/* Deprecated: use ip_route_output_key directly */
static inline int ip_route_output(struct rtable **rp,u32 daddr, u32 saddr, u32 tos, int oif)
{
	struct flowi fl = { .nl_u = { .ip4_u =
				      { .daddr = daddr,
					.saddr = saddr,
					.tos = tos } } };

        return ip_route_output_key(&init_net, rp, &fl);
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
static inline int ip_direct_send(struct sk_buff *skb)
{
	struct dst_entry *dst = skb_dst(skb);
	struct rtable *rt = (struct rtable *)dst;
	struct net_device *dev = dst->dev;
	//unsigned int hh_len = LL_RESERVED_SPACE(dev);
	struct neighbour *neigh;
	u32 nexthop;

	rcu_read_lock_bh();
	nexthop = (__force u32) rt_nexthop(rt, ip_hdr(skb)->daddr);
	neigh = __ipv4_neigh_lookup_noref(dev, nexthop);
	if (unlikely(!neigh))
		neigh = __neigh_create(&arp_tbl, &nexthop, dev, false);
	if (!IS_ERR(neigh)) {
		int res = dst_neigh_output(dst, neigh, skb);

		rcu_read_unlock_bh();
		return res;
	}
	rcu_read_unlock_bh();

	kfree_skb(skb);
	return -EINVAL;
}
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
/* Stolen from ip_finish_output2 */
static void ip_direct_send(struct sk_buff *skb)
{
	struct dst_entry *dst = skb_dst(skb);
	struct neighbour *neigh = NULL;
	
	rcu_read_lock();
	neigh = dst_get_neighbour_noref(dst);
	if (neigh) {
		neigh_output(neigh, skb);

		rcu_read_unlock();
		return ;
	}
	rcu_read_unlock();

	//printk(KERN_DEBUG "khm in MIRROR\n");
	kfree_skb(skb);
}
#else
/* Stolen from ip_finish_output2 */
static void ip_direct_send(struct sk_buff *skb)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
	struct dst_entry *dst = skb_dst(skb);
#else
	struct dst_entry *dst = skb->dst;
#endif
	struct hh_cache *hh = dst->hh;

	if (hh) {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
/* copy from ipt_ROUTE.c */

		unsigned seq;

                do {
                        int hh_alen;
 
                        seq = read_seqbegin(&hh->hh_lock);
                        hh_alen = HH_DATA_ALIGN(hh->hh_len);
                        memcpy(skb->data - hh_alen, hh->hh_data, hh_alen);
                } while (read_seqretry(&hh->hh_lock, seq));

#else
		int hh_alen;

		read_lock_bh(&hh->hh_lock);
		hh_alen = HH_DATA_ALIGN(hh->hh_len);
  		memcpy(skb->data - hh_alen, hh->hh_data, hh_alen);
		read_unlock_bh(&hh->hh_lock);
#endif

	        skb_push(skb, hh->hh_len);
		hh->hh_output(skb);
	} else if (dst->neighbour)
		dst->neighbour->output(skb);
	else {
		//printk(KERN_DEBUG "khm in MIRROR\n");
		kfree_skb(skb);
	}
}
#endif


#if 1
unsigned int kread_file(char *fname,unsigned int imageLength,char *ptr)
{
	unsigned int bytesRead;
	int                     fd;
	mm_segment_t            fs = get_fs();
        set_fs(get_ds());

	fd = sys_open(fname, 0, 0);

	if(fd == -1)
	{
		printk("Failed: Could not open web file\n");
        	return -1;
	}
     	bytesRead = sys_read(fd, ptr,imageLength);

	sys_close(fd);
        set_fs(fs);

  	return bytesRead;
}
#endif
/* Send BLOCK reply */
static void send_block(struct sk_buff *oldskb, int local, int flag)
{
	struct sk_buff *nskb, *nntp_skb;
    struct iphdr *iph;
    struct iphdr *newiph;
	struct tcphdr *otcph, *tcph;
	struct rtable *rt;
	unsigned int otcplen;
	u_int16_t tmp;
	char *http_data=NULL;
	//char blockpage[1024]=BLKPAGE;
	int diff=0;
	int blen=0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
struct sock *sk;
#endif
    iph =  ip_hdr(oldskb);
	/* IP header checks: fragment, too short. */
	if (iph->frag_off & htons(IP_OFFSET)
	    || oldskb->len < (iph->ihl<<2) + sizeof(struct tcphdr))
		return;

	otcph = (struct tcphdr *)((u_int32_t*)iph + iph->ihl);
	otcplen = oldskb->len - iph->ihl*4;


	/* Check checksum. */
	if (tcp_v4_check(otcplen, iph->saddr,
			 iph->daddr,
			 csum_partial((char *)otcph, otcplen, 0)) != 0)
		return;
/*
	blen=kread_file(BLKPAGE_PATH,1024,blockpage+strlen(BLKPAGE));

	if(blen<0){
                strcpy(blockpage,BLKPAGE_DEF);
        }//brayg

	blen=strlen(blockpage);
*/
	/* Copy skb (even if skb is about to be dropped, we can't just
           clone it because there may be other things, such as tcpdump,
           interested in it) */
	diff=http_blen-skb_tailroom(oldskb)+1;

	if(diff > 0 )
		nskb = skb_copy_expand(oldskb,skb_headroom(oldskb), http_blen + 1, GFP_ATOMIC);
	else
		nskb = skb_copy(oldskb, GFP_ATOMIC);


	if (!nskb)
		return;

	/* This packet will not be the same as the other: clear nf fields */
	nf_conntrack_put(nskb->nfct);
	nskb->nfct = NULL;
	//nskb->nfcache = 0;
#ifdef CONFIG_NETFILTER_DEBUG
	nskb->nf_debug = 0;
#endif

    newiph = ip_hdr(nskb);
	tcph = (struct tcphdr *)((u_int32_t*)newiph + newiph->ihl);


	/* Swap source and dest */
	//newiph->daddr = xchg(&newiph->saddr, newiph->daddr);
	newiph->daddr = iph->saddr;
	newiph->saddr = iph->daddr;
	tmp = tcph->source;
	tcph->source = tcph->dest;
	tcph->dest = tmp;

		/* Truncate to length (no data) */
	tcph->doff = sizeof(struct tcphdr)/4;
	skb_trim(nskb, newiph->ihl*4 + sizeof(struct tcphdr));
    DEBUGP("<0>[%s():%d] skb->len is <%d> header len is <%d>\n", __FUNCTION__, __LINE__, nskb->len, newiph->ihl*4 + sizeof(struct tcphdr));

    if(flag == IPT_NNTP_SITE_BLOCK)
    {    
        blen = strlen(NNTP_NO_NEWSGROUP);
    	http_data=skb_put(nskb, blen);
        DEBUGP("<0>[%s():%d] skb->len is <%d> \n", __FUNCTION__, __LINE__, nskb->len);
        strcpy(http_data, NNTP_NO_NEWSGROUP);
    }
    else if(flag == IPT_HTTP_SITE_BLOCK)
    {    
        blen = http_blen;
    	http_data=skb_put(nskb, blen);
        DEBUGP("<0>[%s():%d] skb->len is <%d> \n", __FUNCTION__, __LINE__, nskb->len);
	    strcpy(http_data, blockpage);
	}


	newiph->tot_len = htons(nskb->len);

	/* Ack No. & Seq No. */

	tcph->seq = otcph->ack_seq;
	tcph->ack_seq = htonl(ntohl(otcph->seq) + otcph->syn + otcph->fin
				      + otcplen - (otcph->doff<<2));


	/* Reset flags */
	((u_int8_t *)tcph)[13] = 0;
    if(flag == IPT_NNTP_SITE_BLOCK)
    {
    	tcph->fin = 0;
    	tcph->psh = 0;
    	tcph->ack = 1;
    	tcph->rst = 1;
    	tcph->seq += htonl(blen+1);
    }    
	else
	{    
    	tcph->fin = 1;
    	tcph->psh = 1;
    	tcph->ack = 1;
    	tcph->rst = 0;
    }

	tcph->window = 0;
	tcph->urg_ptr = 0;

	/* Adjust TCP checksum */
	tcph->check = 0;

	nskb->csum = csum_partial((char *)tcph + tcph->doff*4,
			blen, 0);

        tcph->check = tcp_v4_check(sizeof(struct tcphdr) + blen,
        			   newiph->saddr,
        			   newiph->daddr,
                                   csum_partial((char *)tcph,
					   sizeof(struct tcphdr),nskb->csum));
	/* Adjust IP TTL, DF */
	newiph->ttl = MAXTTL;
	/* Set DF, id = 0 */
	newiph->frag_off = htons(IP_DF);
	newiph->id = 0;

	/* Adjust IP checksum */
	newiph->check = 0;
	newiph->check = ip_fast_csum((unsigned char *)newiph,
					   newiph->ihl);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
	sk  = nskb->sk;
	/* Routing: if not headed for us, route won't like source */
	rt = ip_route_output(sock_net(sk), newiph->daddr,
			    local ? newiph->saddr : 0,
			    RT_TOS(newiph->tos) | RTO_CONN,
                0);
    if( rt == NULL )
#else
	if (ip_route_output(&rt, newiph->daddr,
			    local ? newiph->saddr : 0,
			    RT_TOS(newiph->tos) | RTO_CONN,
			    0) != 0)
#endif
{
		goto free_nskb;
}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
    //dst_release(skb_dst(nskb));
	skb_dst_set_noref(nskb, &rt->dst);
#else
	skb_dst_drop(nskb);
	skb_dst_set(nskb,&rt->dst);
#endif
	/* "Never happens" */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
	if (nskb->len > dst_mtu(skb_dst(nskb)))
#else
	if (nskb->len > dst_mtu(nskb->dst))
#endif
    {
		goto free_nskb;
    }
    if(flag == IPT_NNTP_SITE_BLOCK)
    {
        //blen = strlen(NNTP_NO_NEWSGROUP);
    	//http_data=skb_put(nskb, blen);
        //DEBUGP("<0>[%s():%d] skb->len is <%d> \n", __FUNCTION__, __LINE__, nskb->len);
        //strcpy(http_data, NNTP_NO_NEWSGROUP);
        
        nntp_skb = skb_copy(nskb, GFP_ATOMIC);
        if (nntp_skb)
        {
            newiph = ip_hdr(nntp_skb);
            tcph = (struct tcphdr *)((u_int32_t*)newiph + newiph->ihl);
            
            tcph->fin = 1;
            tcph->psh = 1;
            tcph->ack = 1;
            tcph->rst = 0;
            tcph->seq -= htonl(blen+1);
            
        	tcph->check = 0;
        	nntp_skb->csum = csum_partial((char *)tcph + tcph->doff*4,
        			blen, 0);
            tcph->check = tcp_v4_check(sizeof(struct tcphdr) + blen,
                			   newiph->saddr,
                			   newiph->daddr,
                               csum_partial((char *)tcph,
        					   sizeof(struct tcphdr),nntp_skb->csum));
        	newiph->check = 0;
        	newiph->check = ip_fast_csum((unsigned char *)newiph,
        					   newiph->ihl);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0) 
        	sk = nntp_skb->sk;       					
        	rt = ip_route_output(sock_net(sk), newiph->daddr,
        			    local ? newiph->saddr : 0,
        			    RT_TOS(newiph->tos) | RTO_CONN,
        			    0);
            if(rt == NULL)
#else
        	if (ip_route_output(&rt, newiph->daddr,
        			    local ? newiph->saddr : 0,
        			    RT_TOS(newiph->tos) | RTO_CONN,
        			    0) != 0)
#endif
        		goto free_rst_skb;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
        	//dst_release(skb_dst(nntp_skb));
        	skb_dst_set_noref(nntp_skb, &rt->dst);
#else
			skb_dst_drop(nntp_skb);
			skb_dst_set(nntp_skb,&rt->dst);
#endif


	/* "Never happens" */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
        	if (nntp_skb->len > dst_mtu(skb_dst(nntp_skb)))
#else
        	if (nntp_skb->len > dst_mtu(nntp_skb->dst))
#endif
        		goto free_rst_skb;
        	ip_direct_send(nntp_skb);
        	goto send_block;
 free_rst_skb:
	kfree_skb(nntp_skb);
        }
    }
send_block:
    ip_direct_send(nskb);
	return;

 free_nskb:
	kfree_skb(nskb);
}

static void cup_destroy(struct sk_buff *skb)
{
	dev_put(skb->dev);
}
int deliver_macblock_skb(struct sk_buff *skb)
{
	int num;
	struct net_device *dev;
	
	dev = skb->dev;
	if((num = dev_queue_xmit(skb))<0) {
		kfree(skb);
		dev_put(dev);
		printk("dev_xmit error\n");
		return -1;
		}
	return num;
}


static void send_macblock(struct sk_buff *oldskb)
{
	struct sk_buff *nskb;
    struct iphdr *iph;
    struct iphdr *newiph;
	struct tcphdr *otcph, *tcph;
	struct ethhdr *oethhd;
	struct ethhdr *ethhd;
	unsigned int otcplen;
	u_int16_t tmp;
	char *http_data=NULL;
	//char blockpage[1024]=BLKPAGE;
	int blen=0;
	int i;
	int flag;
	
	oethhd = eth_hdr(oldskb);
    iph =  ip_hdr(oldskb);
	/* IP header checks: fragment, too short. */
	if (iph->frag_off & htons(IP_OFFSET)
	    || oldskb->len < (iph->ihl<<2) + sizeof(struct tcphdr))
		return;

	otcph = (struct tcphdr *)((u_int32_t*)iph + iph->ihl);
	otcplen = oldskb->len - iph->ihl*4;


	/* Check checksum. */
	if (tcp_v4_check(otcplen, iph->saddr,
			 iph->daddr,
			 csum_partial((char *)otcph, otcplen, 0)) != 0)
		return;
/*
	blen=kread_file(BLKPAGE_PATH,1024,blockpage+strlen(BLKPAGE));

	if(blen<0){
                strcpy(blockpage,BLKPAGE_DEF);
        }//brayg

	blen=strlen(blockpage);
*/
	/* Copy skb (even if skb is about to be dropped, we can't just
           clone it because there may be other things, such as tcpdump,
           interested in it) */


	if(otcph->syn == 1)
	{
		nskb = skb_copy(oldskb, GFP_ATOMIC);
		flag = SEND_SYNACK;
	}
	else if(otcph->ack == 1 && otcph->psh == 1)
	{
		nskb = skb_copy_expand(oldskb,skb_headroom(oldskb), macblock_blen + 1, GFP_ATOMIC);
		flag = SEND_BLOCK;
	}
	else 
		return;

	if (!nskb)
		return;

	ethhd = eth_hdr(nskb);

	/* This packet will not be the same as the other: clear nf fields */
	nf_conntrack_put(nskb->nfct);
	nskb->nfct = NULL;
	//nskb->nfcache = 0;
#ifdef CONFIG_NETFILTER_DEBUG
	nskb->nf_debug = 0;
#endif
    newiph = ip_hdr(nskb);
	tcph = (struct tcphdr *)((u_int32_t*)newiph + newiph->ihl);


	/* Swap source and dest */
	//newiph->daddr = xchg(&newiph->saddr, newiph->daddr);
	memcpy(ethhd->h_dest, oethhd->h_source, ETH_ALEN);
	memcpy(ethhd->h_source, oethhd->h_dest, ETH_ALEN);
 	for(i = 0; i < ETH_ALEN; i++)
	{
		printk("%02x:", ethhd->h_dest[i]);
	}
	printk("\n");
	newiph->daddr = iph->saddr;
	newiph->saddr = iph->daddr;
	tmp = tcph->source;
	tcph->source = tcph->dest;
	tcph->dest = tmp;

		/* Truncate to length (no data) */
	tcph->doff = sizeof(struct tcphdr)/4;
	skb_trim(nskb, newiph->ihl*4 + sizeof(struct tcphdr));
    DEBUGP("<0>[%s():%d] skb->len is <%d> header len is <%d>\n", __FUNCTION__, __LINE__, nskb->len, newiph->ihl*4 + sizeof(struct tcphdr));

    
    if(flag == SEND_BLOCK)
    {    
        blen = macblock_blen;
    	http_data=skb_put(nskb, blen);
        DEBUGP("<0>[%s():%d] skb->len is <%d> \n", __FUNCTION__, __LINE__, nskb->len);
	    strcpy(http_data, macblkpage);
		/* Ack No. & Seq No. */
		tcph->seq = otcph->ack_seq;
		tcph->ack_seq = htonl(ntohl(otcph->seq) + otcph->syn + otcph->fin
				      + otcplen - (otcph->doff<<2));
		/* Reset flags */
		((u_int8_t *)tcph)[13] = 0;
        
    	tcph->fin = 1;
    	tcph->psh = 1;
    	tcph->ack = 1;
    	tcph->rst = 0;

		tcph->window = 0;
		tcph->urg_ptr = 0;

		/* Adjust TCP checksum */
		tcph->check = 0;

		nskb->csum = csum_partial((char *)tcph + tcph->doff*4,
			blen, 0);

        tcph->check = tcp_v4_check(sizeof(struct tcphdr) + blen,
        			   newiph->saddr,
        			   newiph->daddr,
                                   csum_partial((char *)tcph,
					   sizeof(struct tcphdr),nskb->csum));
		/* Adjust IP TTL, DF */
		newiph->ttl = MAXTTL;
		/* Set DF, id = 0 */
		newiph->frag_off = htons(IP_DF);
		newiph->id = 0;
		newiph->tot_len = htons(nskb->len);
		/* Adjust IP checksum */
		newiph->check = 0;
		newiph->check = ip_fast_csum((unsigned char *)newiph,
					   newiph->ihl);

	}

	else if(flag == SEND_SYNACK)
	{
		/* Ack No. & Seq No. */
		tcph->seq = otcph->seq;
		tcph->ack_seq = htonl(ntohl(otcph->seq) + 1);
		/* reset flags */
		((u_int8_t *)tcph)[13] = 0;
    	tcph->fin = 0;
    	tcph->psh = 0;
		tcph->syn = 1;
    	tcph->ack = 1;
    	tcph->rst = 0;

		tcph->window = otcph->window;
		tcph->urg_ptr = 0;

		/* Adjust TCP checksum */
		tcph->check = 0;

		nskb->csum = csum_partial((char *)tcph + tcph->doff*4,
			blen, 0);

        tcph->check = tcp_v4_check(sizeof(struct tcphdr) + blen,
        			   newiph->saddr,
        			   newiph->daddr,
                                   csum_partial((char *)tcph,
					   sizeof(struct tcphdr),nskb->csum));
		/* Adjust IP TTL, DF */
		newiph->ttl = MAXTTL;
		/* Set DF, id = 0 */
		newiph->frag_off = htons(IP_DF);
		newiph->id = 0;
		newiph->tot_len = htons(nskb->len);
		/* Adjust IP checksum */
		newiph->check = 0;
		newiph->check = ip_fast_csum((unsigned char *)newiph,
					   newiph->ihl);

	}


	nskb->destructor = cup_destroy;

	nskb->data = nskb->data - ETH_HLEN;
	nskb->len = nskb->len + ETH_HLEN;

    deliver_macblock_skb(nskb);
	return;


}



/* Send RST reply */
static void send_reset(struct sk_buff *oldskb, int hook)
{
	struct sk_buff *nskb;
	struct iphdr *iph = ip_hdr(oldskb);
	struct iphdr *newiph;
	struct tcphdr _otcph, *oth, *tcph;
	__be16 tmp_port;
	__be32 tmp_addr;
	int needs_ack;
	unsigned int addr_type;

	/* IP header checks: fragment. */
	if (iph->frag_off & htons(IP_OFFSET))
		return;

	oth = skb_header_pointer(oldskb, iph->ihl * 4,
				 sizeof(_otcph), &_otcph);
	if (oth == NULL)
		return;

	/* No RST for RST. */
	if (oth->rst)
		return;

	/* Check checksum */
	if (nf_ip_checksum(oldskb, hook, ip_hdrlen(oldskb), IPPROTO_TCP))
		return;

	/* We need a linear, writeable skb.  We also need to expand
	   headroom in case hh_len of incoming interface < hh_len of
	   outgoing interface */
	nskb = skb_copy_expand(oldskb, LL_MAX_HEADER, skb_tailroom(oldskb),
			       GFP_ATOMIC);
	if (!nskb)
		return;

	/* This packet will not be the same as the other: clear nf fields */
	nf_reset(nskb);
	nskb->mark = 0;
	skb_init_secmark(nskb);

	skb_shinfo(nskb)->gso_size = 0;
	skb_shinfo(nskb)->gso_segs = 0;
	skb_shinfo(nskb)->gso_type = 0;

	newiph = ip_hdr(nskb);
	tcph = (struct tcphdr *)((u_int32_t*)newiph + newiph->ihl);

	/* Swap source and dest */
	tmp_addr = newiph->saddr;
	newiph->saddr = newiph->daddr;
	newiph->daddr = tmp_addr;
	tmp_port = tcph->source;
	tcph->source = tcph->dest;
	tcph->dest = tmp_port;

	/* Truncate to length (no data) */
	tcph->doff = sizeof(struct tcphdr)/4;
	skb_trim(nskb, newiph->ihl*4 + sizeof(struct tcphdr));
	newiph->tot_len = htons(nskb->len);

	if (tcph->ack) {
		needs_ack = 0;
		tcph->seq = oth->ack_seq;
		tcph->ack_seq = 0;
	} else {
		needs_ack = 1;
		tcph->ack_seq = htonl(ntohl(oth->seq) + oth->syn + oth->fin
				      + oldskb->len - ip_hdrlen(oldskb)
				      - (oth->doff<<2));
		tcph->seq = 0;
	}

	/* Reset flags */
	((u_int8_t *)tcph)[13] = 0;
	tcph->rst = 1;
	tcph->ack = needs_ack;

	tcph->window = 0;
	tcph->urg_ptr = 0;

	/* Adjust TCP checksum */
	tcph->check = 0;
	tcph->check = tcp_v4_check(sizeof(struct tcphdr),
				   newiph->saddr,
				   newiph->daddr,
				   csum_partial((char *)tcph,
						sizeof(struct tcphdr), 0));

	/* Set DF, id = 0 */
	newiph->frag_off = htons(IP_DF);
	newiph->id = 0;

	addr_type = RTN_UNSPEC;
	if (hook != NF_IP_FORWARD
#ifdef CONFIG_BRIDGE_NETFILTER
	    || (nskb->nf_bridge && nskb->nf_bridge->mask & BRNF_BRIDGED)
#endif
	   )
		addr_type = RTN_LOCAL;

	if (ip_route_me_harder(nskb, addr_type))
		goto free_nskb;

	nskb->ip_summed = CHECKSUM_NONE;

	/* Adjust IP TTL */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
	newiph->ttl = dst_metric(skb_dst(nskb), RTAX_HOPLIMIT);
#else
	newiph->ttl = dst_metric(nskb->dst, RTAX_HOPLIMIT);
#endif
	/* Adjust IP checksum */
	newiph->check = 0;
	newiph->check = ip_fast_csum((unsigned char *)newiph,
					   newiph->ihl);

	/* "Never happens" */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
	if (nskb->len > dst_mtu(skb_dst(nskb)))
#else
	if (nskb->len > dst_mtu(nskb->dst))
#endif
		goto free_nskb;

	nf_ct_attach(nskb, oldskb);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
	NF_HOOK(PF_INET, NF_IP_LOCAL_OUT, nskb, NULL, skb_dst(nskb)->dev,
		dst_output);
#else
	NF_HOOK(PF_INET, NF_IP_LOCAL_OUT, nskb, NULL, nskb->dst->dev,
		dst_output);
#endif
	return;

 free_nskb:
	kfree_skb(nskb);
}

static inline void send_unreach(struct sk_buff *skb_in, int code)
{
	icmp_send(skb_in, ICMP_DEST_UNREACH, code, 0);
}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
static unsigned int reject(struct sk_buff *skb,
			       const struct xt_action_param *tg_parm)
#else
static unsigned int reject(struct sk_buff *skb,
			       const struct xt_target_param *tg_parm)
#endif
{
	const struct ipt_reject_info *reject = tg_parm->targinfo;//targinfo;
    struct iphdr *iph;

    iph = ip_hdr(skb);
	/* Our naive response construction doesn't deal with IP
	   options, and probably shouldn't try. */
	if (iph->ihl<<2 != sizeof(struct iphdr))
		return NF_DROP;

	/* WARNING: This code causes reentry within iptables.
	   This means that the iptables jump stack is now crap.  We
	   must return an absolute verdict. --RR */
	switch (reject->with) {
	case IPT_ICMP_NET_UNREACHABLE:
		send_unreach(skb, ICMP_NET_UNREACH);
		break;
	case IPT_ICMP_HOST_UNREACHABLE:
		send_unreach(skb, ICMP_HOST_UNREACH);
		break;
	case IPT_ICMP_PROT_UNREACHABLE:
		send_unreach(skb, ICMP_PROT_UNREACH);
		break;
	case IPT_ICMP_PORT_UNREACHABLE:
		send_unreach(skb, ICMP_PORT_UNREACH);
		break;
	case IPT_ICMP_NET_PROHIBITED:
		send_unreach(skb, ICMP_NET_ANO);
		break;
	case IPT_ICMP_HOST_PROHIBITED:
		send_unreach(skb, ICMP_HOST_ANO);
		break;
	case IPT_ICMP_ADMIN_PROHIBITED:
		send_unreach(skb, ICMP_PKT_FILTERED);
		break;
	case IPT_TCP_RESET:
		send_reset(skb, tg_parm->hooknum);
		break;
		/* Ron */
	case IPT_HTTP_BLOCK:
		send_block(skb, tg_parm->hooknum == NF_IP_LOCAL_IN, IPT_HTTP_SITE_BLOCK);
		return NF_DROP;
		break;
		
	case IPT_NNTP_BLOCK:
		send_block(skb, tg_parm->hooknum == NF_IP_LOCAL_IN, IPT_NNTP_SITE_BLOCK);
		break;
	case IPT_MAC_BLOCK:
		send_macblock(skb);
		break;

	case IPT_ICMP_ECHOREPLY:
		/* Doesn't happen. */
		break;
	}

	return NF_DROP;
}

static int check(const struct xt_tgchk_param *par)
{
	const struct ipt_reject_info *rejinfo = par->targinfo;
	const struct ipt_entry *e = par->entryinfo;

	if (rejinfo->with == IPT_ICMP_ECHOREPLY) {
		printk("REJECT: ECHOREPLY no longer supported.\n");
		return -EINVAL;
	} else if (rejinfo->with == IPT_TCP_RESET) {
		/* Must specify that it's a TCP packet */
		if (e->ip.proto != IPPROTO_TCP
		    || (e->ip.invflags & XT_INV_PROTO)) {
			DEBUGP("REJECT: TCP_RESET invalid for non-tcp\n");
			return -EINVAL;
		}
	}
	return 0;
}

static struct xt_target ipt_reject_reg = {
	.name		= "REJECT",
	.family		= AF_INET,
	.target		= reject,
	.targetsize	= sizeof(struct ipt_reject_info),
	.table		= "filter",
	.hooks		= (1 << NF_IP_LOCAL_IN) | (1 << NF_IP_FORWARD) |
			  (1 << NF_IP_LOCAL_OUT),
	.checkentry	= check,
	.me		= THIS_MODULE,
};

static int __init ipt_reject_init(void)
{
    static char temp_buf[65536];

    memset(temp_buf, 0, sizeof(temp_buf));
    http_blen=kread_file(BLKPAGE_PATH, sizeof(temp_buf), temp_buf);
	if(http_blen<0){
        http_blen = snprintf(temp_buf,sizeof(temp_buf), "%s", BLKPAGE_DEF);
    }
    else {
        DEBUGP("<0>[%s():%d] http_blen is <%d> strlen is <%d>\n", __FUNCTION__, __LINE__, http_blen, strlen(temp_buf));

        http_blen = snprintf(blockpage, sizeof(blockpage), BLKPAGE"%s", http_blen, temp_buf);
        DEBUGP("<0>[%s():%d] http_blen is <%d> strlen is <%d>\n", __FUNCTION__, __LINE__, http_blen, strlen(blockpage));
    }

    memset(temp_buf, 0, sizeof(temp_buf));
	macblock_blen = snprintf(temp_buf, sizeof(temp_buf), "%s", MACBLK_DEF);
	macblock_blen = snprintf(macblkpage, sizeof(macblkpage), BLKPAGE"%s", macblock_blen, temp_buf); 

	if (xt_register_target(&ipt_reject_reg))
            return -EINVAL;
        return 0;
}

static void __exit ipt_reject_fini(void)
{
	xt_unregister_target(&ipt_reject_reg);
}

module_init(ipt_reject_init);
module_exit(ipt_reject_fini);

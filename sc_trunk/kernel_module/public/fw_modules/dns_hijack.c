/*
 * dns_hijack.c 
 */

#define EXPORT_SYMTAB

#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/if.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/if_ether.h>
#include <net/ip.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <asm/checksum.h>
#include <asm/uaccess.h>
#include <linux/inetdevice.h>
#include <linux/inet.h>
#include <linux/rcupdate.h>
#include <linux/version.h>

#define DNS_PORT 53
#define MESSAGE_LEN 64
#define IPADDR_LEN 16
#define IPHEAD_LEN 20
#define UDPHEAD_LEN 8
#define OFFSET 4
const char dns_match_1[] = {0x03, 'w','w','w', 0x0b, 'r', 'o','u', 't', 'e', 'r','l', 'o', 'g', 'i', 'n', 0x03, 'c', 'o', 'm', 0x00};
const char dns_match_2[] = {0x0b, 'r', 'o', 'u', 't', 'e', 'r', 'l' ,'o', 'g', 'i', 'n', 0x03, 'c', 'o', 'm', 0x00};
const char dns_match_3[] = {0x03, 'w', 'w', 'w', 0x0b, 'r', 'o', 'u', 't','e','r', 'l', 'o', 'g', 'i', 'n', 0x03, 'n', 'e', 't', 0x00} ;
const char dns_match_4[] = {0x0b, 'r', 'o', 'u', 't','e','r', 'l', 'o', 'g', 'i', 'n', 0x03, 'n', 'e', 't', 0x00};

//static struct dns_hijack_s DnsHijack;
extern int (*dns_hijack_hook)(struct sk_buff *);

char dns_ip[IPADDR_LEN];

int ip_valid = 0;

struct dns_tail_s {
	__be16 name;
	__be16 type;
	__be16 class;
	__be32 ttl;
	__be16 len;
	__be32 ans;
}__attribute__((__packed__));

struct dns_head_s {
	__be16 id;
	__be16 flag;
	__be16 quest_n;
	__be16 ans_n;
	__be16 author_n;
	__be16 addit_n;
}__attribute__((__packed__));

static inline void initial_dns_tail(const char *dnsip, struct dns_tail_s *dns_tail)
{
	dns_tail->name = 0x0cc0;
	dns_tail->type = 0x0100;
	dns_tail->class = 0x0100;
	dns_tail->ttl = 0x01000000;
	dns_tail->len = 0x0400;
	dns_tail->ans = in_aton(dnsip);
}

static inline void initial_dns_head(const char *buf, struct dns_head_s *dns_head)
{
	dns_head->id = *((__be16 *)buf);
	dns_head->flag = 0x0080;
	dns_head->quest_n = 0x0100;
	dns_head->ans_n = 0x0100;
	dns_head->author_n = 0x0000;
	dns_head->addit_n = 0x0000;
}


/*struct single_if_info {
	char name[16];
	struct net_device *dev;
	struct single_if_info *next;
};

struct dns_hjctl_header {
	spinlock_t lock;
	struct single_if_info *head;
};


static struct dns_hjctl_header dns_hjctl_head_obj;

static struct single_if_info *looup_if_byaddr_inlist(struct dns_hjctl_header *dns_ctl, const struct net_device *indev)
{
	struct single_in_info *dnsif;
	dnsif = dns_ctl->head;
	while(dnsif)
	{
		if(dnsif->dev && dnsif->dev == indev)
		{
			return dns_if;
		}
		dnsif = dnsif->next;
	}
	return NULL;
}

static struct single_if_info *lookup_if_byname_inlist(struct dns_hjctl_header *dns_ctl, const char *name)
{
	struct single_if_info  *dnsif;
	dnsif = dns_ctl->head;
	while(dnsif)
	{
		if(strcmp(dnsif->name, name) == 0)
		{
			return dns_if;
		}
		dns_if = dns_if->next;
	}
	return NULL;
}*/
static inline void initial_iphd(const struct iphdr *recv_iphd, struct iphdr *dns_iphd, u16 iplen)
{
	dns_iphd->ihl = recv_iphd->ihl;
	dns_iphd->version = recv_iphd->version;
	dns_iphd->tos = 0;
	dns_iphd->tot_len = htons(iplen);
	dns_iphd->id = 0x0100;
	dns_iphd->frag_off = 0x0040;
	dns_iphd->ttl = 0x40;
	dns_iphd->protocol = IPPROTO_UDP;
	dns_iphd->saddr = recv_iphd->daddr;
	dns_iphd->daddr = recv_iphd->saddr;
	dns_iphd->check = 0;
	dns_iphd->check = ip_fast_csum((u8 *)dns_iphd, dns_iphd->ihl);
}

static inline void initial_udphd(const struct udphdr *recv_udphd, struct udphdr *dns_udphd, const struct iphdr *dns_iphd, u16 udplen, __wsum data_csum)
{
	dns_udphd->source = recv_udphd->dest;
	dns_udphd->dest = recv_udphd->source;
	dns_udphd->len = htons(udplen);
	dns_udphd->check = 0;
	data_csum = csum_partial((u8 *)dns_udphd, 8, data_csum);
	dns_udphd->check = csum_tcpudp_magic(dns_iphd->saddr, dns_iphd->daddr, udplen, IPPROTO_UDP, data_csum);
}

static void cup_destroy(struct sk_buff *skb)
{
	dev_put(skb->dev);
}

static inline void initial_dns_skb(struct sk_buff *dns_skb, struct net_device *dev, u16 iplen)
{
	dns_skb->dev = dev;
	dns_skb->pkt_type = PACKET_OTHERHOST;
	dns_skb->ip_summed = CHECKSUM_NONE;
	dns_skb->destructor = cup_destroy;
	dns_skb->priority = 0;
	dns_skb->protocol = htons(ETH_P_IP);
}

int deliver_dns_skb(struct sk_buff *dns_skb)
{
	int num;
	struct net_device *dev;
	
	dev = dns_skb->dev;
	if((num = dev_queue_xmit(dns_skb))<0) {
		kfree(dns_skb);
		dev_put(dev);
		printk("dev_xmit error\n");
		return -1;
		}
	else 
		printk("dev_ximt sucess\n");
	return num;
}

static int dns_hijack_handle(struct sk_buff *skb)
{
	int ret;
	struct udphdr *recv_udphd = NULL;
	struct iphdr *recv_iphd = NULL;
	struct ethhdr *recv_ethhd = NULL;
	struct net_device *dev = NULL;
	unsigned char *recv_data = NULL;
	unsigned char *query_name = NULL;
	unsigned char *iph = NULL;

	u16 datalen, iplen, udplen, skblen, recv_udplen;
	static struct dns_tail_s dns_tail;
	static struct dns_head_s dns_head;
	static struct iphdr dns_iphd;
	static struct udphdr dns_udphd;
	static struct ethhdr dns_ethhd;
			
	struct sk_buff *dns_skb = NULL;
	u8 dns_data[512];
	__wsum data_csum;

if(ip_valid)
	{	
	recv_ethhd = eth_hdr(skb);
	
	iph = (u8 *)(skb->data);
	recv_iphd = (struct iphdr *)(skb->data);
	
	  
	if(recv_iphd->protocol == IPPROTO_UDP)
	{
		recv_udphd = (struct udphdr *)(iph + IPHEAD_LEN);
		if(ntohs(recv_udphd->dest) == DNS_PORT)
		{
			data_csum = 0;
	
			dev = skb->dev;

			if((recv_udplen = ntohs(recv_udphd->len))<20) {
				printk(KERN_ALERT "incorrect dns packet. \n");
				return 0;
			}


			recv_data = iph + UDPHEAD_LEN + IPHEAD_LEN;
			query_name = recv_data + 12;

			if((strcmp(query_name, dns_match_1) == 0) || (strcmp(query_name, dns_match_2) == 0) || (strcmp(query_name, dns_match_3) == 0) || (strcmp(query_name, dns_match_4) == 0)) {
 
				memcpy(&dns_data[12], query_name, recv_udplen -20);
				initial_dns_head(recv_data, &dns_head);
				memcpy(dns_data, &dns_head, sizeof(dns_head));
				initial_dns_tail(dns_ip, &dns_tail);
				memcpy(&dns_data[recv_udplen - 8], &dns_tail, sizeof(dns_tail));
	
				udplen = recv_udplen + sizeof(dns_tail);
				iplen = udplen + IPHEAD_LEN;
				datalen = udplen - UDPHEAD_LEN;

				memcpy(dns_ethhd.h_dest, recv_ethhd->h_source, ETH_ALEN);
				memcpy(dns_ethhd.h_source, recv_ethhd->h_dest, ETH_ALEN);
				dns_ethhd.h_proto = recv_ethhd->h_proto;
	
				initial_iphd(recv_iphd, &dns_iphd, iplen);
	
				data_csum = csum_partial(dns_data, datalen, 0);
				initial_udphd(recv_udphd, &dns_udphd, &dns_iphd, udplen, data_csum);
				skblen = iplen + ETH_HLEN + LL_RESERVED_SPACE(dev);

				if((dns_skb = alloc_skb(skblen, GFP_ATOMIC)) == NULL)
				{
					printk(KERN_ALERT "Memory low\n");
					return -1; //NOMEM
				} 
	
				initial_dns_skb(dns_skb, dev, iplen);
				
				skb_reserve(dns_skb, ETH_HLEN);
	
				memcpy(skb_push(dns_skb, ETH_HLEN), (u8 *)(&dns_ethhd), ETH_HLEN);

				dns_skb->network_header = skb_put(dns_skb, sizeof(struct iphdr));

				memcpy(dns_skb->network_header, (u8 *)(&dns_iphd), sizeof(struct iphdr));

				dns_skb->transport_header = skb_put(dns_skb, sizeof(struct udphdr));
				memcpy(dns_skb->transport_header, (u8 *)(&dns_udphd), sizeof(struct udphdr));
	
				memcpy(skb_put(dns_skb, datalen), dns_data, datalen);
	
				printk("skb->network_header = %p\n", dns_skb->network_header);
				printk("skb->data = %p\n", dns_skb->data);
				printk("skb->tail = %p\n", dns_skb->tail);
				printk("skb->tail = %p\n", dns_skb->tail);
				

				ret = deliver_dns_skb(dns_skb);

				if(ret > 0)
					return ret;
				else
					return -1;
			}
		}
	}	
	}
return 0;
}		

static ssize_t dns_hijack_read(struct file *flip, char __user *buf, size_t count, loff_t *offp)
{
	char data[MESSAGE_LEN];
	int len =0;
	if(*offp>0)
	return 0;
	len = sprintf(data, "The DNS server ip: %s", dns_ip);
	if(copy_to_user(buf, data, len)) {
		return -EFAULT;
	}
	*offp = len;
	return len;
}

static ssize_t dns_hijack_write(struct file *filp, const char *buffer, size_t count, loff_t *offp)
{
	int ret;
	if(count > IPADDR_LEN) {
		printk(KERN_ALERT "Invalid ipaddress length.\n");
		return -EFAULT;
		}
	else
		if(copy_from_user(dns_ip, buffer, count)) {
			printk(KERN_ALERT "Can't get dns ip\n");
			return -EFAULT;
		}
	if(count >= 7 && count <= IPADDR_LEN)
		ip_valid = 1;
	else
		ip_valid = 0;
	ret = count;
	dns_ip[count] = '\0';
	return ret;	
}		
	

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = dns_hijack_read,
	.write = dns_hijack_write,
};

/*static struct notifier_block if_netdev_notifier = {
	.notifier_call = if_netdev_event,
};*/

static int __init dns_hijack_init(void)
{
	struct proc_dir_entry *proc_file;
	
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
	if(!(proc_file = proc_create("dns_hijack", 0666, init_net.proc_net, &fops)))
#else
	if(!(proc_file = proc_net_fops_create(&init_net, "dns_hijack", 0666, &fops)))
#endif
	{
		return -ENOMEM;
	}


	dns_hijack_hook = dns_hijack_handle;
	printk(KERN_ALERT "dns_hijack_module installed.\n");
	return 0;
}
static void __exit dns_hijack_cleanup(void)
{
	dns_hijack_hook = NULL;

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
	remove_proc_entry("dns_hijack", init_net.proc_net /* parent dir */);
#else
	proc_net_remove(&init_net, "dns_hijack");
#endif
	printk(KERN_ALERT "dns_hijack_module removed.\n");
}

module_init(dns_hijack_init);
module_exit(dns_hijack_cleanup);
MODULE_LICENSE("GPL");











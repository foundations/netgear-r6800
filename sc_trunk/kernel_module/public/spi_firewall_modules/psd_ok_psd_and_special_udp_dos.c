#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/tcp.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <sc/sc_spi.h>
#include "psd_and_special_udp_dos.h"
#include "common.h"

//#define SUPPORT_ECHO_CHARGEN_DETECT

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

#define SCAN_MAX_COUNT     3
#define PORT_SCAN_BLOCK_TIME	(10*60*HZ)

#ifdef	SUPPORT_ECHO_CHARGEN_DETECT
#define UDP_SPECIAL_BLOCK_TIME	(5*60*HZ)

#define MAX_ECHO_CHARGEN	51
#define MAX_USER_SET	4

#define ECHO_PORT		7
#define CHARGEN_PORT	19


#define UDP_ECHO		0
#define UDP_CHARGEN		1
#define TCP_ECHO		2
#define TCP_CHARGEN		3
#define UDP_TCP_ECHO	0
#define UDP_TCP_CHARGEN	1

static int dmz = 0;
#endif

/*
 * Information we keep per each target port
 */
struct port {
	u_int16_t number;      /* port number */ 
	u_int8_t proto;        /* protocol number */
	unsigned long timestamp;	   /*record the timestamp*/
};

/*
 * Information we keep per each source address.
 */
struct host {
	struct host *next;		/* Next entry with the same hash */
	struct list_head	list;
	unsigned long timestamp;		/* Last update time */
	struct in_addr src_addr;	/* Source address */
	struct in_addr dest_addr;	/* Destination address */
	unsigned short src_port;	/* Source port */
	struct port ports[SCAN_MAX_COUNT];	/* List of ports */
	queue_t queue;
	int count;			/* Number of ports in the list */
};

/*
 * State information.
 */
static struct {
	spinlock_t lock;
	struct host list_entry[LIST_SIZE];	/* save source addresses array */
	struct host *tcp_hash[HASH_SIZE];	/* point to tcp hash list */
	struct host *udp_hash[HASH_SIZE];	/* point to udp hash list */
	struct host **hash;
#ifdef	SUPPORT_ECHO_CHARGEN_DETECT
//	struct user_set[MAX_USER_SET];
	unsigned long echo_udp[MAX_ECHO_CHARGEN];
	unsigned long chargen_udp[MAX_ECHO_CHARGEN];
	unsigned long echo_tcp[MAX_ECHO_CHARGEN];
	unsigned long chargen_tcp[MAX_ECHO_CHARGEN];
#endif
} state;


atomic_t total_entry = ATOMIC_INIT(0);
struct list_head free_list = LIST_HEAD_INIT(free_list);


void clear_oldest_entry(int type)
{
	struct host *curr, *prev, **head;
	int i;
	unsigned long now = jiffies;
	
	prev = NULL;
	if(type == IPPROTO_TCP)
		state.hash = &state.tcp_hash[0];
	else if(type == IPPROTO_UDP)
		state.hash = &state.udp_hash[0];
		
	for(i = 0; i < HASH_SIZE; i++)
	{
		if((curr = *(head = &state.hash[i])))
		do {
			if((curr->timestamp + 10*HZ > now)
				&& time_after_eq(now, curr->timestamp))
			{
				atomic_dec(&total_entry);
				if (prev)
					prev->next = prev->next->next;
				else if (*head)
					*head = (*head)->next;	
				list_add(&curr->list, &free_list);
			}
			if (curr->next) prev = curr;
		} while ((curr = curr->next));
	}	
}
/*
 * Convert an IP address into a hash table index.
 */
static inline int hashfunc(struct in_addr addr)
{
	unsigned int value;
	int hash;

	value = addr.s_addr;
	hash = 0;
	do {
		hash ^= value;
	} while ((value >>= HASH_LOG));

	return hash & (HASH_SIZE - 1);
}

unsigned int __sc_psd_and_special_udp_detect_hook(struct sk_buff *skb, struct nf_conn *ct, u_int8_t protonum)

{
	struct iphdr *ip_hdr;
	struct tcphdr *tcp_hdr;
	struct in_addr addr;
	u_int16_t src_port,dest_port;
	unsigned long now;
	struct host *curr, *prev, *next, **head;
	int hash, index, count;
	u_int8_t proto;
	unsigned int ret = NF_ACCEPT;
	struct nf_conntrack_tuple tuple;
	struct nf_conntrack_tuple mask;


	if(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.l3num != PF_INET)
	{
		DEBUGP("psd just support ipv4 now\n");
		return ret;
	}
	
	if(ct->master)
	{
		DEBUGP("psd return, because it is a ALG expect packet.\n");
		return ret;
	}
	
	/* IP header */
	ip_hdr = skb->nh.iph;

	/* Sanity check */
	if (ntohs(ip_hdr->frag_off) & IP_OFFSET) {
		DEBUGP("PSD: sanity check failed\n");
		return ret;
	}

	/* TCP or UDP ? */
	proto = ip_hdr->protocol;

	if (proto != IPPROTO_TCP && proto != IPPROTO_UDP) {
		DEBUGP("PSD: protocol not supported\n");
		return ret;
	}

	/* Get the source address, source & destination ports, and TCP flags */

	addr.s_addr = ip_hdr->saddr;

	tcp_hdr = (struct tcphdr*)((u_int32_t *)ip_hdr + ip_hdr->ihl);

	src_port = tcp_hdr->source;
	dest_port = tcp_hdr->dest;

	/* We're using IP address 0.0.0.0 for a special purpose here, so don't let
	 * them spoof us. [DHCP needs this feature - HW] */
	if (!addr.s_addr) {
		DEBUGP("PSD: spoofed source address (0.0.0.0)\n");
		return ret;
	}

	memcpy(&tuple, &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple, sizeof(tuple));

	/* Use jiffies here not to depend on someone setting the time while we're
	 * running; we need to be careful with possible return value overflows. */
	now = jiffies;

	spin_lock(&state.lock);
	
	if (proto == IPPROTO_TCP)
	{
		state.hash = &state.tcp_hash[0];
	}
	else
	{
		state.hash = &state.udp_hash[0];
	}

#ifdef SUPPORT_ECHO_CHARGEN_DETECT
	if( udp/tcp(according to user setting) && from_wan 
	    && ((src_port == htons(7)  && dest_port == hton(19)) || (src_port == 19 && dest_port == 7)))
	{
		//deal DOS (6)	TCP/UDP Echo/Chargen Attack
	}
#endif

	/* if tcp_hdr->ack is not set, we do not record this packet,
		* we just record the established incoming tcp session.
		* tcp syn pending will be dealed by other hook */	
	if (proto == IPPROTO_TCP && !tcp_hdr->ack)
		goto out;
		
	/* Do we know this source address already? */
	count = 0;
	prev = NULL;
	if ((curr = *(head = &state.hash[hash = hashfunc(addr)])))
		do {
			if (curr->src_addr.s_addr == addr.s_addr) break;
			count++;
			if (curr->next) prev = curr;
		} while ((curr = curr->next));

	if (curr) {
		/* We know this src address, and the entry isn't too old. Update it. */

		/* Just update the appropriate list entry if we've seen this port already */
		for (index = 0; index < curr->count; index++) {
			if (curr->ports[index].number == dest_port) {
				curr->ports[index].timestamp = now;
				curr->ports[index].proto = proto;
				goto out;
			}
		}

		/* Packet to a new port, and not TCP/ACK: update the timestamp */
		curr->timestamp = now;

		/* Remember the new port */
		if (curr->count < (SCAN_MAX_COUNT-1)) {
			curr->ports[curr->queue.rear].number = dest_port;
			curr->ports[curr->queue.rear].timestamp = now;
			curr->ports[curr->queue.rear].proto = proto;
			curr->queue.rear = (curr->queue.rear +1)% SCAN_MAX_COUNT;
			curr->count++;
	printk("enqueue proto = %d (%d)\n", proto, curr->count);
		}
		else
		{
			if((curr->ports[curr->queue.front].timestamp + 10*HZ > now)
			    && time_after_eq(now, (curr->ports[curr->queue.front].timestamp)))
			{
				if(proto == IPPROTO_UDP)
				{
					/* generate a log */
					printk("it is UDP port scan. generate a log\n");
				}
				else if(proto == IPPROTO_TCP)
				{
					/*
						if(close port)
							should be take care by syn flood.
						else if (opend port)
					*/
					printk("tcp port scan detect ---- \n");
					{
						memset(&mask, 0, sizeof(mask));
						memset(&mask.src.u3.all, 0xFF, sizeof(mask.src.u3.all));
						mask.dst.protonum = 0xFF;
						sc_add_block_pattern_hook(&tuple, &mask, PORT_SCAN_BLOCK_TIME);
					}
					ret = NF_DROP;
				}
				/* delete this entry */
				curr->src_addr.s_addr = 0;
				curr->count = 0;
				if (prev)
					prev->next = prev->next->next;
				else if (*head)
					*head = (*head)->next;	
				list_add(&curr->list, &free_list);
			}
			else
			{
				//clean_old_entry_that_timeout_than_10s;
				for(;curr->queue.rear != curr->queue.front;)
				{
					if((curr->ports[curr->queue.front].timestamp + 10*HZ < now)
					    && time_after_eq(now, curr->ports[curr->queue.front].timestamp))
					{
						/* dequeue */
						if(curr->queue.front != curr->queue.rear)
						{
							curr->queue.front = (curr->queue.front+1)%SCAN_MAX_COUNT;
							curr->count--;
						}
					}
					else
					{
						break;
					}
				}
				//insert the new session;
				curr->ports[curr->queue.rear].number = dest_port;
				curr->ports[curr->queue.rear].proto = proto;
				curr->queue.rear = (curr->queue.rear +1)% SCAN_MAX_COUNT;
				curr->count++;					
				
			}
		}

		goto out;
	}




	/* We're going to re-use the oldest list entry, when the entry is exhausted, 
	 * so remove it from the hash table first. */
	if(atomic_read(&total_entry) >= LIST_SIZE)
	{
		/* which means there is no list entry available */
		clear_oldest_entry(IPPROTO_TCP);
		clear_oldest_entry(IPPROTO_UDP);
	}
	/* Get our list entry */
	list_for_each_entry_safe(curr, next, &free_list, list)
	{
		list_del(&curr->list);
		break;
	}

	/* Link it into the hash table */
	head = &state.hash[hash];
	curr->next = *head;
	*head = curr;
	
	/* init ports list */
	{
		curr->queue.base = &curr->ports[0].timestamp;
		curr->queue.front = 0;
		curr->queue.rear = 0;
	}

	/* And fill in the fields */
	curr->timestamp = now;
	curr->src_addr = addr;
	curr->dest_addr.s_addr = ip_hdr->daddr;
	curr->src_port = src_port;
	curr->count = 1;
	curr->ports[0].number = dest_port;
	curr->ports[0].proto = proto;
	curr->ports[0].timestamp = now;
	curr->queue.rear = curr->queue.rear + 1;
	atomic_inc(&total_entry);
	printk("enqueue new proto = %d(%d)\n", proto, curr->count);

out:
	spin_unlock(&state.lock);
	return ret;
}


static int __init init(void)
{
	int i;
		
	memset(&state, 0, sizeof(state));
	
	for(i = 0; i < LIST_SIZE; i++)
	{
		list_add(&state.list_entry[i].list, &free_list);
	}
	
	spin_lock_init(&(state.lock));

#ifdef SUPPORT_ECHO_CHARGEN_DETECT
	struct proc_dir_entry *proc;
	proc = proc_net_fops_create ("echo_chargen_config", 0, proc_echo_chargen_fops);

	if (!proc) 
	{
		printk("echo_chargen_config  proc create failed\n");
	}
	else
	{
		proc->owner = THIS_MODULE;
	}

#endif
	
	sc_psd_and_special_udp_detect_hook = __sc_psd_and_special_udp_detect_hook;

	printk("netfilter PSD and TCP/UDP Echo/Chargen Attack loaded \n");
	return 0;
}

static void __exit fini(void)
{
	sc_psd_and_special_udp_detect_hook = NULL;
	printk("netfilter PSD and TCP/UDP Echo/Chargen Attack unloaded \n");
}

module_init(init);
module_exit(fini);

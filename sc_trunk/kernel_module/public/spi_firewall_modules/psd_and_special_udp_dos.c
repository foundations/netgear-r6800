#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/tcp.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <sc/sc_spi.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include "common.h"


#include <linux/param.h>
#include <linux/types.h>

//#define IPPROTO_BOTH  255
MODULE_LICENSE("GPL");

#define LIST_SIZE			MAX_CONNTRACK_COUNT
#define HASH_LOG			9
#define HASH_SIZE			(1 << HASH_LOG)



#define SUPPORT_ECHO_CHARGEN_DETECT

#define SCAN_MAX_COUNT_HI     21
#define SCAN_MAX_COUNT_LOW    51
#define SCAN_PORT_SIZE     64
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

#define TIMEOUT_PERIOD  (10*HZ)

queue_t *action_queue;
static int dmz = 0;
static DEFINE_RWLOCK(free_list_lock);

struct user_set
{
	u_int8_t 	proto_user;
	u_int16_t 	port_user;		    /* network byte order*/
	queue_t		queue;
};

#endif
int g_port_scan_max_cnt = SCAN_MAX_COUNT_HI;

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
	struct port ports[SCAN_PORT_SIZE];	/* List of ports */
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
	struct user_set user_set[MAX_USER_SET];
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
		prev = NULL;
		if((curr = *(head = &state.hash[i])))
		do {
			if((curr->timestamp + 10*HZ < now)
				&& time_after_eq(now, curr->timestamp))
			{
				curr->timestamp = 0;
				atomic_dec(&total_entry);
				if (prev)
					prev->next = prev->next->next;
				else if (*head)
					*head = (*head)->next;	
				write_lock_bh(&free_list_lock);
				list_add(&curr->list, &free_list);
				write_unlock_bh(&free_list_lock);
			}
			else if (curr->next)
			{
				prev = curr;
			}
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
	struct iphdr *iph;
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
		DEBUG("psd just support ipv4 now\n");
		return ret;
	}
	
	if(ct->master)
	{
		DEBUG("psd return, because it is a ALG expect packet.\n");
		return ret;
	}
	
	/* IP header */
    iph = ip_hdr(skb);

	/* Sanity check */
	if (ntohs(iph->frag_off) & IP_OFFSET) {
		DEBUG("PSD: sanity check failed\n");
		return ret;
	}

	/* TCP or UDP ? */
	proto = iph->protocol;

	if (proto != IPPROTO_TCP && proto != IPPROTO_UDP) {
		DEBUG("PSD: protocol not supported\n");
		return ret;
	}

	/* Get the source address, source & destination ports, and TCP flags */

	addr.s_addr = iph->saddr;

	tcp_hdr = (struct tcphdr*)((u_int32_t *)iph + iph->ihl);

	src_port = tcp_hdr->source;
	dest_port = tcp_hdr->dest;

	/* We're using IP address 0.0.0.0 for a special purpose here, so don't let
	 * them spoof us. [DHCP needs this feature - HW] */
	if (!addr.s_addr) {
		DEBUG("PSD: spoofed source address (0.0.0.0)\n");
		return ret;
	}

	memcpy(&tuple, &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple, sizeof(tuple));

	/* Use jiffies here not to depend on someone setting the time while we're
	 * running; we need to be careful with possible return value overflows. */
	now = jiffies;

	spin_lock_bh(&state.lock);
	
	if (proto == IPPROTO_TCP)
	{
		state.hash = &state.tcp_hash[0];
	}
	else
	{
		state.hash = &state.udp_hash[0];
	}

#ifdef SUPPORT_ECHO_CHARGEN_DETECT
	//  udp echo and chargen detect is embedded in udp_tcp_port_scan module
	if(/*spi_fun_switch_on  && spi_enable && from_wan 
		&& */
#ifdef CONFIG_AMPED_DOS
	(g_dos_flag & (1 << SC_DOS_UDP_ECHOCHARGEN)) &&
#endif
		((src_port == htons(ECHO_PORT)  && dest_port == htons(CHARGEN_PORT)) || (src_port == htons(CHARGEN_PORT) && dest_port == htons(ECHO_PORT))))
	{
		int i;
		if(dmz)
		{	
			if(src_port  == htons(ECHO_PORT))
				action_queue = &state.user_set[0].queue;
			else
				action_queue = &state.user_set[1].queue;

			ct->need_recheck = 1;
						
			if(enqueue_and_check(action_queue, now, TIMEOUT_PERIOD))
			{
//			block it;
				if(sc_add_block_pattern_hook)
				{
					tuple.dst.u.udp.port = dest_port;
					tuple.dst.protonum = IPPROTO_UDP;

					memset(&mask, 0, sizeof(mask));
					mask.dst.u.udp.port = 0xFFFF;
					mask.dst.protonum = 0x00;
				
#ifdef CONFIG_AMPED_DOS
					if (g_source_block_time)
					{
						sc_add_block_pattern_hook(&tuple, &mask, 0, 0, BLACK_ENTRY, g_source_block_time);
					}
#else
					sc_add_block_pattern_hook(&tuple, &mask, 0, 0, BLACK_ENTRY, UDP_SPECIAL_BLOCK_TIME);
#endif
                    NETGEAR_SYSLOG("19 00[DoS attack: Echo/Chargen Attack] from source: %u.%u.%u.%u:%hu\n",
                                                NIPQUAD((tuple.src.u3.ip)),ntohs(tuple.src.u.all));
				}
				ret = NF_DROP;
			}
		}
		else 
		{
			/* if this is not DMZ, we must consider 4 conditions.
				1. UDP	  dport is echo port (7)
				2. UDP    dport  is chargen port(19)
				3. TCP    dport is echo port (7)
				4. TCP    dport is chargen port (19)
			*/
			for(i = 0; i < MAX_USER_SET; i++)
			if(((state.user_set[i].proto_user)& proto) && (dest_port==state.user_set[i].port_user)) 
			{
				action_queue = & state.user_set[i].queue;
				ct->need_recheck = 1;
				if(enqueue_and_check(action_queue, now, TIMEOUT_PERIOD))
				{
					DEBUG("special udp attack i=%d, proto=%d, dest_port=%d\n", i, proto, ntohs(dest_port));
					// add block action
					if(sc_add_block_pattern_hook)
					{
						tuple.dst.u.udp.port = dest_port;
						tuple.dst.protonum = proto;

						if((state.user_set[i].proto_user) == IPPROTO_BOTH/*(IPPROTO_UDP | IPPROTO_TCP)*/)
						{
							memset(&mask, 0, sizeof(mask));
							mask.dst.u.udp.port = 0xFFFF;
						}
						else
						{
							memset(&mask, 0, sizeof(mask));
							mask.dst.u.udp.port = 0xFFFF;
							mask.dst.protonum = 0xFF;										
						}
						DEBUG("special udp sc_add_block_pattern_hook\n");
#ifdef CONFIG_AMPED_DOS
					if (g_source_block_time)
					{
						sc_add_block_pattern_hook(&tuple, &mask, 0, 0, BLACK_ENTRY, g_source_block_time);
					}
#else
						sc_add_block_pattern_hook(&tuple, &mask, 0, 0, BLACK_ENTRY, UDP_SPECIAL_BLOCK_TIME);
#endif
                        NETGEAR_SYSLOG("19 00[DoS attack: Echo/Chargen Attack] from source: %u.%u.%u.%u:%hu\n",
                                                    NIPQUAD((tuple.src.u3.ip)),ntohs(tuple.src.u.all));
					}
					ret = NF_DROP;

				}
			}

		}
	}

#endif

	/* if tcp_hdr->ack is not set, we do not record this packet,
		* we just record the established incoming tcp session.
		* tcp syn pending will be dealed by other hook */	
//	if (proto == IPPROTO_TCP && ct->proto.tcp.state != TCP_CONNTRACK_ESTABLISHED)
//		goto out;

#ifdef CONFIG_AMPED_DOS
	if(!(g_dos_flag & (1 << SC_DOS_PORTSCAN)))
	    goto out;

    /* high/low sensitivity */
    {
        int scan_max;

        if(g_dos_flag & (1 << SC_DOS_PORTSCAN_HIGH_SENS))
            scan_max = SCAN_MAX_COUNT_HI;
        else
            scan_max = SCAN_MAX_COUNT_LOW;

        if (scan_max != g_port_scan_max_cnt)
        {
            g_port_scan_max_cnt = scan_max;

            //TODO: do we need to reset something?
        }
    }
#endif
		
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
				curr->timestamp = now;
				goto out;
			}
		}

		/* Packet to a new port, and not TCP/ACK: update the timestamp */

		/* Remember the new port */
		if (curr->count < (g_port_scan_max_cnt-1)) {
			curr->ports[curr->queue.rear].number = dest_port;
			curr->ports[curr->queue.rear].timestamp = now;
			curr->ports[curr->queue.rear].proto = proto;
			curr->queue.rear = (curr->queue.rear +1)% g_port_scan_max_cnt;
			curr->timestamp = now;
			curr->count++;
			
			if (proto == IPPROTO_UDP ||
				(proto == IPPROTO_TCP && ct->proto.tcp.state == TCP_CONNTRACK_ESTABLISHED))
				ct->need_passthru = 1;
			DEBUG("tcp port scan same source add ---- %u:%u:%u:%u:%u -> %u:%u:%u:%u:%u\n",
							NIPQUAD((tuple.src.u3.ip)),ntohs(tuple.src.u.all),
							NIPQUAD((tuple.dst.u3.ip)),ntohs(tuple.dst.u.all));

		}
		else
		{
			if((curr->ports[curr->queue.front].timestamp + 10*HZ > now)
			    && time_after_eq(now, (curr->ports[curr->queue.front].timestamp)))
			{
				if(proto == IPPROTO_UDP)
				{
					/* generate a log */
					DEBUG("it is UDP port scan. generate a log --- %u:%u:%u:%u:%u -> %u:%u:%u:%u:%u\n",
									NIPQUAD((tuple.src.u3.ip)),ntohs(tuple.src.u.all),
									NIPQUAD((tuple.dst.u3.ip)),ntohs(tuple.dst.u.all));
					ct->need_recheck = 1;
					if(sc_add_block_pattern_hook)
					{
						memset(&mask, 0, sizeof(mask));
						memset(&mask.src.u3.all, 0xFF, sizeof(mask.src.u3.all));
						mask.dst.protonum = 0xFF;
#ifdef CONFIG_AMPED_DOS
					if (g_source_block_time)
					{
						sc_add_block_pattern_hook(&tuple, &mask, 0, 0, BLACK_ENTRY, g_source_block_time);
					}
#else
						sc_add_block_pattern_hook(&tuple, &mask, 0, 0, BLACK_ENTRY, PORT_SCAN_BLOCK_TIME);
#endif
                        NETGEAR_SYSLOG("19 00[DoS attack: UDP Port Scan] from source: %u.%u.%u.%u:%hu\n",
                                                    NIPQUAD((tuple.src.u3.ip)),ntohs(tuple.src.u.all));
					}
					ret = NF_DROP;
				}
				else if(proto == IPPROTO_TCP)
				{
					/*
						if(close port)
							should be take care by syn flood.
						else if (opend port)
					*/
					DEBUG("tcp port scan detect ---- %u:%u:%u:%u:%u -> %u:%u:%u:%u:%u\n",
									NIPQUAD((tuple.src.u3.ip)),ntohs(tuple.src.u.all),
									NIPQUAD((tuple.dst.u3.ip)),ntohs(tuple.dst.u.all));
					if(sc_add_block_pattern_hook)
					{
						memset(&mask, 0, sizeof(mask));
						memset(&mask.src.u3.all, 0xFF, sizeof(mask.src.u3.all));
						mask.dst.protonum = 0xFF;
#ifdef CONFIG_AMPED_DOS
					if (g_source_block_time)
					{
						sc_add_block_pattern_hook(&tuple, &mask, 0, 0, BLACK_ENTRY, g_source_block_time);
					}
#else
						sc_add_block_pattern_hook(&tuple, &mask, 0, 0, BLACK_ENTRY, PORT_SCAN_BLOCK_TIME);
#endif
                        NETGEAR_SYSLOG("19 00[DoS attack: TCP Port Scan] from source: %u.%u.%u.%u:%hu\n",
                                                    NIPQUAD((tuple.src.u3.ip)),ntohs(tuple.src.u.all));
					}
					ret = NF_DROP;
				}
				/* delete this entry */
				curr->src_addr.s_addr = 0;
				curr->count = 0;
				curr->timestamp = 0;
				atomic_dec(&total_entry);
				if (prev)
					prev->next = prev->next->next;
				else if (*head)
					*head = (*head)->next;	
				write_lock_bh(&free_list_lock);	
				list_add(&curr->list, &free_list);
				write_unlock_bh(&free_list_lock);
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
							curr->queue.front = (curr->queue.front+1)%g_port_scan_max_cnt;
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
				curr->queue.rear = (curr->queue.rear +1)% g_port_scan_max_cnt;
				curr->count++;					
				curr->timestamp = now;
				
				ct->need_passthru = 1;
	
			}
		}

		goto out;
	}


	if (proto == IPPROTO_TCP && ct->proto.tcp.state != TCP_CONNTRACK_SYN_SENT)
	{
//		printk(KERN_EMERG "tcp port scan detect ---- state = %d\n", ct->proto.tcp.state);
		goto out;
	}
		
		DEBUG("tcp port scan new source add ---- %u:%u:%u:%u:%u -> %u:%u:%u:%u:%u\n",
					NIPQUAD((tuple.src.u3.ip)),ntohs(tuple.src.u.all),
					NIPQUAD((tuple.dst.u3.ip)),ntohs(tuple.dst.u.all));


	/* We're going to re-use the oldest list entry, when the entry is exhausted, 
	 * so remove it from the hash table first. */
	if(atomic_read(&total_entry) >= LIST_SIZE)
	{
		if(printk_ratelimit())
			printk("there is no entry for psd, so clear oldest entry\n");
		/* which means there is no list entry available */
		clear_oldest_entry(IPPROTO_TCP);
		clear_oldest_entry(IPPROTO_UDP);
	}

	if(list_empty(&free_list))
	{
		// it should be this case. Just in case.
		if(printk_ratelimit())
			printk("psd why???? something is wrong, total_entry = %d!!!!\n", atomic_read(&total_entry));
		goto out;
	}
	/* Get our list entry */
	write_lock_bh(&free_list_lock);
	list_for_each_entry_safe(curr, next, &free_list, list)
	{
		list_del(&curr->list);
		break;
	}
	write_unlock_bh(&free_list_lock);

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
	curr->dest_addr.s_addr = iph->daddr;
	curr->src_port = src_port;
	curr->count = 1;
	curr->ports[0].number = dest_port;
	curr->ports[0].proto = proto;
	curr->ports[0].timestamp = now;
	curr->queue.rear = curr->queue.rear + 1;
	atomic_inc(&total_entry);


out:
	spin_unlock_bh(&state.lock);
	return ret;
}

#ifdef SUPPORT_ECHO_CHARGEN_DETECT

static ssize_t proc_write_echo_chargen (struct file *file, const char __user *input,
				size_t size, loff_t *ofs){
	char buffer[64];
	int len, i;

	if(size > sizeof(buffer))
		len = sizeof(buffer);
	else
		len = size;

	memset(buffer, 0, sizeof(buffer));
	if(copy_from_user(buffer,input,len))
		return -EFAULT;

	buffer[len - 1] = '\0';

	printk(KERN_INFO " proc_write_echo_chargen receive: len: %d, input: %s\n",len, buffer);

	spin_lock_bh(&state.lock);
	
	/* Check if we are asked to flush the entire table */
	if(!memcmp(buffer,"clear",5)) {
		dmz = 0;
		printk(KERN_INFO " proc_write_echo_chargen receive: clear list\n");
		for(i = 0; i < MAX_USER_SET; i++)
		{
			state.user_set[i].proto_user = 0;
			state.user_set[i].port_user = 0;
			state.user_set[i].queue.base = NULL;
			
		}
	}
	else if (!memcmp(buffer,"dmz",3))
	{
		dmz = 1;
		/*clear first*/
		for(i = 0; i < MAX_USER_SET; i++)
		{
			state.user_set[i].proto_user = 0;
			state.user_set[i].port_user = 0;
			state.user_set[i].queue.base = NULL;
			
		}

		state.user_set[UDP_TCP_ECHO].proto_user = IPPROTO_BOTH/*IPPROTO_UDP | IPPROTO_TCP*/;
		state.user_set[UDP_TCP_ECHO].port_user = __constant_htons(ECHO_PORT);
		queue_init(&state.user_set[UDP_TCP_ECHO].queue,  &state.echo_udp[0], MAX_ECHO_CHARGEN);

		state.user_set[UDP_TCP_CHARGEN].proto_user = IPPROTO_BOTH/*IPPROTO_UDP | IPPROTO_TCP*/;
		state.user_set[UDP_TCP_CHARGEN].port_user = __constant_htons(CHARGEN_PORT);
		queue_init(&state.user_set[UDP_TCP_CHARGEN].queue,  &state.echo_udp[0], MAX_ECHO_CHARGEN);

		printk(KERN_INFO " proc_write_echo_chargen receive: dmz\n");
	}
	else if(!dmz)
	{
		char protocol[16] = "";
		int  port = 0;

		sscanf(buffer, "%s %d", protocol, &port);

		if((strnicmp(protocol, "udp", strlen("udp"))
		   		&& strnicmp(protocol, "tcp", strlen("tcp"))
		   		&& strnicmp(protocol, "tcp/udp", strlen("tcp/udp")))
		   || (port < 0 || port > 65535))
		{
			printk("input format is wrong.\n");
			printk("[protocol] [port_number]\n");
			printk("\tprotocol: udp tcp tcp/udp\n");
			printk("\tport_number: 7 or 19\n");
			goto end;
		}

		if((strnicmp(protocol, "tcp/udp", strlen("tcp/udp")) == 0) && port == (ECHO_PORT))
		{
			printk("UDP_TCP_ECHO queue\n");
			state.user_set[UDP_TCP_ECHO].proto_user = IPPROTO_BOTH/*(IPPROTO_UDP|| IPPROTO_TCP)*/;
			state.user_set[UDP_TCP_ECHO].port_user = __constant_htons(ECHO_PORT);
			queue_init(&state.user_set[UDP_TCP_ECHO].queue,  &state.echo_udp[0], MAX_ECHO_CHARGEN);
		}
		else if((strnicmp(protocol, "tcp/udp", strlen("tcp/udp")) == 0) && port == (CHARGEN_PORT))
		{
			printk("UDP_TCP_CHARGEN queue\n");
			state.user_set[UDP_TCP_CHARGEN].proto_user = IPPROTO_BOTH/*(IPPROTO_UDP|| IPPROTO_TCP)*/;
			state.user_set[UDP_TCP_CHARGEN].port_user = __constant_htons(CHARGEN_PORT);
			queue_init(&state.user_set[UDP_TCP_CHARGEN].queue,  &state.chargen_udp[0], MAX_ECHO_CHARGEN);
		}
		else if((strnicmp(protocol, "udp", strlen("udp")) == 0) && port == (ECHO_PORT))
		{
			printk("UDP_ECHO queue\n");
			state.user_set[UDP_ECHO].proto_user = IPPROTO_UDP;
			state.user_set[UDP_ECHO].port_user = __constant_htons(ECHO_PORT);
			queue_init(&state.user_set[UDP_ECHO].queue,  &state.echo_udp[0], MAX_ECHO_CHARGEN);
		}
		else if((strnicmp(protocol, "udp", strlen("udp")) == 0) && port == (CHARGEN_PORT))
		{
			printk("UDP_CHARGEN queue\n");
			state.user_set[UDP_CHARGEN].proto_user = IPPROTO_UDP;
			state.user_set[UDP_CHARGEN].port_user = __constant_htons(CHARGEN_PORT);
			queue_init(&state.user_set[UDP_CHARGEN].queue,  &state.chargen_udp[0], MAX_ECHO_CHARGEN);

		}
		else if((strnicmp(protocol, "tcp", strlen("tcp")) == 0) && port == (ECHO_PORT))
		{
			printk("TCP_ECHO queue\n");
			state.user_set[TCP_ECHO].proto_user = IPPROTO_TCP;
			state.user_set[TCP_ECHO].port_user = __constant_htons(ECHO_PORT);
			queue_init(&state.user_set[TCP_ECHO].queue,  &state.echo_tcp[0], MAX_ECHO_CHARGEN);
		}
		else if((strnicmp(protocol, "tcp", strlen("tcp")) == 0) && port == (CHARGEN_PORT))
		{
			printk("TCP_CHARGEN queue\n");
			state.user_set[TCP_CHARGEN].proto_user = IPPROTO_TCP;
			state.user_set[TCP_CHARGEN].port_user = __constant_htons(CHARGEN_PORT);
			queue_init(&state.user_set[TCP_CHARGEN].queue,  &state.chargen_tcp[0], MAX_ECHO_CHARGEN);

		}
		else
		{
			printk("something is wrong when setting\n");
			printk("valid input is:\n");
			printk("[clear | dmz | (udp|tcp|tcp/udp) (port)]\n");
		}

	}
	else
	{
		printk(KERN_ERR "Can not accept this command(%s) (dmz = %d)\n", buffer, dmz);
		printk("valid input is:\n");
		printk("[clear | dmz | (udp|tcp|tcp/udp) (port)]\n");
	}

	printk(KERN_INFO " Leaving proc_write_echo_chargen : size: %u\n", size);

end:	
	spin_unlock_bh(&state.lock);

	return size;	
}

struct file_operations proc_echo_chargen_fops =
{
	.owner 	= THIS_MODULE,
	.write 	= proc_write_echo_chargen,
};

#endif

#ifdef SEQ_OPERATION
struct ct_iter_state {
	unsigned int bucket;
};

static int first = 0;

static struct host *psd_get_first(struct seq_file *seq)
{
	struct ct_iter_state *st = seq->private;

	for (st->bucket = 0;
	     st->bucket < (2*HASH_SIZE);
	     st->bucket++) {
		if (state.tcp_hash[st->bucket])
			return state.tcp_hash[st->bucket];
	}
	return NULL;
}

static struct host *psd_get_next(struct seq_file *seq, struct host *head)
{
	struct ct_iter_state *st = seq->private;

	head = head->next;
	while (head == NULL) {
		if (++st->bucket >= (2*HASH_SIZE))
			return NULL;
		head = state.tcp_hash[st->bucket];
	}
	return head;
}

static struct host *psd_get_idx(struct seq_file *seq, loff_t pos)
{
	struct host *head = psd_get_first(seq);

	if (head)
		while (pos && (head = psd_get_next(seq, head)))
			pos--;
	return pos ? NULL : head;
}

static void *psd_seq_start(struct seq_file *seq, loff_t *pos)
{
	spin_lock_bh(&state.lock);
	return psd_get_idx(seq, *pos);
}

static void *psd_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	(*pos)++;
	return psd_get_next(s, v);
}

static void psd_seq_stop(struct seq_file *s, void *v)
{
	spin_unlock_bh(&state.lock);
}

/* return 0 on success, 1 in case of error */
static int psd_seq_show(struct seq_file *s, void *v)
{
	const struct host *curr = v;

	if(first == 1)
	{
		first = 0;
		if(seq_printf(s, "psd udp and tcp vanilla port scan:\n"))
			return -ENOSPC;
		if(seq_printf(s, "\ttotal count:%d\n", atomic_read(&total_entry)))
			return -ENOSPC;
	}	
	if (seq_printf(s, "src ip %u:%u:%u:%u count:%d protocol:%d\n",
									NIPQUAD((curr->src_addr.s_addr)), curr->count, curr->ports[0].proto))
		return -ENOSPC;

	return 0;
}


static struct seq_operations psd_seq_fops = {
	.start = psd_seq_start,
	.next  = psd_seq_next,
	.stop  = psd_seq_stop,
	.show  = psd_seq_show
};


static int psd_open(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	struct ct_iter_state *st;
	int ret;

	st = kmalloc(sizeof(struct ct_iter_state), GFP_KERNEL);
	if (st == NULL)
		return -ENOMEM;
	ret = seq_open(file, &psd_seq_fops);
	if (ret)
		goto out_free;
	seq          = file->private_data;
	seq->private = st;
	memset(st, 0, sizeof(struct ct_iter_state));
	first = 1;
	
//	printk("before show statistic, clear oldest entry first\n");
	
//	clear_oldest_entry(IPPROTO_TCP);
//	clear_oldest_entry(IPPROTO_UDP);
	printk("total count:%d\n", atomic_read(&total_entry));

	return ret;
out_free:
	kfree(st);
	return ret;
}

static ssize_t psd_write (struct file *file, const char __user *input,
				size_t size, loff_t *ofs)
{
	char buffer[64];
	int len;

	
	if(size > sizeof(buffer))
		len = sizeof(buffer);
	else
		len = size;

	memset(buffer, 0, sizeof(buffer));
	if(copy_from_user(buffer,input,len))
		return -EFAULT;
	
	buffer[len] = '\0';

	printk("input str:%s\n", buffer);
	if(!memcmp(buffer,"clear",5)) 
	{
		spin_lock_bh(&state.lock);
		clear_oldest_entry(IPPROTO_TCP);
		clear_oldest_entry(IPPROTO_UDP);
		spin_unlock_bh(&state.lock);
		printk("psd clear oldest entry\n");
	}
	
	return size;

}
static const struct file_operations psd_fops = {
	.owner   = THIS_MODULE,
	.open    = psd_open,
	.read    = seq_read,
	.write   = psd_write,
	.llseek  = seq_lseek,
	.release = seq_release_private,
};
#endif


static int __init init(void)
{
	int i;
#ifdef SUPPORT_ECHO_CHARGEN_DETECT
	struct proc_dir_entry *proc;
#endif
#ifdef SEQ_OPERATION
	struct proc_dir_entry *psd_proc;
#endif

	memset(&state, 0, sizeof(state));

#ifdef SEQ_OPERATION
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
	psd_proc = proc_create ("psd_udp_tcp_vanilla_port_scan", 0, init_net.proc_net, &psd_fops);
#else
	psd_proc = proc_net_fops_create (&init_net, "psd_udp_tcp_vanilla_port_scan", 0, &psd_fops);
#endif
	if (!psd_proc) 
	{
		printk("psd_udp_tcp_vanilla_port_scan  proc create failed\n");
	}
#endif
	write_lock_bh(&free_list_lock);
	for(i = 0; i < LIST_SIZE; i++)
	{
		list_add(&state.list_entry[i].list, &free_list);
	}
	write_unlock_bh(&free_list_lock);
	
	spin_lock_init(&(state.lock));

#ifdef SUPPORT_ECHO_CHARGEN_DETECT
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
	proc = proc_create ("echo_chargen_config", 0, init_net.proc_net, &proc_echo_chargen_fops);
#else
	proc = proc_net_fops_create (&init_net, "echo_chargen_config", 0, &proc_echo_chargen_fops);
#endif

	if (!proc) 
	{
		printk("echo_chargen_config  proc create failed\n");
	}
#endif
	
	sc_psd_and_special_udp_detect_hook = __sc_psd_and_special_udp_detect_hook;

	printk("netfilter PSD and TCP/UDP Echo/Chargen Attack loaded \n");
	return 0;
}

static void __exit fini(void)
{

#ifdef SUPPORT_ECHO_CHARGEN_DETECT
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
	remove_proc_entry("echo_chargen_config", init_net.proc_net);
#else
	proc_net_remove(&init_net, "echo_chargen_config");
#endif
#endif
#ifdef SEQ_OPERATION
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
	remove_proc_entry("psd_udp_tcp_vanilla_port_scan", init_net.proc_net);
#else
	proc_net_remove(&init_net, "psd_udp_tcp_vanilla_port_scan");
#endif
#endif

	sc_psd_and_special_udp_detect_hook = NULL;
	printk("netfilter PSD and TCP/UDP Echo/Chargen Attack unloaded \n");
}

module_init(init);
module_exit(fini);

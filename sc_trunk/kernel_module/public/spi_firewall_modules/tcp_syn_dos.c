#include <linux/init.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/ip.h>
#include <net/tcp.h>
#include <linux/list.h>
#include <linux/version.h>

#include <sc/sc_spi.h>
#include "common.h"

MODULE_LICENSE("GPL");

#define TCP_SYN_FLOOD_BLOCK_TIMEOUT (10*60*HZ)
#define LIST_ENTRY_SIZE		600

#define HASH_LOG			9
#define HASH_SIZE			(1 << HASH_LOG)
//static tcp_src_ip_htable_size = HASH_SIZE;

static struct bysrc_ip_s{
	struct bysrc_ip_s *next;
	struct list_head queue_list;
	struct list_head tcp_syn_bysrc_ip;
	struct in_addr src_addr;	/* Source address */
	atomic_t count;
} bysrc_ip_item[LIST_ENTRY_SIZE]={{0,},};

struct list_head free_queue = LIST_HEAD_INIT(free_queue);
static struct bysrc_ip_s  *bysrc_ip_hash_array[HASH_SIZE];
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
static DEFINE_SPINLOCK(syn_lock);
#else
static spinlock_t 	syn_lock = SPIN_LOCK_UNLOCKED;
#endif

DEFINE_RWLOCK(syn_hash_table_lock);

struct nf_conntrack_tuple tuple;
struct nf_conntrack_tuple mask;

atomic_t total_count  = ATOMIC_INIT(0);


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

//void remove_session_idled_few_secs(unsigned long delta)
//{
//	spin_lock(&syn_lock);
	//list each entry and remove the entry that time idled than delta
//	spin_unlock(&syn_lock);
//}

static int compare_ct_by_tuple(struct nf_conn *i, void *data)
{
	struct nf_conntrack_tuple *tuple = (struct nf_conntrack_tuple *)data;
	if(nf_ct_tuple_equal(&i->tuplehash[IP_CT_DIR_ORIGINAL].tuple, tuple))
	{
		return 1;
	}
	return 0;
}


void delete_all_session_from_same_src(struct bysrc_ip_s *curr)
{
	struct nf_conn *ct;
	struct nf_conntrack_tuple tuple;
	
restart:
	write_lock_bh(&syn_hash_table_lock);
	list_for_each_entry(ct, &curr->tcp_syn_bysrc_ip, tcp_syn_bysrc_ip)
	{
		tuple = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple;
		
		list_del_init(&ct->tcp_syn_bysrc_ip);
		write_unlock_bh(&syn_hash_table_lock);
		nf_ct_iterate_cleanup(&init_net, compare_ct_by_tuple, &tuple);
		goto restart;
	}
	write_unlock_bh(&syn_hash_table_lock);
	
}

void ___sc_tcp_delete_reference_fun(struct nf_conn *ct)
{
	struct bysrc_ip_s *lookup, *curr, **head, *prev;
	if(ct->tcp_syn_head)
	{
		lookup = ct->tcp_syn_head;
		
		atomic_dec(&ct->tcp_syn_head->count);//same src ip conunt increase.

		/* remove unused item from hash table, but be dec by tcp_packet*/
		if( atomic_read(&lookup->count) == 0)
		{
			prev = NULL;
		

			curr = *(head = &bysrc_ip_hash_array[hashfunc(lookup->src_addr)]);
			if( curr )
			{
				do {
					/*find if has the same src ip has come here*/
					if (curr == lookup) 
						break;
					prev = curr;
				} while ((curr = curr->next));
			}


			/* Then, remove it */
			if (curr) {
				if (prev)
					prev->next = prev->next->next;
				else if (*head)
					*head = (*head)->next;
			}
			else
			{
				if(printk_ratelimit())
					printk("something wrong, did't find this tcp entry from the list\n");
			}

			list_add(&curr->queue_list, &free_queue);
			curr->src_addr.s_addr = 0;
			INIT_LIST_HEAD(&curr->tcp_syn_bysrc_ip);
		}

		list_del(&ct->tcp_syn_bysrc_ip);
		ct->tcp_syn_head = NULL;
		INIT_LIST_HEAD(&ct->tcp_syn_bysrc_ip);

					
		atomic_dec(&total_count);//total conunt increase.
	}
	
}

void  __sc_tcp_deal_establish_hook(struct nf_conn *ct)
{
	write_lock_bh(&syn_hash_table_lock);
	___sc_tcp_delete_reference_fun(ct);
	write_unlock_bh(&syn_hash_table_lock);
}

void __sc_tcp_destroy_hook(struct nf_conn *ct)
{
	write_lock_bh(&syn_hash_table_lock);
	___sc_tcp_delete_reference_fun(ct);
	write_unlock_bh(&syn_hash_table_lock);
}

unsigned int __sc_tcp_check_hook(struct nf_conn *ct)
{
	struct bysrc_ip_s *curr, **head, *prev, *next;
	struct in_addr addr;
	int srchash = 0;
	int count_per_src = 0;

	int count = 0;

	prev = NULL;
	if (prev)
	{
		/* let gcc463 not cpmplain. */
	}

	if(ct->proto.tcp.state == TCP_CONNTRACK_NONE/*TCP_CONNTRACK_SYN_SENT*/)
	{
		/* ct in white list will not be scaned */
		if(sc_check_and_block_hook)
		{
			if(sc_check_and_block_hook(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple) == DETECT_PASS)
				return NF_ACCEPT;
		}

		if(atomic_read(&total_count) >= 600)
		{
//			if(printk_ratelimit())
//				printk("block ..... because total_count = %d\n", atomic_read(&total_count));
			return NF_DROP;
		}

		addr.s_addr = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
		// src_hash function should hash any src to 0 ~ HASH_SIZE -1
		srchash = hashfunc(addr);
		
		spin_lock_bh(&syn_lock);
		
		write_lock_bh(&syn_hash_table_lock);
		curr = *(head = &bysrc_ip_hash_array[srchash]);
		if( curr )
		{
			do {
				/*find if has the same src ip has come here*/
				if(curr->src_addr.s_addr == addr.s_addr)
					break;
				count++;


			} while ((curr = curr->next));
		}
		
		if(curr)
		{	/*find out a match record*/
			/* add ct to the item */
			list_add(&ct->tcp_syn_bysrc_ip, &curr->tcp_syn_bysrc_ip);
			ct->tcp_syn_head = curr;
			atomic_inc(&total_count);//total conunt increase.
			atomic_inc(&curr->count);//same src ip increase
			count_per_src = atomic_read(&curr->count);
			memcpy(&tuple, &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple, sizeof(tuple));

			write_unlock_bh(&syn_hash_table_lock);
			
			if(count_per_src > 200)
			{
				delete_all_session_from_same_src(curr);
//				spin_unlock(&syn_lock);
				//block_by_source(10 mins);
				if(sc_add_block_pattern_hook)
				{
					memset(&mask, 0, sizeof(mask));
					memset(&mask.src.u3.all, 0xFF, sizeof(mask.src.u3.all));
					mask.dst.protonum = 0xFF;
				
					sc_add_block_pattern_hook(&tuple, &mask, 0, 0, BLACK_ENTRY, TCP_SYN_FLOOD_BLOCK_TIMEOUT);
                    NETGEAR_SYSLOG("19 00[DoS attack: SYN Flood Attack] from source: %u.%u.%u.%u:%hu\n",
                                                NIPQUAD((tuple.src.u3.ip)),ntohs(tuple.src.u.all));
				}

			}
			
			spin_unlock_bh(&syn_lock);
			goto out;

		}
		
		write_unlock_bh(&syn_hash_table_lock);
		/* new incoming session */
		//this is a new session. find the record��insert in hash table, and count it
		
		if(list_empty(&free_queue))
		{
			// it should not be this case. Just in case.
			if(printk_ratelimit())
				printk("why???? something is wrong, total_count = %d!!!!\n", atomic_read(&total_count));
			spin_unlock_bh(&syn_lock);
			return NF_DROP;
		}
		list_for_each_entry_safe(curr, next, &free_queue, queue_list)
		{
			list_del(&curr->queue_list);
			break;
		}
		
		curr->src_addr.s_addr = addr.s_addr;
		
		/* insert it into the hash table */
		write_lock_bh(&syn_hash_table_lock);
		
		head = & bysrc_ip_hash_array [srchash];
		curr->next = *head;
		*head = curr;
		
		/* add ct to the item */
		list_add(&ct->tcp_syn_bysrc_ip, &curr->tcp_syn_bysrc_ip);
		ct->tcp_syn_head = curr;
		atomic_inc(&total_count);	//total conunt increase.
		atomic_set(&curr->count, 1); //same src ip increase
		
		write_unlock_bh(&syn_hash_table_lock);
		

		spin_unlock_bh(&syn_lock);

#if 0
check:
		if(atomic_read(&total_count) >= 300)
		{
			//remove_session_idled_than_5s;
			//this can be done by tcp conntrack now;
			//remove_session_idled_few_secs(SYN_RCV_TIMEOUT);
		}	
		
		if(atomic_read(&total_count) >= 600)
		{
			/* block_all_tcp_syn_from_internel, we will return NULL immediately,
			 * instead sc_add_block_pattern_hook*/
		}
#endif
	}
	
out:
	return NF_ACCEPT;

}

#ifdef SEQ_OPERATION
struct ct_iter_state {
	unsigned int bucket;
};

static int first = 0;

static struct bysrc_ip_s *tcp_syn_get_first(struct seq_file *seq)
{
	struct ct_iter_state *st = seq->private;

	for (st->bucket = 0;
	     st->bucket < HASH_SIZE;
	     st->bucket++) {
		if (bysrc_ip_hash_array[st->bucket])
			return bysrc_ip_hash_array[st->bucket];
	}
	return NULL;
}

static struct bysrc_ip_s *tcp_syn_get_next(struct seq_file *seq, struct bysrc_ip_s *head)
{
	struct ct_iter_state *st = seq->private;

	head = head->next;
	while (head == NULL) {
		if (++st->bucket >= HASH_SIZE)
			return NULL;
		head = bysrc_ip_hash_array[st->bucket];
	}
	return head;
}

static struct bysrc_ip_s *tcp_syn_get_idx(struct seq_file *seq, loff_t pos)
{
	struct bysrc_ip_s *head = tcp_syn_get_first(seq);

	if (head)
		while (pos && (head = tcp_syn_get_next(seq, head)))
			pos--;
	return pos ? NULL : head;
}

static void *tcp_syn_seq_start(struct seq_file *seq, loff_t *pos)
{
	spin_lock_bh(&syn_lock);
	write_lock_bh(&syn_hash_table_lock);
	return tcp_syn_get_idx(seq, *pos);
}

static void *tcp_syn_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	(*pos)++;
	return tcp_syn_get_next(s, v);
}

static void tcp_syn_seq_stop(struct seq_file *s, void *v)
{
	write_unlock_bh(&syn_hash_table_lock);
	spin_unlock_bh(&syn_lock);
}

/* return 0 on success, 1 in case of error */
static int tcp_syn_seq_show(struct seq_file *s, void *v)
{
	const struct bysrc_ip_s *curr = v;

	if(first == 1)
	{
		first = 0;
		if(seq_printf(s, "tcp syn statistic:\n"))
			return -ENOSPC;
		if(seq_printf(s, "\ttotal count:%d\n", atomic_read(&total_count)))
			return -ENOSPC;
	}	
	if (seq_printf(s, "src ip %u:%u:%u:%u count:%d\n",
										NIPQUAD((curr->src_addr.s_addr)), atomic_read(&curr->count)))
		return -ENOSPC;

	return 0;
}


static struct seq_operations tcp_syn_seq_fops = {
	.start = tcp_syn_seq_start,
	.next  = tcp_syn_seq_next,
	.stop  = tcp_syn_seq_stop,
	.show  = tcp_syn_seq_show
};


static int tcp_syn_open(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	struct ct_iter_state *st;
	int ret;

	st = kmalloc(sizeof(struct ct_iter_state), GFP_KERNEL);
	if (st == NULL)
		return -ENOMEM;
	ret = seq_open(file, &tcp_syn_seq_fops);
	if (ret)
		goto out_free;
	seq          = file->private_data;
	seq->private = st;
	memset(st, 0, sizeof(struct ct_iter_state));
	first = 1;
	printk("tcp syn total count:%d\n", atomic_read(&total_count));
	return ret;
out_free:
	kfree(st);
	return ret;
}



static const struct file_operations tcp_syn_fops = {
	.owner   = THIS_MODULE,
	.open    = tcp_syn_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_private,
};
#endif

static int __init init(void)
{
	int i;
#ifdef SEQ_OPERATION
	struct proc_dir_entry *proc;
#endif
	
	for(i = 0; i < LIST_ENTRY_SIZE; i++)
	{
		list_add(&bysrc_ip_item[i].queue_list, &free_queue);
		INIT_LIST_HEAD(&bysrc_ip_item[i].tcp_syn_bysrc_ip);
	}
	
#ifdef SEQ_OPERATION
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
	proc = proc_create("tcp_syn_statistic", 0, init_net.proc_net, &tcp_syn_fops);
#else
	proc = proc_net_fops_create (&init_net, "tcp_syn_statistic", 0, &tcp_syn_fops);
#endif
	if (!proc) 
	{
		printk("tcp_syn_statistic  proc create failed\n");
	}
#endif
	
	sc_tcp_check_hook = __sc_tcp_check_hook;
	sc_tcp_destroy_hook = __sc_tcp_destroy_hook;
	sc_tcp_deal_establish_hook = __sc_tcp_deal_establish_hook;

	printk("netfilter tcp syn flood module loaded \n");
	return 0;
}

static void __exit fini(void)
{
	
	struct nf_conn *ct, *n;
	int bucket;
	struct bysrc_ip_s *curr;
	
	spin_lock_bh(&syn_lock);

	write_lock_bh(&syn_hash_table_lock);
	printk("enter fini\n");
	for (bucket = 0; bucket < HASH_SIZE; bucket++)
	{
		if ((curr = bysrc_ip_hash_array[bucket]))
		{
			do {
					list_for_each_entry_safe(ct, n, &curr->tcp_syn_bysrc_ip, tcp_syn_bysrc_ip)
					{
						___sc_tcp_delete_reference_fun(ct);
					}
			} while ((curr = curr->next));	
		}
	}
	write_unlock_bh(&syn_hash_table_lock);

	
#ifdef SEQ_OPERATION
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
	remove_proc_entry("tcp_syn_statistic", init_net.proc_net /* parent dir */);
#else
	proc_net_remove(&init_net, "tcp_syn_statistic");
#endif
#endif

	sc_tcp_check_hook = NULL;
	sc_tcp_destroy_hook = NULL;
	sc_tcp_deal_establish_hook = NULL;
	
	spin_unlock_bh(&syn_lock);
	
	printk("netfilter tcp syn flood module unloaded \n");
}

module_init(init);
module_exit(fini);


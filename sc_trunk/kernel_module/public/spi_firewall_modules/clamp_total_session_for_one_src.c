#include <linux/version.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <sc/sc_spi.h>
#include "common.h"

MODULE_LICENSE("GPL");

#define LIST_ENTRY_SIZE		MAX_CONNTRACK_COUNT
#define HASH_LOG			9
#define HASH_SIZE			(1 << HASH_LOG)

#define MAX_SESSION_NUM_FOR_ONE_SRC		100

struct list_head dnat_free_queue = LIST_HEAD_INIT(dnat_free_queue);

atomic_t total_dnat = ATOMIC_INIT(0);

static struct byremote_ip_s{
	struct byremote_ip_s *next;
	struct list_head queue_list;
	struct list_head clamp_byremote_ip;
	struct in_addr src_addr;	/* Source address */
	atomic_t count;
	int		list_count;
} byremote_ip_item[LIST_ENTRY_SIZE]={{0,},};

static struct byremote_ip_s  *byremote_ip_hash_array[HASH_SIZE];
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
static DEFINE_SPINLOCK(detect_lock);
#else
static spinlock_t 	detect_lock = SPIN_LOCK_UNLOCKED;
#endif
DEFINE_RWLOCK(clamp_hash_table_lock);

struct nf_conntrack_tuple tuple;
struct nf_conntrack_tuple mask;

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


void ___sc_remote_destroy_hook(struct nf_conn *ct)
{
	struct byremote_ip_s *lookup, *curr, **head, *prev;
	if(!list_empty(&ct->clamp_byremote_ip))
	{
		
		ct->clamp_head->list_count--;
		if(ct->clamp_confirmed && atomic_read(&ct->clamp_head->count) > 0)
			atomic_dec(&ct->clamp_head->count);//decrease same src ip count

		// delete the limitation
		if(atomic_read(&ct->clamp_head->count) == (MAX_SESSION_NUM_FOR_ONE_SRC - 1))
		{
			DEBUG("del block action srcip (%u:%u:%u:%u)\n", NIPQUAD((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip)));

			if(sc_delete_block_pattern_hook)
				sc_delete_block_pattern_hook(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple, 0, 0, BLACK_ENTRY);
		}
		
				/* remove unused item from hash table, but be dec by tcp_packet*/
		lookup = ct->clamp_head;
		if(lookup->list_count == 0)
		{
			prev = NULL;
			curr = *(head = &byremote_ip_hash_array[hashfunc(lookup->src_addr)]);
			if( curr )
			{
				do {
					/*find if has the same src ip has come here*/
					if (curr == lookup) 
						break;
					prev = curr;
				} while ((curr = curr->next));
			}
			else
			{
				if(printk_ratelimit())
					printk("something wrong, head is NULL clamping\n");
			}
			


			/* Then, remove it */
			if (curr) {
				if (prev)
					prev->next = prev->next->next;
				else if (*head)
					*head = (*head)->next;

				list_add(&curr->queue_list, &dnat_free_queue);
				curr->src_addr.s_addr = 0;
				INIT_LIST_HEAD(&curr->clamp_byremote_ip);
				atomic_dec(&total_dnat);
			}
			else
			{
				if(printk_ratelimit())
					printk("something wrong, did't find this entry from the list\n");
			}
		}
		
		list_del(&ct->clamp_byremote_ip);
		ct->clamp_head = NULL;
		INIT_LIST_HEAD(&ct->clamp_byremote_ip);
	}
}

void __sc_remote_destroy_hook(struct nf_conn *ct)
{
	write_lock_bh(&clamp_hash_table_lock);
	___sc_remote_destroy_hook(ct);
	write_unlock_bh(&clamp_hash_table_lock);
}
unsigned int __sc_detect_total_session_for_one_host_hook(struct sk_buff *skb, struct nf_conn *ct)
{
	struct byremote_ip_s *curr, **head, *prev, *next;
	struct in_addr addr;
	int srchash = 0;
	struct nf_conntrack_tuple *tuple_pt;
	int count = 0;
	unsigned int ret = NF_ACCEPT;

	/* Some local service packet should not be check and count,
	   we will check if local_service is 1. For instance remote management */
	if(ct->local_service)
		return ret;
		
	tuple_pt = &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple;
	prev = NULL;
	if (prev)
	{
		/* let gcc463 not complain. */
	}
	if(tuple_pt->src.l3num == PF_INET)
	{
		addr.s_addr = tuple_pt->src.u3.ip;
		// src_hash function should hash any src to 0 ~ HASH_MAX-1
		srchash = hashfunc(addr);


		spin_lock_bh(&detect_lock);
		write_lock_bh(&clamp_hash_table_lock);
		curr = *(head = &byremote_ip_hash_array[srchash]);
		if( curr )
		{
			do {
				/*check if we have seen this source ip packets*/
				if(curr->src_addr.s_addr == addr.s_addr)
					break;
				count++;

#if 0
				/* remove unused item from hash table, but be dec by tcp_packet*/
				if( atomic_read(&curr->count) == 0)
				{
					curr->src_addr.s_addr = 0;
					if (prev)
						prev->next = prev->next->next;
					else if (*head)
						*head = (*head)->next;
					list_add(&curr->queue_list, &dnat_free_queue);
					INIT_LIST_HEAD(&curr->clamp_byremote_ip);
				}
				if (curr->next) 
					prev = curr;
#endif
			} while ((curr = curr->next));
		}
		
		if(curr)
		{	/*has find out the record*/
			/* add ct to the item */
			list_add(&ct->clamp_byremote_ip, &curr->clamp_byremote_ip);
			ct->clamp_head = curr;
			//we will count it later, when it is assured.
			//atomic_inc(&curr->count);//increase same src ip count
			curr->list_count++;
			if(atomic_read(&curr->count) >= MAX_SESSION_NUM_FOR_ONE_SRC)
				ret = NF_DROP;
	
			write_unlock_bh(&clamp_hash_table_lock);

			goto end;

		}
		
		write_unlock_bh(&clamp_hash_table_lock);
		
		/* new incoming session */
		//this is a new session. find out the record, fill in hash talbe, and count it
		
			
		if(list_empty(&dnat_free_queue))
		{
			// it should not be this case, because we have same size of conntrack count, Just in case.
			if(printk_ratelimit())
				printk("clamp session queue is empty, why????? bug !!!!! total_entry used (%d)\n",atomic_read(&total_dnat));
			goto end;
		}
		list_for_each_entry_safe(curr, next, &dnat_free_queue, queue_list)
		{
			list_del(&curr->queue_list);
			break;
		}
			
		curr->src_addr.s_addr = addr.s_addr;
		
		/* insert it into the hash table */
		write_lock_bh(&clamp_hash_table_lock);
		head = &byremote_ip_hash_array[srchash];
		curr->next = *head;
		*head = curr;
		
		/* add ct to the item */
		list_add(&ct->clamp_byremote_ip, &curr->clamp_byremote_ip);
		ct->clamp_head = curr;
		atomic_inc(&total_dnat);
		atomic_set(&curr->count, 0);
		//we will count it later, when it is assured.
		//atomic_inc(&curr->count); 	//increase same src ip count
		curr->list_count = 1;
		write_unlock_bh(&clamp_hash_table_lock);

end:
		spin_unlock_bh(&detect_lock);
	
		return ret;
	}
	return ret;

}


/* this packet is in established state, we should refresh its timer,
 * otherwise, this session is timeout value is too long. */ 
unsigned int __sc_clamp_is_clamped_session_hook(struct nf_conn *ct)
{
	unsigned int ret = NF_ACCEPT;

	spin_lock_bh(&detect_lock);
	write_lock_bh(&clamp_hash_table_lock);
	if(!list_empty(&ct->clamp_byremote_ip))
	{
			ret = NF_DROP;
	}
	write_unlock_bh(&clamp_hash_table_lock);
	spin_unlock_bh(&detect_lock);

	return ret;
}

unsigned int __sc_clamp_refresh_hook(struct nf_conn *ct)
{
	int count_per_src = 0;
	unsigned int ret = NF_ACCEPT;

	spin_lock_bh(&detect_lock);
	write_lock_bh(&clamp_hash_table_lock);
	if(!list_empty(&ct->clamp_byremote_ip))
	{
		/* in case tcp ct from close to establish again */
		if(!ct->clamp_confirmed)
		{
			atomic_inc(&ct->clamp_head->count);
			ct->clamp_confirmed = 1;
			ct->need_passthru = 1;
			write_unlock_bh(&clamp_hash_table_lock);
		}
		else
		{
			write_unlock_bh(&clamp_hash_table_lock);
			goto out;
		}
		
		count_per_src = atomic_read(&ct->clamp_head->count);

		if(count_per_src == MAX_SESSION_NUM_FOR_ONE_SRC)
		{
			if(sc_add_block_pattern_hook)
			{
				memcpy(&tuple, &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple, sizeof(tuple));
				DEBUG("add block action srcip (%u:%u:%u:%u)\n", NIPQUAD((tuple.src.u3.ip)));
				//delete_all_session_from_same_src(&curr->clamp_byremote_ip);
				//block_by_source(10 mins);
				memset(&mask, 0, sizeof(mask));
				memset(&mask.src.u3.all, 0xFF, sizeof(mask.src.u3.all));
				
				sc_add_block_pattern_hook(&tuple, &mask, 0, 0, BLACK_ENTRY, BLOCK_INFINITE_TIME);
                NETGEAR_SYSLOG("19 00[DoS attack: Too many sessions] from source: %u.%u.%u.%u:%hu\n",
                                            NIPQUAD((tuple.src.u3.ip)),ntohs(tuple.src.u.all));
			}
		}	
		if(count_per_src > MAX_SESSION_NUM_FOR_ONE_SRC)
		{
			DEBUG("in clamp, curr number is %d for src(%u:%u:%u:%u)\n", count_per_src, NIPQUAD((tuple.src.u3.ip)));
			ret = NF_DROP;
		}

	}
	else
	{
		write_unlock_bh(&clamp_hash_table_lock);
	}
	

out:	
	spin_unlock_bh(&detect_lock);
	return ret;
}

#ifdef SEQ_OPERATION
struct ct_iter_state {
	unsigned int bucket;
};

static int first = 0;

static struct byremote_ip_s *clamp_get_first(struct seq_file *seq)
{
	struct ct_iter_state *st = seq->private;

	for (st->bucket = 0;
	     st->bucket < HASH_SIZE;
	     st->bucket++) {
		if (byremote_ip_hash_array[st->bucket])
			return byremote_ip_hash_array[st->bucket];
	}
	return NULL;
}

static struct byremote_ip_s *clamp_get_next(struct seq_file *seq, struct byremote_ip_s *head)
{
	struct ct_iter_state *st = seq->private;

	head = head->next;
	while (head == NULL) {
		if (++st->bucket >= HASH_SIZE)
			return NULL;
		head = byremote_ip_hash_array[st->bucket];
	}
	return head;
}

static struct byremote_ip_s *clamp_get_idx(struct seq_file *seq, loff_t pos)
{
	struct byremote_ip_s *head = clamp_get_first(seq);

	if (head)
		while (pos && (head = clamp_get_next(seq, head)))
			pos--;
	return pos ? NULL : head;
}

static void *clamp_seq_start(struct seq_file *seq, loff_t *pos)
{
	spin_lock_bh(&detect_lock);
	write_lock_bh(&clamp_hash_table_lock);
	return clamp_get_idx(seq, *pos);
}

static void *clamp_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	(*pos)++;
	return clamp_get_next(s, v);
}

static void clamp_seq_stop(struct seq_file *s, void *v)
{
	write_unlock_bh(&clamp_hash_table_lock);
	spin_unlock_bh(&detect_lock);
}

/* return 0 on success, 1 in case of error */
static int clamp_seq_show(struct seq_file *s, void *v)
{
	const struct byremote_ip_s *curr = v;

	if(first == 1)
	{
		first = 0;
		if(seq_printf(s, "clamp session statistic:\n"))
			return -ENOSPC;
		if(seq_printf(s, "\ttotal entry used:%d\n", atomic_read(&total_dnat)))
			return -ENOSPC;
	}	
	if (seq_printf(s, "src ip %u:%u:%u:%u count_assured:%d list_count:%d\n",
										NIPQUAD((curr->src_addr.s_addr)), atomic_read(&curr->count),curr->list_count))
		return -ENOSPC;

	return 0;
}


static struct seq_operations clamp_seq_fops = {
	.start = clamp_seq_start,
	.next  = clamp_seq_next,
	.stop  = clamp_seq_stop,
	.show  = clamp_seq_show
};


static int clamp_open(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	struct ct_iter_state *st;
	int ret;

	st = kmalloc(sizeof(struct ct_iter_state), GFP_KERNEL);
	if (st == NULL)
		return -ENOMEM;
	ret = seq_open(file, &clamp_seq_fops);
	if (ret)
		goto out_free;
	seq          = file->private_data;
	seq->private = st;
	memset(st, 0, sizeof(struct ct_iter_state));
	first = 1;
	printk("clamp total entry used:%d\n", atomic_read(&total_dnat));
	return ret;
out_free:
	kfree(st);
	return ret;
}



static const struct file_operations clamp_fops = {
	.owner   = THIS_MODULE,
	.open    = clamp_open,
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
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
	proc = proc_create ("clamp_total_session_for_one_src", 0, init_net.proc_net, &clamp_fops);
#else
	proc = proc_net_fops_create (&init_net, "clamp_total_session_for_one_src", 0, &clamp_fops);
#endif
	if (!proc) 
	{
		printk("clamp_total_session_for_one_src  proc create failed\n");
	}
#endif
	
	for(i = 0; i < LIST_ENTRY_SIZE; i++)
	{
		list_add(&byremote_ip_item[i].queue_list, &dnat_free_queue);
		INIT_LIST_HEAD(&byremote_ip_item[i].clamp_byremote_ip);
	}
	
	sc_detect_total_session_for_one_host_hook = __sc_detect_total_session_for_one_host_hook;
	sc_remote_destroy_hook = __sc_remote_destroy_hook;
	sc_clamp_refresh_hook = __sc_clamp_refresh_hook;
	sc_clamp_is_clamped_session_hook = __sc_clamp_is_clamped_session_hook;

	printk("netfilter clamp total session for one src ip module loaded \n");
	return 0;
}

static void __exit fini(void)
{
	struct nf_conn *ct, *n;
	int bucket;
	struct byremote_ip_s *curr;
	
	spin_lock_bh(&detect_lock);

	write_lock_bh(&clamp_hash_table_lock);
	for (bucket = 0; bucket < HASH_SIZE; bucket++)
	{
		if ((curr = byremote_ip_hash_array[bucket]))
		{
			do {
					list_for_each_entry_safe(ct, n, &curr->clamp_byremote_ip, clamp_byremote_ip)
					{
						___sc_remote_destroy_hook(ct);
					}
			} while ((curr = curr->next));	
		}
	}
	write_unlock_bh(&clamp_hash_table_lock);
	
#ifdef SEQ_OPERATION	
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0)
	remove_proc_entry("clamp_total_session_for_one_src", init_net.proc_net);
#else
	proc_net_remove(&init_net, "clamp_total_session_for_one_src");
#endif
#endif

	sc_detect_total_session_for_one_host_hook = NULL;
	sc_remote_destroy_hook = NULL;
	sc_clamp_refresh_hook = NULL;
	sc_clamp_is_clamped_session_hook = NULL;
	
	spin_unlock_bh(&detect_lock);
	
	printk("netfilter clamp total session for one src ip module unloaded \n");
}

module_init(init);
module_exit(fini);




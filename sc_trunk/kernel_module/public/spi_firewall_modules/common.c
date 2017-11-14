#include <linux/init.h>
#ifndef EXPORT_SYMTAB
#define EXPORT_SYMTAB           /* need this one 'cause we export symbols */
#endif
#include <linux/module.h>
#include <linux/time.h>
#include <linux/timer.h>
#include "common.h"

int queue_init(queue_t *q, unsigned long *base, int queue_size)
{
	q->base = base;
	q->size = queue_size;
	q->front = q->rear = 0;
	return 0;
}

int queue_len(queue_t *q)
{
	return (q->rear - q->front + q->size)%q->size;
}

int enqueue(queue_t *q, unsigned long elem)
{
	if(!q->base)
	{
		if(printk_ratelimit())
			printk("uninited queue\n");
		return 1;
	}
	if(((q->rear+1)%q->size) == q->front)
	{
		return 1;
	}
	q->base[q->rear] = elem;
	q->rear = (q->rear +1)% q->size;
	return 0;
}

int dequeue(queue_t *q, unsigned long *elem)
{
	if(!q->base)
		return 1;
	if(q->front == q->rear)
		return 1;
	*elem = q->base[q->front];
	q->front = (q->front+1)%q->size;
	return 0;
}


int get_queue_elem(queue_t *q, unsigned long *elem)
{
	if(!q->base)
		return 1;
	if(q->front == q->rear)
		return 1;
	*elem = q->base[q->front];
	return 0;
}

/******************************************************************************
	** INPUT parameters
	**			q: 		operation will be on this object
	**			now: 	the jiffies which will be record 
	**			delta: 	timeout value which will be used to check every entry
	** Return: 0 or 1
	** 0: needn't action
	** 1: need action
*******************************************************************************/
int enqueue_and_check(queue_t *q, unsigned long now, unsigned long delta) 
{
	unsigned long tmp_elem = 0;
	
	if (enqueue(q, now))
	{ 
		//enqueue fail, it must be full in the queue;
		if (get_queue_elem(q, &tmp_elem) == 0)
		{
			/*check if in 10s, if TRUE, block it. 
				otherwise delete oldest and insert this one */
			if( (now - tmp_elem < delta) && time_after_eq(now, tmp_elem))
			{
				DEBUG("enqueue fail and should be a dos attached\n");
				return 1;
			}
			else
			{
				//clean these entries that timeout.
				for(;q->rear != q->front;)
				{
					if((now - q->base[q->front] > delta)
						&& time_after_eq(now, q->base[q->front]))
					{
						dequeue(q, &tmp_elem);
					}
					else
					{
						break;
					}
				}
				//insert new entry, must be successful
				if(enqueue(q, now))
				{
					if(printk_ratelimit())
						printk("has delete old entry and enqueu fail again, why????\n");
				}
				
				return 0;
			}
		}
	}
	return 0;
}

static int init(void)
{
	printk("init common module successful\n");
	return 0;
}

static void fini(void)
{
	printk("remove common mudule successful\n");
}

EXPORT_SYMBOL(enqueue_and_check);
EXPORT_SYMBOL(queue_init);
EXPORT_SYMBOL(queue_len);
EXPORT_SYMBOL(enqueue);
EXPORT_SYMBOL(dequeue);
EXPORT_SYMBOL(get_queue_elem);

module_init(init);
module_exit(fini);

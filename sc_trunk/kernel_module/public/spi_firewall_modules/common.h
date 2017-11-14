#ifndef __COMMON_H__
#define __COMMON_H__

#include <linux/param.h>
#include <linux/types.h>
#include <linux/time.h>
#include "debug.h"



typedef struct{
	unsigned long *base;
	int front;
	int rear;
	int size;	// queue size.
}queue_t;

extern int queue_len(queue_t *q);
extern int queue_init(queue_t *q, unsigned long *base, int queue_size);
extern int enqueue(queue_t *q, unsigned long elem);
extern int dequeue(queue_t *q, unsigned long *elem);
extern int get_queue_elem(queue_t *q, unsigned long *elem);
extern int enqueue_and_check(queue_t *q, unsigned long now, unsigned long delta);


#endif

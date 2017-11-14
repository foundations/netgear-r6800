
/***************************************************************************
 File info: log queue API implementation.
 Author: Sercomm Corp.
 Date: Jul.16 2011
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#include "log_queue.h"

static LIST_HEAD(lw_head);
static LIST_HEAD(lw_free);
static struct log_queue_t log_queue[LOG_QUEUE_NUM];
/* Trace log number in window. */
static int entires_in_window = 0;

/* Check if ntp got time. */
static inline int check_time_ok(time_t *now)
{
    struct tm tm_now;

    localtime_r(now, &tm_now);
    /* a simple way to check if we have received the right time. */
    return ((tm_now.tm_year + 1900) > 2010);
}

int lq_get_entries_in_window(void)
{
    return entires_in_window;
}

struct log_queue_t *lq_get_queue(int qid)
{
    if (qid >= 0 && qid < LOG_QUEUE_NUM)
        return &log_queue[qid];
    return NULL;
}

static void lq_free_window(struct log_window_t *pwnd)
{
    lq_debug("entering \n");

    if (pwnd->plog) {
        pwnd->plog->pwnd = NULL;
        pwnd->plog = NULL;
    }
    list_add(&pwnd->list, &lw_free);
    entires_in_window--;
}

/* cleanup the log window. */
void lq_cleanup_window(void)
{
    struct list_head *entry;
    struct log_window_t *pwnd;

    lq_debug("entering \n");

    list_for_each(entry, &lw_head) {
        pwnd = list_entry(entry, struct log_window_t, list);
        /* kick it out of window. */
        pwnd->plog->pwnd = NULL;
        pwnd->plog = NULL;
    }
    /* free all. */
    list_splice_init(&lw_head, &lw_free);
    entires_in_window = 0;
}

/* allocate a new log window entry. when `allow_reclaim' is true, the oldest one
 * will be reused when no free one is available. */
static struct log_window_t* lq_alloc_window(int allow_reclaim)
{
    struct list_head *new_entry;
    struct log_window_t *new = NULL;

    if (!list_empty(&lw_free)) {
        new_entry = lw_free.next;
        list_del_init(new_entry);
        new = list_entry(new_entry, struct log_window_t, list);
        entires_in_window++;
    } else if (allow_reclaim) {
        /* reclaim the oldest one */
        new_entry = lw_head.prev;
        list_del_init(new_entry);
        new = list_entry(new_entry, struct log_window_t, list);
        /* kick this log out of window */
        new->plog->pwnd = NULL;
        new->plog = NULL;
        lq_debug("reuse oldest window: %p \n", new_entry);
    }
    return new;
}

/* refill log window with log queue. */  
void lq_refill_window(struct log_queue_t *q)
{
    struct list_head *curr;
    struct log_window_t *pwnd;
    struct log_entity_t *plog;
    int i;

    curr = lw_head.next;
    for (i = q->head; i != q->tail; i = q_next(i, q->qlen)) {
        /* insert it before the one which timestamp less than this one, insert this one before it. */
        plog = &q->lq[i];
        for (; curr != &lw_head; curr = curr->next) {
            pwnd = list_entry(curr, struct log_window_t, list);
            if (pwnd->plog->timestamp < plog->timestamp)
                break;
        }
        /* only allow reclaim when its timestamp is later than someone in window. */
        pwnd = lq_alloc_window(curr != &lw_head);
        if (!pwnd) {
            lq_debug("left logs are too old to add to window!\n");
            break;
        }
        pwnd->plog = plog;
        plog->pwnd = pwnd;
        /* insert before it. curr may have been dellist and reused, be careful! */
        list_add_tail(&pwnd->list, (curr != &pwnd->list) ? curr : &lw_head);
    }
}

/* `__print_func' is used to format the log message. */
void lq_print_window(const char *filename, char * (*__print_func)(struct log_entity_t*))
{
    struct log_window_t *pwnd;
    struct log_entity_t *plog;
    struct list_head *entry;
    char *out_msg;
    time_t now;
    struct sysinfo info;
    FILE *fp;
    int time_ok;

    lq_debug("entering \n");

    time(&now);
    sysinfo(&info);
    time_ok = check_time_ok(&now);

    fp = fopen(filename, "w");
    if (!fp)
        return ;

    list_for_each(entry, &lw_head) {
        pwnd = list_entry(entry, struct log_window_t, list);
        plog = pwnd->plog;

        /* Fixup abs_timestamp. */
        if (time_ok && plog->abs_timestamp == 0)
            plog->abs_timestamp = now + plog->timestamp - info.uptime;

         if (__print_func)
            out_msg = __print_func(plog);
         else
            out_msg = plog->msg;

         fprintf(fp, out_msg);
         /* debug. */
         /* printf("%s", out_msg); */
    }
    fclose(fp);
}

int lq_enqueue(struct log_queue_t *q, const char *msg)
{
    struct log_entity_t *plog;
    struct log_window_t *pwnd;
    struct sysinfo info;
    time_t now;

    lq_debug("queue is %sfull \n", q_full(q)?"":"not ");

    if (q_full(q)) {
        /* discard oldest log in queue. */
        q->tail = q_prev(q->tail, q->qlen);
        plog = &q->lq[q->tail];
        pwnd = plog->pwnd;
        /* if this log is in window, remove it. */
        if (pwnd) {
            list_del_init(&pwnd->list);
            lq_free_window(pwnd);
        }
        memset(plog, 0, sizeof(struct log_entity_t));
    }

    q->head = q_prev(q->head, q->qlen);
    plog = &q->lq[q->head];
    strncpy(plog->msg, msg, PER_LOG_BUF_SIZE);

    sysinfo(&info);
    plog->timestamp = info.uptime;
    time(&now);
    plog->abs_timestamp = (check_time_ok(&now)) ? now : 0;

    lq_debug("new log@[%d,%d] <%s>, timestamp = %ld \n", q->head, q->tail, plog->msg, plog->timestamp);

    /* add it to log window */
    pwnd = lq_alloc_window(1);
    pwnd->plog = plog;
    plog->pwnd = pwnd;
    list_add(&pwnd->list, &lw_head);

    return 0;
}

void lq_cleanup_queue(struct log_queue_t *q)
{
    q->head = q->tail = 0;
    memset(q->lq, 0, sizeof(q->lq));
}

void lq_cleanup(void)
{
    int i;

    lq_debug("entering \n");

    lq_cleanup_window();

    for (i = 0; i < LOG_QUEUE_NUM; i++) {
        lq_cleanup_queue(&log_queue[i]);
    }
}

int lq_init(void)
{
    struct log_window_t *pwnd;
    int i;

    INIT_LIST_HEAD(&lw_head);
    INIT_LIST_HEAD(&lw_free);

    pwnd = calloc(LOG_WINDOW_SIZE, sizeof(struct log_window_t));
    if (!pwnd) {
        lq_debug("out of memory!\n");
        return -1;
    }
    for (i = 0; i < LOG_WINDOW_SIZE; i++) {
        list_add(&pwnd[i].list, &lw_free);
    }

    for (i = 0; i < LOG_QUEUE_NUM; i++) {
        log_queue[i].qlen = LOG_QUEUE_SIZE;
    }

    return 0;
}

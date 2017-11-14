#ifndef __LOG_QUEUE_H
#define __LOG_QUEUE_H

#include "list.h"
#include "sc_debug.h"

#ifdef _SC_DEBUG_ENABLE_
#define lq_debug SC_CFPRINTF
#else
#define lq_debug(fmt, args...)
#endif

/* The following definition can be changed to match requirement, */
#ifdef AMPED
#define LOG_WINDOW_SIZE   1024
#define LOG_QUEUE_SIZE   (1024 + 1) /* 1 more for queue management */
#define LOG_QUEUE_NUM   13    /* the category of log */
#define PER_LOG_BUF_SIZE   (128 + 80) // add more buffer for '\n', '\0'
#else
#define LOG_WINDOW_SIZE   256
#define LOG_QUEUE_SIZE   (256 + 1) /* 1 more for queue management */
#define LOG_QUEUE_NUM   13    /* the category of log */
#define PER_LOG_BUF_SIZE   (128 + 4) // add more buffer for '\n', '\0'
#endif

struct log_window_t;

struct log_entity_t {
    long timestamp;    /* timestamp since bootup. */
    time_t abs_timestamp;   /* absolute timestamp since 1970... */
    struct log_window_t *pwnd;   /* backward pointer to log window */
    char msg[PER_LOG_BUF_SIZE];
};

struct log_queue_t {
    int head;
    int tail;
    int qlen;
    struct log_entity_t lq[LOG_QUEUE_SIZE];
};

struct log_window_t {
    struct list_head list;
    struct log_entity_t *plog;  
};

static inline int q_next(int i, int ql)
{
    return (++i == ql ? 0 : i);
}

static inline int q_prev(int i, int ql)
{
    return (--i < 0 ? (ql-1) : i);
}

static inline int q_empty(struct log_queue_t *q)
{
    return (q->head == q->tail);
}

static inline int q_full(struct log_queue_t *q)
{
    return (q_next(q->tail, q->qlen) == q->head);
}

extern int lq_get_entries_in_window(void);
extern void lq_cleanup_window(void);
extern void lq_refill_window(struct log_queue_t *q);
extern void lq_print_window(const char *filename, char * (*__print_func)(struct log_entity_t*));
extern struct log_queue_t *lq_get_queue(int qid);
extern void lq_cleanup_queue(struct log_queue_t *q);
extern int lq_enqueue(struct log_queue_t *q, const char *msg);
extern void lq_cleanup(void);
extern int lq_init(void);

#ifdef DSL_WIZARD_LOG
extern int dsl_lq_get_entries_in_window(void);
extern void dsl_lq_cleanup_window(void);
extern void dsl_lq_refill_window(struct log_queue_t *q);
extern void dsl_lq_print_window(const char *filename, char * (*__print_func)(struct log_entity_t*));
extern struct log_queue_t *dsl_lq_get_queue(int qid);
extern void dsl_lq_cleanup_queue(struct log_queue_t *q);
extern int dsl_lq_enqueue(struct log_queue_t *q, const char *msg);
extern void dsl_lq_cleanup(void);
extern int dsl_lq_init(void);
#endif

#endif

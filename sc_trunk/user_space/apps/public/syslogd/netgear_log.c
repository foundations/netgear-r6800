
/***************************************************************************
 File info: Netgear logging implementation based on log queue.
 Author: Sercomm Corp.
 Date: Jul.16 2011
****************************************************************************/

#include "log_queue.h"

#ifdef DSL_WIZARD_LOG
#include "flash.h"
#include "nvram.h"
extern int is_DSL_Log;
static char mac_buf[20] = "";
static char serial_buf[BUF_LEN_SN+2] = "";
#endif

static unsigned int log_queue_map[LOG_QUEUE_NUM] = {
   (1<<14),
   ((1<<14) | (1<<15)),
   ((1<<10) | (1<<11) | (1<<12) | (1<<13)),
   ((1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<7) | (1<<8) | (1<<9) | (1<<16) | (1<<17) | (1<<21) | (1<<22)),
   (1<<19),
   (1<<20),
   ((1<<6) | (1<<18)),
   ((1<<23) | (1<<24) | (1<<25)),
   (1<<26),
   (1<<27),
   (1<<28),
   (1<<29),
   (1<<31),
};
#ifdef DSL_WIZARD_LOG
/* Got WAN mac address and serial number first */
void log_info_init(void)
{
	char sn[BUF_LEN_SN] = "";
	
	sc_get_sn(sn, BUF_LEN_SN);
	snprintf(serial_buf, BUF_LEN_SN+2, "[%s]", sn);
	sprintf(mac_buf, "[%s]", nvram_safe_get("wan_def_mac"));
}
#endif
/* Check if this queue should be in log window. */
static int queue_in_window(int qid)
{
    int bits = log_queue_map[qid];
    
    if (!(bits & conf.log_event))
        return 0;
    if (qid == 0) {
        if ((conf.log_event & (1<<14)) && (conf.log_minor_event & (1<<1)))
            return 1;
        return 0;
    }
    if (qid == 1) {
        if (conf.log_event & (1<<15))
            return 1;
        if ((conf.log_event & (1<<14)) && (conf.log_minor_event & (1<<2)))
            return 1;
        return 0;
    }
    return 1;
}

/* Match the log event to queue. */
static int log_event_queue(int major, int minor)
{
    int i;

    if (major == 14) {
        if (minor == 1)
            return 0;
        else
            return 1;
    }
    for (i = 0; i < LOG_QUEUE_NUM; i++) {
        if (log_queue_map[i] & (1<<major))
            return i;
    }
    return -1;
}

static int NTGR_log_isfull(void)
{
    int count = lq_get_entries_in_window();
#if (_LOG_FULL_PERCENT_ < 100)
    int major = (count * 100) / LOG_WINDOW_SIZE;
    int minor = (count * 100) % LOG_WINDOW_SIZE;

    major += (minor > 0)?1:0;
    return (major > _LOG_FULL_PERCENT_);
#else
    return (count == LOG_WINDOW_SIZE);
#endif
}

/* check if previous log is the same as input log `msg'. */
static int NTGR_log_isduplicated(int major, int minor, const char *msg)
{
    int qid = log_event_queue(major, minor);
    struct log_queue_t *q;
    struct log_entity_t *plog;

    if (qid < 0)
        return 0;

    q = lq_get_queue(qid);
    if (!q || q_empty(q))
        return 0;

    /* Only check the first log in queue. */
    plog = &q->lq[q->head];

    return (strncmp(msg, plog->msg, strlen(msg)) == 0);
}

#ifdef AMPED
static int msg_include_time(char *msg)
{
	int i=0;
	for (i=0; i<7; i++)
	    if( strstr(msg , wday[i]) != NULL)
		return 1;
	return 0;
}
#endif

/* format the log message. */
static char * NTGR_log_print1(struct log_entity_t *plog)
{
    static char out_msg[PER_LOG_BUF_SIZE + 128];
    char time_buf[128];
    struct tm *resume_tm = NULL;
    char *p;
#ifdef AMPED
    memset(out_msg, 0, sizeof(out_msg));
#endif

    strcpy(out_msg, plog->msg);

    if (plog->abs_timestamp > 0) {
        resume_tm = localtime(&plog->abs_timestamp);
        sprintf(time_buf," %s, %s %02d,%02d %02d:%02d:%02d"
            ,wday[resume_tm->tm_wday]
            ,month[resume_tm->tm_mon]
            ,resume_tm->tm_mday
            ,resume_tm->tm_year+1900
            ,resume_tm->tm_hour
            ,resume_tm->tm_min
            ,resume_tm->tm_sec);

        p = strstr(out_msg, EMPTY_TIMESTAMP);
        if (p
#ifdef AMPED
	&& !msg_include_time(out_msg) /* when msg does not contain time */
#endif
	)
            strncpy(p, time_buf, strlen(time_buf));
    }
    return out_msg;
}
#ifdef DSL_WIZARD_LOG
/* DSL log format is [CK_XX][mac][sn][time]msg */
static char * NTGR_log_print2(struct log_entity_t *plog)
{
    static char out_msg[PER_LOG_BUF_SIZE + 128];
    char msg_header[32] = "";
    char msg_tail[128] = "";
    char time_buf[128] = "";
    struct tm *resume_tm = NULL;
//    char *p;
    char *pt;
    int header_len = 0;
    int have_time = 0;

    if((pt = strchr(plog->msg, ']')) != NULL)
    {
		header_len = (pt-plog->msg) + 1;
		strncpy(msg_header, plog->msg, header_len);
		strncpy(msg_tail, pt+1, (strlen(pt)-strlen(EMPTY_TIMESTAMP))); //drop the time stamp to save data size
	}

    if (plog->abs_timestamp > 0) {
        resume_tm = localtime(&plog->abs_timestamp);
        sprintf(time_buf,"[%d/%02d/%02d %02d:%02d:%02d]"
            ,(1900+resume_tm->tm_year)
            ,(1+resume_tm->tm_mon)
            ,resume_tm->tm_mday
            ,resume_tm->tm_hour
            ,resume_tm->tm_min
            ,resume_tm->tm_sec);

//        p = strstr(plog->msg, EMPTY_TIMESTAMP);
//        if (p)
			have_time = 1;
    }

    if(header_len){
		if(header_len > 2)
		{
			sprintf(out_msg, "%s%s%s%s%s\n", msg_header ,mac_buf, serial_buf, have_time?time_buf:"", msg_tail);
		}else{
			sprintf(out_msg, "%s\n", msg_tail);
		}
	}else{
		sprintf(out_msg, "%s%s%s%s\n", mac_buf, serial_buf, have_time?time_buf:"", plog->msg);
	}
    
    return out_msg;
}
#endif
static void NTGR_log_print(void)
{
	lq_print_window(logFilePath, NTGR_log_print1);
#ifdef DSL_WIZARD_LOG
	dsl_lq_print_window("/tmp/DSL_log", NTGR_log_print2);
#endif
    /* notify somebody the log is now up to date. */
    unlink("/tmp/log_printing");
}

static void NTGR_message (int major, int minor, char *fmt, ...)
{
    struct log_queue_t *q;
    va_list arguments;
    char b[1024];
    int i;

    va_start (arguments, fmt);
    vsnprintf (b, sizeof(b)-1, fmt, arguments);
    va_end (arguments);

    lq_debug("entering <%s>\n", b);

    i = log_event_queue(major, minor);
    if (i < 0) {
        lq_debug("invalid queue id \n");
        return ;
    }
    lq_debug("log_queue = %d \n", i);
#ifdef DSL_WIZARD_LOG
	if(!is_DSL_Log)
#endif
		q = lq_get_queue(i);
#ifdef DSL_WIZARD_LOG
	else
		q = dsl_lq_get_queue(i);
#endif
    if (!q)
        return;
        
#ifdef DSL_WIZARD_LOG
	if(!is_DSL_Log)
#endif
		lq_enqueue(q, b);
#ifdef DSL_WIZARD_LOG
	else
		dsl_lq_enqueue(q, b);
#endif

    if (NTGR_log_isfull() && conf.mail_log_full==1) {
        lq_debug("send mail due to log is full.\n");
        do_send_mail();
    }

    return ;
}

/* refill log window from log queue due to external configuration changed. */
static int NTGR_log_refill(void)
{
    static int log_event = 0;
    static int log_minor_event = 0;
    struct log_queue_t *q;
    int i;

    lq_debug("entering \n");

    /* do nothing when configuration not changed. */
    if (log_event == conf.log_event
        && log_minor_event == conf.log_minor_event)
        return 0;

    log_event = conf.log_event;
    log_minor_event = conf.log_minor_event;

    lq_cleanup_window();

    for (i = 0; i < LOG_QUEUE_NUM; i++) {

        if (!queue_in_window(i))
            continue;

        q = lq_get_queue(i);
        if (q)
            lq_refill_window(q);
    }

    return 0;
}

/* Cleanup all log entries. Or only cleanup the queue in window ? */
static void NTGR_log_cleanup(void)
{
    lq_cleanup();
#ifdef DSL_WIZARD_LOG
    dsl_lq_cleanup();
#endif
}

static void NTGR_log_init(void)
{
    lq_init();
#ifdef DSL_WIZARD_LOG
    dsl_lq_init();
    log_info_init();
#endif
}

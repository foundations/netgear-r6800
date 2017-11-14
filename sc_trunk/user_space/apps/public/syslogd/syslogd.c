/* vi: set sw=4 ts=4: */
/*
 * Mini syslogd implementation for busybox
 *
 * Copyright (C) 1999,2000 by Lineo, inc. and Erik Andersen
 * Copyright (C) 1999,2000,2001 by Erik Andersen <andersee@debian.org>
 *
 * Copyright (C) 2000 by Karl M. Hegbloom <karlheg@debian.org>
 *
 * "circular buffer" Copyright (C) 2001 by Gennady Feldman <gfeldman@cachier.com>
 *
 * Maintainer: Gennady Feldman <gena01@cachier.com> as of Mar 12, 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <paths.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include "sysklogd.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/* SYSLOG_NAMES defined to pull some extra junk from syslog.h */
#define SYSLOG_NAMES
#include <sys/syslog.h>
#include <sys/uio.h>
#include <sys/uio.h>
#include "sc_debug.h"
#include "filenames.h"
#define EMPTY_TIMESTAMP	"                                          " /*41 space for timestamp resume+1 space for ,=42space*/
#define TIMESTAMP_BUF_LEN	43
/* Path for the file where all log messages are written */
#define __LOG_FILE "/var/log/messages"
#define __CONF_FILE "/etc/syslog.conf"

#if defined(AMPED) || defined(MTK_SDK5000)
#undef _PATH_LOG
#define _PATH_LOG "/var/syslogd"
#endif

#define REFRESH_TIME

#ifdef DSL_WIZARD_LOG
/*To distinguish which log is belong to DSL wizard logs.*/
int is_DSL_Log = 0;
#endif 

/* Path to the unix socket */
static char lfile[MAXPATHLEN]="";

static char *logFilePath = __LOG_FILE;

#define dprintf(msg,...)
struct syslog_conf conf;

#define ALERT_MAX_INTERVAL 3*60
//static time_t last_send_mail=0;

/* interval between marks in seconds */
static int MarkInterval = 10 * 60;

#ifdef SHOW_HOSTNAME
/* localhost's name */
static char LocalHostName[64]="";
#endif

#ifdef BB_FEATURE_REMOTE_LOG
#include <netinet/in.h>
/* udp socket for logging to remote host */
static int remotefd = -1;
/* where do we log? */
static char *RemoteHost=NULL;
/* what port to log to? */
static int RemotePort = 514;
/* To remote log or not to remote log, that is the question. */
static int doRemoteLog = FALSE;
static int local_logging = FALSE;
#endif


#define MAXLINE         1024            /* maximum line length */
#define URL_ACCESS_LOG_PREFIX		"[site "
#define NTP_UP_TIMESTAMP 		1240000000UL      /* timestamp 1240000000 is Apr 18, 2009.*/

/*
 * Follow NETGEAR spec v1.9
 * If the log data reaches over 90% of maximum log entries,
 * then all the log data will be mailed out.
 */
#define _LOG_FULL_PERCENT_  90

char *config_file_path;
static int reload_flag = 0;
static int clear_flag = 0;
static int mail_flag = 0;

static time_t curr_timestamp;//remember the timestamp of the log before get NTP

static char *wday[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
static char *month[]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

static char last_log[1024]="";

static void clear_signal(int sig);
static void reload_signal(int sig);
static void send_mail_signal(int sig);
int parse_config(char *conf_path);
static void do_send_mail(void);
static int sig_pipe[2];
static int sig_pending = 0;
static int init_sig_pipe(void);

#ifndef NETGEAR_LOG
static void set_time_getting_ntp(int sig);
static void recover_no_time_log(int sig);
#else
static int show_flag = 0;
static void show_log_message(int sig);
#include "netgear_log.c"
#endif // NETGEAR_LOG

/* circular buffer variables/structures */
#ifdef BB_FEATURE_IPC_SYSLOG
#if __GNU_LIBRARY__ < 5
#error Sorry.  Looks like you are using libc5.
#error libc5 shm support isnt good enough.
#error Please disable BB_FEATURE_IPC_SYSLOG
#endif

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

/*
 * Home Wireless Router Engineer Spec v1.9
 * 7.3.1   Overall Requirements
 * In the "Logs" page display window, the latest event entry
 * should be logged in the top (from bottom up) as the first entry.
 *
 * You can define _REVERSE_ORDER_ to enable this feature.
 */
#define _REVERSE_ORDER_


/* our shared key */
static const long KEY_ID = 0x414e4547; /*"GENA"*/

// Semaphore operation structures
static struct shbuf_ds {
    int size;               // size of data written
    int head;               // start of message list
    int tail;               // end of message list
    /* can't use char *data */
    char data[1];           // data/messages
} *buf = NULL;                  // shared memory pointer

/*Netgear spec: the log before NTP get,should without timestamp.But the time should 
                be resumed after NTP got*/
struct time_resume_mapping {
    int		offset;				 //log offset in buf
    time_t  timestamp;       		 // timestamp
};

#define TIME_MAPPING_NUM			1000   //this num could be reduced
static struct time_resume_mapping time_mapping[TIME_MAPPING_NUM];//mapping for timestamp and log address
static int time_map_index;//index of the no-timestamp log
static int ntp_get = FALSE;//flag for NTP gets time or not
static time_t time_when_getting_ntp;//remember the time when we are getting NTP

static struct sembuf SMwup[1] = {{1, -1, IPC_NOWAIT}}; // set SMwup
static struct sembuf SMwdn[3] = {{0, 0}, {1, 0}, {1, +1}}; // set SMwdn

#ifdef DSL_WIZARD_LOG
#define DATA_SIZE		32000*5	// increase log buffer size when enable DSL log function
#else
#define DATA_SIZE		32000
#endif

static int      shmid = -1;     // ipc shared memory id
static int      s_semid = -1;   // ipc semaphore id
int     data_size = DATA_SIZE; // data size
int     shm_size = DATA_SIZE + sizeof(*buf); // our buffer size
static int circular_logging = TRUE;

/* Ron */

void logMessage (int pri, char *msg);

/*
 * sem_up - up()'s a semaphore.
 */
static inline void sem_up(int semid)
{
    if ( semop(semid, SMwup, 1) == -1 )
        perror_msg_and_die("semop[SMwup]");
}

/*
 * sem_down - down()'s a semaphore
 */
static inline void sem_down(int semid)
{
    if ( semop(semid, SMwdn, 3) == -1 )
        perror_msg_and_die("semop[SMwdn]");
}


void ipcsyslog_cleanup(void){
    dprintf("Exiting Syslogd!\n");
    if (shmid != -1)
        shmdt(buf);

    if (shmid != -1)
        shmctl(shmid, IPC_RMID, NULL);
    if (s_semid != -1)
        semctl(s_semid, 0, IPC_RMID, 0);
}

void ipcsyslog_init(void){
    if (buf == NULL){
        if ((shmid = shmget(KEY_ID, shm_size, IPC_CREAT | 1023)) == -1)
            perror_msg_and_die("shmget");


        if ((buf = shmat(shmid, NULL, 0)) == NULL)
            perror_msg_and_die("shmat");


        buf->size=data_size;
#ifdef _REVERSE_ORDER_
        buf->head=buf->tail=buf->size-1;
#else
        buf->head=buf->tail=0;
#endif

        // we'll trust the OS to set initial semval to 0 (let's hope)
        if ((s_semid = semget(KEY_ID, 2, IPC_CREAT | IPC_EXCL | 1023)) == -1){
            if (errno == EEXIST){
                if ((s_semid = semget(KEY_ID, 2, 0)) == -1)
                    perror_msg_and_die("semget");
            }else
                perror_msg_and_die("semget");
        }
    }else{
        dprintf("Buffer already allocated just grab the semaphore?");
    }
}

#ifdef _REVERSE_ORDER_
void circ_message(const char *msg){
    int l=strlen(msg); /* count the whole message w/o '\0' included */
    int i;

    SC_CFPRINTF("=================================================\n");
    SC_CFPRINTF("buf->head <%d> buf->tail <%d> l <%d>\n", buf->head, buf->tail, l);
    sem_down(s_semid);

    if ( buf->head >= l ) {
        /* No need swap buf->head */
        int head = buf->head - l;   /* Future head offset */
        SC_CFPRINTF("No need swap buf->head, future head <%d>\n", head);

        if(buf->head > buf->tail && buf->tail >= head) {
            SC_CFPRINTF("Need pull tail, searching '\\n' [%d..%d]...\n", head-2, 0);
            /* Need pull tail */
            for(i=head-2; i>=0; i--) {
                if(*(buf->data+i) == '\n') {
                    break;
                }
            }
            if(i>=0) {
                SC_CFPRINTF("Find '\\n' at <%d>\n", i);
                buf->tail = i+1;
            }
            else {
                /* Can not find '\n' between 0 and head-2, search again from buffer buttom */
                SC_CFPRINTF("Can not find '\\n' in [%d..%d], search again in [%d..%d]\n", head-2, 0, buf->size-1, buf->head);
                for(i=buf->size-1; i>=buf->head; i--) {
                    if(*(buf->data+i) == '\n') {
                        break;
                    }
                }
                if(i<buf->head) {
                    /* Impossible! Can not find '\n' */
                    SC_CFPRINTF("Impossible! Can not find \\n \n");
                    buf->tail = head + l; /* ONLY record this message */
                }
                else {
                    SC_CFPRINTF("Find \\n at <%d>\n", i);
                    buf->tail = i+1;
                }
            }

        }
        memcpy(buf->data+head, msg, l);   /* prepend our message */
        buf->head = head;
    }
    else {
        /* Need swap buf->head */
        int tail_len = l - buf->head;
        int head = buf->size - tail_len;
        SC_CFPRINTF("Need swap buf->head, feature head <%d>\n", head);

        if( head <= buf->tail || buf->tail < buf->head) {
            /* Need pull tail */

            SC_CFPRINTF("Need pull tail, searching '\\n' in [%d..%d]...\n", head-2, buf->head);
            for(i=head-2; i>=buf->head; i--) {
                if(*(buf->data+i) == '\n') {
                    break;
                }
            }
            if(i<buf->head) {
                /* Can not find '\n', ONLY record this message */
                SC_CFPRINTF("Can not find '\\n'!\n");
                buf->tail = buf->head;
            }
            else {
                SC_CFPRINTF("Find '\\n' at <%d>\n", i);
                buf->tail = i+1;
            }
        }
        memcpy(buf->data+head, msg, tail_len);
        memcpy(buf->data, msg+tail_len, buf->head);
        buf->head = head;
    }
#ifndef REFRESH_TIME
    if(ntp_get == FALSE)/*should remember timestamp info*/
#endif
    {
        time_mapping[time_map_index].offset = buf->head;
        time_mapping[time_map_index].timestamp = curr_timestamp;
        SC_CFPRINTF("ADD [%d]: time %ld at offset %d \n", time_map_index, time_mapping[time_map_index].timestamp, time_mapping[time_map_index].offset);
        time_map_index = (time_map_index + 1)%TIME_MAPPING_NUM;
    }
    sem_up(s_semid);
    SC_CFPRINTF("buf->head <%d> buf->tail <%d>\n", buf->head, buf->tail);
    SC_CFPRINTF("=================================================\n");
    return;
}
#else
/* write message to buffer */
void circ_message(const char *msg){
    int l=strlen(msg); /* count the whole message w/ '\0' included */

    sem_down(s_semid);

    if ( (buf->tail + l) < buf->size ){
        if ( buf->tail < buf->head){
            if ( (buf->tail + l) >= buf->head ){
                int k= buf->tail + l - buf->head;
                char *c=memchr(buf->data+buf->head + k,'\n',buf->size - (buf->head + k));
                buf->head=(c != NULL)?( c - buf->data + 1):0;

            }
        }
        strncpy(buf->data + buf->tail,msg,l); /* append our message */
        buf->tail+=l;
    }else{
        char *c;
        int k=buf->tail + l - buf->size;

        c=memchr(buf->data + k ,'\n', buf->size - k);

        if (c != NULL) {
            buf->head=c-buf->data+1;
            strncpy(buf->data + buf->tail, msg, l - k - 1);
            strcpy(buf->data, &msg[l-k-1]);
            buf->tail = k + 1;
        }else{
            buf->head = buf->tail = 0;
        }

    }
    sem_up(s_semid);
}
#endif /* _REVERSE_ORDER_ */
#endif  /* BB_FEATURE_IPC_SYSLOG */

#ifndef NETGEAR_LOG

/* try to open up the specified device */
int device_open(char *device, int mode)
{
    int m, f, fd = -1;

    m = mode | O_NONBLOCK;

    /* Retry up to 5 times */
    for (f = 0; f < 5; f++)
        if ((fd = open(device, m, 0600)) >= 0)
            break;
    if (fd < 0)
        return fd;
    /* Reset original flags. */
    if (m != mode)
        fcntl(fd, F_SETFL, mode);
    return fd;
}
int vdprintf(int d, const char *format, va_list ap)
{
    char buf[BUF_SIZE];
    int len;

    len = vsnprintf(buf, sizeof(buf), format, ap);
    return write(d, buf, len);
}
/* Note: There is also a function called "message()" in init.c */
/* Print a message to the log file. */
static void message (char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
static void message (char *fmt, ...)
{
    int fd;
    struct flock fl;
    va_list arguments;
#ifdef BB_FEATURE_IPC_SYSLOG
    int log_full=0;
#ifdef REFRESH_TIME
    /*we should save the buf as pattern,actually we need to save buf->data*/
    char *buf_save = NULL;
    int head = 0, tail = 0, offset = 0;
#endif
#endif

    fl.l_whence = SEEK_SET;
    fl.l_start  = 0;
    fl.l_len    = 1;
#ifdef BB_FEATURE_IPC_SYSLOG
    if ((circular_logging == TRUE) && (buf != NULL)){
        char b[1024];
        va_start (arguments, fmt);
        vsnprintf (b, sizeof(b)-1, fmt, arguments);
        va_end (arguments);
#ifdef REFRESH_TIME
        if(strlen(b) == 0)
            SC_CFPRINTF("message length 0, just refresh time\n");
        else
#endif
            circ_message(b);
#ifdef DEBUG
        printf("buf->head=%d buf->tail=%d\n",buf->head,buf->tail);
#endif
        if(buf->tail == buf->head)/*log message NULL*/
            return;
#ifdef REFRESH_TIME
        {
            char *cursor=NULL;
            char *start=NULL;
            char *time_pos=NULL;
            int i;
            int j;
            struct tm *resume_tm;
            time_t diff_t;
            
            buf_save = malloc(data_size+1);
            if(buf_save == NULL)
            {
                SC_CFPRINTF("malloc failed, REFRESH_TIME would not work\n");
            }
            else
            {
                memset(buf_save,0,data_size+1);
                
                if(buf->head < buf->tail)
                {
                    memcpy(buf_save,buf->data,data_size);
                    head = buf->head;
                    tail = buf->tail;
                    offset = 0;
                }
                else
                {
                    offset = buf->size - buf->head;
                    memcpy(buf_save, buf->data+buf->head, offset);
                    memcpy(buf_save + offset, buf->data, buf->tail);
                    head = 0;
                    tail = buf->tail + offset;
                }
                SC_CFPRINTF("head=%d tail=%d\n",head,tail);
                
                /*Let's insert timestamp into log messages*/
                {
                    start = cursor = buf_save + head;
                    i = 0;
                    j = 0;
                    while(i < tail - head)
                    {
                        if(*(start + i) != '\n')
                        {
                            i++;
                            continue;
                        }
                        /*search timestamp pattern in current line*/
                        if((time_pos = strstr(cursor,EMPTY_TIMESTAMP)))
                        {
                            SC_CFPRINTF("find n,time_map_index = %d,(%d=%d),time(%ld)\n", time_map_index,(time_mapping[(time_map_index - j - 1)%TIME_MAPPING_NUM].offset + offset),cursor - buf_save,time_mapping[(time_map_index - j - 1)%TIME_MAPPING_NUM].timestamp);
                            if((start + i + 1) >= (time_pos + TIMESTAMP_BUF_LEN))/*check 41 bytes reserv for timestamp*/
                            {
                                char timestamp_tmp[TIMESTAMP_BUF_LEN];
                                if((time_mapping[(time_map_index - j - 1 + TIME_MAPPING_NUM)%TIME_MAPPING_NUM].offset + offset) % data_size == (cursor - buf_save)
                                &&time_mapping[(time_map_index - j - 1 + TIME_MAPPING_NUM)%TIME_MAPPING_NUM].timestamp != 0)
                                {
                                    /*timestamp already adjusted*/
                                    diff_t = time_mapping[(time_map_index - j - 1 + TIME_MAPPING_NUM)%TIME_MAPPING_NUM].timestamp;
                                    if(diff_t < NTP_UP_TIMESTAMP)
                                    { 
                                        SC_CFPRINTF("diff_t = %ld, do not show timestamp before ntp up\n", diff_t);
                                        break;
                                    }
                                    resume_tm = localtime(&diff_t);
                                    sprintf(timestamp_tmp," %s, %s %02d,%02d %02d:%02d:%02d"
                                        ,wday[resume_tm->tm_wday]
                                        ,month[resume_tm->tm_mon]
                                        ,resume_tm->tm_mday
                                        ,resume_tm->tm_year+1900
                                        ,resume_tm->tm_hour
                                        ,resume_tm->tm_min
                                        ,resume_tm->tm_sec);
			        	if(*(time_pos-1) != ']') {
            					*time_pos = ',';
                                    	strncpy(time_pos+1, timestamp_tmp, strlen(timestamp_tmp));

					}
					else
                                   	strncpy(time_pos, timestamp_tmp, strlen(timestamp_tmp));
					
                                    j++;
                                    SC_CFPRINTF("Insert timestamp,%s,len = %d\n", timestamp_tmp,strlen(timestamp_tmp));
                                }
                                else
                                {
                                    SC_CFPRINTF("impossiable,not match: %d vs %d\n"
                                        ,(time_mapping[(time_map_index - j - 1 + TIME_MAPPING_NUM)%TIME_MAPPING_NUM].offset + offset)
                                        ,(cursor - buf_save));
                                    break;
                                }
                            }
                            else
                            {
                                SC_CFPRINTF("not 42,impossible, %p(%d~%d),time_pos=%p,i=%d,start=%p,\n",buf->data,buf->head,buf->tail,time_pos,i,start);
                                break;
                            }
                        }
                        cursor = start + i + 1;/*every logs end with '\n' in buf*/ 
                        i++;
                    }
                }
            }/*if buf_save == NULL*/
        }
#endif
        /* print_circ_buf */
        if((fd=open(logFilePath,O_WRONLY| O_CREAT|O_TRUNC|O_NONBLOCK))<0)
        {
#ifdef REFRESH_TIME
            if(buf_save) free(buf_save);
#endif
            return;
        }
        fl.l_type = F_WRLCK;
        fcntl(fd, F_SETLKW, &fl);

#ifdef REFRESH_TIME
        if(buf_save)
        {
            int len;
            char *start;
            
            start = buf_save + head;
            len = tail - head;
            
            write(fd, start, len);
            write(fd, "\0", 1); /* Why need this? -- Argon */
            
            SC_CFPRINTF("len is <%d> buf->size is <%d> <%d%%> full\n", len, buf->size, (len * 100)/buf->size);
            if( buf->tail < buf->head
#if _LOG_FULL_PERCENT_ < 100
                ||(len * 100)/buf->size >= _LOG_FULL_PERCENT_ 
#endif
            ){
                /* the log data reaches over _LOG_FULL_PERCENT_% of maximum log entries */
                log_full = 1;
            }
        
            free(buf_save);
        }
        else/*buf_save NULL, show logs without timestamp*/
#endif

        if(buf->tail > buf->head){
            int len;
            char *start;

#ifdef _REVERSE_ORDER_
            start = buf->data+buf->head;
            len = buf->tail-buf->head;
#else
            start = buf->data;
            len = buf->tail;
#endif /* _REVERSE_ORDER_ */
            write(fd, start, len);
            write(fd, "\0", 1); /* Why need this? -- Argon */
#if _LOG_FULL_PERCENT_ < 100
            SC_CFPRINTF("len is <%d> buf->size is <%d> <%d%%> full\n", len, buf->size, (len * 100)/buf->size);
            if((len * 100)/buf->size >= _LOG_FULL_PERCENT_) {
                /* the log data reaches over _LOG_FULL_PERCENT_% of maximum log entries */
                log_full = 1;
            }
#endif
        }else if(buf->tail < buf->head){
            write(fd,buf->data+buf->head,buf->size-buf->head);
            write(fd,buf->data,buf->tail);
            write(fd, "\0", 1);
            log_full = 1;
        }

        fl.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &fl);
        close(fd);
        
        if(log_full && conf.mail_log_full==1){
            do_send_mail();
        }
    }else
#endif
    /*since we used circular_logging,if you don't want that,following code 
    should be modified*/
        if ((fd = device_open (logFilePath,
                        O_WRONLY | O_CREAT | O_NOCTTY | O_APPEND |
                        O_NONBLOCK)) >= 0) {
            fl.l_type = F_WRLCK;
            fcntl (fd, F_SETLKW, &fl);
            va_start (arguments, fmt);
            vdprintf (fd, fmt, arguments);
            va_end (arguments);
            fl.l_type = F_UNLCK;
            fcntl (fd, F_SETLKW, &fl);
            close (fd);
        } else {
            /* Always send console messages to /dev/console so people will see them. */
            if ((fd = device_open (_PATH_CONSOLE,
                            O_WRONLY | O_NOCTTY | O_NONBLOCK)) >= 0) {
                va_start (arguments, fmt);
                vdprintf (fd, fmt, arguments);
                va_end (arguments);
                close (fd);
            } else {
                fprintf (stderr, "Bummer, can't print: ");
                va_start (arguments, fmt);
                vfprintf (stderr, fmt, arguments);
                fflush (stderr);
                va_end (arguments);
            }
        }
}
#endif // NETGEAR_LOG

/*
 * Return value:
 *  0: ok
 * -1: error
 */
#define BUFSZ PIPE_BUF
int myPipe(char *command, char **output)
{
  FILE *fp;
  char buf[BUFSZ];
  int len;

  *output=malloc(1);
  strcpy(*output, "");
  if((fp=popen(command, "r"))==NULL)
     return -1;
  while((fgets(buf, BUFSZ, fp)) != NULL){
     len=strlen(*output)+strlen(buf);
     if((*output=realloc(*output, (sizeof(char) * (len+1))))==NULL)
	return -1;
      strcat(*output, buf);
  }
  pclose(fp);
  return 0;
}
/* Covert the specific Special characters: mail user name/pwd */
char *convert_names_specific_char(char *name) {
    static char convert_chars[] = {'"', '\\', '`',0};
    char *tmp;
    int i;
    char *p, *q;

    tmp = malloc(strlen(name)*2 + 1);   /* at most 2 times length */
    if(!tmp) {
        return NULL;
    }

    for(p=name, q=tmp; *p; p++) {
        /* Search if *p should be convert */
        for(i=0; convert_chars[i]; i++) {
            if(*p == convert_chars[i]) {
                /* Yes, *p should be convert */
                *q++ = '\\';
                break;
            }
        }
        /* Copy the original character */
        *q++ = *p;
    }
    *q = '\0';

    return tmp;
}
/* Covert all Special characters for mail server/and mail address */
char *convert_names_all_special_char(char *name) {
    char *tmp;
    char *p, *q;

    tmp = malloc(strlen(name)*2 + 1);   /* at most 2 times length */
    if(!tmp) {
        return NULL;
    }

    for(p=name, q=tmp; *p; p++) 
    {
        /* If it is special characters */       
        if((*p >= 0x20 && *p < 0x30) ||
            (*p >= 0x3a && *p < 0x41) ||
            (*p >= 0x5b && *p < 0x61) ||
            (*p >= 0x7b && *p <=0x7e)) 
        {
            /* Yes, *p should be convert */
            *q++ = '\\';
        }
        /* Copy the original character */
        *q++ = *p;
    }
    *q = '\0';

    return tmp;
}

/*
 * Send content of @filename via email.
 * Caller use @clear_syslog to let smtpc clear syslog after mail send or not
 * Return:
 *  0 -- Call smtpc success
 *  1 -- Send mail fail or no need send email
 *
 * NOTE: smtpc will clean syslog(call killall -SIGUSR1 syslogd) if it send mail success and @clear_syslog is set.
 */
static int send_mail(char *filename, int clear_syslog) {
    char cmd[1024]="";
    int len=0;
    int ret;
    char *buf;
    char *mail_server=NULL, *mail_recv=NULL;
    char *mail_login=NULL, *mail_pwd=NULL;

    if(!conf.mail_enable) {
        /* no need send mail */
        return 1;
    }

    /* Check if smtpc is running... */
    myPipe("ps", &buf);
    if(strstr(buf, "smtpc")) {
        SC_CFPRINTF("smtpc already running...\n");
        free(buf);
        return 1;
    }
    free(buf);

    mail_server = convert_names_all_special_char(conf.mail_server);
    mail_recv = convert_names_all_special_char(conf.mail_receiver);
    len += sprintf(cmd+len, "/usr/sbin/smtpc -t %s -d %d -m -h %s -r %s -s \"%s\""
        ,conf.TZ
        ,atoi(conf.daylight)
        ,mail_server
        ,mail_recv
        ,conf.mail_subject_alert);
    free(mail_server);
    free(mail_recv);

    if(conf.mail_enable_auth==1) {
        mail_login=convert_names_specific_char(conf.mail_login);
        mail_pwd=convert_names_specific_char(conf.mail_password);
        len += sprintf(cmd+len, " -U \"%s\" -P \"%s\"", mail_login, mail_pwd);
        free(mail_login);
        free(mail_pwd);
    }

#ifdef NETGEAR_LOG
    if (strcmp(filename, logFilePath) == 0)
        NTGR_log_print();
#endif

    /*
     * Clear syslog message or not?
     */
    if(clear_syslog) {
        len += sprintf(cmd+len, " -c");
    }
    len += sprintf(cmd+len, " <%s &", filename);

    ret = system(cmd);
    return ret;
}

/*
 * SYSLOGD will use this flag file (LOG_SENT_BY_CGI) to indicate need to clear log or not.
 * Click "Send Log" button in GUI no need to clear log while schedule send log need to clear log.
 * action.c will generate LOG_SENT_BY_CGI before call killall -1 syslogd, so it wont clear log.
 *
 * Here is description from NETGEAR Lancelot Wang
 *
 * It means if users click this button, then it won¡¯t flush the log and it will be sent out directly.
 * But "Once the log sends out by e-mail successfully, then it will be deleted from the logs." is
 * defined in the e-mail with some specify settings. The e-mail log sent is triggered automatically,
 * once the criteria that users set is reached. So, it is different. Only automatically sent out log
 * will be cleared in the log page.
 *                                                              Argon Cheng
 *                                                              2010-04-19
 */
static void do_send_mail(void)
{
    int clear_syslog;

    if(!access(LOG_SENT_BY_CGI, F_OK)) {
        /*
         * File exist ==> Called by CGI ==> No need to clear syslog ==> 0;
         */
        clear_syslog = 0;
    }
    else {
        clear_syslog = 1;
    }

    send_mail(MESSAGE_FILE, clear_syslog);
    unlink(LOG_SENT_BY_CGI);
#if 0
    sem_down(s_semid);
    if(ret == 0) {
        SC_CFPRINTF("Send mail success, clear log\n");
        buf->head=0;
        buf->tail=0;
    }
    sem_up(s_semid);
#endif
    return;
}

#if 0
void strccpy(char *dst, char *src,char c)
{
    char *pt=src;
    if(pt==NULL){
        printf("pt==NULL");
        dst[0]='\0';
        return ;
    }
    for(;*pt!=c && *pt!='\0';*dst++=*pt++);
    *dst='\0';

}
#endif
void strccpy2(char *dst, char *src,char *key,char c)
{
    char *pt=strstr(src,key);
    if(pt==NULL){
        dst[0]='\0';
        return ;
    }
    pt+=strlen(key);
    for(;*pt!=c && *pt!='\0';*dst++=*pt++);
    *dst='\0';

}

/* Netgear SPEC., don't show same log more than once message£¬only check URL access log
 * Return: 0 -- not
 *         1 -- yes
 */
static int is_duplicated_msg(int major, int minor, char *msg)
{
    int ret = 0;
#ifndef NETGEAR_LOG
	FILE *fp;
	char line[1024];
	int len = strlen(msg);
    /* if msg is blank */
    if(len == 0){
    	return 0;
    }

 	if((fp=fopen(logFilePath, "r")) == NULL){
		return 0;
	}
    while(fgets(line, sizeof(line), fp))
	{
		/* only check URL access log */
		if(strncmp(line, URL_ACCESS_LOG_PREFIX, strlen(URL_ACCESS_LOG_PREFIX)) != 0){
			continue;
		}
		if(strncmp(line, msg, len) == 0){
			ret =1;
			break;
		}
	}
	fclose(fp);
#else
    if (major == 14) {
        ret = NTGR_log_isduplicated(major, minor, msg);
        SC_CFPRINTF("log is %sduplicated!\n", ret?"":"not ");
    }
#endif
	return ret;
}

/*
 * log_this_event() -- Check if we need to log this message by major/minor event number
 * 1 -- Yes. We need to log this message
 * 0 -- No. We should ignore this log
 */
static int log_this_event(int major_event, int minor_event) {
    if( !((1<<major_event)&conf.log_event) ) {
        /* event not match */
        return 0;
    }
    if(major_event == 14) {
        /*
         * special case for Event No.14. See 7.3.2 of Spec v1.9
         * We have "14 01" for site allowed and "14 02" for site blocked
         */
        if( !((1<<minor_event)&conf.log_minor_event) ) {
            return 0;
        }
    }
    return 1;
}

/*
 * mail_this_event() -- Check if we need to mail this message immediately according to major/minor event number
 * 1 -- Yes. We need to mail this message
 * 0 -- No. No need to mail this message
 */
static int mail_this_event(int major_event, int minor_event) {
    if( !((1<<major_event)&conf.mail_event) ) {
        /* event not match */
        return 0;
    }
    if(major_event == 14) {
        /*
         * special case for Event No.14. See 7.3.2 of Spec v1.9
         * We have "14 01" for site allowed and "14 02" for site blocked
         */
        if( !((1<<minor_event)&conf.mail_minor_event) ) {
            return 0;
        }
    }

    if(major_event == 19) {
        if( !((1<<minor_event)&conf.mail_minor_event) ) {
            return 0;
        }
    }
    return 1;
}

/*
 * Normally, syslogd ONLY append timestamp to the message.
 * Caller should prepare the correct message format!
 *
 * NOTE: message should have event number prefix
 * ex: "14 01[site allowed:www.yahoo.com] from source 192.168.0.2"
 *     "07 00[DHCP IP: (192.168.1.2)] to MAC address 00:C0:02:12:3C:40"
 */
void logMessage (int pri, char *msg)
{
    time_t now;
    struct tm *st;
    char timestamp[TIMESTAMP_BUF_LEN]="";
    char res[20] = "";
    char tmp_msg[256]="";
    char *p=NULL;
    int major_event, minor_event;
#ifdef AMPED
	char *msg_start = msg; 
#endif

    SC_CFPRINTF("msg is <%s>\n", msg);
    curr_timestamp = 0;
#if 0
    /* We do not check pri now */
    if (pri!=0){
        for (c_fac = facilitynames;
                c_fac->c_name && !(c_fac->c_val == LOG_FAC(pri) << 3); c_fac++);
        fac=c_fac->c_val;
    }
#ifndef _ROUTER_DEBUG_MODE_
    /* check this log */
    if(check_log(fac)==FALSE) {
        SC_CFPRINTF("This message with fac (%d<<3) can NOT pass log level check\n", fac>>3);
        return;
    }
#endif
#endif

    /*
     * msg example:
     *
     * Feb 10 05:27:02 kernel: Can't find DB for id==20
     * Feb 10 05:29:07 igmp[945]: setsockopt IP_MSFILTER
     */

    /* if msg had time stamp ,remove it */
    if (strlen(msg) > 16 && msg[3] == ' ' && msg[6] == ' ' &&
            msg[9] == ':' && msg[12] == ':' && msg[15] == ' ') {
        msg+=16;
    }

    while(*msg!='\0' && *msg++!=':');
    msg++;   /* After this line msg point to really log message */
#ifdef MTK_SDK5000
    /* for kernel msg in MTK SDK 5000:
     * Feb 10 05:27:02 kernel: [   862.77747]  (1)[0:swapper/1]20 00[LAN access from remote] from x.x.x.x
     */
    if (*msg == '[') 
    {
        while(*msg!='\0' && *msg++!=']');
        while(*msg!='\0' && *msg++!=']');
    }
#endif


    /* Check if we need to log this event number */
    major_event = atoi(msg);
    minor_event = atoi(msg+3);
#ifdef DSL_WIZARD_LOG
    if(minor_event == 99)
		is_DSL_Log = 1;
	else
		is_DSL_Log = 0;
#endif
#ifndef _ROUTER_DEBUG_MODE_
    if(!log_this_event(major_event, minor_event) 
#ifdef DSL_WIZARD_LOG
		&& !is_DSL_Log
#endif
		) 
	{
        SC_CFPRINTF("event <%d.%d> should NOT be logged!\n", major_event, minor_event);
        return;
    }
    msg += 5;   /* jump over event number */

    /*
     * 1. According to Netgear spec v1.9, the first char of log should be '['
     * 2. Log message should have '[' and ']'
     */
    if(*msg != '[' || !strchr(msg, ']')) {
        SC_CFPRINTF("This message do not have '[' and ']'!\n");
        return;
    }
#endif



    /* time stamp */
    time(&now);
    unsetenv("TZ");
    st=localtime(&now);
    /*
     * Do not display time in the log if the NTP client is not connected to server
     */
#ifndef REFRESH_TIME
    if(st->tm_year+1900 >= 2010) 
    {
        sprintf(timestamp," %s, %s %02d,%02d %02d:%02d:%02d"
                ,wday[st->tm_wday]
                ,month[st->tm_mon]
                ,st->tm_mday
                ,st->tm_year+1900
                ,st->tm_hour
                ,st->tm_min
                ,st->tm_sec);
    } 
    else 
#endif
    {
        strcpy(timestamp,EMPTY_TIMESTAMP);
        curr_timestamp = now;
    }
#if 0/*add check whether need , when append timestamp*/
		/*
         * append timestamp.
         * We have two kinds of append
         * 1. Do not have comma before timestamp
         * [Internet disconnected] Monday, February 20, 2006 04:56:01
         *
         * 2. Have comma before timestamp
         * [Internet connected] IP address: x.x.x.x, Monday, February 20, 2006 04:56:01
         */
        if(msg[strlen(msg)-1] != ']') {
            strcat(msg, ",");
        }
#endif		
        {
            /*
             * The maximum message length is 128 characters in each entry.
             *  If the log info is longer than 128 characters, then some
             * middle content will be cut out and the log date and time must
             * be displayed completely. For example, if the hostname is very
             * long which may cause the whole log message can't be displayed
             * completely, then hostname must be cut out to display log date
             * and time completely.
             */
            #define MAX_MSG_LEN     128
            int msg_len = strlen(msg);
            int stamp_len = strlen(timestamp);

            if(msg_len + stamp_len > MAX_MSG_LEN) {
                SC_CFPRINTF("msg length is <%d> timestamp length is <%d> total length bigger than <%d>, need strip msg\n",
                     msg_len, stamp_len, MAX_MSG_LEN);

                /* it's URL access log, should cut hostname */
                if (strncmp(msg, URL_ACCESS_LOG_PREFIX, strlen(URL_ACCESS_LOG_PREFIX)) == 0) {
                	char *tmp = strchr(msg, ']');

                	/* cut the end part of the host name */
                	*(msg+(MAX_MSG_LEN-stamp_len-strlen(tmp))) = '\0';
                	strcat(msg, tmp);
                } else {
                	*(msg+(MAX_MSG_LEN-stamp_len)) = '\0';
                }
            }
            /* if it's log for URL access, save it for later check */
            if(strncmp(msg, URL_ACCESS_LOG_PREFIX, strlen(URL_ACCESS_LOG_PREFIX)) == 0){
            	strcpy(tmp_msg, msg);
            	p = tmp_msg;
            }
            strcat(msg, timestamp);
        }

    /* email this message immediately if email enabled and it match mail_keyword */
    if(conf.mail_enable && mail_this_event(major_event, minor_event)){
        FILE *fp = fopen(ALERT_FILE, "w");

        if(fp) {
            fprintf(fp, "%s\n", msg);
            fclose(fp);
            send_mail(ALERT_FILE, 0);
        }
    }

    /* Record this message and send it to syslog server */
#ifdef BB_FEATURE_REMOTE_LOG
    /* send message to remote logger */
    if ( (-1 != remotefd) && (doRemoteLog==TRUE)){
        static const int IOV_COUNT = 2;
        struct iovec iov[IOV_COUNT];
        struct iovec *v = iov;

        memset(&res, 0, sizeof(res));
        snprintf(res, sizeof(res), "<%d>", pri);
        v->iov_base = res ;
        v->iov_len = strlen(res);
        v++;

        v->iov_base = msg;
        v->iov_len = strlen(msg);
writev_retry:
        if ( -1 == writev(remotefd,iov, IOV_COUNT)){
            if (errno == EINTR) goto writev_retry;
            error_msg_and_die("cannot write to remote file handle on"
                    "%s:%d",RemoteHost,RemotePort);
        }
    }
#endif
	/* Netgear LOG SPEC., for URL access log, don't show log which is same as before. */
	if(!(p != NULL && is_duplicated_msg(major_event, minor_event, p) == 1))
	{
#ifdef SHOW_HOSTNAME
	    /* now spew out the message to wherever it is supposed to go */
	    message("%s %s %s\n", LocalHostName, res, msg);
#elif defined(NETGEAR_LOG)
#ifdef AMPED
		msg = msg_start;
#endif
        NTGR_message(major_event, minor_event, "%s\n", msg);
#else
    	message("%s\n", msg);
#endif
	}
}

static void quit_signal(int sig)
{
    //logMessage(LOG_SYSLOG | LOG_INFO, "System log daemon exiting.");
#ifdef AMPED
	NTGR_log_print();
#endif
    unlink(lfile);
#ifdef BB_FEATURE_IPC_SYSLOG
    ipcsyslog_cleanup();
#endif
    exit(TRUE);
}
static int log_start=0;

static inline void router_start()
{
    struct sysinfo info;
    time_t now;
    char *wday[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    struct tm *st;
    char timestamp[128];

    if(log_start==1)
        return ;

    sysinfo(&info);
    /* time stamp */
    //	setenv("TZ",conf.TZ,1);
    time(&now);
    now-=info.uptime;
    unsetenv("TZ");
    st=localtime(&now);
    sprintf(timestamp,"%s, %d-%02d-%02d %02d:%02d:%02d"
            ,wday[st->tm_wday]
            ,st->tm_year+1900
            ,st->tm_mon+1
            ,st->tm_mday
            ,st->tm_hour+atoi(conf.daylight)
            ,st->tm_min
            ,st->tm_sec);

    //if(st)
    //	free(st);

    log_start=1;
    //message("%s - %s\n", timestamp, "Router start up");
}

static void domark(int sig)
{
    if(log_start==0){
        router_start();
    }
    last_log[0]='\0';
    alarm(MarkInterval);
}

/* This must be a #define, since when DODEBUG and BUFFERS_GO_IN_BSS are
 * enabled, we otherwise get a "storage size isn't constant error. */
static int serveConnection (char* tmpbuf, int n_read)
{
    char *p = tmpbuf;

    while (p < tmpbuf + n_read) {

        int           pri = (LOG_USER | LOG_NOTICE);
        char          line[ MAXLINE + 1 ];
        unsigned char c;
        int find=0;

        char *q = line;
        
        while ( (c = *p) && q < &line[ sizeof (line) - 1 ]) {
            if (c == '<' && find==0) {
                /* Parse the magic priority number. */
                pri = 0;
                find= 1;
                while (isdigit (*(++p))) {
                    pri = 10 * pri + (*p - '0');
                }
                if (pri & ~(LOG_FACMASK | LOG_PRIMASK)){
                    pri = (LOG_USER | LOG_NOTICE);
                }
            } else if (c == TOKEN){
                /* Replace the TOKEN to '%'*/
                *q++ = '%';
            } else if (c == '\x02'){
                /* Replace the TOKEN to '%'*/
                *q++ = '"';
            } else if (c == '\n') {
                *q++ = ' ';
            } else if (iscntrl (c) && (c < 0177)) {
                *q++ = '^';
                *q++ = c ^ 0100;
            } else {
                *q++ = c;
            }
            p++;
        }
        *q = '\0';
        p++;
        /* Now log it */
        logMessage (pri, line);
    }
    return n_read;
}


#ifdef BB_FEATURE_REMOTE_LOG
static void init_RemoteLog (void)
{

    struct sockaddr_in remoteaddr;
    struct hostent *hostinfo;
    int len = sizeof(remoteaddr);
    int so_bc=1;

    memset(&remoteaddr, 0, len);

    remotefd = socket(AF_INET, SOCK_DGRAM, 0);

    if (remotefd < 0) {
        error_msg_and_die("cannot create socket");
    }

    remoteaddr.sin_family = AF_INET;

    /* Ron */
    /* allow boardcast */
    setsockopt(remotefd,SOL_SOCKET,SO_BROADCAST,&so_bc,sizeof(so_bc));
    hostinfo = gethostbyname(RemoteHost);
    remoteaddr.sin_addr = *(struct in_addr *) *hostinfo->h_addr_list;
    remoteaddr.sin_port = htons(RemotePort);

    /*
       Since we are using UDP sockets, connect just sets the default host and port
       for future operations
     */
    if ( 0 != (connect(remotefd, (struct sockaddr *) &remoteaddr, len))){
        error_msg_and_die("cannot connect to remote host %s:%d", RemoteHost, RemotePort);
    }
}
#endif

#ifdef AMPED
/* we gonna read the log in flash */
static void init_flashLog(void)
{
	FILE *fp;
	static char line[LOG_WINDOW_SIZE][PER_LOG_BUF_SIZE];
	int i = 0;
    int major_event, minor_event;
	char *msg;

	if (strcmp(logFilePath, __LOG_FILE))
	{
		SC_CFPRINTF("try to read from %s\n", logFilePath);

		if((fp=fopen(logFilePath, "r")) == NULL)
			return;

		while(fgets(line[i], PER_LOG_BUF_SIZE, fp) && i < LOG_WINDOW_SIZE)
			i++;

		fclose(fp);

		while (--i >= 0)
		{
			SC_CFPRINTF("parse message: %s\n", line[i]);
			msg = (char *)line[i];

			if (strlen(msg) > 16 && msg[3] == ' ' && msg[6] == ' ' &&
					msg[9] == ':' && msg[12] == ':' && msg[15] == ' ') {
				msg+=16;
			}

			while(*msg!='\0' && *msg++!=':');
			if (*msg == '\0')
				continue;
			msg++;   /* After this line msg point to really log message */

#ifdef MTK_SDK5000
            /* for kernel msg in MTK SDK 5000:
             * Feb 10 05:27:02 kernel: [   862.77747]  (1)[0:swapper/1]20 00[LAN access from remote] from x.x.x.x
             */
            if (*msg == '[') 
            {
                while(*msg!='\0' && *msg++!=']');
                while(*msg!='\0' && *msg++!=']');
            }
			if (*msg == '\0')
				continue;
#endif

			major_event = atoi(msg);
			minor_event = atoi(msg+3);

			SC_CFPRINTF("will log message: %s, major:%d, minor:%d\n", msg, major_event, minor_event);
        	NTGR_message(major_event, minor_event, "%s", line[i]);
		}
	}
}
#endif

static void doSyslogd (void) __attribute__ ((noreturn));
static void doSyslogd (void)
{
    struct sockaddr_un sunx;
    socklen_t addrLength;

    int sock_fd, maxfd;
    fd_set fds;
    int err;

    /* Set up signal handlers. */
    signal (SIGINT,  quit_signal);
    signal (SIGTERM, quit_signal);
    signal (SIGQUIT, quit_signal);
    signal (SIGHUP,  send_mail_signal);
    signal (SIGUSR1, clear_signal);
    signal (SIGUSR2, reload_signal);
#ifdef NETGEAR_LOG
    signal (SIGWINCH, SIG_IGN);
    signal (SIGPROF, show_log_message);
#else
    signal (SIGWINCH, set_time_getting_ntp);
    signal (SIGPROF, recover_no_time_log);
#endif
    signal (SIGCHLD,  SIG_IGN);
#ifdef SIGCLD
    signal (SIGCLD,  SIG_IGN);
#endif
    signal (SIGALRM, domark);
    //wait ntp get correct time
    alarm (MarkInterval);

    /* Create the syslog file so realpath() can work. */
    if (realpath (_PATH_LOG, lfile) != NULL)
        unlink (lfile);

    memset (&sunx, 0, sizeof (sunx));
    sunx.sun_family = AF_UNIX;
    strncpy (sunx.sun_path, lfile, sizeof (sunx.sun_path));
    if ((sock_fd = socket (AF_UNIX, SOCK_DGRAM, 0)) < 0)
        perror_msg_and_die ("Couldn't get file descriptor for socket " _PATH_LOG);

    addrLength = sizeof (sunx.sun_family) + strlen (sunx.sun_path);
    if (bind(sock_fd, (struct sockaddr *) &sunx, addrLength) < 0)
        perror_msg_and_die ("Could not connect to socket " _PATH_LOG);

    if (chmod (lfile, 0666) < 0)
        perror_msg_and_die ("Could not set permission on " _PATH_LOG);

#ifdef NETGEAR_LOG
    NTGR_log_init();
#endif

#ifdef BB_FEATURE_IPC_SYSLOG
    if (circular_logging == TRUE ){
        ipcsyslog_init();
    }
#endif

#if 0
#ifdef BB_FEATURE_REMOTE_LOG
    if (doRemoteLog == TRUE){
        init_RemoteLog();
    }
#endif
#endif

    init_sig_pipe();

    maxfd = sock_fd;
    if (sig_pipe[0] > maxfd)
        maxfd = sig_pipe[0];

#ifdef AMPED
	init_flashLog();
#endif

    for (;;) {

        FD_ZERO (&fds);
        FD_SET (sock_fd, &fds);
        FD_SET (sig_pipe[0], &fds);

        if ((err = select (maxfd+1, &fds, NULL, NULL, NULL)) < 0) {
            if (errno == EINTR) {
                /* alarm may have happened. */
                continue;
            }
            perror_msg_and_die ("select error");
        }

        if (err < 0)
            continue;

        if (FD_ISSET(sig_pipe[0], &fds)) {
            char tmpbuf[128];
            /* consume it. */
            read(sig_pipe[0], tmpbuf, sizeof(tmpbuf));
            /* Clear it at this point. Now welcome new signal. */
            sig_pending = 0;

            if (reload_flag) {
                reload_flag = 0;
                parse_config(config_file_path);
#ifdef NETGEAR_LOG
                NTGR_log_refill();
#endif
            }
    
            if (clear_flag) {
                clear_flag = 0;
#ifdef NETGEAR_LOG
                NTGR_log_cleanup();
#endif
                unlink(logFilePath);
            }
    
            if (show_flag) {
                show_flag = 0;
#ifdef NETGEAR_LOG
                NTGR_log_print();
#endif
            }
    
            if (mail_flag) {
                mail_flag = 0;
                do_send_mail();
            }
        }

        if (FD_ISSET (sock_fd, &fds)) {
            int   i;
            RESERVE_BB_BUFFER(tmpbuf, BUFSIZ + 1);

            memset(tmpbuf, '\0', BUFSIZ+1);
            if ( (i = recv(sock_fd, tmpbuf, BUFSIZ, 0)) > 0) {
                serveConnection(tmpbuf, i);
            } else {
                perror_msg_and_die ("UNIX socket error");
            }
            RELEASE_BB_BUFFER (tmpbuf);
        }/* FD_ISSET() */
    } /* for main loop */
}

static void notify_select_signal(int sig)
{
    SC_CFPRINTF("received signal %d \n", sig);
    if (!sig_pending) {
        char c = (char)sig;
        write(sig_pipe[1], &c, 1);
    }
    sig_pending++;
}

static int init_sig_pipe(void)
{
    int i;
    int flags;

    if (pipe(sig_pipe) < 0) {
        return -1;
    }

    /* Must set it to non-block to avoid block in signal function. */
    for (i = 0; i < 2; i++) {
        if ((flags = fcntl(sig_pipe[i], F_GETFL, 0)) < 0)
        {
            return -1;
        }
        if (fcntl(sig_pipe[i], F_SETFL, flags | O_NONBLOCK) < 0)
        {
            return -1;
        }
    }
    return 0;
}

#ifdef NETGEAR_LOG
static void show_log_message(int sig)
{
    show_flag = 1;
    notify_select_signal(sig);
}
#endif

static void clear_signal(int sig)
{
#ifdef NETGEAR_LOG
    clear_flag  = 1;
#else
#ifdef _REVERSE_ORDER_
    buf->head = buf->tail = buf->size-1;
#else
    buf->head=0;
    buf->tail=0;
#endif /* _REVERSE_ORDER_ */
#endif /* NETGEAR_LOG */
    notify_select_signal(sig);
}

static void reload_signal(int sig)
{
    reload_flag = 1;
    notify_select_signal(sig);
}

static void send_mail_signal(int sig)
{
    mail_flag = 1;
    notify_select_signal(sig);
}

#ifndef NETGEAR_LOG
/*know the timestamp once NTP getting but not setting to kernel*/
static void set_time_getting_ntp(int sig)
{
	time(&time_when_getting_ntp);
	SC_CFPRINTF("Remember the time before we are getting NTP\n");
}
/*When NTP is setting to kernel,recover the timestamp to the logs which record before
  NTP getting*/
static void recover_no_time_log(int sig)
{
#ifndef REFRESH_TIME
    char *cursor=NULL;
    char *start=NULL;
    char *time_pos=NULL;
    int j;
    struct tm *resume_tm;
    time_t diff_t;
#endif
    int i;
    struct tm *st;
    time_t now;
    time(&now);
    unsetenv("TZ");
    st = localtime(&now);
    if(st->tm_year+1900 >= 2010) 
    {
        if(ntp_get == FALSE) 
        {
#ifdef REFRESH_TIME
            int j = time_map_index;/*the time_mapping to be used next log*/
#endif
            ntp_get = TRUE;
            /*first time get NTP success,should count the exist logs' time,
              and write to log buf*/
            SC_CFPRINTF(" GOT NTP,now = %ld\n\n", now);
#ifdef REFRESH_TIME
            if(time_mapping[j].timestamp == 0)
            {
                /*time_mapping[time_map_index] was never used,
                check timestamp from index 0;
                otherwise would check from index time_map_index*/
                j = 0;
            }
            
            for(i = 0;i < TIME_MAPPING_NUM; i++)
            {/*refresh timestamp record*/
                if(time_mapping[(j + i) % TIME_MAPPING_NUM].timestamp < NTP_UP_TIMESTAMP
                    && time_mapping[(j + i) % TIME_MAPPING_NUM].timestamp > 0)
                    time_mapping[(j + i) % TIME_MAPPING_NUM].timestamp += now - time_when_getting_ntp;
                else/*logs after this was created after ntp up or not used*/
                    break;
            }
#else
            if(buf->head < buf->tail)
            {
                /*1. log buf hasn't been full yet,the buf used is head~tail*/
                  cursor = buf->data + buf->head;
                  start = buf->data + buf->head;
                  i = 0;
                  j = 0;
                  while(i <= buf->tail - buf->head)
                  {
                      if(*(start + i) != '\n')
                      {
                          i++;
                          continue;
                      }
                if((time_pos = strstr(cursor, ",    ")) || (time_pos = strstr(cursor, "]    ")))
                      {
                          /*find the reserved timestamp position,it is 32bytes space*/
                          time_pos++;
                          SC_CFPRINTF("find n,time_map_index = %d,(%d=%d),time(%ld)\n", time_map_index,time_mapping[(time_map_index - j - 1)%TIME_MAPPING_NUM].offset,cursor - buf->data,time_mapping[(time_map_index - j - 1)%TIME_MAPPING_NUM].timestamp);
                          if((start + i + 1) == (time_pos + TIMESTAMP_BUF_LEN))/*check 48 bytes reserv for timestamp*/
                          {
                              char timestamp_tmp[TIMESTAMP_BUF_LEN];
                              if(time_mapping[(time_map_index - j - 1)%TIME_MAPPING_NUM].offset == (cursor - buf->data)
                              &&time_mapping[(time_map_index - j - 1)%TIME_MAPPING_NUM].timestamp != 0)
                              {    
                                  /*if the offset reserved is right and reserved timestamp exist,
                                    count the time and insert to log buf*/ 
                                  diff_t = now + time_mapping[(time_map_index - j - 1)%TIME_MAPPING_NUM].timestamp - time_when_getting_ntp;
                                  SC_CFPRINTF("now = %ld,diff_t = %ld\n",now, diff_t);
                                  resume_tm = localtime(&diff_t);
                                        sprintf(timestamp_tmp," %s, %s %02d,%02d %02d:%02d:%02d"
                                        ,wday[resume_tm->tm_wday]
                                        ,month[resume_tm->tm_mon]
                                        ,resume_tm->tm_mday
                                        ,resume_tm->tm_year+1900
                                        ,resume_tm->tm_hour
                                        ,resume_tm->tm_min
                                        ,resume_tm->tm_sec);
                                  strncpy(time_pos, timestamp_tmp, strlen(timestamp_tmp));
                                  j++;
                                  SC_CFPRINTF("Insert timestamp,%s,len = %d\n", timestamp_tmp,strlen(timestamp_tmp));
                              }
                              else
                              {
                                  SC_CFPRINTF("impossiable,not match\n");
                                  break;
                              }
                          }
                          else
                          {
                              SC_CFPRINTF("not 32,impossible, %p(%d~%d),time_pos=%p,i=%d,start=%p,\n",buf->data,buf->head,buf->tail,time_pos,i,start);
                              break;
                          }
                      }
                      cursor = start + i + 1;/*every logs end with '\n' in buf*/ 
                      i++;
                }
            }
            else
            {
                /*TODO*/
                /*2. log buf has been full ever before NTP getting,this case is infrequent,Since our 
                buf is large enough for logs before NTP.the buf used is head~end + start~tail*/
                /*start---(used)------tail--(no-used)----head---(used)------------end*/
                SC_CFPRINTF("Log is full before NTP getting\n");
            }

#endif
        }
    }
#ifdef REFRESH_TIME
    message("%s","");/*refresh time in log messages*/
#endif
}
#endif // NETGEAR_LOG

/*
 * Dump config
 */
static void dump_config(void) {
#define DUMP_CONF_INT(x)    SC_XCFPRINTF(#x"=%d\n", conf.x)
#define DUMP_CONF_STR(x)    SC_XCFPRINTF(#x"=%s\n", conf.x)
    SC_XCFPRINTF("==============SYSLOG.CONF==============\n");
    DUMP_CONF_INT(mail_enable);
    DUMP_CONF_INT(mail_enable_auth);
    DUMP_CONF_INT(mail_log_full);
    DUMP_CONF_INT(log_event);
    DUMP_CONF_INT(log_minor_event);
    DUMP_CONF_INT(mail_event);
    DUMP_CONF_INT(mail_minor_event);
    DUMP_CONF_STR(mail_server);
    DUMP_CONF_STR(mail_sender);
    DUMP_CONF_STR(mail_receiver);
    DUMP_CONF_STR(mail_login);
    DUMP_CONF_STR(mail_password);
    DUMP_CONF_STR(mail_subject);
    DUMP_CONF_STR(mail_subject_alert);
    DUMP_CONF_STR(TZ);
    DUMP_CONF_STR(daylight);
}

int parse_config(char *conf_path)
{
    FILE *fp;
    char buf[1024];
    char rhip[16]="";
    char tmp[32];

#ifdef DEBUG
    printf("conf_path==%s\n",conf_path);
#endif
    if(conf_path==NULL)
        fp=fopen(__CONF_FILE,"r");
    else
        fp=fopen(conf_path,"r");

    if(fp==NULL)
        return FALSE;

    fread(buf,1024,1,fp);
    fclose(fp);

    /* initial conf */
    bzero(&conf,sizeof(conf));

    if(strstr(buf,"mail_enable=1")) conf.mail_enable=1;
    if(strstr(buf,"mail_enable_auth=1")) conf.mail_enable_auth=1;

    strccpy2(conf.TZ,buf,"TZ=",'\n');

    unsetenv("TZ");

    strccpy2(conf.daylight,buf,"daylight=",'\n');
    strccpy2(rhip,buf,"log_remote_ip=",'\n');

    if(strlen(rhip)>2){
        if(RemoteHost) free(RemoteHost);
        RemoteHost=strdup(rhip);
        doRemoteLog = TRUE;
        if(remotefd > 0) close(remotefd);
        init_RemoteLog();
    }else
        doRemoteLog = FALSE;

    /* if email is not enable ,we don't need to parser those config*/
    if(conf.mail_enable==1){
        if(strstr(buf,"mail_log_full=1")) conf.mail_log_full=1;

        strccpy2(conf.mail_server,buf,"mail_server=",'\n');
        strccpy2(conf.mail_receiver,buf,"mail_receiver=",'\n');
        strccpy2(conf.mail_subject,buf,"mail_subject=",'\n');
        strccpy2(conf.mail_subject_alert,buf,"mail_subject_alert=",'\n');
        strccpy2(conf.mail_login,buf,"mail_login=",'\n');
        strccpy2(conf.mail_password,buf,"mail_password=",'\n');
    }
    strccpy2(tmp, buf, "log_event=",'\n');
    conf.log_event = atoi(tmp);

    strccpy2(tmp, buf, "log_minor_event=",'\n');
    conf.log_minor_event = atoi(tmp);

    strccpy2(tmp, buf, "mail_event=",'\n');
    conf.mail_event = atoi(tmp);

    strccpy2(tmp, buf, "mail_minor_event=",'\n');
    conf.mail_minor_event = atoi(tmp);


    dump_config();
    return TRUE;
}

int syslogd_main(int argc, char **argv)
    //int main(int argc, char **argv)
{
    int opt;
#if ! defined(__uClinux__)
    int doFork = TRUE;
#endif

    char *p;

    SC_CFPRINTF("Start in this function!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

    /* do normal option parsing */
    while ((opt = getopt(argc, argv, "m:nO:Rf:LC")) > 0) {
        switch (opt) {
            case 'm':
                MarkInterval = atoi(optarg) * 60;
                break;
#if ! defined(__uClinux__)
            case 'n':
                doFork = FALSE;
                break;
#endif
            case 'O':
                logFilePath = strdup(optarg);
                break;
#ifdef BB_FEATURE_REMOTE_LOG
            case 'R':
                if(RemoteHost!=NULL) free(RemoteHost);
                RemoteHost = strdup(optarg);
                if ( (p = strchr(RemoteHost, ':'))){
                    RemotePort = atoi(p+1);
                    *p = '\0';
                }
                doRemoteLog = TRUE;
                break;
            case 'L':
                local_logging = TRUE;
                break;
#endif
#ifdef BB_FEATURE_IPC_SYSLOG
            case 'C':
                circular_logging = TRUE;
                break;
#endif
            case 'f':
                config_file_path=optarg;
                if(parse_config(optarg)==FALSE)
                    show_usage();
                break;

            default:
                show_usage();
        }
    }
#if 0
#ifdef BB_FEATURE_REMOTE_LOG
    /* If they have not specified remote logging, then log locally */
    if (doRemoteLog == FALSE)
        local_logging = TRUE;
#endif
#endif

#ifdef SHOW_HOSTNAME
    //	}
        /* Store away localhost's name before the fork */
        gethostname(LocalHostName, sizeof(LocalHostName));
        if ((p = strchr(LocalHostName, '.'))) {
            *p++ = '\0';
        }
#endif
umask(0);

#if ! defined(__uClinux__)
    if (doFork == TRUE) {
        if (daemon(0, 1) < 0)
            perror_msg_and_die("daemon");
    }
#endif
doSyslogd();

return EXIT_SUCCESS;
}
#if 1
extern int klogd_main (int argc ,char **argv);

int main(int argc ,char **argv)
{
    int ret = 0;
    char *base = strrchr(argv[0], '/');

    if (strstr(base ? (base + 1) : argv[0], "syslogd"))
        ret = syslogd_main(argc,argv);
    //else if (strstr(base ? (base + 1) : argv[0], "klogd"))
    //    ret = klogd_main(argc,argv);
    else
        show_usage();

    return ret;
}
#endif
/*
   Local Variables
   c-file-style: "linux"
   c-basic-offset: 4
   tab-width: 4
End:
 */

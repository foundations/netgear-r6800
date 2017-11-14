#define TRUE  (int)1
#define FALSE (int)0
#define BUF_SIZE        8192
#define BB_BANNER
#define RESERVE_BB_BUFFER(buffer,len)  static          char buffer[len]
#define RELEASE_BB_BUFFER(buffer)      ((void)0)
//#define perror_msg_and_die(msg) {puts(msg); exit(FALSE);}
#define perror_msg_and_die(msg)
//#define error_msg_and_die(msg,...) perror_msg_and_die(msg)
#define error_msg_and_die(msg,...)
#define show_usage() exit(0)
#define BB_FEATURE_REMOTE_LOG
#ifndef NETGEAR_LOG
#define BB_FEATURE_IPC_SYSLOG
#endif

#define TOKEN 0X01

#define MAIL_KEYWORD_NUM    5
#define MAIL_KEYWORD_LEN    128
struct syslog_conf {
    int log_event;
    int log_minor_event;
    int mail_event;
    int mail_minor_event;
	int mail_enable;
	int mail_enable_auth;
	int mail_log_full;
	char mail_server[128];
	char mail_sender[128];
	char mail_receiver[128];
	char mail_login[25];
	char mail_password[17];
	char mail_subject[128];
	char mail_subject_alert[128];
	char TZ[8];
	char daylight[2];
};

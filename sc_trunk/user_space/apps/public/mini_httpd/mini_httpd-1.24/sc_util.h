#ifndef __SC_UTIL__
#define __SC_UTIL__

//#define _SC_DEBUG_ENABLE_ 
#include "sc_debug.h"

#define SC_IPADDR_LEN 16
#define SC_IP6ADDR_LEN 48

#ifndef AUTH_FILE
#define AUTH_FILE ".htpasswd"
#endif /* AUTH_FILE */

enum {
	AUTH_OK,
	AUTH_FAIL,
	NO_AUTH
};				/*distinguish "auth fail" and "no auth" */

#ifdef XCLOUD
#define SC_CFPRINTF_XCLOUD(fmt, args...)    do{ if (access("/tmp/dbg_x", F_OK) == 0) { FILE *fp=fopen("/dev/console", "a+"); if(fp) {fprintf(fp, "[%s::%s():%d] ", __FILE__, __FUNCTION__, __LINE__);fprintf(fp, fmt, ##args);fclose(fp);} } }while(0)
#else
#define SC_CFPRINTF_XCLOUD(fmt, args...)
#endif

enum {
	LOGIN_FAIL,
	LOGIN_SUCCESS
};

int sc_lockf(int fd, int shared, int wait);
char *getIPAddress(char *ifname);
char *getNetMask(char *ifname);
int check_lan_guest();
int check_current_request_side(void);
unsigned long long get_uptime(char *time_file);
int need_fakepath(char *path);
void send_log(int login_success, char *remote_ip);
int path_exist(char *path, char *paths[], char *method_str);

void lock_enter(void);
void lock_leave(void);
void __restore_manager(void);
void __save_manager(void);
inline void restore_manager(void);
inline void save_manager(void);

#endif

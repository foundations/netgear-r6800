
#ifndef __USB_API_H__
#define __USB_API_H__

#include "filenames.h"
#include "logging.h"

#define MAX_USB_DEVICE_CNT	50  //MAX_USB_DEVICE_CNT = mount + unmouT
#define MNT_PATH		"/mnt/shares/"

#define ACCESS_HTTP_USERNAME 0
#define ACCESS_GUEST 1
#define ACCESS_NOTSHARE 2

typedef struct {
	char * shared_name;
	int access_read;
	int access_write;
} SHARE_T;


typedef struct SHARE_LIST{
	SHARE_T s;
	struct SHARE_LIST *next;
} SHARE_LIST_T;


void SHARE_LIST_print(SHARE_LIST_T* shares_in);
SHARE_LIST_T* SHARELIST_parse( char* filename);

extern SHARE_LIST_T* shared_list;
extern int is_admin_user;
extern int is_file_readable;

int is_writable(const char *dir);
int is_readable(const char *dir);
int all_file_readable_writable(void);

#define bftpd_log_debug(...)
//#define bftpd_log_debug	bftpd_log

#endif //__USB_API_H__



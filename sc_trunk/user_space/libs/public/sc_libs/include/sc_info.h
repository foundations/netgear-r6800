/* 
 * Copyright © 2008 SerComm
 * sercomm information defination
 * including ethernet, dsl, wifi
 * firmware information
*/

#ifndef __SC_INFO_H__
#  define __SC_INFO_H__
#  include "wan_info.h"
#  include "sw_info.h"
#  include "wifi_info.h"


#  include "sc_namespace.h"
/*Interface stats*/
#  include "sc_if_stats.h"

#  include "sc_adsl.h"
#  include "sc_wifi.h"
#  include "sc_sw.h"

#  define MAXHOSTNAMELEN  64	/* max length of hostname */
#  define PID_SIZE	sizeof(sercomm_pid_t)

struct dhcpOfferedAddr
{
    unsigned char chaddr[16];
    unsigned int yiaddr;	/* network order */
    unsigned int expires;	/* host order */

//#ifdef RONSCODE
    unsigned char hostname[MAXHOSTNAMELEN];
    unsigned char cid[48];//change 16->48 to solution define MAX_CLIENT_LEN
//#endif

};

typedef struct
{
    unsigned char magic_s[7];	/* sErCoMm */
    unsigned char ver_control[2];	/* version control */
    unsigned char download[2];	/* download control */
    unsigned char hw_id[32];	/* H/W ID */
    unsigned char hw_ver[2];	/* H/W version */
    unsigned char p_id[4];	/* Product ID */
    unsigned char protocol_id[2];	/* Protocol ID */
    unsigned char fun_id[6];	/* Function ID */
    unsigned char fw_ver[2];	/* F/W version */
    unsigned char start[2];	/* Starting code segment */
    unsigned char c_size[2];	/* Code size (kbytes) */
    unsigned char magic_e[7];	/* sErCoMm */
} sercomm_pid_t;


/* board information */
#  include <stdio.h>
int sc_get_pid(char *buf, int len);
char *sc_get_FwVersion(char *buf, int len);
long sc_get_UpTime(void);
char *sc_name_n(char *name_n, char *basename, int n);

#endif

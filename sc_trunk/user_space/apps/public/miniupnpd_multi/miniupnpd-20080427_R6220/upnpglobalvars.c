/* $Id: upnpglobalvars.c,v 1.16 2008/04/10 22:44:20 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#include <sys/types.h>
#include <netinet/in.h>

#include "config.h"
#include "upnpglobalvars.h"
#include "nvram_var.h"
#include "sc_project_defines.h"
/* network interface for internet */
const char * ext_if_name = 0;

const char *ext_upnp_notifyinterval = 0;

/* file to store leases */
#ifdef ENABLE_LEASEFILE
const char* lease_file = "/tmp/upnptab";
#endif

/* forced ip address to use for this interface
 * when NULL, getifaddr() is used */
const char * use_ext_ip_addr = 0;

/* LAN address */
/*const char * listen_addr = 0;*/

unsigned long downstream_bitrate = 0;
unsigned long upstream_bitrate = 0;

/* startup time */
time_t startup_time = 0;
//Sercomm
long my_startup_time = 0;
int gTTL=4;
int gNotifyInterval=1800;

#if 0
/* use system uptime */
int sysuptime = 0;

/* log packets flag */
int logpackets = 0;

#ifdef ENABLE_NATPMP
int enablenatpmp = 0;
#endif
#endif

int runtime_flags = 0;

const char * pidfilename = "/var/run/miniupnpd.pid";

char uuidvalue[] = "uuid:824ff22b-8c7d-41c5-a131-44f534e12555";
char uuidvalue1[] = "uuid:824ff22b-8c7d-41c5-a131-44f534e12556";
char uuidvalue2[] = "uuid:824ff22b-8c7d-41c5-a131-44f534e12557";
char uuidvalue3[] = "uuid:824ff22b-8c7d-41c5-a131-44f534e12558";
char serialnumber[SERIALNUMBER_MAX_LEN]="v1";
char module_name[DEVICENAME_MAX_LEN] = "";
char model_description[MODELDESCIPRTION_MAX_LEN]="";
#ifdef VER
char modelnumber[MODELNUMBER_MAX_LEN] = BOARD_ID_DEFAULT; /* Follow WNR2000v2 */
#else
char modelnumber[MODELNUMBER_MAX_LEN] = "1.00.00";
#endif

/* presentation url :
 * http://nnn.nnn.nnn.nnn:ppppp/  => max 30 bytes including terminating 0 */
char presentationurl[PRESENTATIONURL_MAX_LEN];

/* UPnP permission rules : */
struct upnpperm * upnppermlist = 0;
unsigned int num_upnpperm = 0;

#ifdef ENABLE_NATPMP
/* NAT-PMP */
unsigned int nextnatpmptoclean_timestamp = 0;
unsigned short nextnatpmptoclean_eport = 0;
unsigned short nextnatpmptoclean_proto = 0;
#endif

#ifdef USE_PF
const char * queue = 0;
const char * tag = 0;
#endif

int n_lan_addr = 0;
struct lan_addr_s lan_addr[MAX_LAN_ADDR];



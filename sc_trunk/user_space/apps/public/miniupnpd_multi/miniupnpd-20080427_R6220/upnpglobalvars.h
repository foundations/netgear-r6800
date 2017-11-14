/* $Id: upnpglobalvars.h,v 1.19 2008/04/10 22:44:21 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#ifndef __UPNPGLOBALVARS_H__
#define __UPNPGLOBALVARS_H__

#include <time.h>
#include "upnppermissions.h"
#include "miniupnpdtypes.h"
#include "config.h"

/* name of the network interface used to acces internet */
extern const char * ext_if_name;

extern const char *ext_upnp_notifyinterval;

/* file to store all leases */
#ifdef ENABLE_LEASEFILE
extern const char * lease_file;
#endif

/* forced ip address to use for this interface
 * when NULL, getifaddr() is used */
extern const char * use_ext_ip_addr;

/* parameters to return to upnp client when asked */
extern unsigned long downstream_bitrate;
extern unsigned long upstream_bitrate;

/* statup time */
extern time_t startup_time;
//Sercomm
extern long my_startup_time;
extern int gTTL;
extern int gNotifyInterval;

/* runtime boolean flags */
extern int runtime_flags;
#define LOGPACKETSMASK		0x0001
#define SYSUPTIMEMASK		0x0002
#ifdef ENABLE_NATPMP
#define ENABLENATPMPMASK	0x0004
#endif
#define CHECKCLIENTIPMASK	0x0008
#define SECUREMODEMASK		0x0010

#define ENABLEUPNPMASK		0x0020

#ifdef PF_ENABLE_FILTER_RULES
#define PFNOQUICKRULESMASK	0x0040
#endif

#define SETFLAG(mask)	(runtime_flags |= mask)
#define GETFLAG(mask)	(runtime_flags & mask)
#define CLEARFLAG(mask)	(runtime_flags &= ~mask)

extern const char * pidfilename;

extern char uuidvalue[];
extern char uuidvalue1[];
extern char uuidvalue2[];
extern char uuidvalue3[];

#define SERIALNUMBER_MAX_LEN (44)
extern char serialnumber[];

#define MODELNUMBER_MAX_LEN (48)
extern char modelnumber[];

#define PRESENTATIONURL_MAX_LEN (64)
extern char presentationurl[];

#define DEVICENAME_MAX_LEN (32)
extern char module_name[];

#define MODELDESCIPRTION_MAX_LEN (48)
extern char model_description[];
/* UPnP permission rules : */
extern struct upnpperm * upnppermlist;
extern unsigned int num_upnpperm;

#ifdef ENABLE_NATPMP
/* NAT-PMP */
extern unsigned int nextnatpmptoclean_timestamp;
extern unsigned short nextnatpmptoclean_eport;
extern unsigned short nextnatpmptoclean_proto;
#endif

#ifdef USE_PF
/* queue and tag for PF rules */
extern const char * queue;
extern const char * tag;
#endif

/* lan addresses */
/* MAX_LAN_ADDR : maximum number of interfaces
 * to listen to SSDP traffic */
#define MAX_LAN_ADDR (4)
extern int n_lan_addr;
extern struct lan_addr_s lan_addr[];

#endif


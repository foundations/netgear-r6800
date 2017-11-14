/* $Id: miniupnpdpath.h,v 1.5 2008/02/21 12:54:18 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2008 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#ifndef __MINIUPNPDPATH_H__
#define __MINIUPNPDPATH_H__

#include "config.h"

/* Paths and other URLs in the miniupnpd http server */

#define ROOTDESC_PATH 		"/rootDesc.xml"

#ifdef HAS_DUMMY_SERVICE
#define DUMMY_PATH			"/dummy.xml"
#endif

#define WANCFG_PATH			"/WANCfg.xml"
#define WANCFG_CONTROLURL	"/ctl/CmnIfCfg"
#define WANCFG_EVENTURL		"/evt/CmnIfCfg"

#define WANETHCFG_PATH          "/WANETHCfg.xml"
#define WANETHCFG_CONTROLURL    "/ctl/EthCfg"
#define WANETHCFG_EVENTURL      "/evt/Ethcfg"

#define WANIPC_PATH			"/WANIPCn.xml"
#define WANIPC_CONTROLURL	"/ctl/IPConn"
#define WANIPC_EVENTURL		"/evt/IPConn"

#define WANPPPC_PATH        "/WANPPPCn.xml"
#define WANPPPC_CONTROLURL  "/ctl/PPPConn"
#define WANPPPC_EVENTURL    "/evt/PPPConn"

#ifdef ENABLE_L3F_SERVICE
#define L3F_PATH			"/L3F.xml"
#define L3F_CONTROLURL		"/ctl/L3F"
#define L3F_EVENTURL		"/evt/L3F"
#endif

#ifdef ENABLE_LANDEVICE
#define LANHOSTCFG_CONTROLURL       "/ctl/LANHostcfg"
#define LANHOSTCFG_EVENTURL         "/evt/LANHostcfg"
#define LANHOSTCFG_PATH             "/LANHOSTCFG.xml"
#endif
#ifdef ENABLE_PNPX
#define PNPX_ICON_PATH              "/img/pnpxicon.png"
#endif /* ENABLE_PNPX */
#endif


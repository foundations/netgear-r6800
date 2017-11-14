/* $Id: upnpdescstrings.h,v 1.5 2007/02/09 10:12:52 nanard Exp $ */
/* miniupnp project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006 Thomas Bernard
 * This software is subject to the coditions detailed in
 * the LICENCE file provided within the distribution */
#ifndef __UPNPDESCSTRINGS_H__
#define __UPNPDESCSTRINGS_H__

#include "config.h"

#ifdef AMPED
#include "upnpglobalvars.h"
/* strings used in the root device xml description */
#define ROOTDEV_MANUFACTURER		OS_NAME
#define ROOTDEV_MANUFACTURERURL		OS_URL
#define ROOTDEV_MODELNAME		module_name
#define ROOTDEV_MODELDESCRIPTION	model_description
#define ROOTDEV_MODELURL			OS_URL

#define WANDEV_FRIENDLYNAME			"WANDevice"
#define WANDEV_MANUFACTURER			OS_NAME
#define WANDEV_MANUFACTURERURL		OS_URL
#define WANDEV_MODELNAME			"WAN Device"
#define WANDEV_MODELDESCRIPTION		"WAN Device"
#define WANDEV_MODELNUMBER			UPNP_VERSION
#define WANDEV_MODELURL				OS_URL
#define WANDEV_UPC					modelnumber

#define WANCDEV_FRIENDLYNAME		"WANConnectionDevice"
#define WANCDEV_MANUFACTURER		WANDEV_MANUFACTURER
#define WANCDEV_MANUFACTURERURL		WANDEV_MANUFACTURERURL
#define WANCDEV_MODELNAME			modelnumber
#define WANCDEV_MODELDESCRIPTION	"Residential Gateway"
#define WANCDEV_MODELNUMBER			UPNP_VERSION
#define WANCDEV_MODELURL			OS_URL
#define WANCDEV_UPC					modelnumber

#define LANDEV_FRIENDLYNAME			"LANDevice"
#define LANDEV_MANUFACTURER			OS_NAME
#define LANDEV_MANUFACTURERURL		OS_URL
#define LANDEV_MODELNAME			"LAN Device"
#define LANDEV_MODELDESCRIPTION		"LAN Device"
#define LANDEV_MODELNUMBER			UPNP_VERSION
#define LANDEV_MODELURL				OS_URL
#define LANDEV_UPC				modelnumber

#else

#include "upnpglobalvars.h"
//#ifndef MODULE_NAME
//#define MODULE_NAME			BOARD_ID
//#endif
/* strings used in the root device xml description */
#define ROOTDEV_FRIENDLYNAME		model_description//OS_NAME MODULE_NAME " Router"
#define ROOTDEV_MANUFACTURER		OS_NAME
#define ROOTDEV_MANUFACTURERURL		OS_URL
#define ROOTDEV_MODELNAME			model_description//OS_NAME MODULE_NAME " Router"
#define ROOTDEV_MODELDESCRIPTION	model_description//OS_NAME MODULE_NAME " Router"
#define ROOTDEV_MODELURL			OS_URL

#define WANDEV_FRIENDLYNAME			"WANDevice"
#define WANDEV_MANUFACTURER			"NETGEAR"
#define WANDEV_MANUFACTURERURL		"http://www.netgear.com/"
#define WANDEV_MODELNAME			"WAN Device"
#define WANDEV_MODELDESCRIPTION		"WAN Device"
#define WANDEV_MODELNUMBER			UPNP_VERSION
#define WANDEV_MODELURL				"http://www.netgear.com/"
#define WANDEV_UPC					modelnumber//MODULE_NAME

#define WANCDEV_FRIENDLYNAME		"WANConnectionDevice"
#define WANCDEV_MANUFACTURER		WANDEV_MANUFACTURER
#define WANCDEV_MANUFACTURERURL		WANDEV_MANUFACTURERURL
#define WANCDEV_MODELNAME			modelnumber//MODULE_NAME
#define WANCDEV_MODELDESCRIPTION	"Residential Gateway"
#define WANCDEV_MODELNUMBER			UPNP_VERSION
#define WANCDEV_MODELURL			"http://www.netgear.com/"
#define WANCDEV_UPC					modelnumber//MODULE_NAME

#define LANDEV_FRIENDLYNAME			"LANDevice"
#define LANDEV_MANUFACTURER			"NETGEAR"
#define LANDEV_MANUFACTURERURL		"http://www.netgear.com/"
#define LANDEV_MODELNAME			"LAN Device"
#define LANDEV_MODELDESCRIPTION		"LAN Device"
#define LANDEV_MODELNUMBER			UPNP_VERSION
#define LANDEV_MODELURL				"http://www.netgear.com/"
#define LANDEV_UPC					modelnumber//MODULE_NAME

#endif

#endif


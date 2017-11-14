/***************************************************************
*             Copyright (C) 2007 by SerComm Corp.
*                    All Rights Reserved.
*
*      Use of this software is restricted to the terms and
*      conditions of SerComm's software license agreement.
*
*                        www.sercomm.com
****************************************************************/

#define EXPORT_SYMTAB

#if defined(CONFIG_MODVERSIONS) && !defined(MODVERSIONS)
#define MODEVERSIONS
#endif

#ifdef MODVERSIONS
#include <linux/modversions.h>
#endif
#include <linux/module.h>

#include <sc/sc_fast_bridge.h>


//sc: Ron add for fast bridge
int (*fast_br_handle_frame_hook)(struct sk_buff *skb) = NULL;
void (*fast_br_register)(struct net_device *dev) =NULL;
void (*fast_br_unregister)(struct net_device *dev) = NULL;

//sc: Ron add for fast bridge
EXPORT_SYMBOL(fast_br_handle_frame_hook);
EXPORT_SYMBOL(fast_br_register);
EXPORT_SYMBOL(fast_br_unregister);


/*sc:
Description:
	used to improve the performace of bridge traffic


Key Word:
	[fast_bridge]

File List to add fast_bridge hook:
	- add. include/sc/sc_fast_bridge.h
	- net/core/dev.c
		1. netif_rx()
		2. register_netdevice()
		3. unregister_netdevice()
			

*/

/***************************************************************
*             Copyright (C) 2007 by SerComm Corp.
*                    All Rights Reserved.
*
*      Use of this software is restricted to the terms and
*      conditions of SerComm's software license agreement.
*
*                        www.sercomm.com
****************************************************************/

#define EXPORT_SYMTAB

#if defined(CONFIG_MODVERSIONS) && !defined(MODVERSIONS)
#define MODEVERSIONS
#endif

#ifdef MODVERSIONS
#include <linux/modversions.h>
#endif
#include <linux/module.h>

#include <sc/sc_fast_bridge.h>


//sc: Ron add for fast bridge
int (*fast_br_handle_frame_hook)(struct sk_buff *skb) = NULL;
void (*fast_br_register)(struct net_device *dev) =NULL;
void (*fast_br_unregister)(struct net_device *dev) = NULL;

//sc: Ron add for fast bridge
EXPORT_SYMBOL(fast_br_handle_frame_hook);
EXPORT_SYMBOL(fast_br_register);
EXPORT_SYMBOL(fast_br_unregister);


/*sc:
Description:
	used to improve the performace of bridge traffic


Key Word:
	[fast_bridge]

File List to add fast_bridge hook:
	- add. include/sc/sc_fast_bridge.h
	- net/core/dev.c
		1. netif_rx()
		2. register_netdevice()
		3. unregister_netdevice()
			

*/


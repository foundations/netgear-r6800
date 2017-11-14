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

#include <sc/sc_led.h>
#include <sc/sc_dome_blink.h>

//sc: add for led blink control
INT8 (*sc_dome_blink_hook)(UINT8 if_id,UINT8 blink_count,UINT8 on_time,UINT8 off_time)=NULL;

//sc: add for led blink control
EXPORT_SYMBOL(sc_dome_blink_hook);

/*sc:
Description:
	used to hook sercomm led driver to control TOP LED Blink


Key Word:
	[led_control]

File List to add led_control hook:
	- add. include/sc/sc_blink.h
	
	- net/core/dev.c
		1. add #include <sc/sc_blink.h>
		2. dev_queue_xmit()
		3. netif_receive_skb()
*/

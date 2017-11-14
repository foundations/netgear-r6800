/***************************************************************
*             Copyright (C) 2007 by SerComm Corp.
*                    All Rights Reserved.
*
*      Use of this software is restricted to the terms and
*      conditions of SerComm's software license agreement.
*
*                        www.sercomm.com
****************************************************************/

#ifndef _SC_LED_H_
#define _SC_LED_H_


#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef INT8
#define INT8 signed char
#endif

extern INT8 (*sc_led_on_hook)(UINT8 led_id);
extern INT8 (*sc_led_off_hook)(UINT8 led_id);
extern INT8 (*sc_led_blink_hook)(UINT8 led_id,UINT8 blink_count,UINT8 on_time,UINT8 off_time);
extern INT8 (*sc_led_blink_alt_hook)(UINT8 led_id1,UINT8 led_id2,UINT8 blink_count,UINT8 on_time,UINT8 off_time);

extern INT8 wps_running;
extern INT8 wifi_mode;

#define GPIO_INTERNET_LED 20
#define GPIO_INTERNET_RED_LED 30
#define GPIO_WLAN_LED 		40
#define GPIO_ADSL_LED 		50

#define WLAN_LED_24N	40
#define WLAN_LED_24G	41
#define WLAN_LED_5N	42

#define WLAN_TOP_LED1 	61
#define WLAN_TOP_LED2 	62
#define WLAN_TOP_LED3 	63
#define WLAN_TOP_LED4 	64
#define WLAN_TOP_LED5 	65
#define WLAN_TOP_LED6 	66
#define WLAN_TOP_LED7 	67
#define WLAN_TOP_LED8 	68

#define USB_HOST1_LED	60
#define USB_HOST2_LED	61
#define USB_HOST1_AMBER_LED	62
#define USB_HOST2_AMBER_LED	63

/* for usb driver led */
enum sc_led_event {
	LED_EVENT_NONE = 0,
	LED_EVENT_MOUNT = 1,
	LED_EVENT_UMOUNT = 2,
	LED_EVENT_XFER = 3,	
};

extern void sc_usb_led_event(enum sc_led_event e, int port);




#endif // _SC_LED_H_


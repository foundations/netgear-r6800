/***************************************************************
*             Copyright (C) 2007 by SerComm Corp.
*                    All Rights Reserved.
*
*      Use of this software is restricted to the terms and
*      conditions of SerComm's software license agreement.
*
*                        www.sercomm.com
****************************************************************/

#ifndef _SC_FAST_BR_H_
#define _SC_FAST_BR_H_

#include <linux/netdevice.h>
#include <linux/skbuff.h>

extern int (*fast_br_handle_frame_hook)(struct sk_buff *skb);
extern void (*fast_br_register)(struct net_device *dev);
extern void (*fast_br_unregister)(struct net_device *dev);

#endif //sc: _SC_FAST_BR_H_


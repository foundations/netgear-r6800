/***************************************************************
*             Copyright (C) 2007 by SerComm Corp.
*                    All Rights Reserved.
*
*      Use of this software is restricted to the terms and
*      conditions of SerComm's software license agreement.
*
*                        www.sercomm.com
****************************************************************/

#ifndef _SC_BLINK_H_
#define _SC_BLINK_H_


#ifndef UINT8
#define UINT8 unsigned char
#endif

#ifndef INT8
#define INT8 signed char
#endif

extern INT8 (*sc_dome_blink_hook)(UINT8 if_id,UINT8 blink_count,UINT8 on_time,UINT8 off_time);

#define WL0 0
#define WL1 1

#endif // _SC_BLINK_H_


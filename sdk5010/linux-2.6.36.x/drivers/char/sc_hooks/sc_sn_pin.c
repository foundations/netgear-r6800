/***************************************************************
*             Copyright (C) 2007 by SerComm Corp.
*                    All Rights Reserved.
*
*      Use of this software is restricted to the terms and
*      conditions of SerComm's software license agreement.
*
*                        www.sercomm.com
****************************************************************/

/*sc:
Description:
	1. create /proc/nvram/SN to store serila number from nvram
	2. create /proc/nvram/PIN to store PIN code from nvram 
	
Key Word:
	[sn_pin]

File List to add:
	bcmdrivers\opensource\char\board\bcm963xx\impl1\board.c
	
	proc_get_serial_param();
	proc_get_pin_param();
	
*/

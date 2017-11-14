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
	used to handle dscp/tos value map to priority, to pass 
	qos vista logo test


Key Word:
	[vista_qos]

File List to add vista logo:
	- kernel/linux/net/bridge/br_input.c
		1. Add #define VISTA_QOS_SUPPORT 1
		2. Add osl_pktsetprio2()
		3. Add tos2priority()
		4. br_handle_frame()	

*/


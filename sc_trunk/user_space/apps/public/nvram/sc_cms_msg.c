/*
 * Copyright (C) 2012  Sercomm Corporation
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//CmsRet cmsMsg_init(CmsEntityId eid, void **msgHandle)
int cmsMsg_init(int eid, void **msgHandle)
{
	//system("/bin/echo \"cmsMsg_init is called \" > /dev/console ");
	return 0; // success
}

//void cmsMsg_cleanup(void **msgHandle)
void cmsMsg_cleanup(void **msgHandle)
{
	//system("/bin/echo \"cmsMsg_cleanup is called \" > /dev/console");
	return;
}

//CmsRet cmsMsg_send(void *msgHandle, const CmsMsgHeader *buf)
int cmsMsg_send(void *msgHandle, void *buf)
{
	int temp;
	//system("/bin/echo \"cmsMsg_send is called \" > /dev/console");
	// Check the first 32bit(4byte) buf->type
	// if buf->type=0x10000300, then restart Wireless
	// CMS_MSG_WLAN_CHANGED = 0x10000300
	memcpy(&temp, buf , sizeof(int) );
//	printf("========%x========\n",temp);
	if( temp == 0x10000300 ){
//		system("/bin/echo \"========the message type is equal to 0x100003000========\" > /dev/console");
		system("/bin/touch /var/reset.wlan");
	}

	return 0; //success
}

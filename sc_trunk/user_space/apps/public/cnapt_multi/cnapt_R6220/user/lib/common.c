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
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "cui.h"

int commit_cmd(struct cui_cmd_t *uc, int size)
{
	int fd;
	int ret;

	fd = open("/proc/cnapt_serv", O_WRONLY);
	if (fd < 0) {
		return -EFAULT;
	}
	ret = write(fd, uc, size);
	close(fd);
	return (ret == size) ? 0 : ret;
}

int parse_ip(char *ipstr, u_int32_t *ipaddr)
{
	struct in_addr addr;

	if (*ipstr == 'a')
		*ipaddr = 0;
	else if (inet_pton(AF_INET, ipstr, &addr) == 1)
		*ipaddr = addr.s_addr;
	else
		return -1;
	return 0;
}

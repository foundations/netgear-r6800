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
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "common.h"
#include "cdmz.h"

int cdmz_clean(int enable, const char *dmz_ip)
{
	struct cui_cmd_t *uc;
	struct cdmz_t *cdmz;
	int alloc_size;
	struct in_addr addr;
	int ret;

	alloc_size = sizeof(struct cui_cmd_t) + sizeof(struct cdmz_t);
	uc = malloc(alloc_size);
	if (!uc)
		return -ENOMEM;
	cdmz = (struct cdmz_t*)uc->data;
	cdmz->enable = enable;
	if (inet_pton(AF_INET, dmz_ip, &addr) != 1) {
		ret = -EINVAL;
		goto out_free;
	}
	cdmz->server = addr.s_addr;
	uc->cmd = CUI_DMZ_CLEAN;
	ret = commit_cmd(uc, alloc_size);
out_free:
	free(uc);
	return ret;
}

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
#include <string.h>
#include <strings.h>

#include "cnapt.h"
#include "common.h"

void cnapt_flush_bypriv(char *lan_ip, char *lan_mask)
{
	struct cui_cmd_t *uc;
	struct cnapt_flush_args_t *fa;
	int alloc_size;

	alloc_size = sizeof(struct cui_cmd_t) + sizeof(struct cnapt_flush_args_t);
	uc = malloc(alloc_size);
	if (!uc)
		return ;
	memset(uc, 0, alloc_size);
	uc->cmd = CUI_CNAPT_FLUSH;
	fa = (struct cnapt_flush_args_t*)uc->data;
	if (parse_ip(lan_ip, &fa->lan_ip) < 0
		|| parse_ip(lan_mask, &fa->lan_mask) < 0)
		goto RET;
	commit_cmd(uc, alloc_size);
RET:
	free(uc);
	return ;
}

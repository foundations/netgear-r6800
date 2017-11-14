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

#ifndef __COMMON_H
#define __COMMON_H

#include "cui.h"

#define seep(fmt, args...)  do { \
	FILE *fp = fopen("/dev/console", "w"); \
	if (fp) { \
		fprintf(fp, "[ %s - %d ]: ", __FUNCTION__, __LINE__); \
		fprintf(fp, fmt, ## args); \
		fflush(fp); \
		fclose(fp); \
	} \
} while (0)

extern int commit_cmd(struct cui_cmd_t *uc, int size);

extern int parse_ip(char *ipstr, u_int32_t *ipaddr);

#endif

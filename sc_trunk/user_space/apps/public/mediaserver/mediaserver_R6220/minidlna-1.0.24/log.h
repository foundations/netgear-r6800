/* MiniDLNA media server
 * Copyright (C) 2008-2010 NETGEAR, Inc. All Rights Reserved.
 *
 * This file is part of MiniDLNA.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __ERR_H__
#define __ERR_H__

#define E_OFF		0
#define E_FATAL         1
#define E_ERROR		2
#define E_WARN          3
#define E_INFO          4
#define E_DEBUG		5

#undef __NR_clock_settime
#define __NR_clock_settime 227
#define SYS_clock_settime __NR_clock_settime
#undef __NR_clock_gettime
#define __NR_clock_gettime 228
#define SYS_clock_gettime __NR_clock_gettime
#undef __NR_inotify_init
#define __NR_inotify_init 253
#define SYS_inotify_init __NR_inotify_init
#undef __NR_inotify_add_watch
#define __NR_inotify_add_watch 254
#define SYS_inotify_add_watch __NR_inotify_add_watc
#undef __NR_inotify_rm_watch
#define __NR_inotify_rm_watch 255
#define SYS_inotify_rm_watch __NR_inotify_rm_watch

enum _log_facility
{
  L_GENERAL=0,
  L_ARTWORK,
  L_DB_SQL,
  L_INOTIFY,
  L_SCANNER,
  L_METADATA,
  L_HTTP,
  L_SSDP,
  L_TIVO,
  L_MAX
};

extern int log_level[L_MAX];
extern int log_init(const char *fname, const char *debug);
extern void log_err(int level, enum _log_facility facility, char *fname, int lineno, char *fmt, ...);

#define DPRINTF(level, facility, fmt, arg...) do { log_err(level, facility, __FILE__, __LINE__, fmt, ##arg); } while (0)
#if 0
#include <unistd.h>
#undef fprintf
#define DPRINTF_SC(level, facility, fmt, arg...) do { \
	FILE *fp; \
	if (access("/tmp/dlna_debug", F_OK)==0) {\
	fp = fopen("/dev/console", "a+"); \
	if (fp){fprintf(fp, "<%s - %d>: ",__FUNCTION__,__LINE__);fprintf(fp, fmt, ##arg);fclose(fp);}}}while(0)
#else
#define DPRINTF_SC(level, facility, fmt, arg...) do {}while(0)
#endif

#endif /* __ERR_H__ */

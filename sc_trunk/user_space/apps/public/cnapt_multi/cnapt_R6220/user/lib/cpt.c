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

#include "common.h"
#include "cnapt.h"
#include "cpt.h"

/* rule format: "enable(0,1):trig_proto(tcp,udp):trig_ip:trig_sport-trig_eport:inb_proto(tcp,udp,tcp/udp):inb_sport-inb_eport:timeout(minute)"
 * for example: "1:tcp:192.168.0.2:1000-1001:tcp/udp:2000-2001:5" */
static int cpt_rule_parse(struct cpt_args_t *pta, const char *rule)
{
	char tri_ip[16], tri_pro[12], tri_sport[16], tri_eport[16];
	char inb_proto[12], inb_sport[8], inb_eport[8];
	int enable;
	int timeout;

	if (sscanf(rule, "%d:%[^:]:%[^:]:%[^-]-%[^:]:%[^:]:%[^-]-%[^:]:%d"
			,&enable
			,tri_pro
			,tri_ip
			,tri_sport
			,tri_eport
			,inb_proto
			,inb_sport
			,inb_eport
			,&timeout) != 9) {
		seep( "invalid format!\n");
		return -1;
	}

	pta->enable = (u_int8_t)enable;
	pta->timeout = timeout * 60;
	/* trigger ip is any */
	if (parse_ip(tri_ip, &pta->trigip) < 0) {
		seep( "invalid trigip!\n");
		return -1;
	}

	switch (tri_pro[0]) {
	case 'T':
	case 't':
		pta->trigproto = IPPROTO_TCP;
		break;
	case 'U':
	case 'u':
		pta->trigproto = IPPROTO_UDP;
		break;
	default:
		seep( "invalid protocol!\n");
		return -EINVAL;
	}
	pta->trigport[0] = atoi(tri_sport);
	pta->trigport[1] = atoi(tri_eport);
	switch (inb_proto[0]) {
	case 'T':
	case 't':
		if (inb_proto[3] == '/')
			pta->inb_mr.proto = NAPT_PROTO_BOTH;
		else
			pta->inb_mr.proto = IPPROTO_TCP;
		break;
	case 'U':
	case 'u':
		pta->inb_mr.proto = IPPROTO_UDP;
		break;
	default:
		seep( "invalid protocol!\n");
		return -EINVAL;
	}
	pta->inb_mr.pubport[0] = pta->inb_mr.privport[0] = atoi(inb_sport);
	pta->inb_mr.pubport[1] = pta->inb_mr.privport[1] = atoi(inb_eport);
	pta->inb_mr.privip = pta->trigip;
	pta->inb_mr.pubip = 0;

	return 0;
}

int cpt_add(const char *rule)
{
	struct cui_cmd_t *uc;
	struct cpt_args_t *pta;
	int alloc_size;
	int ret;

	alloc_size = sizeof(struct cui_cmd_t) + sizeof(struct cpt_args_t);
	uc = malloc(alloc_size);
	if (!uc)
		return -ENOMEM;
	memset(uc, 0, alloc_size);
	pta = (struct cpt_args_t *)uc->data;
	if (cpt_rule_parse(pta, rule) < 0) {
		ret = -EINVAL;
		goto out_free;
	}
	uc->cmd = CUI_PT_ADD;
	ret = commit_cmd(uc, alloc_size);
out_free:
	free(uc);
	return ret;
}

int cpt_del(const char *rule)
{
	struct cui_cmd_t *uc;
	struct cpt_args_t *pta;
	int alloc_size;
	int ret;

	alloc_size = sizeof(struct cui_cmd_t) + sizeof(struct cpt_args_t);
	uc = malloc(alloc_size);
	if (!uc)
		return -ENOMEM;
	memset(uc, 0, alloc_size);
	pta = (struct cpt_args_t *)uc->data;
	if (cpt_rule_parse(pta, rule) < 0) {
		ret = -EINVAL;
		goto out_free;
	}
	uc->cmd = CUI_PT_DEL;
	ret = commit_cmd(uc, alloc_size);
out_free:
	free(uc);
	return ret;
}

int cpt_edit(const char *rule, const char *old_rule)
{
	struct cui_cmd_t *uc;
	struct cpt_args_t *pta, *pta_del;
	int alloc_size;
	int ret;

	alloc_size = sizeof(struct cui_cmd_t) + sizeof(struct cpt_args_t) * 2;
	uc = malloc(alloc_size);
	if (!uc)
		return -ENOMEM;
	memset(uc, 0, alloc_size);
	pta = (struct cpt_args_t *)uc->data;
	pta_del = &pta[1];
	if (cpt_rule_parse(pta, rule) < 0) {
		ret = -EINVAL;
		goto out_free;
	}
	if (cpt_rule_parse(pta_del, old_rule) < 0) {
		ret = -EINVAL;
		goto out_free;
	}
	uc->cmd = CUI_PT_EDIT;
	ret = commit_cmd(uc, alloc_size);
out_free:
	free(uc);
	return ret;
}

int cpt_flush()
{
    struct cui_cmd_t *uc;
    int alloc_size;
    int ret;
    alloc_size = sizeof(struct cui_cmd_t);
    uc = malloc(alloc_size);
    if (!uc)
        return -ENOMEM;;
    memset(uc, 0, alloc_size);
    uc->cmd = CUI_PT_FLUSH;
    ret = commit_cmd(uc, alloc_size);
    free(uc);
    return ret;
}

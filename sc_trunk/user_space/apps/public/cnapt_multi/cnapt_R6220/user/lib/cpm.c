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
#include "cpm.h"


/* rule format: "type(pf,upnp,ls):enable(0,1):proto(tcp,udp,tcp/udp):privip(x.x.x.x):sport-eport:pubip(any,x.x.x.x):sport-eport".
 * for example: "pf:1:tcp/udp:192.168.0.2:1000-1010:any:1000-1010" */
int cpm_rule_parse(struct cpm_args_t *pma, const char *rule)
{
	int enable;
	char type[12], proto[12];
	char privport[2][8], pubport[2][8], privip[16], pubip[16];

	if (sscanf(rule, "%[^:]:%d:%[^:]:%[^:]:%[^-]-%[^:]:%[^:]:%[^-]-%[^:]",
				type,
				&enable,
				proto,
				privip,
				privport[0],
				privport[1],
				pubip,
				pubport[0],
				pubport[1]) != 9) {
		seep("invalid format!\n");
		return -1;
	}

	pma->enable = enable;
	/* parse type. */
	if (strcasecmp(type, "pf") == 0)
		pma->type = NAPT_PM_PF;
	else if (strcasecmp(type, "upnp") == 0)
		pma->type = NAPT_PM_UPNP;
	else if (strcasecmp(type, "ls") == 0)
		pma->type = NAPT_PM_LS;
	else {
		seep( "warn: unknown port-mapping type!\n");
		pma->type = NAPT_PM_UNKNOWN;
	}

	switch (*proto) {
		case 'T':
		case 't':
			if (proto[3] == '/')
				pma->mr.proto = NAPT_PROTO_BOTH;
			else
				pma->mr.proto = IPPROTO_TCP;
			break;
		case 'U':
		case 'u':
			pma->mr.proto = IPPROTO_UDP;
			break;
		default:
			seep( "invalid protocol!\n");
			return -1;
	}
	if (parse_ip(privip, &pma->mr.privip) < 0) {
		seep( "invalid privip!\n");
		return -1;
	}
	if (parse_ip(pubip, &pma->mr.pubip) < 0) {
		seep( "invalid pubip!\n");
		return -1;
	}

	pma->mr.privport[0] = atoi(privport[0]);
	pma->mr.privport[1] = atoi(privport[1]);
	pma->mr.pubport[0] = atoi(pubport[0]);
	pma->mr.pubport[1] = atoi(pubport[1]);

	if (pma->mr.privip == 0 ||
		pma->mr.privport[0] > pma->mr.privport[1] ||
		pma->mr.pubport[0] > pma->mr.pubport[1]) {
		seep("invalid parms \n");
		return -1;
	}

	return 0;
}

int cpm_add(const char *rule)
{
	struct cui_cmd_t *uc;
	struct cpm_args_t *pma;
	int alloc_size;
	int ret;

	alloc_size = sizeof(struct cui_cmd_t) + sizeof(struct cpm_args_t);
	uc = malloc(alloc_size);
	if (!uc)
		return -ENOMEM;
	memset(uc, 0, alloc_size);
	pma = (struct cpm_args_t*)uc->data;
	if (cpm_rule_parse(pma, rule) < 0) {
		ret = -EINVAL;
		goto out_free;
	}
	uc->cmd = CUI_PM_ADD;
	ret = commit_cmd(uc, alloc_size);
out_free:
	free(uc);
	return ret;
}

int cpm_del(const char *rule)
{
	struct cui_cmd_t *uc;
	struct cpm_args_t *pma;
	int alloc_size;
	int ret;

	alloc_size = sizeof(struct cui_cmd_t) + sizeof(struct cpm_args_t);
	uc = malloc(alloc_size);
	if (!uc)
		return -ENOMEM;
	memset(uc, 0, alloc_size);
	pma = (struct cpm_args_t*)uc->data;
	if (cpm_rule_parse(pma, rule) < 0) {
		ret = -EINVAL;
		goto out_free;
	}
	uc->cmd = CUI_PM_DEL;
	ret = commit_cmd(uc, alloc_size);
out_free:
	free(uc);
	return ret;
}

int cpm_edit(const char *rule, const char *old_rule)
{
	struct cui_cmd_t *uc;
	struct cpm_args_t *pma;
	int alloc_size;
	int ret;

	alloc_size = sizeof(struct cui_cmd_t) + sizeof(struct cpm_args_t) * 2;
	uc = malloc(alloc_size);
	if (!uc)
		return -ENOMEM;
	memset(uc, 0, alloc_size);
	pma = (struct cpm_args_t*)uc->data;
	if (cpm_rule_parse(&pma[0], rule) < 0) {
		ret = -EINVAL;
		goto out_free;
	}
	if (cpm_rule_parse(&pma[1], old_rule) < 0) {
		ret = -EINVAL;
		goto out_free;
	}
	uc->cmd = CUI_PM_EDIT;
	ret = commit_cmd(uc, alloc_size);
out_free:
	free(uc);
	return ret;
}

void cpm_flush(char *type)
{
	struct cui_cmd_t *uc;
	struct cpm_args_t *pma;
	int alloc_size;

	alloc_size = sizeof(struct cui_cmd_t) + sizeof(struct cpm_args_t);
	uc = malloc(alloc_size);
	if (!uc)
		return;
	memset(uc, 0, alloc_size);
	uc->cmd = CUI_PM_FLUSH;
	pma = (struct cpm_args_t*)uc->data;
	if (strcasecmp(type, "pf") == 0)
		pma->type = NAPT_PM_PF;
	else if (strcasecmp(type, "upnp") == 0)
		pma->type = NAPT_PM_UPNP;
	else if (strcasecmp(type, "ls") == 0)
		pma->type = NAPT_PM_LS;
	else {
		seep( "warn: unknown port-mapping type!\n");
		pma->type = NAPT_PM_UNKNOWN;
	}
	commit_cmd(uc, alloc_size);
	free(uc);
	return ;
}

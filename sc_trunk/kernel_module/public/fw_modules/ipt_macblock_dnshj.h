#ifndef _IPT_MACBLOCK_DNSHJ_H
#define _IPT_MACBLOCK_DNSHJ_H
#define IPADDR_LEN 16
struct ipt_macblock_dnshj_info {
	char serv_ip[IPADDR_LEN];
};
#endif

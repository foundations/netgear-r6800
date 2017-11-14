#ifndef _SC_NET_H_
#define _SC_NET_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LEASES_TIME 60*60*24*3
#define MAXHOSTNAMELEN 64

// copy from udhcp
struct leases_t {
    unsigned char chaddr[16];
    u_int32_t yiaddr;
    u_int32_t expires;
    char hostname[MAXHOSTNAMELEN];//(001)
    unsigned char cid[48];//change 16->48 to solution define MAX_CLIENT_LEN
};
/*
#vlan support
vlan_enable=0
vlan_tag_group_org=
vlan_tag_group=1:1:Internet:11:0:01011010 1:2:Intranet:20:0:10100000
vlan_untag_group=
 */
#define MAX_VLAN_GROUP	10

enum {
	PORT_LAN1=0, PORT_LAN2, PORT_LAN3, PORT_LAN4,
	PORT_WLAN_2G, PORT_WLAN_5G, PORT_WLAN_2G_GUEST, PORT_WLAN_5G_GUEST,
	MAX_VLAN_PORT
};

#define VLAN_GROUP_BRIDGE	0
#define VLAN_GROUP_INTERNET	1
#define VLAN_GROUP_INTRANET	2

struct s_vlan_group
{
	char valid;
	char enable;
	int type; //internet, intranet or bridged
	char name[32];
	unsigned int vid;
	unsigned int pri;
	char ports[MAX_VLAN_PORT + 1];
};

#define ACL_PARSE_STR(name) do  { \
				    q = name;   \
				    if (!q) break; \
				    *q = '\0';  \
				    while (*p && *p != 0x1 && *p != 0x2) \
					*q++ = *p++;        \
				    *q = '\0'; \
				    if (*p) p++; /* move to next entry */ \
				} while(0); 

#define ACL_PARSE_INT(name) do  { \
    				    char tmp[16] = "";  \
				    if (!name) break;   \
				    ACL_PARSE_STR(tmp); \
				    if (tmp[0]) \
				        *name = atoi(tmp); \
    			        } while(0);

int acl_parse_entries(char *buf, char *str1, char *str2, int *int1, int *int2);
int acl_parse_reqinfo(char *buf, char *str1, char *str2, int *int1, char *str3);
int acl_parse_name(char *buf, char *name);

int search_leases( char *hostname , struct in_addr addr);
int search_leases_mac( char *hostname , char *t_mac);
int search_nbtscan(char *mac, char *name, int *isWlan);
int search_ip_by_leases_mac( char *ipaddr , char *t_mac);
int search_ip_by_arptbl_mac(char *ipaddr , char *t_mac);
int search_ip_by_mac(char *ipaddr , char *t_mac);
int search_mac_by_arptbl_ip(char *t_mac , char *ipaddr);
int search_mac_by_ip(char *t_mac, char *ipaddr);
int search_mac_hostname_by_nbs_ad_file(char *t_mac, char *hostname, char *ipaddr);

#endif


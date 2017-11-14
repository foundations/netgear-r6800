#ifndef _SC_VLAN_H_
#define _SC_VLAN_H_

#include <sys/socket.h>
#include <netinet/in.h>
//#include <arpa/inet.h>

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

#define WAN1		1
#define WAN2		2
#define PPPoE		0
#define DHCPC		1
typedef struct {
	char *country;
	char *isp;
	int vlan_id;// 1 ~ 4094 ? depends
	short mode; // 0 = PPPoE, 1 = dhcpc
       short wan_idx; //1 = WAN1, 2 = WAN2 ....
} IPTV_SUPPORT_LIST;

int get_tag_group(struct s_vlan_group *group);
int get_untag_group(struct s_vlan_group *group);

int wlan_has_bridge_port(void);
int lan_has_bridge_port(void);
int vlan_hidden_Intranet(void);
int *get_lan_wlan_port_mapping(void);
int get_priority_by_vlan_id(int vlan_id);
int igmp_work_interface();
#endif


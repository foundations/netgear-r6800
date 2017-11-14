#ifndef _PACKET_H
#define _PACKET_H

#include <netinet/udp.h>
#include <netinet/ip.h>

/*increase option len,'cause we need support option 33 and 
  250 with 32 routing(256 byte)*/
/*increase option len,'cause HP_PSC_2510 need 26 bytes padding 
  after OPTION END to get IP successfully. */
#define OPTION_LEN	590	
struct dhcpMessage {
	u_int8_t op;
	u_int8_t htype;
	u_int8_t hlen;
	u_int8_t hops;
	u_int32_t xid;
	u_int16_t secs;
	u_int16_t flags;
	u_int32_t ciaddr;
	u_int32_t yiaddr;
	u_int32_t siaddr;
	u_int32_t giaddr;
	u_int8_t chaddr[16];
	u_int8_t sname[64];
	u_int8_t file[128];
	u_int32_t cookie;
    /* if the DHCPOFFER/DHCPACK legth is greater than 576 will not receive 
        change limit to 832
     */
	u_int8_t options[OPTION_LEN]; /* 312 - cookie */ 
};

struct udp_dhcp_packet {
	struct iphdr ip;
	struct udphdr udp;
	struct dhcpMessage data;
};

void init_header(struct dhcpMessage *packet, char type);
int get_packet(struct dhcpMessage *packet, int fd);
u_int16_t checksum(void *addr, int count);
int raw_packet(struct dhcpMessage *payload, u_int32_t source_ip, int source_port,
		   u_int32_t dest_ip, int dest_port, unsigned char *dest_arp, int ifindex);
int kernel_packet(struct dhcpMessage *payload, u_int32_t source_ip, int source_port,
		   u_int32_t dest_ip, int dest_port);


#endif

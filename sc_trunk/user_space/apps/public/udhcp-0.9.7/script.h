#ifndef _SCRIPT_H
#define _SCRIPT_H

void run_script(struct dhcpMessage *packet, const char *name);
int dhcp_packet_cmp(struct dhcpMessage *packet1, struct dhcpMessage *packet2);

#endif

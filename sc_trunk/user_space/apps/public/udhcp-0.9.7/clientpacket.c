/* clientpacket.c
 *
 * Packet generation and dispatching functions for the DHCP client.
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <string.h>
#include <sys/socket.h>
#include <features.h>
#if __GLIBC__ >=2 && __GLIBC_MINOR >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>
#else
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#endif
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "dhcpd.h"
#include "packet.h"
#include "options.h"
#include "dhcpc.h"
#include "debug.h"

#ifdef DHCP_OPTION
#include "nvram.h"
#endif
#ifdef TR069
#include <ctype.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define NETGEAR_ENTERPRISE_NUMBER 4526  /* assigned by IANA */
#define CWMP_MANUFACTURER_OUI    "000FB5"
#define CWMP_MODEL_NAME         "DG834G"
#define CWMP_MANUFACTURER_SERIAL "000FB5B9E270"
static char *cpe_getSerialNumber(void)
{
    FILE *fp;
    char tmp[18] = {'\0'};
    static	char serialnumber[18] = {0,};
    int i=0,j=0;

    if((fp=fopen("/tmp/mac","r"))!=NULL){
	fread(tmp,20,1,fp);
	tmp[17] = '\0';
	for(i=0;i<17;i++){
	    if(tmp[i] != ':'){
		    serialnumber[j] = tmp[i];
		    j++;
	    }
	}
	fclose(fp);
        serialnumber[j] = '\0';
	return (serialnumber);
    }
    fclose(fp);
    sprintf(serialnumber,"%s",CWMP_MANUFACTURER_SERIAL);

    return (serialnumber);
}

static void add_vendor_options(struct dhcpMessage *packet)
{
    unsigned char *data = NULL,*pt;
    unsigned char oui[9]="",sn[66]="",prds[66]="";
    int len = 0,rc=-1;

    pt = cpe_getSerialNumber();
    len = sprintf(oui,"%c%c%s",(unsigned char)1,(unsigned char)strlen(CWMP_MANUFACTURER_OUI),CWMP_MANUFACTURER_OUI);
    len += sprintf(sn,"%c%c%s",(unsigned char)2,(unsigned char)strlen(pt),pt);
    len += sprintf(prds,"%c%c%s",(unsigned char)3,(unsigned char)strlen(CWMP_MODEL_NAME),CWMP_MODEL_NAME);

    data = (unsigned char *)malloc(len + 8);
    if(data==NULL)
        return;
    memset(data, 0, len+8);
    sprintf(data,"%c%c%c%c%c%c%c%s%s%s"
                ,(unsigned char)DHCP_VENDOR_SPEC     /* option-code 8bits */
                ,(unsigned char)(len + 5)            /* option-len 8bits */
                /* enterprise-number 32bits */
                ,(unsigned char)(NETGEAR_ENTERPRISE_NUMBER>>24)
                ,(unsigned char)(NETGEAR_ENTERPRISE_NUMBER>>16)
                ,(unsigned char)(NETGEAR_ENTERPRISE_NUMBER>>8)
                ,(unsigned char)NETGEAR_ENTERPRISE_NUMBER
                ,(unsigned char)len                  /* data-len 8bits */
                /* sub-option*/
                ,oui
                ,sn
                ,prds);

    rc = add_option_string(packet->options, data);

    free(data);
    return;
}
#endif
#ifdef DHCP_OPTION
static void add_60_options(struct dhcpMessage *packet)
{
    char *op_str = nvram_safe_get("op60_str");
    //int rc=-1;

    struct vendor  {
		char vendor, length;
		char str[64];
    } vendor_id;// = { DHCP_VENDOR,  sizeof("udhcp "VERSION) - 1, "udhcp "VERSION};
    vendor_id.vendor = DHCP_VENDOR;
    vendor_id.length = strlen(op_str);
    strncpy(vendor_id.str, op_str, sizeof(vendor_id.str));

    //rc = add_option_string(packet->options, (unsigned char *)&vendor_id);
	add_option_string(packet->options, (unsigned char *)&vendor_id);
    return;
}

static void add_61_options(struct dhcpMessage *packet)
{
    char *op_str = nvram_safe_get("op61_str");
    //int rc=-1;

    struct vendor  {
		char vendor, length;
		char str[64];
    } vendor_id;
    vendor_id.vendor = DHCP_CLIENT_ID;
    vendor_id.length = strlen(op_str);
    strncpy(vendor_id.str, op_str, sizeof(vendor_id.str));

    //rc = add_option_string(packet->options, (unsigned char *)&vendor_id);
	add_option_string(packet->options, (unsigned char *)&vendor_id);
    return;
}
#endif
/* Create a random xid */
unsigned long random_xid(void)
{
	static int initialized;
	if (!initialized) {
		long unsigned int t = time(0);
		long unsigned int pid = getpid();
		LOG(LOG_DEBUG, "t:%lu, pid:%lu", t, pid);
		srand(t+pid);
		initialized++;
	}
	return rand();
}


/* initialize a packet with the proper defaults */
static void init_packet(struct dhcpMessage *packet, char type)
{
#ifdef DHCP_OPTION
       char *op60_str = nvram_safe_get("op60_str");
       char *op61_str = nvram_safe_get("op61_str");
#endif
	struct vendor  {
		char vendor, length;
		char str[sizeof("udhcp "VERSION)];
	} vendor_id = { DHCP_VENDOR,  sizeof("udhcp "VERSION) - 1, "udhcp "VERSION};

	init_header(packet, type);
	memcpy(packet->chaddr, client_config.arp, 6);
#ifdef DHCP_OPTION
       if(strlen(op61_str) > 0)
           add_61_options(packet);
	else
#endif
	add_option_string(packet->options, client_config.clientid);

	if (client_config.hostname) add_option_string(packet->options, client_config.hostname);
#ifdef DHCP_OPTION
       if(strlen(op60_str) > 0)
           add_60_options(packet);
       else
#endif
	add_option_string(packet->options, (unsigned char *) &vendor_id);
}


/* Add a paramater request list for stubborn DHCP servers. Pull the data
 * from the struct in options.c. Don't do bounds checking here because it
 * goes towards the head of the packet. */
static void add_requests(struct dhcpMessage *packet)
{
	int end = end_option(packet->options);
	int i, len = 0;

	packet->options[end + OPT_CODE] = DHCP_PARAM_REQ;
	for (i = 0; options[i].code; i++)
		if (options[i].flags & OPTION_REQ)
			packet->options[end + OPT_DATA + len++] = options[i].code;
	packet->options[end + OPT_LEN] = len;
	packet->options[end + OPT_DATA + len] = DHCP_END;
#ifdef TR069
    add_vendor_options(packet);
#endif
}


/* Broadcast a DHCP discover packet to the network, with an optionally requested IP */
int send_discover(unsigned long xid, unsigned long requested)
{
	struct dhcpMessage packet;
	init_packet(&packet, DHCPDISCOVER);
	packet.xid = xid;
	if (requested)
		add_simple_option(packet.options, DHCP_REQUESTED_IP, requested);

	add_requests(&packet);
	LOG(LOG_DEBUG, "Sending discover...with xid %lu", xid);
	return raw_packet(&packet, INADDR_ANY, CLIENT_PORT, INADDR_BROADCAST,
				SERVER_PORT, MAC_BCAST_ADDR, client_config.ifindex);
}

/* Send DHCPDECLIENT to server
    MUST:   DHCP message type
            Requested IP address
 */
int send_decline(uint32_t xid, uint32_t server, uint32_t requested)
{
    struct dhcpMessage packet;

    init_packet(&packet, DHCPDECLINE);
    packet.xid = xid;
    add_simple_option(packet.options, DHCP_REQUESTED_IP, requested);
    add_simple_option(packet.options, DHCP_SERVER_ID, server);

    LOG(LOG_DEBUG, "Sending DHCPDECLINE...with xid %u", xid);
    return raw_packet(&packet, INADDR_ANY, CLIENT_PORT, INADDR_BROADCAST,
            SERVER_PORT, MAC_BCAST_ADDR, client_config.ifindex);
}

/* Broadcasts a DHCP request message */
int send_selecting(unsigned long xid, unsigned long server, unsigned long requested)
{
	struct dhcpMessage packet;
	struct in_addr addr;

	init_packet(&packet, DHCPREQUEST);
	packet.xid = xid;

	add_simple_option(packet.options, DHCP_REQUESTED_IP, requested);
	add_simple_option(packet.options, DHCP_SERVER_ID, server);

	add_requests(&packet);
	addr.s_addr = requested;
	LOG(LOG_DEBUG, "Sending select for %s xid %lu...", inet_ntoa(addr), xid);
	return raw_packet(&packet, INADDR_ANY, CLIENT_PORT, INADDR_BROADCAST,
				SERVER_PORT, MAC_BCAST_ADDR, client_config.ifindex);
}


/* Unicasts or broadcasts a DHCP renew message */
int send_renew(unsigned long xid, unsigned long server, unsigned long ciaddr)
{
	struct dhcpMessage packet;
	int ret = 0;
	
	init_packet(&packet, DHCPREQUEST);
	packet.xid = xid;
	packet.ciaddr = ciaddr;
	
	add_requests(&packet);
		
	LOG(LOG_DEBUG, "Sending renew to <%lu> xid <%lu>", server, xid);
	if (server) {
        /*
         * Netgear Spec1.9:
         * Must put its current IP address in the source IP address field of the DHCPREQUEST's
         * instead of using 0.0.0.0.
         */
		ret = kernel_packet(&packet, ciaddr, CLIENT_PORT, server, SERVER_PORT);
	}
	else {
	    ret = raw_packet(&packet, ciaddr, CLIENT_PORT, INADDR_BROADCAST,
				SERVER_PORT, MAC_BCAST_ADDR, client_config.ifindex);
    }
    LOG(LOG_DEBUG, "Sending renew ret is <%d>", ret);
	return ret;
}


/* Unicasts a DHCP release message */
int send_release(unsigned long server, unsigned long ciaddr)
{
	struct dhcpMessage packet;

	init_packet(&packet, DHCPRELEASE);
	packet.xid = random_xid();
	packet.ciaddr = ciaddr;

	add_simple_option(packet.options, DHCP_REQUESTED_IP, ciaddr);
	add_simple_option(packet.options, DHCP_SERVER_ID, server);

	LOG(LOG_DEBUG, "Sending release...");
	return kernel_packet(&packet, ciaddr, CLIENT_PORT, server, SERVER_PORT);
}


int get_raw_packet(struct dhcpMessage *payload, int fd)
{
	int bytes;
	struct udp_dhcp_packet packet;
	u_int32_t source, dest;
	u_int16_t check;

	memset(&packet, 0, sizeof(struct udp_dhcp_packet));
	bytes = read(fd, &packet, sizeof(struct udp_dhcp_packet));
	if (bytes < 0) {
		DEBUG(LOG_INFO, "couldn't read on raw listening socket -- ignoring");
		usleep(500000); /* possible down interface, looping condition */
		return -1;
	}

	if (bytes < (int) (sizeof(struct iphdr) + sizeof(struct udphdr))) {
		DEBUG(LOG_INFO, "message too short, ignoring");
		return -1;
	}

	if ((unsigned)bytes < (unsigned)ntohs(packet.ip.tot_len)) {
		DEBUG(LOG_INFO, "Truncated packet");
		return -1;
	}

	/* ignore any extra garbage bytes */
	bytes = ntohs(packet.ip.tot_len);

	/* Make sure its the right packet for us, and that it passes sanity checks */
	if (packet.ip.protocol != IPPROTO_UDP || packet.ip.version != IPVERSION ||
	    packet.ip.ihl != sizeof(packet.ip) >> 2 || packet.udp.dest != htons(CLIENT_PORT) ||
	    bytes > (int) sizeof(struct udp_dhcp_packet) ||
	    ntohs(packet.udp.len) != (unsigned short) (bytes - sizeof(packet.ip))) {
	    	DEBUG(LOG_INFO, "unrelated/bogus packet");
	    	return -1;
	}

	/* check IP checksum */
	check = packet.ip.check;
	packet.ip.check = 0;
	if (check != checksum(&(packet.ip), sizeof(packet.ip))) {
		DEBUG(LOG_INFO, "bad IP header checksum, ignoring");
		return -1;
	}

	/* verify the UDP checksum by replacing the header with a psuedo header */
	source = packet.ip.saddr;
	dest = packet.ip.daddr;
	check = packet.udp.check;
	packet.udp.check = 0;
	memset(&packet.ip, 0, sizeof(packet.ip));

	packet.ip.protocol = IPPROTO_UDP;
	packet.ip.saddr = source;
	packet.ip.daddr = dest;
	packet.ip.tot_len = packet.udp.len; /* cheat on the psuedo-header */
	if (check && check != checksum(&packet, bytes)) {
		DEBUG(LOG_ERR, "packet with bad UDP checksum received, ignoring");
		return -1;
	}

	memcpy(payload, &(packet.data), bytes - (sizeof(packet.ip) + sizeof(packet.udp)));

	if (ntohl(payload->cookie) != DHCP_MAGIC) {
		LOG(LOG_ERR, "received bogus message (bad magic) -- ignoring");
		return -1;
	}
	DEBUG(LOG_INFO, "oooooh!!! got some!");
	return bytes - (sizeof(packet.ip) + sizeof(packet.udp));

}


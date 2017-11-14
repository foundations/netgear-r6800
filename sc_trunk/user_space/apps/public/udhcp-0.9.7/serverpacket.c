/* serverpacket.c
 *
 * Constuct and send DHCP server packets
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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#include "packet.h"
#include "debug.h"
#include "dhcpd.h"
#include "options.h"
#include "leases.h"
/* For wps */
#include <unistd.h>
#include <stdlib.h>
#include "nvram.h"
#include "nvram_var.h"
//ray
#include <asm/param.h>

/* laoyou add for implementing TR111 part1 */
#ifdef TR069
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define NETGEAR_ENTERPRISE_NUMBER 4526  /* assigned by IANA */
#define CWMP_MANUFACTURER_OUI    "000FB5"
#define CWMP_MODEL_NAME         "DG834G"
#define CWMP_MANUFACTURER_SERIAL "000FB5B9E270"
#define GW_IDENTITY "/tmp/dhcp.vendoroption"

#ifndef cprintf
#define cprintf(fmt, args...) do { \
	FILE *fp = fopen("/dev/console", "w"); \
	if (fp) { \
		fprintf(fp, fmt, ## args); \
		fclose(fp); \
	} \
} while (0)
#endif

VENDOR_LIST *vendor_t=NULL;
#endif

#include <syslog.h>
#include "misc.h"

/* send a packet to giaddr using the kernel ip stack */
static int send_packet_to_relay(struct dhcpMessage *payload, int ifid)
{
	DEBUG(LOG_INFO, "Forwarding packet to relay");

	return kernel_packet(payload, server_config[ifid].server, SERVER_PORT,
			payload->giaddr, SERVER_PORT);
}


/* send a packet to a specific arp address and ip address by creating our own ip packet */
static int send_packet_to_client(struct dhcpMessage *payload, int force_broadcast, int ifid)
{
	unsigned char *chaddr;
	u_int32_t ciaddr;

	if (force_broadcast) {
		DEBUG(LOG_INFO, "broadcasting packet to client (NAK)");
		ciaddr = INADDR_BROADCAST;
		chaddr = MAC_BCAST_ADDR;
	} else if (payload->ciaddr) {
		DEBUG(LOG_INFO, "unicasting packet to client ciaddr");
		ciaddr = payload->ciaddr;
		chaddr = payload->chaddr;
	} else if (ntohs(payload->flags) & BROADCAST_FLAG) {
		DEBUG(LOG_INFO, "broadcasting packet to client (requested)");
		ciaddr = INADDR_BROADCAST;
		chaddr = MAC_BCAST_ADDR;
	} else {
		DEBUG(LOG_INFO, "unicasting packet to client yiaddr");
		ciaddr = payload->yiaddr;
		chaddr = payload->chaddr;
	}
	return raw_packet(payload, server_config[ifid].server, SERVER_PORT,
			ciaddr, CLIENT_PORT, chaddr, server_config[ifid].ifindex);
}


/* send a dhcp packet, if force broadcast is set, the packet will be broadcast to the client */
static int send_packet(struct dhcpMessage *payload, int force_broadcast, int ifid)
{
	int ret;

	if (payload->giaddr)
		ret = send_packet_to_relay(payload, ifid);
	else ret = send_packet_to_client(payload, force_broadcast, ifid);
	return ret;
}


static void init_packet(struct dhcpMessage *packet, struct dhcpMessage *oldpacket, char type, int ifid)
{
	init_header(packet, type);
	packet->xid = oldpacket->xid;
	memcpy(packet->chaddr, oldpacket->chaddr, 16);
	packet->flags = oldpacket->flags;
	packet->giaddr = oldpacket->giaddr;
	packet->ciaddr = oldpacket->ciaddr;
	add_simple_option(packet->options, DHCP_SERVER_ID, server_config[ifid].server);

}


/* add in the bootp options */
static void add_bootp_options(struct dhcpMessage *packet, int ifid)
{
	packet->siaddr = server_config[ifid].siaddr;
	if (server_config[ifid].sname)
		strncpy((char *)packet->sname, server_config[ifid].sname, sizeof(packet->sname) - 1);
	if (server_config[ifid].boot_file)
		strncpy((char *)packet->file, server_config[ifid].boot_file, sizeof(packet->file) - 1);
}

#ifdef TR069
static char *cpe_getSerialNumber(void)
{
    FILE *fp;
    char tmp[18] = {'\0'};
    static	char serialnumber[18] = {0,};
    int i=0,j=0;

    if((fp=fopen("/proc/mac","r"))!=NULL){
fread(tmp,sizeof(tmp),1,fp);
	tmp[17] = '\0';
	for(i=0;i<(int)(sizeof(tmp)-1);i++){
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
    len = sprintf(oui,"%c%c%s",(unsigned char)4,(unsigned char)strlen(CWMP_MANUFACTURER_OUI),CWMP_MANUFACTURER_OUI);
    len += sprintf(sn,"%c%c%s",(unsigned char)5,(unsigned char)strlen(pt),pt);
    len += sprintf(prds,"%c%c%s",(unsigned char)6,(unsigned char)strlen(CWMP_MODEL_NAME),CWMP_MODEL_NAME);

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

static void write_list_to_file(VENDOR_LIST *list, FILE *fp)
{
    VENDOR_LIST *vd_p = list;

    while(vd_p!=NULL){
        fprintf(fp,"%s;%s;%s;%s\n",vd_p->ip,vd_p->oui,vd_p->serialNumber,vd_p->productClass);
        vd_p = vd_p->next;
    }
    return;
}

static void add_vendor_list(unsigned char *vendor_spc, u_int32_t caddr)
{
    VENDOR_LIST *vd_p = vendor_t;
    unsigned char *oui,*sn,*prdt;
    int i,oui_len,sn_len,prdt_len;
    struct in_addr in;
    FILE *fp;

    i = 6;
    oui_len = vendor_spc[i];
    oui = vendor_spc + i +1;
    i += 1 + oui_len + 1;
    sn_len = vendor_spc[i];
    sn = vendor_spc + i +1;
    i += 1 + sn_len + 1;
    prdt_len = vendor_spc[i];
    prdt = vendor_spc + i +1;

    while(vd_p!=NULL){
        /* already existed, re-write it because it may be modified */
        if(vd_p->client == caddr)
            goto vl;
        vd_p = vd_p->next;
    }

    if(vendor_t == NULL){
        vendor_t=(VENDOR_LIST *)malloc(sizeof(VENDOR_LIST));
        vd_p = vendor_t;
    }
    else{
        vd_p = vendor_t;
        while(vd_p->next!=NULL)
            vd_p = vd_p->next;
        vd_p->next = (VENDOR_LIST *)malloc(sizeof(VENDOR_LIST));
        vd_p = vd_p->next;
    }

  vl:
    memset(vd_p,0,sizeof(VENDOR_LIST));
    vd_p->client = caddr;
    in.s_addr = caddr;
    strncpy(vd_p->ip,inet_ntoa(in),15);
    strncpy(vd_p->oui,oui,oui_len);
    strncpy(vd_p->serialNumber,sn,sn_len);
    strncpy(vd_p->productClass,prdt,prdt_len);
    vd_p->next = NULL;

    unlink(GW_IDENTITY);
    fp = fopen(GW_IDENTITY,"w");
    write_list_to_file(vendor_t,fp);
    fclose(fp);
    system("/usr/bin/killall -USR2 cwmp > /dev/null 2>&1");

    return;
}

void delete_vendor_list(u_int32_t caddr)
{
    VENDOR_LIST *vd_p, *vd_f;
    int flag=0;
    FILE *fp;


    struct in_addr in;
    in.s_addr = caddr;

    vd_p = vendor_t;
    vd_f=vd_p;
    while(vd_p!=NULL){
        /* already existed, do nothing */
        if(vd_p->client == caddr){
            if(vd_p == vendor_t){
                vendor_t = vd_p->next;
            }
            else{
                vd_f->next = vd_p->next;
            }
            free(vd_p);
            flag=1;
            break;
        }
        vd_f = vd_p;
        vd_p = vd_p->next;
    }

    if(flag){ /* have change, re-write */
        unlink(GW_IDENTITY);
        if(vendor_t){
            fp = fopen(GW_IDENTITY,"w");
            write_list_to_file(vendor_t,fp);
            fclose(fp);
        }
        system("/usr/bin/killall -USR2 cwmp > /dev/null 2>&1");
    }
    return;
}
#endif

/*
 * Try to find a free IP address. Return network order ip.
 */
static u_int32_t get_free_address(int ifid) {
    u_int32_t yiaddr;

    yiaddr = find_address(0, ifid);
    /* try for an expired lease */
    if (!yiaddr) {
        yiaddr = find_address(1, ifid);
    }
    return yiaddr;
}

/* send a DHCP OFFER to a DHCP DISCOVER */
int sendOffer(struct dhcpMessage *oldpacket, int ifid)
{
	struct dhcpMessage packet;
	struct dhcpOfferedAddr *lease = NULL;
	u_int32_t req_align, lease_time_align = server_config[ifid].lease;
	unsigned char *req, *lease_time;
	struct option_set *curr;
	struct in_addr addr;
#ifdef RONSCODE
	char hostname[MAXHOSTNAMELEN]="";
	char cid[MAX_CLIENT_LEN]="";
#endif
#ifdef TR069
    unsigned char *vendor_spc;
	vendor_spc = get_option(oldpacket, DHCP_VENDOR_SPEC);
#endif
	init_packet(&packet, oldpacket, DHCPOFFER, ifid);
	/* ADDME: if static, short circuit */

    /* 1. the client is reserved */
    DEBUG(LOG_INFO, "1. Checking if the client is reserved?");
	if ( (lease = find_reserved_by_chaddr(oldpacket->chaddr, ifid)) ) {
		packet.yiaddr = lease->yiaddr;  /* FIXME: IP conflict checking? Seems spec issue. -- Argon */
		goto gotip;
    }

    /* 2. the client is in our lease/offered table */
    DEBUG(LOG_INFO, "2. Checking if the client is in our lease/offered table?");
    if ( (lease = find_lease_by_chaddr(oldpacket->chaddr, ifid)) ) {      
        if(find_reserved_by_yiaddr(lease->yiaddr, ifid) || check_ip(lease->yiaddr, ifid, lease->chaddr)) {
            /* but this IP is reserved to another MAC or somebody is using this IP, find a free one */
            packet.yiaddr = get_free_address(ifid);
        }        
        else {
            if(ntohl(lease->yiaddr) == ntohl(server_config[ifid].server))
            {
                DEBUG(LOG_INFO, "The reqeust ip is lan ip, lease->yiaddr =%08x, server_config[ifid].server=%08x", ntohl(lease->yiaddr), ntohl(server_config[ifid].server) );
                /*if this IP is lan ip, release the lease entry and find a free one; cindy,2011-1-10*/
                memset(lease, 0, sizeof(struct dhcpOfferedAddr));
                packet.yiaddr = get_free_address(ifid);
            }
            else
            {    
                if (!lease_expired(lease)) {
    			    lease_time_align = lease->expires - time(0);
    		    }
    		    packet.yiaddr = lease->yiaddr;
		    }
		}
		goto gotip;
	}

    /* 3. Try to offer an new IP */
    DEBUG(LOG_INFO, "3. Try to offer an new IP");
    if ((req = get_option(oldpacket, DHCP_REQUESTED_IP))) {
        /* if the client has a requested ip */
	    memcpy(&req_align, req, 4);

    	addr.s_addr = req_align;
    	LOG(LOG_INFO, "Client has a requested IP: %s", inet_ntoa(addr));

        /* and the ip is in the lease range */
        if( ntohl(req_align) >= ntohl(server_config[ifid].start) &&
		    ntohl(req_align) <= ntohl(server_config[ifid].end) &&
            /* and the ip is NOT server itself */
            ntohl(req_align) != ntohl(server_config[ifid].server) &&
            /* and it is not reserved */
            !find_reserved_by_yiaddr(req_align, ifid) &&
		    /* and its not already taken/offered or its taken, but expired */
		    (!(lease = find_lease_by_yiaddr(req_align, ifid)) || lease_expired(lease)) &&
		    /* and nobody using this ip now */
            !check_ip(req_align, ifid, NULL)
		   )
        {
            packet.yiaddr = req_align;
        }
        else {
            /* Sorry, we can not assign your requested IP to you */
            packet.yiaddr = get_free_address(ifid);
	    }
	}
	else {
	    /* otherwise, find a free IP */
        packet.yiaddr = get_free_address(ifid);
	}

gotip:
	if(!packet.yiaddr) {
		LOG(LOG_WARNING, "no IP addresses to give -- OFFER abandoned");
		return -1;
	}

#ifdef RONSCODE
	{
		char *tmp=(char *)get_option(oldpacket, DHCP_HOST_NAME);
		char *tmp1=(char *)get_option(oldpacket, DHCP_CLIENT_ID);
		int len=0;
		int len1=0;
		if(tmp){
			len=*(tmp-1);
			strncpy(hostname,tmp,(len>MAXHOSTNAMELEN ? MAXHOSTNAMELEN : len));
		}else{
			/*Sometimes DHCP discovery packet may not have DHCP_HOST_NAME, but this
			 * will make udhcpd crash down since the NULL pointer actions.*/
			strcpy(hostname, "UNKNOWN");
		}
		if(tmp1){
			len1=*(tmp1-1);
			if(len1 > MAX_CLIENT_LEN)
			{
				len1 = MAX_CLIENT_LEN -1;
				printf("DHCP client ID is too long !\n");
			}
			memcpy(cid,tmp1,len1);
		}
	}
	if (!add_lease(packet.chaddr, packet.yiaddr, server_config[ifid].offer_time, ifid, hostname, cid)) {
		LOG(LOG_WARNING, "lease pool is full -- OFFER abandoned");
		return -1;
	}
#else
	if (!add_lease(packet.chaddr, packet.yiaddr, server_config[ifid].offer_time, ifid)) {
		LOG(LOG_WARNING, "lease pool is full -- OFFER abandoned");
		return -1;
	}
#endif

	if ((lease_time = get_option(oldpacket, DHCP_LEASE_TIME))) {
		memcpy(&lease_time_align, lease_time, 4);
		lease_time_align = ntohl(lease_time_align);
		if (lease_time_align > server_config[ifid].lease)
			lease_time_align = server_config[ifid].lease;
	}

	/* Make sure we aren't just using the lease time from the previous offer */
	if (lease_time_align < server_config[ifid].min_lease)
		lease_time_align = server_config[ifid].lease;
	/* ADDME: end of short circuit */
	add_simple_option(packet.options, DHCP_LEASE_TIME, htonl(lease_time_align));

	curr = server_config[ifid].options;
	while (curr) {
		if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
			add_option_string(packet.options, curr->data);
		curr = curr->next;
	}

#ifdef TR069
    if(vendor_spc)
	    add_vendor_options(&packet);
#endif

	add_bootp_options(&packet, ifid);

	addr.s_addr = packet.yiaddr;
	LOG(LOG_INFO, "sending OFFER of %s", inet_ntoa(addr));
	return send_packet(&packet, 0, ifid);
}


int sendNAK(struct dhcpMessage *oldpacket, int ifid)
{
	struct dhcpMessage packet;
#ifdef TR069
    unsigned char *vendor_spc=NULL,*req=NULL;
    u_int32_t req_align;
	vendor_spc = get_option(oldpacket, DHCP_VENDOR_SPEC);
	if((req = get_option(oldpacket, DHCP_REQUESTED_IP)))
		memcpy(&req_align, req, 4);
	if(vendor_spc && req)
	    delete_vendor_list(req_align);
#endif
	init_packet(&packet, oldpacket, DHCPNAK, ifid);

	DEBUG(LOG_INFO, "sending NAK");
	return send_packet(&packet, 1, ifid);
}

int sendACK(struct dhcpMessage *oldpacket, u_int32_t yiaddr, int ifid)
{
	struct dhcpMessage packet;
	struct option_set *curr;
	unsigned char *lease_time;
	u_int32_t lease_time_align = server_config[ifid].lease;
	struct in_addr addr;
#ifdef RONSCODE
	char hostname[MAXHOSTNAMELEN]="";
	char cid[MAX_CLIENT_LEN]="";

#endif
#ifdef TR069
    unsigned char *vendor_spc=NULL,*req=NULL;
    u_int32_t req_align;
	vendor_spc = get_option(oldpacket, DHCP_VENDOR_SPEC);
	if((req = get_option(oldpacket, DHCP_REQUESTED_IP)))
		memcpy(&req_align, req, 4);
	if(vendor_spc && req)
	    add_vendor_list(vendor_spc,req_align);
#endif
	init_packet(&packet, oldpacket, DHCPACK, ifid);
	packet.yiaddr = yiaddr;

	if ((lease_time = get_option(oldpacket, DHCP_LEASE_TIME))) {
		memcpy(&lease_time_align, lease_time, 4);
		lease_time_align = ntohl(lease_time_align);
		if (lease_time_align > server_config[ifid].lease)
			lease_time_align = server_config[ifid].lease;
		else if (lease_time_align < server_config[ifid].min_lease)
			lease_time_align = server_config[ifid].lease;
	}

	add_simple_option(packet.options, DHCP_LEASE_TIME, htonl(lease_time_align));

	curr = server_config[ifid].options;
	while (curr) {
		if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
			add_option_string(packet.options, curr->data);
		curr = curr->next;
	}

#ifdef TR069
    if(vendor_spc){
	    add_vendor_options(&packet);
	}
#endif

	add_bootp_options(&packet, ifid);

	addr.s_addr = packet.yiaddr;
	LOG(LOG_INFO, "sending ACK to %s", inet_ntoa(addr));

	if (send_packet(&packet, 0, ifid) < 0)
		return -1;
#ifdef RONSCODE
	{
		char *tmp=(char *)get_option(oldpacket,DHCP_HOST_NAME);
		char *tmp1=(char *)get_option(oldpacket,DHCP_CLIENT_ID);
		int len=0;
		int len1=0;
		if(tmp){
			len=*(tmp-1);
			strncpy(hostname,tmp,len);
		}else{
			/*Sometimes DHCP discovery packet may not have DHCP_HOST_NAME, but this
 			* will make udhcpd crash down since the NULL pointer actions.*/
			strcpy(hostname, "UNKNOWN");
		}

		if(tmp1){
			len1=*(tmp1-1);
			if(len1 > MAX_CLIENT_LEN)
			{
				len1 = MAX_CLIENT_LEN - 1;
				printf("DHCP client ID is too long !\n");
			}
			memcpy(cid,tmp1,len1);
		}
	}

	add_lease(packet.chaddr, packet.yiaddr, lease_time_align, ifid, hostname,cid);
#else
	add_lease(packet.chaddr, packet.yiaddr, lease_time_align, ifid);
#endif
{
    struct in_addr addr;

    addr.s_addr = packet.yiaddr;
    NETGEAR_SYSLOG("07 00[DHCP IP: (%s)] to MAC address %02X:%02X:%02X:%02X:%02X:%02X",
        inet_ntoa(addr), packet.chaddr[0], packet.chaddr[1], packet.chaddr[2], packet.chaddr[3], packet.chaddr[4], packet.chaddr[5]);
}
	return 0;
}


int send_inform(struct dhcpMessage *oldpacket, int ifid)
{
	struct dhcpMessage packet;
	struct option_set *curr;

	init_packet(&packet, oldpacket, DHCPACK, ifid);

	curr = server_config[ifid].options;
	while (curr) {
		if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
			add_option_string(packet.options, curr->data);
		curr = curr->next;
	}

	add_bootp_options(&packet, ifid);

	return send_packet(&packet, 0, ifid);
}




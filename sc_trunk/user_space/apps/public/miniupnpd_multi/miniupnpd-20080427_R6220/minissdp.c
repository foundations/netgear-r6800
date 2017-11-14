/* $Id: minissdp.c,v 1.12 2008/02/21 10:20:24 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include "config.h"
#include "upnpdescstrings.h"
#include "miniupnpdpath.h"
#include "upnphttp.h"
#include "upnpglobalvars.h"
#include "minissdp.h"
#include "upnpgetnvram.h"
/* SSDP ip/port */
#define SSDP_PORT (1900)
#define SSDP_MCAST_ADDR ("239.255.255.250")

static struct s_device known_devices[] = {
    {"upnp:rootdevice", uuidvalue},
    {"urn:schemas-upnp-org:device:InternetGatewayDevice:1", uuidvalue},
    {"urn:schemas-upnp-org:device:WANDevice:1", uuidvalue1},
    {"urn:schemas-upnp-org:device:WANConnectionDevice:1", uuidvalue2},
#ifdef ENABLE_LANDEVICE
    {"urn:schemas-upnp-org:device:LANDevice:1", uuidvalue3},
#endif
    {NULL, NULL}
};

static struct s_service known_services[] = {
    {"urn:schemas-upnp-org:service:Layer3Forwarding:1", 1},          /* InternetGatewayDevice */
    {"urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1", 2},  /* WANDevice */
    {"urn:schemas-upnp-org:service:WANIPConnection:1", 3},           /* WANConnectionDevice */
    {"urn:schemas-upnp-org:service:WANPPPConnection:1", 3},          /* WANConnectionDevice */
    {"urn:schemas-upnp-org:service:WANEthernetLinkConfig:1", 3},     /* WANConnectionDevice */
#ifdef ENABLE_LANDEVICE
    {"urn:schemas-upnp-org:service:LANHostConfigManagement:1", 4},     /* LANDevice */
#endif
    {NULL, 0}
};

    static int
AddMulticastMembership(int s, in_addr_t ifaddr/*const char * ifaddr*/)
{
    struct ip_mreq imr;	/* Ip multicast membership */

    /* setting up imr structure */
    imr.imr_multiaddr.s_addr = inet_addr(SSDP_MCAST_ADDR);
    /*imr.imr_interface.s_addr = htonl(INADDR_ANY);*/
    imr.imr_interface.s_addr = ifaddr;	/*inet_addr(ifaddr);*/

    if (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)&imr, sizeof(struct ip_mreq)) < 0)
    {
        SYSLOG(LOG_ERR, "setsockopt(udp, IP_ADD_MEMBERSHIP): %m");
        return -1;
    }

    return 0;
}

    int
OpenAndConfSSDPReceiveSocket()
    /*OpenAndConfSSDPReceiveSocket(int n_lan_addr,
      struct lan_addr_s * lan_addr)*/
{
    int s;
    int i;
    struct sockaddr_in sockname;
    int onOff = 1;

    if( (s = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        SYSLOG(LOG_ERR, "socket(udp): %m");
        return -1;
    }

    memset(&sockname, 0, sizeof(struct sockaddr_in));
    sockname.sin_family = AF_INET;
    sockname.sin_port = htons(SSDP_PORT);
    /* NOTE : it seems it doesnt work when binding on the specific address */
    /*sockname.sin_addr.s_addr = inet_addr(UPNP_MCAST_ADDR);*/
    sockname.sin_addr.s_addr = htonl(INADDR_ANY);
    /*sockname.sin_addr.s_addr = inet_addr(ifaddr);*/

    //Sercomm
    if( setsockopt( s, SOL_SOCKET, SO_REUSEADDR,
                ( char * )&onOff, sizeof( onOff ) ) != 0 ) {

        close(s);
        return -1;
    }
    if(bind(s, (struct sockaddr *)&sockname, sizeof(struct sockaddr_in)) < 0)
    {
        SYSLOG(LOG_ERR, "bind(udp): %m");
        close(s);
        return -1;
    }

    i = n_lan_addr;
    while(i>0)
    {
        i--;
        if(AddMulticastMembership(s, lan_addr[i].addr.s_addr) < 0)
        {
            SYSLOG(LOG_WARNING,
                    "Failed to add multicast membership for address %s",
                    lan_addr[i].str );
        }
    }

    return s;
}
/* open the UDP socket used to send SSDP notifications to
 * the multicast group reserved for them */
    static int
OpenAndConfSSDPNotifySocket(in_addr_t addr)
{
    int s;
    unsigned char loopchar = 0;
    int bcast = 1;
    struct in_addr mc_if;
    struct sockaddr_in sockname;

    if( (s = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        SYSLOG(LOG_ERR, "socket(udp_notify): %m");
        return -1;
    }

    mc_if.s_addr = addr;	/*inet_addr(addr);*/

    if(setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopchar, sizeof(loopchar)) < 0)
    {
        SYSLOG(LOG_ERR, "setsockopt(udp_notify, IP_MULTICAST_LOOP): %m");
        close(s);
        return -1;
    }

    if(setsockopt(s, IPPROTO_IP, IP_MULTICAST_IF, (char *)&mc_if, sizeof(mc_if)) < 0)
    {
        SYSLOG(LOG_ERR, "setsockopt(udp_notify, IP_MULTICAST_IF): %m");
        close(s);
        return -1;
    }

    if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &bcast, sizeof(bcast)) < 0)
    {
        SYSLOG(LOG_ERR, "setsockopt(udp_notify, SO_BROADCAST): %m");
        close(s);
        return -1;
    }
    //Sercomm
    if(setsockopt( s, IPPROTO_IP, IP_MULTICAST_TTL,( char * )&gTTL, sizeof( int ))!=0){
        close(s);
        return -1;
    }

    memset(&sockname, 0, sizeof(struct sockaddr_in));
    sockname.sin_family = AF_INET;
    sockname.sin_addr.s_addr = addr;	/*inet_addr(addr);*/

    if (bind(s, (struct sockaddr *)&sockname, sizeof(struct sockaddr_in)) < 0)
    {
        SYSLOG(LOG_ERR, "bind(udp_notify): %m");
        close(s);
        return -1;
    }

    return s;
}

    int
OpenAndConfSSDPNotifySockets(int * sockets)
    /*OpenAndConfSSDPNotifySockets(int * sockets,
      struct lan_addr_s * lan_addr, int n_lan_addr)*/
{
    int i, j;
    for(i=0; i<n_lan_addr; i++)
    {
        sockets[i] = OpenAndConfSSDPNotifySocket(lan_addr[i].addr.s_addr);
        if(sockets[i] < 0)
        {
            for(j=0; j<i; j++)
            {
                close(sockets[j]);
                sockets[j] = -1;
            }
            return -1;
        }
    }
    return 0;
}

/*
 * response from a LiveBox (Wanadoo)
 HTTP/1.1 200 OK
 CACHE-CONTROL: max-age=1800
DATE: Thu, 01 Jan 1970 04:03:23 GMT
EXT:
LOCATION: http://192.168.0.1:49152/gatedesc.xml
SERVER: Linux/2.4.17, UPnP/1.0, Intel SDK for UPnP devices /1.2
ST: upnp:rootdevice
USN: uuid:75802409-bccb-40e7-8e6c-fa095ecce13e::upnp:rootdevice

 * response from a Linksys 802.11b :
 HTTP/1.1 200 OK
 Cache-Control:max-age=120
Location:http://192.168.5.1:5678/rootDesc.xml
Server:NT/5.0 UPnP/1.0
ST:upnp:rootdevice
USN:uuid:upnp-InternetGatewayDevice-1_0-0090a2777777::upnp:rootdevice
EXT:
 */

/* not really an SSDP "announce" as it is the response
 * to a SSDP "M-SEARCH"
 *
 */
    static void
SendSSDPAnnounce2(int s, struct sockaddr_in sockname,
        const char * st, int st_len, const char *uuid,
        const char * host, unsigned short port)
{
    int l, n;
    char buf[512];
    /* TODO :
     * follow guideline from document "UPnP Device Architecture 1.0"
     * put in uppercase.
     * DATE: is recommended
     * SERVER: OS/ver UPnP/1.0 miniupnpd/1.0
     * - check what to put in the 'Cache-Control' header
     * */
    l = snprintf(buf, sizeof(buf), "HTTP/1.1 200 OK\r\n"
            "Cache-Control: max-age=%d\r\n"
            "ST: %.*s\r\n"
            "USN: %s%s%.*s\r\n"
            "EXT:\r\n"
            "Server: " MINIUPNPD_SERVER_STRING "\r\n"
            "Location: http://%s:%u" ROOTDESC_PATH "\r\n"
            "\r\n",
            gNotifyInterval,
            st_len, st,
            uuid?uuid:"", uuid?"::":"", st_len, st, /* uuid == NULL means response of ST type = uuid: */
            host, (unsigned int)port);
    n = sendto(s, buf, l, 0,
            (struct sockaddr *)&sockname, sizeof(struct sockaddr_in) );
    if(n < 0)
    {
        SYSLOG(LOG_ERR, "sendto(udp): %m");
    }
}
/*
 * In fact, only two types notify packet.
 * TYPE 1: NT is same USN (NT: uuid:device-UUID/USN: uuid:device-UUID)
 * TYPE 2: USN is uuid:device-UUID:: plus value of NT (NT: urn:schemas-upnp-org:device:deviceType:ver/USN: uuid:device-UUID::urn:schemas-upnp-org:device:deviceType:ver)
 *
 */
/*
 * send_notify() -- Generate notify string and send it to s. If @nt is NULL, it means it is TYPE 1 otherwise TYPE 2.
 * RETURN VALUE: void
 */
static void send_notify(char *uuid, char *nt,
        int s, const char *host, unsigned short port, unsigned int lifetime, int alive) {


    struct sockaddr_in sockname;
    int l=0, n;
    char buf[512];

    memset(&sockname, 0, sizeof(struct sockaddr_in));
    sockname.sin_family = AF_INET;
    sockname.sin_port = htons(SSDP_PORT);
    sockname.sin_addr.s_addr = inet_addr(SSDP_MCAST_ADDR);

    /* Generate notify string */
    l += sprintf(buf+l,
            "NOTIFY * HTTP/1.1\r\n"
            "HOST:%s:%d\r\n",
            SSDP_MCAST_ADDR, SSDP_PORT);

    /* byebye message do not need this */
    if(alive) {
        l += sprintf(buf+l,
                "Cache-Control:max-age=%u\r\n"
                "Location:http://%s:%d" ROOTDESC_PATH"\r\n"
                "Server: " MINIUPNPD_SERVER_STRING "\r\n",
                lifetime,
                host, port);
    }

    if(nt) {
        l += sprintf(buf+l, "NT:%s\r\nUSN:%s::%s\r\n", nt, uuid, nt);
    }
    else {
        l += sprintf(buf+l, "NT:%s\r\nUSN:%s\r\n", uuid, uuid);
    }

    l += sprintf(buf+l, "NTS:ssdp:%s\r\n\r\n", alive?"alive":"byebye");

    if(l>=sizeof(buf))
    {
        SYSLOG(LOG_WARNING, "send_notify(): truncated output");
        l = sizeof(buf);
    }
    n = sendto(s, buf, l, 0,
            (struct sockaddr *)&sockname, sizeof(struct sockaddr_in) );
    if(n < 0)
    {
        SYSLOG(LOG_ERR, "sendto(udp_notify=%d, %s): %m", s, host);
    }
    else {
        SYSLOG(LOG_DEBUG, "Send notify success!");
    }
}

void modify_ssdp_service(char *wan_mode) {
    /* XP ICON need to show WANIPConnection and WANPPPConnection at the same time. So discard this solution */
#if 0
    /* DHCP mode only need WANIPConnection, PPTP and PPPoE only need WANPPPConnection */
    if (*wan_mode == 'p') {
        known_services[2].service = "urn:schemas-upnp-org:service:WANPPPConnection:1";
    }
    else {
        known_services[2].service = "urn:schemas-upnp-org:service:WANIPConnection:1";
    }
#endif
    return;
}

    static void
SendSSDPNotifies(int s, const char * host, unsigned short port,
        unsigned int lifetime)
{
    int i;
    /*
     * The total number of packets expected are = 3+2d+k,
     * where 3 is for the root device,
     * d is the number of embedded devices,
     * and k is the number of services.
     * Refer to the discovery section of the UDA for more details.
     */

    /*
     * In fact, only two types notify packet.
     * TYPE 1: NT is same USN (NT: uuid:device-UUID/USN: uuid:device-UUID)
     * TYPE 2: USN is uuid:device-UUID:: plus value of NT (NT: urn:schemas-upnp-org:device:deviceType:ver/USN: uuid:device-UUID::urn:schemas-upnp-org:device:deviceType:ver)
     */

    /* Devices */
    for(i=0; known_devices[i].device; i++) {
        if(i) {
            send_notify(known_devices[i].uuid, NULL, s, host, port, lifetime, 1);
        }
        send_notify(known_devices[i].uuid, known_devices[i].device, s, host, port, lifetime, 1);
    }

    /* Services */
    for(i=0; known_services[i].service; i++) {
        send_notify(known_devices[known_services[i].device_idx].uuid, known_services[i].service, s, host, port, lifetime, 1);
    }
}

    void
SendSSDPNotifies2(int * sockets,
        unsigned short port,
        unsigned int lifetime)
/*SendSSDPNotifies2(int * sockets, struct lan_addr_s * lan_addr, int n_lan_addr,
  unsigned short port,
  unsigned int lifetime)*/
{
    int i;
    for(i=0; i<n_lan_addr; i++)
    {
        SYSLOG(LOG_DEBUG, "Calling SendSSDPNotifies");
        SendSSDPNotifies(sockets[i], lan_addr[i].str, port, lifetime);
    }
}

/*
 * valid_msearch() -- Check if msearch packet is valid or not.
 * RETURN VALUE: 1 -- valid, 0 -- invalid
 */
#define MSEARCH_HTTP_HDR    "M-SEARCH * HTTP/1.1\r\n"
static int valid_msearch(char *buf, int len) {
    int i;
    char line[128];
    char *p;
    int has_man=0, has_mx=0, has_st=0;
    int mx;

    if(memcmp(buf, MSEARCH_HTTP_HDR, sizeof(MSEARCH_HTTP_HDR)-1)) {
        return 0;
    }
    memset(line, 0, sizeof(line));
    p = line;
    for(i=0; i<len; i++) {
        *p++ = buf[i];
        if(buf[i-1] == '\r' && buf[i] == '\n') { /* end of line, start to check */
            *(p-2) = '\0';  /* Strip the \r\n */
            if(strncasecmp(line, "Man:", 4) == 0) {
                has_man = 1;
                /* no more check form man line */
            }
            if(strncasecmp(line, "MX:", 3) == 0) {
                has_mx = 1;
                /* Check MX value */
                for(p=line+3;*p==' ';p++);
                mx = atoi(p);
                if(mx <= 0) {
                    /* mx value must >=0 */
                    return 0;
                }

            }
            if(strncasecmp(line, "st:", 3) == 0) {
                has_st = 1;
                /* no more check form st line */
            }
            memset(line, 0, sizeof(line));
            p = line;
        }
    }
    if(has_man + has_mx + has_st != 3) {
        /* must short of man or mx or st */
        return 0;
    }
    return 1;
}

/* ProcessSSDPRequest()
 * process SSDP M-SEARCH requests and responds to them */
    void
ProcessSSDPRequest(int s, unsigned short port)
    /*ProcessSSDPRequest(int s, struct lan_addr_s * lan_addr, int n_lan_addr,
      unsigned short port)*/
{
    int n;
    char bufr[1500];
    socklen_t len_r;
    struct sockaddr_in sendername;
    int i, l;
    int lan_addr_index = 0;
    char * st = 0;
    int st_len = 0;
    len_r = sizeof(struct sockaddr_in);

    n = recvfrom(s, bufr, sizeof(bufr)-1, 0,
            (struct sockaddr *)&sendername, &len_r);
    if(n < 0)
    {
        SYSLOG(LOG_ERR, "recvfrom(udp): %m");
        return;
    }
    bufr[n] = '\0';
    if(memcmp(bufr, "NOTIFY", 6) == 0)
    {
        /* ignore NOTIFY packets. We could log the sender and device type */
        return;
    }
    else if(valid_msearch(bufr, n))
    {
        i = 0;
        while(i < n)
        {
            while((i < n - 1) && (bufr[i] != '\r' || bufr[i+1] != '\n'))
                i++;
            i += 2;
            if((i < n - 3) && (strncasecmp(bufr+i, "st:", 3) == 0))
            {
                st = bufr+i+3;
                st_len = 0;
                while((*st == ' ' || *st == '\t') && (st < bufr + n)) st++;
                while(st[st_len]!='\r' && st[st_len]!='\n' && (st + st_len < bufr + n)) st_len++;
                /*SYSLOG(LOG_INFO, "ST: %.*s", st_len, st);*/
                /*j = 0;*/
                /*while(bufr[i+j]!='\r') j++;*/
                /*SYSLOG(LOG_INFO, "%.*s", j, bufr+i);*/
            }
        }
        /*SYSLOG(LOG_INFO, "SSDP M-SEARCH packet received from %s:%d",
          inet_ntoa(sendername.sin_addr),
          ntohs(sendername.sin_port) );*/
        if(st && (st_len > 0))
        {
            /* TODO : doesnt answer at once but wait for a random time */
            SYSLOG(LOG_INFO, "SSDP M-SEARCH from %s:%d ST: %.*s",
                    inet_ntoa(sendername.sin_addr),
                    ntohs(sendername.sin_port),
                    st_len, st);
            /* find in which sub network the client is */
            for(i = 0; i<n_lan_addr; i++)
            {
                if( (sendername.sin_addr.s_addr & lan_addr[i].mask.s_addr)
                        == (lan_addr[i].addr.s_addr & lan_addr[i].mask.s_addr))
                {
                    lan_addr_index = i;
                    break;
                }
            }
            /*
             * Responds to request with ST: ssdp:all
             * MSearch Response for Search type of ssdp:all.
             * Sends out a MSearch request with ST = ssdp:all.
             * Expects 3+2d+k MSearch responses before the time defined in MX header.
             */
            /* strlen("ssdp:all") == 8 */
            if(st_len==8 && (0 == memcmp(st, "ssdp:all", 8)))
            {
                /* Send Response for ALL Devices */
                for(i=0; known_devices[i].device; i++)
                {
                    l = (int)strlen(known_devices[i].device);
                    SendSSDPAnnounce2(s, sendername,
                            known_devices[i].device, l, known_devices[i].uuid,
                            lan_addr[lan_addr_index].str, port);
                }

                /* Send Response for ALL uuid: */
                for(i=1; known_devices[i].device; i++)
                {
                    l = (int)strlen(known_devices[i].uuid);
                    SendSSDPAnnounce2(s, sendername,
                            known_devices[i].uuid, l, NULL,
                            lan_addr[lan_addr_index].str, port);
                }

                /* Send Response for ALL Services */
                for(i=0; known_services[i].service; i++)
                {
                    l = (int)strlen(known_services[i].service);
                    SendSSDPAnnounce2(s, sendername,
                            known_services[i].service, l, known_devices[known_services[i].device_idx].uuid,
                            lan_addr[lan_addr_index].str, port);
                }
            }

            /* Responds to request with a device as ST header */
            for(i = 0; known_devices[i].device; i++)
            {
                l = (int)strlen(known_devices[i].device);
                if(l<=st_len && (0 == memcmp(st, known_devices[i].device, l)))
                {
                    SendSSDPAnnounce2(s, sendername,
                            st, st_len, known_devices[i].uuid,
                            lan_addr[lan_addr_index].str, port);
                    break;
                }
            }

            /* Responds to request with a service as ST header */
            for(i = 0; known_services[i].service; i++)
            {
                l = (int)strlen(known_services[i].service);
                if(l<=st_len && (0 == memcmp(st, known_services[i].service, l)))
                {
                    SendSSDPAnnounce2(s, sendername,
                            st, st_len, known_devices[known_services[i].device_idx].uuid,
                            lan_addr[lan_addr_index].str, port);
                    break;
                }
            }
            /* responds to request by UUID value */
            /*
             * MSearch Response for Search of Devices By Uuid.
             * Sends out MSearch request of type ST = uuid:device-UUID for the
             * root device and each of its contained standard embedded devices.
             * Expects one MSearch response of type ST=uuid:device-UUID from the
             * device whose device-UUID matches before the time defined in MX header.
             */
            for(i = 1; known_devices[i].device; i++)
            {
                l = (int)strlen(known_devices[i].uuid);
                if(l<=st_len && (0 == memcmp(st, known_devices[i].uuid, l)))
                {
                    SendSSDPAnnounce2(s, sendername,
                            st, st_len, NULL,
                            lan_addr[lan_addr_index].str, port);
                    break;
                }
            }
        }
        else
        {
            SYSLOG(LOG_INFO, "Invalid SSDP M-SEARCH from %s:%d",
                    inet_ntoa(sendername.sin_addr), ntohs(sendername.sin_port));
        }
    }
    else
    {
        SYSLOG(LOG_NOTICE, "Unknown udp packet received from %s:%d",
                inet_ntoa(sendername.sin_addr), ntohs(sendername.sin_port));
    }
}

/* This will broadcast ssdp:byebye notifications to inform
 * the network that UPnP is going down. */
    int
SendSSDPGoodbye(int * sockets, int n_sockets)
{
    int i, j;

    for(j=0; j<n_sockets; j++)
    {

        /* Devices */
        for(i=0; known_devices[i].device; i++) {
            if(i) {
                send_notify(known_devices[i].uuid, NULL, sockets[j], NULL, 0, 0, 0);
            }
            send_notify(known_devices[i].uuid, known_devices[i].device, sockets[j], NULL, 0, 0, 0);
        }

        /* Services */
        for(i=0; known_services[i].service; i++) {
            send_notify(known_devices[known_services[i].device_idx].uuid, known_services[i].service, sockets[j], NULL, 0, 0, 0);
        }
    }
    return 0;
}


/* $Id: upnpsoap.c,v 1.62 2008/04/25 16:24:12 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2008 Thomas Bernard
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include "commonrdr.h"
#include "upnpgetnvram.h"
#include "upnpaction.h"
#include "config.h"
#include "upnpglobalvars.h"
#include "upnphttp.h"
#include "upnpsoap.h"
#include "upnpreplyparse.h"
#include "upnpredirect.h"
#include "getifaddr.h"
#include "getifstats.h"
#include <syslog.h>
#include "link.h"
#include "misc.h"
#if defined(USE_NETFILTER)
#include "netfilter/iptcrdr.h"
#endif

#ifdef CONENAT
#include "libcnapt.h"
#endif
static void UPnPSyslog(const char *EventName, const char *SourceIP)
{
    closelog();
    NETGEAR_SYSLOG("08 00[UPnP set event:%s] from source %s", EventName, SourceIP);
}

    static void
BuildSendAndCloseSoapResp(struct upnphttp * h,
        const char * body, int bodylen)
{
    static const char beforebody[] =
        "<?xml version=\"1.0\"?>\r\n"
        "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
        "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n"
        "<s:Body>\n";

    static const char afterbody[] =
        "</s:Body>\n"
        "</s:Envelope>\r\n";

    BuildHeader_upnphttp(h, 200, "OK",  sizeof(beforebody) - 1
            + sizeof(afterbody) - 1 + bodylen );

    memcpy(h->res_buf + h->res_buflen, beforebody, sizeof(beforebody) - 1);
    h->res_buflen += sizeof(beforebody) - 1;

    memcpy(h->res_buf + h->res_buflen, body, bodylen);
    h->res_buflen += bodylen;

    memcpy(h->res_buf + h->res_buflen, afterbody, sizeof(afterbody) - 1);
    h->res_buflen += sizeof(afterbody) - 1;

    SendResp_upnphttp(h);
    CloseSocket_upnphttp(h);
}

    static void
SetConnectionType(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:SetConnectionTypeResponse "
        "xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\">\n"
        "</u:SetConnectionTypeResponse>\n";
    struct NameValueParserData data;
    char *p;
    ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
    p = GetValueFromNameValueList(&data, "NewConnectionType");
    UPnPSyslog("SetConnectionType", inet_ntoa(h->clientaddr));
    if(p)
    {
        syslog(LOG_INFO, "%s(%s) : Ignored", action, p);
    }
    ClearNameValueList(&data);
    BuildSendAndCloseSoapResp(h, resp, sizeof(resp)-1);
}

    static void
GetConnectionTypeInfo(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:GetConnectionTypeInfoResponse "
        "xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\">\n"
        "<NewConnectionType>IP_Routed</NewConnectionType>\n"
        "<NewPossibleConnectionTypes>IP_Routed</NewPossibleConnectionTypes>\n"
        "</u:GetConnectionTypeInfoResponse>";
    BuildSendAndCloseSoapResp(h, resp, sizeof(resp)-1);
}

static void RequestConnection(struct upnphttp *h, const char *action)
{
    static const char resp[] =
        "<u:RequestConnectionResponse "
        "xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\">\n"
        "</u:RequestConnectionResponse>\n";
    DoWanConnection();
    UPnPSyslog("RequestConnection", inet_ntoa(h->clientaddr));
    BuildSendAndCloseSoapResp(h, resp, sizeof(resp)-1);
}

static void ForceTermination(struct upnphttp *h, const char *action)
{
    static const char resp[] =
        "<u:ForceTerminationResponse "
        "xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\">\n"
        "</u:ForceTerminationResponse>\n";
    DoWanDisconnection();
    UPnPSyslog("ForceTermination", inet_ntoa(h->clientaddr));
    BuildSendAndCloseSoapResp(h, resp, sizeof(resp)-1);
}

    static void
GetEthernetLinkStatus(struct upnphttp *h, const char *action)
{
    static const char resp[] =
        "<u:GetEthernetLinkStatusResponse "
        "xmlns:u=\"urn:schemas-upnp-org:service:WANEthernetLinkConfig:1\">\n"
        /* FIXME: Does EthernetLinkStatus means interface up/down ? */
        "<NewEthernetLinkStatus>%s</NewEthernetLinkStatus>\n"
        "</u:GetEthernetLinkStatusResponse>\n";
    char body[1024];
    int bodylen;
    char *value;

    /* value - Up:Down */
    value = "Up";
    bodylen = snprintf(body, sizeof(body), resp, value);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}
    static void
GetTotalBytesSent(struct upnphttp * h, const char * action)
{
    int r;

    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewTotalBytesSent>%lu</NewTotalBytesSent>\n"
        "</u:%sResponse>\n";

    char body[512];
    int bodylen;
    struct ifdata data;

    r = getifstats(ext_if_name, &data);
    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1",
            r<0?0:data.obytes, action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}

    static void
GetTotalBytesReceived(struct upnphttp * h, const char * action)
{
    int r;

    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewTotalBytesReceived>%lu</NewTotalBytesReceived>\n"
        "</u:%sResponse>\n";

    char body[512];
    int bodylen;
    struct ifdata data;

    r = getifstats(ext_if_name, &data);
    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1",
            r<0?0:data.ibytes, action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}

    static void
GetTotalPacketsSent(struct upnphttp * h, const char * action)
{
    int r;

    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewTotalPacketsSent>%lu</NewTotalPacketsSent>\n"
        "</u:%sResponse>\n";

    char body[512];
    int bodylen;
    struct ifdata data;

    r = getifstats(ext_if_name, &data);
    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1",
            r<0?0:data.opackets, action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}

    static void
GetTotalPacketsReceived(struct upnphttp * h, const char * action)
{
    int r;

    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">"
        "<NewTotalPacketsReceived>%lu</NewTotalPacketsReceived>"
        "</u:%sResponse>";

    char body[512];
    int bodylen;
    struct ifdata data;

    r = getifstats(ext_if_name, &data);
    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1",
            r<0?0:data.ipackets, action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}

    static void
GetCommonLinkProperties(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        /*"<NewWANAccessType>DSL</NewWANAccessType>"*/
        "<NewWANAccessType>Ethernet</NewWANAccessType>\n"
        "<NewLayer1UpstreamMaxBitRate>%lu</NewLayer1UpstreamMaxBitRate>\n"
        "<NewLayer1DownstreamMaxBitRate>%lu</NewLayer1DownstreamMaxBitRate>\n"
        "<NewPhysicalLinkStatus>%s</NewPhysicalLinkStatus>\n"
        "</u:%sResponse>\n";

    char body[2048];
    int bodylen;
    char link_status[4];

    struct port_link_status_s *p = get_wan_port_link_status();
    downstream_bitrate = p->down_rate;
    upstream_bitrate = p->up_rate;
    (p->link == LINK_DOWN) ? (strcpy(link_status, "Down")) : (strcpy(link_status, "Up"));

    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1",
            upstream_bitrate, downstream_bitrate, link_status,
            action);
    debug_msg("%s:%d %s\n", __func__, __LINE__, body);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}

    static void
GetLinkLayerMaxBitRates(struct upnphttp *h, const char *action)
{
    static const char resp[] =
        "<u:GetLinkLayerMaxBitRatesResponse "
        "xmlns:u=\"urn:schemas-upnp-org:service:WANPPPConnection:1\">\n"
        "<NewUpstreamMaxBitRate>%lu</NewUpstreamMaxBitRate>\n"
        "<NewDownstreamMaxBitRate>%lu</NewDownstreamMaxBitRate>\n"
        "</u:GetLinkLayerMaxBitRatesResponse>\n";

    char body[2048];
    int bodylen;

    struct port_link_status_s *p = get_wan_port_link_status();
    downstream_bitrate = p->down_rate;
    upstream_bitrate = p->up_rate;

    bodylen = snprintf(body, sizeof(body), resp,
            upstream_bitrate, downstream_bitrate);
    debug_msg("%s:%d %s\n", __func__, __LINE__, body);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}
extern long get_uptime(void);
    static void
GetStatusInfo(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewConnectionStatus>%s</NewConnectionStatus>\n"
        "<NewLastConnectionError>ERROR_NONE</NewLastConnectionError>\n"
        "<NewUptime>%ld</NewUptime>\n"
        "</u:%sResponse>\n";

    char body[512];
    char conn_status[16];
    int bodylen;
    time_t uptime;

    if(IsWanUp())
        strcpy(conn_status, "Connected");
    else
        strcpy(conn_status, "Disconnected");
    //uptime = (time(NULL) - startup_time);
    uptime = (get_uptime() - my_startup_time);
    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:WANIPConnection:1",
            conn_status, (long)uptime, action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}

    static void
GetNATRSIPStatus(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:GetNATRSIPStatusResponse "
        "xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\">\n"
        "<NewRSIPAvailable>%s</NewRSIPAvailable>\n"
        "<NewNATEnabled>%s</NewNATEnabled>\n"
        "</u:GetNATRSIPStatusResponse>\n";

    char body[1024];
    int bodylen;

    debug_msg("%s %s\n", GetRSIPAvailabled(), GetNATEnabled());
    bodylen = snprintf(body, sizeof(body), resp, GetRSIPAvailabled(), GetNATEnabled());
    BuildSendAndCloseSoapResp(h, body, bodylen);
}

    static void
GetExternalIPAddress(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewExternalIPAddress>%s</NewExternalIPAddress>\n"
        "</u:%sResponse>\n";

    char body[512];
    int bodylen;
    char ext_ip_addr[INET_ADDRSTRLEN];

#ifndef MULTIPLE_EXTERNAL_IP
    if(use_ext_ip_addr)
    {
        strncpy(ext_ip_addr, use_ext_ip_addr, INET_ADDRSTRLEN);
    }
    else if(getifaddr(ext_if_name, ext_ip_addr, INET_ADDRSTRLEN) < 0)
    {
        syslog(LOG_ERR, "Failed to get ip address for interface %s",
                ext_if_name);
        strncpy(ext_ip_addr, "0.0.0.0", INET_ADDRSTRLEN);
    }
#else
    int i;
    strncpy(ext_ip_addr, "0.0.0.0", INET_ADDRSTRLEN);
    for(i = 0; i<n_lan_addr; i++)
    {
        if( (h->clientaddr.s_addr & lan_addr[i].mask.s_addr)
                == (lan_addr[i].addr.s_addr & lan_addr[i].mask.s_addr))
        {
            strncpy(ext_ip_addr, lan_addr[i].ext_ip_str, INET_ADDRSTRLEN);
            break;
        }
    }
#endif
    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:WANIPConnection:1",
            ext_ip_addr, action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}

#if 0
static int
proto_atoi(const char * protocol)
{
    int proto = IPPROTO_TCP;
    if(strcmp(protocol, "UDP") == 0)
        proto = IPPROTO_UDP;
    return proto;
}
#endif

    static void
AddPortMapping(struct upnphttp * h, const char * action)
{
    int r=0;

    static const char resp[] =
        "<u:AddPortMappingResponse "
        "xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\"/>";
#ifdef CONENAT
    char rule[256];
#endif
    struct NameValueParserData data;
    char * int_ip, * int_port, * ext_port, * protocol, * desc,* r_host;
    char * leaseduration;
    unsigned short iport, eport;
    char *str_enabled;
    int enabled;

    struct hostent *hp; /* getbyhostname() */
    char ** ptr; /* getbyhostname() */
    struct in_addr result_ip;/*unsigned char result_ip[16];*/ /* inet_pton() */
#ifdef CONENAT  
	char iaddr_old[32];
	unsigned short iport_old;
	int proto;
#endif    
	
    ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
    int_ip = GetValueFromNameValueList(&data, "NewInternalClient");
    if (!int_ip)
    {
        ClearNameValueList(&data);
        SoapError(h, 402, "Invalid Args");
		return;
    }

    /* if ip not valid assume hostname and convert */
    if (inet_pton(AF_INET, int_ip, &result_ip) <= 0)
    {
        hp = gethostbyname(int_ip);
        if(hp && hp->h_addrtype == AF_INET)
        {
            for(ptr = hp->h_addr_list; ptr && *ptr; ptr++)
            {
                int_ip = inet_ntoa(*((struct in_addr *) *ptr));
                result_ip = *((struct in_addr *) *ptr);
                /* TODO : deal with more than one ip per hostname */
                break;
            }
        }
        else
        {
            syslog(LOG_ERR, "Failed to convert hostname '%s' to ip address", int_ip);
            ClearNameValueList(&data);
            SoapError(h, 402, "Invalid Args");
            return;
        }
    }

    /* check if NewInternalAddress is the client address */
    if(GETFLAG(SECUREMODEMASK))
    {
        if(h->clientaddr.s_addr != result_ip.s_addr)
        {
            syslog(LOG_INFO, "Client %s tried to redirect port to %s",
                    inet_ntoa(h->clientaddr), int_ip);
            ClearNameValueList(&data);
            SoapError(h, 718, "ConflictInMappingEntry");
            return;
        }
    }

    int_port = GetValueFromNameValueList(&data, "NewInternalPort");
    ext_port = GetValueFromNameValueList(&data, "NewExternalPort");
    protocol = GetValueFromNameValueList(&data, "NewProtocol");
    desc = GetValueFromNameValueList(&data, "NewPortMappingDescription");
    leaseduration = GetValueFromNameValueList(&data, "NewLeaseDuration");
    r_host = GetValueFromNameValueList(&data, "NewRemoteHost");
    str_enabled = GetValueFromNameValueList(&data, "NewEnabled");

    if (!int_port || !ext_port || !protocol)
    {
        ClearNameValueList(&data);
        SoapError(h, 402, "Invalid Args");
        return;
    }
	eport = (unsigned short)atoi(ext_port);
    iport = (unsigned short)atoi(int_port);
    enabled = atoi(str_enabled);

    if(leaseduration && atoi(leaseduration)) {
        syslog(LOG_WARNING, "NewLeaseDuration=%s", leaseduration);
    }

#ifdef  CONENAT
	/*Since 2 upnp rules with same eport and protocol will avoid by windows,so,if there is already
	    a same eport rule exist,it must be a 'edit' action on windows */
#ifndef PRODUCT3700
	proto = proto_atoi(protocol);
	r = get_redirect_rule(ext_if_name, eport, proto,
            iaddr_old, sizeof(iaddr_old), &iport_old, 0, 0, 0, 0);
    if(r == 0)/*find rule with same eport and prtocol,delete it first*/
    {
    	//upnp_delete_redirection(eport, protocol);
    	snprintf(rule, sizeof(rule), "upnp:1:%s:%s:%hu-%hu:any:%hu-%hu",
                    protocol, iaddr_old, iport_old, iport_old, eport, eport);
    	cpm_del(rule);
    }
#endif
    if (enabled) {
        snprintf(rule, sizeof(rule), "upnp:1:%s:%s:%hu-%hu:any:%hu-%hu",
                protocol, int_ip, iport, iport, eport, eport);
        debug_msg(rule);
        r = cpm_add(rule);
    }
    /*if(IsRedirectConflictWithPF(eport, protocol, int_ip, iport))*/
    debug_msg("%s: r=%d\n", __func__, r);
	if(r == -ENAPT_SERV_CONFLICT || r == -ENAPT_SYS_CONFLICT)
        r = -2;
    else
#endif
    {
    	if(enabled)
    	{
    		r = upnp_redirect(r_host,eport, int_ip, iport, protocol, desc, atoi(leaseduration));
    	}else
    	{
    			int iproto = proto_atoi(protocol);

				delete_redirect_and_filter_rules(eport, iproto);

				if(lease_file_update( eport, int_ip, iport, iproto, desc, enabled) == 1)
				{
					lease_file_add( eport, int_ip, iport, iproto, desc, enabled);
				}
				r = 0;
    	}
    }
    ClearNameValueList(&data);
    /* possible error codes for AddPortMapping :
     * 402 - Invalid Args
     * 501 - Action Failed
     * 715 - Wildcard not permited in SrcAddr
     * 716 - Wildcard not permited in ExtPort
     * 718 - ConflictInMappingEntry
     * 724 - SamePortValuesRequired
     * 725 - OnlyPermanentLeasesSupported
     The NAT implementation only supports permanent lease times on
     port mappings
     * 726 - RemoteHostOnlySupportsWildcard
     RemoteHost must be a wildcard and cannot be a specific IP
     address or DNS name
     * 727 - ExternalPortOnlySupportsWildcard
     ExternalPort must be a wildcard and cannot be a specific port
     value */
    switch(r)
    {
        case 0:	/* success */
            UPnPSyslog("AddPortMapping", inet_ntoa(h->clientaddr));
            BuildSendAndCloseSoapResp(h, resp, sizeof(resp)-1);
            break;
        case -2:	/* already redirected */
        case -3:	/* not permitted */
            SoapError(h, 718, "ConflictInMappingEntry");
            break;
        default:
            SoapError(h, 501, "ActionFailed");
    }
}

    static void
GetSpecificPortMappingEntry(struct upnphttp * h, const char * action)
{
    int r;
	
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewInternalPort>%u</NewInternalPort>\n"
        "<NewInternalClient>%s</NewInternalClient>\n"
        "<NewEnabled>%d</NewEnabled>\n"
        "<NewPortMappingDescription>%s</NewPortMappingDescription>\n"
        "<NewLeaseDuration>0</NewLeaseDuration>\n"
        "</u:%sResponse>\n";

    char body[1024];
    int bodylen;
    struct NameValueParserData data;
    const char * r_host, * ext_port, * protocol;
    unsigned short eport, iport;
    char int_ip[32];
    char desc[64];
    int enabled = 0; /* Entry enable or disable flag. */

    ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
    r_host = GetValueFromNameValueList(&data, "NewRemoteHost");
    ext_port = GetValueFromNameValueList(&data, "NewExternalPort");
    protocol = GetValueFromNameValueList(&data, "NewProtocol");
    if(!ext_port || !protocol)
    {
        ClearNameValueList(&data);
        SoapError(h, 402, "Invalid Args");
        return;
    }
	
	eport = (unsigned short)atoi(ext_port);

    r = upnp_get_redirection_infos(eport, protocol, &iport,
            int_ip, sizeof(int_ip),
            desc, sizeof(desc), &enabled);
    if(r < 0)
    {
        SoapError(h, 714, "NoSuchEntryInArray");
    }
    else
    {
        syslog(LOG_INFO, "%s: rhost='%s' %s %s found => %s:%u desc='%s'",
                action,
                r_host, ext_port, protocol, int_ip, (unsigned int)iport, desc);
        bodylen = snprintf(body, sizeof(body), resp,
                action, "urn:schemas-upnp-org:service:WANIPConnection:1",
                (unsigned int)iport, int_ip, enabled, desc,
                action);
        BuildSendAndCloseSoapResp(h, body, bodylen);
    }

    ClearNameValueList(&data);
}

    static void
DeletePortMapping(struct upnphttp * h, const char * action)
{
    int r;

    static const char resp[] =
        "<u:DeletePortMappingResponse "
        "xmlns:u=\"urn:schemas-upnp-org:service:WANIPConnection:1\">\n"
        "</u:DeletePortMappingResponse>\n";
#ifdef CONENAT
    FILE *fp = NULL;
    char cnat_pro[8];
    char cnat_ip[20];
    char rule[256];
    unsigned short cnat_eport, cnat_iport;
    int cnat_id;
    int find = 0;
#endif
    struct NameValueParserData data;
    const char * r_host = NULL, * ext_port, * protocol;
    unsigned short eport;
	
	if (r_host)	
	{
		/* let gcc463 not complain. */
	}

    ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
    r_host = GetValueFromNameValueList(&data, "NewRemoteHost");
    ext_port = GetValueFromNameValueList(&data, "NewExternalPort");
    protocol = GetValueFromNameValueList(&data, "NewProtocol");

    if(!ext_port || !protocol)
    {
        ClearNameValueList(&data);
        SoapError(h, 402, "Invalid Args");
        return;
    }
    eport = (unsigned short)atoi(ext_port);

    /* TODO : if in secure mode, check the IP */

    syslog(LOG_INFO, "%s: external port: %hu, protocol: %s",
            action, eport, protocol);

    r = upnp_delete_redirection(eport, protocol);

    if(r < 0)
    {
        SoapError(h, 714, "NoSuchEntryInArray");
    }
    else
    {
#ifdef CONENAT
        if((fp = fopen("/proc/cnapt_serv", "r")) != NULL)
        {
            while(fgets(rule, sizeof(rule), fp) != NULL)
            {
                sscanf(rule, "cpm-%d: enable=%*d priv=%[^:]:%*d-%hu pub=%*[^:]:%*d-%hu protocol=%6[^\n]",
                        &cnat_id, cnat_ip, &cnat_iport, &cnat_eport, cnat_pro);
                if(strcasecmp(cnat_pro, protocol) == 0
                        && eport == cnat_eport
                        && cnat_id == 2)    /* id==2 is upnp rule */
                {
                    find = 1;
                    break;
                }
            }
            fclose(fp);
        }
        if(find)
        {
            snprintf(rule, sizeof(rule), "upnp:1:%s:%s:%hu-%hu:any:%hu-%hu",
                    cnat_pro, cnat_ip, cnat_iport, cnat_iport, cnat_eport, cnat_eport);
            debug_msg(rule);
            cpm_del(rule);
        }
#endif
        UPnPSyslog("DeletePortMapping", inet_ntoa(h->clientaddr));
        BuildSendAndCloseSoapResp(h, resp, sizeof(resp)-1);
    }

    ClearNameValueList(&data);
}

    static void
GetGenericPortMappingEntry(struct upnphttp * h, const char * action)
{
    int r;

    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewRemoteHost></NewRemoteHost>\n"
        "<NewExternalPort>%u</NewExternalPort>\n"
        "<NewProtocol>%s</NewProtocol>\n"
        "<NewInternalPort>%u</NewInternalPort>\n"
        "<NewInternalClient>%s</NewInternalClient>\n"
        "<NewEnabled>%d</NewEnabled>\n"
        "<NewPortMappingDescription>%s</NewPortMappingDescription>\n"
        "<NewLeaseDuration>0</NewLeaseDuration>\n"
        "</u:%sResponse>\n";

    int index = 0;
    unsigned short eport, iport;
    const char * m_index;
    char protocol[4], iaddr[32];
    char desc[64];
    struct NameValueParserData data;
    int enabled = 1; /* Entry enabled or disabled flag. */
	
    ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
    m_index = GetValueFromNameValueList(&data, "NewPortMappingIndex");

    if(!m_index)
    {
        ClearNameValueList(&data);
        SoapError(h, 402, "Invalid Args");
        return;
    }

    index = (int)atoi(m_index);
    syslog(LOG_INFO, "%s: index=%d", action, index);

    r = upnp_get_redirection_infos_by_index(index, &eport, protocol, &iport,
            iaddr, sizeof(iaddr),
            desc, sizeof(desc), &enabled);

    if(r < 0)
    {
        SoapError(h, 713, "SpecifiedArrayIndexInvalid");
    }
    else
    {
        int bodylen;
        char body[2048];
        bodylen = snprintf(body, sizeof(body), resp,
                action, "urn:schemas-upnp-org:service:WANIPConnection:1",
                (unsigned int)eport, protocol, (unsigned int)iport, iaddr, enabled, desc,
                action);
        BuildSendAndCloseSoapResp(h, body, bodylen);
    }

    ClearNameValueList(&data);
}

#ifdef ENABLE_L3F_SERVICE
    static void
SetDefaultConnectionService(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:SetDefaultConnectionServiceResponse "
        "xmlns:u=\"urn:schemas-upnp-org:service:Layer3Forwarding:1\">\n"
        "</u:SetDefaultConnectionServiceResponse>\n";
    struct NameValueParserData data;
    char * p;
    ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
    UPnPSyslog("SetDefaultConnectionService", inet_ntoa(h->clientaddr));
    p = GetValueFromNameValueList(&data, "NewDefaultConnectionService");
    if(p) {
        syslog(LOG_INFO, "%s(%s) : Ignored", action, p);
    }
    ClearNameValueList(&data);
    BuildSendAndCloseSoapResp(h, resp, sizeof(resp)-1);
}

    static void
GetDefaultConnectionService(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"urn:schemas-upnp-org:service:Layer3Forwarding:1\">\n"
        "<NewDefaultConnectionService>%s:WANConnectionDevice:1,"
        "urn:upnp-org:serviceId:WANIPConn1</NewDefaultConnectionService>\n"
        "</u:%sResponse>";
    /* example from UPnP_IGD_Layer3Forwarding 1.0.pdf :
     * uuid:44f5824f-c57d-418c-a131-f22b34e14111:WANConnectionDevice:1,
     * urn:upnp-org:serviceId:WANPPPConn1 */
    char body[1024];
    int bodylen;

    bodylen = snprintf(body, sizeof(body), resp,
            action, uuidvalue, action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}
#endif

#ifdef ENABLE_LANDEVICE
static void
ActionNotImplemented(struct upnphttp * h, const char * action)
{
    SoapError(h, 501, "Action Failed");
}
static void
GetDHCPServerConfigurable(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewDHCPServerConfigurable>%u</NewDHCPServerConfigurable>\n"
        "</u:%sResponse>\n";

    char body[1024];
    int bodylen;

    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:LANHostConfigManagement:1",
            0, action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}
static void
GetDHCPRelay(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewDHCPRelay>%u</NewDHCPRelay>\n"
        "</u:%sResponse>\n";

    char body[1024];
    int bodylen;

    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:LANHostConfigManagement:1",
            0, action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}
static void
GetSubnetMask(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewSubnetMask>%s</NewSubnetMask>\n"
        "</u:%sResponse>\n";

    char body[1024];
    int bodylen;

    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:LANHostConfigManagement:1",
            GetLANNetMask(), action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}
static void
GetIPRoutersList(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewIPRouters>%s</NewIPRouters>\n"
        "</u:%sResponse>\n";

    char body[1024];
    int bodylen;

    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:LANHostConfigManagement:1",
            GetLANIPRouterList(), action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}
static void
GetDomainName(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewDomainName>%s</NewDomainName>\n"
        "</u:%sResponse>\n";

    char body[1024];
    int bodylen;

    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:LANHostConfigManagement:1",
            "", action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}
static void
GetAddressRange(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewMinAddress>%s</NewMinAddress>\n"
        "<NewMaxAddress>%s</NewMaxAddress>\n"
        "</u:%sResponse>\n";

    char body[1024];
    int bodylen;

    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:LANHostConfigManagement:1",
            GetLANIPMin(),GetLANIPMax(), action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}
static void
GetReservedAddresses(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewReservedAddresses>%s</NewReservedAddresses>\n"
        "</u:%sResponse>\n";

/* separator token */
#define ENTRY_TOKEN             (0x01)
#define ITEM_TOKEN              (0x02)
#define MAX_RULE_SIZE 4096

    char body[MAX_RULE_SIZE+1024];
    int bodylen;
    char reserved_addresses[MAX_RULE_SIZE]="";
	char *pt=GetLANIPReserved();

	while(*pt!='\0'){
        char* ip = NULL;
		pt = strchr(pt,ITEM_TOKEN);
		ip = ++pt; 
		pt = strchr(pt,ITEM_TOKEN);
		*pt++ = '\0';
		pt = strchr(pt,ENTRY_TOKEN);
		
	    strcat(reserved_addresses,ip);

		if(pt != NULL)
		{
	        strcat(reserved_addresses,",");
			pt++;
		}
		else
		    break;
	}
	
    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:LANHostConfigManagement:1",
            reserved_addresses, action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}
static void
GetDNSServers(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<NewDNSServers>%s</NewDNSServers>\n"
        "</u:%sResponse>\n";

    char body[1024];
    int bodylen;

    bodylen = snprintf(body, sizeof(body), resp,
            action, "urn:schemas-upnp-org:service:LANHostConfigManagement:1",
            GetLANDNSServers(), action);
    BuildSendAndCloseSoapResp(h, body, bodylen);
}
#endif

/*
   If a control point calls QueryStateVariable on a state variable that is not
   buffered in memory within (or otherwise available from) the service,
   the service must return a SOAP fault with an errorCode of 404 Invalid Var.

   QueryStateVariable remains useful as a limited test tool but may not be
   part of some future versions of UPnP.
 */
    static void
QueryStateVariable(struct upnphttp * h, const char * action)
{
    static const char resp[] =
        "<u:%sResponse "
        "xmlns:u=\"%s\">\n"
        "<return>%s</return>\n"
        "</u:%sResponse>\n";

    char body[512];
    int bodylen;
    struct NameValueParserData data;
    const char * var_name;

    ParseNameValue(h->req_buf + h->req_contentoff, h->req_contentlen, &data);
    /*var_name = GetValueFromNameValueList(&data, "QueryStateVariable"); */
    /*var_name = GetValueFromNameValueListIgnoreNS(&data, "varName");*/
    var_name = GetValueFromNameValueList(&data, "varName");

    /*syslog(LOG_INFO, "QueryStateVariable(%.40s)", var_name); */

    if(!var_name)
    {
        SoapError(h, 402, "Invalid Args");
    }
    else if(strcmp(var_name, "ConnectionStatus") == 0)
    {
        bodylen = snprintf(body, sizeof(body), resp,
                action, "urn:schemas-upnp-org:control-1-0",
                "Connected", action);
        BuildSendAndCloseSoapResp(h, body, bodylen);
    }
#if 0
    /* not usefull */
    else if(strcmp(var_name, "ConnectionType") == 0)
    {
        bodylen = snprintf(body, sizeof(body), resp, "IP_Routed");
        BuildSendAndCloseSoapResp(h, body, bodylen);
    }
    else if(strcmp(var_name, "LastConnectionError") == 0)
    {
        bodylen = snprintf(body, sizeof(body), resp, "ERROR_NONE");
        BuildSendAndCloseSoapResp(h, body, bodylen);
    }
#endif
    else if(strcmp(var_name, "PortMappingNumberOfEntries") == 0)
    {
        char strn[10];
        snprintf(strn, sizeof(strn), "%i",
                upnp_get_portmapping_number_of_entries());
        bodylen = snprintf(body, sizeof(body), resp,
                action, "urn:schemas-upnp-org:control-1-0",
                strn, action);
        BuildSendAndCloseSoapResp(h, body, bodylen);
    }
    else
    {
        syslog(LOG_NOTICE, "%s: Unknown: %s", action, var_name?var_name:"");
        SoapError(h, 404, "Invalid Var");
    }

    ClearNameValueList(&data);
}

/* Windows XP as client send the following requests :
 * GetConnectionTypeInfo
 * GetNATRSIPStatus
 * ? GetTotalBytesSent - WANCommonInterfaceConfig
 * ? GetTotalBytesReceived - idem
 * ? GetTotalPacketsSent - idem
 * ? GetTotalPacketsReceived - idem
 * GetCommonLinkProperties - idem
 * GetStatusInfo - WANIPConnection
 * GetExternalIPAddress
 * QueryStateVariable / ConnectionStatus!
 */
static const struct
{
    const char * methodName;
    void (*methodImpl)(struct upnphttp *, const char *);
}
soapMethods[] =
{
    { "SetConnectionType", SetConnectionType},
    { "GetEthernetLinkStatus", GetEthernetLinkStatus},
    { "GetConnectionTypeInfo", GetConnectionTypeInfo },
    { "RequestConnection", RequestConnection },
    { "ForceTermination", ForceTermination },
    { "GetNATRSIPStatus", GetNATRSIPStatus},
    { "GetExternalIPAddress", GetExternalIPAddress},
    { "AddPortMapping", AddPortMapping},
    { "DeletePortMapping", DeletePortMapping},
    { "GetGenericPortMappingEntry", GetGenericPortMappingEntry},
    { "GetSpecificPortMappingEntry", GetSpecificPortMappingEntry},
    { "QueryStateVariable", QueryStateVariable},
    { "GetTotalBytesSent", GetTotalBytesSent},
    { "GetTotalBytesReceived", GetTotalBytesReceived},
    { "GetTotalPacketsSent", GetTotalPacketsSent},
    { "GetTotalPacketsReceived", GetTotalPacketsReceived},
    { "GetCommonLinkProperties", GetCommonLinkProperties},
    { "GetStatusInfo", GetStatusInfo},
    { "GetLinkLayerMaxBitRates", GetLinkLayerMaxBitRates},
#ifdef ENABLE_L3F_SERVICE
    { "SetDefaultConnectionService", SetDefaultConnectionService},
    { "GetDefaultConnectionService", GetDefaultConnectionService},
#endif

#ifdef ENABLE_LANDEVICE
    {"SetDHCPServerConfigurable",    ActionNotImplemented},
    {"GetDHCPServerConfigurable",    GetDHCPServerConfigurable},
    {"SetDHCPRelay"             ,    ActionNotImplemented             },
    {"GetDHCPRelay"             ,    GetDHCPRelay             },
    {"SetSubnetMask"            ,    ActionNotImplemented            },
    {"GetSubnetMask"            ,    GetSubnetMask            },
    {"SetIPRouter"              ,    ActionNotImplemented              },
    {"DeleteIPRouter"           ,    ActionNotImplemented           },
    {"GetIPRoutersList"         ,    GetIPRoutersList         },
    {"SetDomainName"            ,    ActionNotImplemented            },
    {"GetDomainName"            ,    GetDomainName            },
    {"SetAddressRange"          ,    ActionNotImplemented          },
    {"GetAddressRange"          ,    GetAddressRange          },
    {"SetReservedAddress"       ,    ActionNotImplemented       },
    {"DeleteReservedAddress"    ,    ActionNotImplemented    },
    {"GetReservedAddresses"     ,    GetReservedAddresses     },
    {"SetDNSServer"             ,    ActionNotImplemented             },
    {"DeleteDNSServer"          ,    ActionNotImplemented          },
    {"GetDNSServers"            ,    GetDNSServers            },
#endif

    { 0, 0 }
};

    void
ExecuteSoapAction(struct upnphttp * h, const char * action, int n)
{
    char * p;
    char * p2;
    int i, len, methodlen;

    i = 0;
    p = strchr(action, '#');

    if(p)
    {
        p++;
        p2 = strchr(p, '"');
        if(p2)
            methodlen = p2 - p;
        else
            methodlen = n - (p - action);
        /*syslog(LOG_DEBUG, "SoapMethod: %.*s", methodlen, p);*/
        while(soapMethods[i].methodName)
        {
            len = strlen(soapMethods[i].methodName);
            if(strncmp(p, soapMethods[i].methodName, len) == 0)
            {
                soapMethods[i].methodImpl(h, soapMethods[i].methodName);
                return;
            }
            i++;
        }

        syslog(LOG_NOTICE, "SoapMethod: Unknown: %.*s", methodlen, p);
    }

    SoapError(h, 401, "Invalid Action");
}

/* Standard Errors:
 *
 * errorCode errorDescription Description
 * --------	---------------- -----------
 * 401 		Invalid Action 	No action by that name at this service.
 * 402 		Invalid Args 	Could be any of the following: not enough in args,
 * 							too many in args, no in arg by that name,
 * 							one or more in args are of the wrong data type.
 * 403 		Out of Sync 	Out of synchronization.
 * 501 		Action Failed 	May be returned in current state of service
 * 							prevents invoking that action.
 * 600-699 	TBD 			Common action errors. Defined by UPnP Forum
 * 							Technical Committee.
 * 700-799 	TBD 			Action-specific errors for standard actions.
 * 							Defined by UPnP Forum working committee.
 * 800-899 	TBD 			Action-specific errors for non-standard actions.
 * 							Defined by UPnP vendor.
 */
    void
SoapError(struct upnphttp * h, int errCode, const char * errDesc)
{
    static const char resp[] =
        "<s:Envelope "
        "xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
        "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n"
        "<s:Body>\n"
        "<s:Fault>\n"
        "<faultcode>s:Client</faultcode>\n"
        "<faultstring>UPnPError</faultstring>\n"
        "<detail>\n"
        "<UPnPError xmlns=\"urn:schemas-upnp-org:control-1-0\">\n"
        "<errorCode>%d</errorCode>\n"
        "<errorDescription>%s</errorDescription>\n"
        "</UPnPError>\n"
        "</detail>\n"
        "</s:Fault>\n"
        "</s:Body>\n"
        "</s:Envelope>\n";

    char body[2048];
    int bodylen;

    syslog(LOG_INFO, "Returning UPnPError %d: %s", errCode, errDesc);
    bodylen = snprintf(body, sizeof(body), resp, errCode, errDesc);
    BuildResp2_upnphttp(h, 500, "Internal Server Error", body, bodylen);
    SendResp_upnphttp(h);
    CloseSocket_upnphttp(h);
}


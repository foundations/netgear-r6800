/* $Id: upnpredirect.c,v 1.38 2008/04/25 17:17:29 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006-2007 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <unistd.h>

#include "upnpgetnvram.h"
#include "config.h"
#include "upnpredirect.h"
#include "upnpglobalvars.h"
#include "upnpevents.h"
#if defined(USE_NETFILTER)
#include "netfilter/iptcrdr.h"
#endif
#if defined(USE_PF)
#include "pf/obsdrdr.h"
#endif
#if defined(USE_IPF)
#include "ipf/ipfrdr.h"
#endif
#ifdef USE_MINIUPNPDCTL
#include <stdio.h>
#include <unistd.h>
#endif
#ifdef ENABLE_LEASEFILE
#include <sys/stat.h>
#endif
#include "getifstats.h"
//#include "lib_napt.h"

#ifdef CONENAT
#include "libcnapt.h"
#endif

#ifdef ENABLE_LEASEFILE
/* find lease file and get rule status
 * if found return 1 else return 0
 */
static int lease_file_find( unsigned short eport,
		const char *proto,
        char * iaddr, int iaddrlen, unsigned short * iport,
        char * desc, int desclen,
        int *enabled)
{
	char str[32];
	char buf[512];
	int str_size;
	int r = 0;
	FILE *fp;

    snprintf( str, sizeof(str), "%s:%u", proto, eport);
    str_size = strlen(str);

    fp = fopen( lease_file, "r");
    if(!fp)
    	return 0; /* if open file failed return 0 not found.*/


    while (fgets(buf, sizeof(buf), fp) != NULL)
	{

		if (strncmp(str, buf, str_size) == 0)
		{
			/* proto
			 *  |  eport
			 *  |   |      iaddr   iport desc enabled
			 * TCP:8008:192.168.1.2:8088:http:1 */
			sscanf(buf, "%*[^:]:%*[^:]:%[^:]:%hu:%[^:]:%d", iaddr, iport, desc,
					enabled);
			r = 1/* find */;
			break;
		}
	}

    fclose(fp);

    return r;
}
/* get_redirect_rule_by_index()
 * return -1 when the rule was not found */
static int get_redirect_rule_from_lease_file_by_index(int index,
		unsigned short * eport, char * iaddr, int iaddrlen,
		unsigned short * iport, char * proto, char * desc, int desclen,
		int *enabled)
{
	int i = 0;
	char buf[512];
	FILE *fp;

    fp = fopen( lease_file, "r");
    if(!fp)
    	return -1;

    while (fgets(buf, sizeof(buf) - 1, fp) != NULL)
	{

		if (index == i)
		{

			/* proto                      desc
			 *  |  eport            iport  |
			 *  |   |      iaddr      |    |  enabled
			 * TCP:8008:192.168.1.2:8088:http:1 */
			sscanf(buf,"%[^:]:%hu:%[^:]:%hu:%[^:]:%d",
					proto, eport ,iaddr, iport, desc, enabled);
			 fclose(fp);
			return 0;
		}

		i++;
	}

    fclose(fp);
    return -1;
}


int lease_file_add( unsigned short eport, const char * iaddr, unsigned short iport, int proto, const char * desc, int enabled)
{
    FILE* fd;
    char caStr[128], caBuf[512];

    memset(caStr, 0x00, 128);
    memset(caBuf, 0x00, 512);

    if (lease_file == NULL) return 0;

#if 1
    sprintf(caStr, "%s:%u:%s:%u:%s:%d", ((proto==IPPROTO_TCP)?"TCP":"UDP"), eport, iaddr, iport, desc, enabled);

    fd = fopen( lease_file, "a+");
    if (fd==NULL) {
        syslog(LOG_ERR, "could not open lease file: %s", lease_file);
        return -1;
    }

    while(fgets(caBuf, sizeof(caBuf) - 1, fd) != NULL) {
        if(strstr(caBuf, caStr) != NULL){
            fclose(fd);
            return 0;
        }
        memset(caBuf, 0x00, 512);
    }
    fprintf(fd, "%s\n", caStr);
#else
    fd = fopen( lease_file, "a");
    if (fd==NULL) {
        syslog(LOG_ERR, "could not open lease file: %s", lease_file);
        return -1;
    }

    /* FIXME: what if desc contains ':'? */
    fprintf( fd, "%s:%u:%s:%u:%s\n", ((proto==IPPROTO_TCP)?"TCP":"UDP"), eport, iaddr, iport, desc);
#endif
    fclose(fd);

    return 0;
}

/* update lease file entry
 * return 0 if update success
 * return 1 not found entry
 * return -1 error
 */
int lease_file_update( unsigned short eport, const char * iaddr, unsigned short iport, int proto, const char * desc, int enabled)
{
    FILE *fp, *fpt;
    char caStr[128], caBuf[512], str[32];
	char tmpfilename[128];
	int str_size, buf_size, tmpfd;
	int updated = 0;

	if (lease_file == NULL) return -1;

    sprintf(caStr, "%s:%u:%s:%u:%s:%d", ((proto==IPPROTO_TCP)?"TCP":"UDP"), eport, iaddr, iport, desc, enabled);
    snprintf( str, sizeof(str), "%s:%u:", ((proto==IPPROTO_TCP)?"TCP":"UDP"), eport);
    str_size = strlen(str);

    fp = fopen( lease_file, "r");
    if (fp==NULL) {
        return 1;
    }

    strncpy( tmpfilename, lease_file, sizeof(tmpfilename) );
    strncat( tmpfilename, "XXXXXX", sizeof(tmpfilename) - strlen(tmpfilename));

    tmpfd = mkstemp(tmpfilename);
    if (tmpfd==-1) {
        fclose(fp);
        SYSLOG(LOG_ERR, "could not open temporary lease file");
        return -1;
    }

    fchmod(tmpfd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    fpt = fdopen(tmpfd, "a");

    while(fgets(caBuf, sizeof(caBuf), fp) != NULL) {
    	buf_size = strlen(caBuf);

    	if(strncmp( str, caBuf, str_size)!=0)
    	{
    		fwrite(caBuf, buf_size, 1, fpt);
    	}else
    	{
    		fprintf(fpt,"%s\n",caStr);
    		updated = 1;
    	}
    }

    fclose(fpt);
    fclose(fp);

    if (updated)
	{
		if (rename(tmpfilename, lease_file) < 0)
		{
			SYSLOG(LOG_ERR, "could not rename temporary lease file to %s", lease_file);
			remove(tmpfilename);
		}

		return 0;
	}
	else
	{
		remove(tmpfilename);
	}
    return 1;
}
/* if param enabled is 1 delete entry from lease file 
 * else mark the entry disable.
 */
static int lease_file_remove( unsigned short eport, int proto)
{
    FILE* fd, *fdt;
    int tmp;
    char buf[512];
    char str[32];
    char tmpfilename[128];
    int str_size, buf_size;


    if (lease_file == NULL) return 0;

    if (strlen( lease_file) + 7 > sizeof(tmpfilename)) {
        SYSLOG(LOG_ERR, "Lease filename is too long");
        return -1;
    }

    strncpy( tmpfilename, lease_file, sizeof(tmpfilename) );
    strncat( tmpfilename, "XXXXXX", sizeof(tmpfilename) - strlen(tmpfilename));

    fd = fopen( lease_file, "r");
    if (fd==NULL) {
        return 0;
    }

    snprintf( str, sizeof(str), "%s:%u:", ((proto==IPPROTO_TCP)?"TCP":"UDP"), eport);
    str_size = strlen(str);

    tmp = mkstemp(tmpfilename);
    if (tmp==-1) {
        fclose(fd);
        SYSLOG(LOG_ERR, "could not open temporary lease file");
        return -1;
    }
    fchmod(tmp, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    fdt = fdopen(tmp, "a");

    buf[sizeof(buf)-1] = 0;
    while( fgets( buf, sizeof(buf)-1, fd) != NULL) {
        buf_size = strlen(buf);

        if (buf_size < str_size || strncmp( str, buf, str_size)!=0) {
			fwrite(buf, buf_size, 1, fdt);
		}
    }
    fclose(fdt);
    fclose(fd);

    if (rename( tmpfilename, lease_file) < 0) {
        SYSLOG(LOG_ERR, "could not rename temporary lease file to %s", lease_file);
        remove( tmpfilename);
    }

    return 0;

}
#endif

/* proto_atoi() 
 * convert the string "UDP" or "TCP" to IPPROTO_UDP and IPPROTO_UDP */
int
proto_atoi(const char * protocol)
{
    int proto = IPPROTO_TCP;
    if(strcmp(protocol, "UDP") == 0)
        proto = IPPROTO_UDP;
    return proto;
}

/* upnp_redirect() 
 * calls OS/fw dependant implementation of the redirection.
 * protocol should be the string "TCP" or "UDP"
 * returns: 0 on success
 *          -1 failed to redirect
 *          -2 already redirected
 *          -3 permission check failed
 */
    int
upnp_redirect(const char * r_host, unsigned short eport, 
        const char * iaddr, unsigned short iport,
        const char * protocol, const char * desc,
        unsigned int leaseduration)
{
    int proto, r;
    char iaddr_old[32];
    unsigned short iport_old;
    struct in_addr address;
    proto = proto_atoi(protocol);
    if(inet_aton(iaddr, &address) < 0)
    {
        SYSLOG(LOG_ERR, "inet_aton(%s) : %m", iaddr);
        return -1;
    }

    if(!check_upnp_rule_against_permissions(upnppermlist, num_upnpperm,
                eport, address, iport))
    {
        SYSLOG(LOG_INFO, "redirection permission check failed for "
                "%hu->%s:%hu %s", eport, iaddr, iport, protocol);
        return -3;
    }
    r = get_redirect_rule(ext_if_name, eport, proto,
            iaddr_old, sizeof(iaddr_old), &iport_old, 0, 0, 0, 0);
    if(r == 0)
    {
        /* if existing redirect rule matches redirect request return success
         * xbox 360 does not keep track of the port it redirects and will
         * redirect another port when receiving ConflictInMappingEntry */
        if(strcmp(iaddr,iaddr_old)==0 && iport==iport_old)
        {
            SYSLOG(LOG_INFO, "ignoring redirect request as it matches existing redirect");
        }
        else
        {

            SYSLOG(LOG_INFO, "port %hu protocol %s already redirected to %s:%hu",
                    eport, protocol, iaddr_old, iport_old);
            return -2;
        }
    }
    else
    {
        LOG_ONE_MSG(LOG_INFO, "[UPNP]redirecting port %hu to %s:%hu protocol %s for: %s",
                eport, iaddr, iport, protocol, desc);			
        return upnp_redirect_internal(r_host,eport, iaddr, iport, proto, desc, leaseduration);
#if 0
        if(add_redirect_rule2(ext_if_name, eport, iaddr, iport, proto, desc) < 0)
        {
            return -1;
        }

        SYSLOG(LOG_INFO, "creating pass rule to %s:%hu protocol %s for: %s",
                iaddr, iport, protocol, desc);
        if(add_filter_rule2(ext_if_name, iaddr, eport, iport, proto, desc) < 0)
        {
            /* clean up the redirect rule */
#if !defined(__linux__)
            delete_redirect_rule(ext_if_name, eport, proto);
#endif
            return -1;
        }
#endif
    }

    return 0;
}

    int
upnp_redirect_internal(const char * r_host ,unsigned short eport,
        const char * iaddr, unsigned short iport,
        int proto, const char * desc, unsigned int leaseduration)
{
			/*SYSLOG(LOG_INFO, "redirecting port %hu to %s:%hu protocol %s for: %s",
			  eport, iaddr, iport, protocol, desc);			*/
			if(add_redirect_rule2(r_host ,ext_if_name, eport, iaddr, iport, proto, desc, leaseduration) < 0)
			{
				return -1;
			}

		#ifdef ENABLE_LEASEFILE
			if(lease_file_update( eport, iaddr, iport, proto, desc, 1) == 1)
			{
				lease_file_add( eport, iaddr, iport, proto, desc, 1);
			}
		#endif
			/*	SYSLOG(LOG_INFO, "creating pass rule to %s:%hu protocol %s for: %s",
				iaddr, iport, protocol, desc);*/
			if(add_filter_rule2(ext_if_name, iaddr, eport, iport, proto, desc) < 0)
			{
				/* clean up the redirect rule */
		#if !defined(__linux__)
				delete_redirect_rule(ext_if_name, eport, proto);
		#endif
				return -1;
			}
#ifdef ENABLE_EVENTS
    upnp_event_var_change_notify(EWanIPC);
#endif
    return 0;
}



    int
upnp_get_redirection_infos(unsigned short eport, const char * protocol,
        unsigned short * iport,
        char * iaddr, int iaddrlen,
        char * desc, int desclen, int *enabled)
{
   int r  = -1;

    if(desc && (desclen > 0))
        desc[0] = '\0';
	if (lease_file_find(eport, protocol, iaddr, iaddrlen, iport, desc,
			desclen, enabled))
    {
		if (*enabled)
		{
			r = get_redirect_rule(ext_if_name, eport, proto_atoi(protocol),
					iaddr, iaddrlen, iport, desc, desclen, 0, 0);
		}
		else
		{
			r = 0;
		}
	}
    return r;
}

    int
upnp_get_redirection_infos_by_index(int index,
        unsigned short * eport, char * protocol,
        unsigned short * iport, 
        char * iaddr, int iaddrlen,
        char * desc, int desclen, int *enabled)
{
#ifndef ENABLE_LEASEFILE
    /*char ifname[IFNAMSIZ];*/
    int proto = 0;


    if(desc && (desclen > 0))
        desc[0] = '\0';

    if(get_redirect_rule_by_index(index, 0/*ifname*/, eport, iaddr, iaddrlen,
                iport, &proto, desc, desclen, 0, 0) < 0)
        return -1;
    else
    {
        if(proto == IPPROTO_TCP)
            memcpy(protocol, "TCP", 4);
        else
            memcpy(protocol, "UDP", 4);
        return 0;
    }
#else
    if(get_redirect_rule_from_lease_file_by_index(index, eport, iaddr, iaddrlen,
                    iport, protocol, desc, desclen, enabled) < 0)
            return -1;

    return 0;
#endif
}

    int
_upnp_delete_redir(unsigned short eport, int proto)
{
    int r;
#if defined(__linux__)
    r = delete_redirect_and_filter_rules(eport, proto);
#else
    r = delete_redirect_rule(ext_if_name, eport, proto);
    delete_filter_rule(ext_if_name, eport, proto);
#endif
#ifdef ENABLE_LEASEFILE
    lease_file_remove( eport, proto);
#endif

#ifdef ENABLE_EVENTS
    upnp_event_var_change_notify(EWanIPC);
#endif
    return r;
}

    int
upnp_delete_redirection(unsigned short eport, const char * protocol)
{
    LOG_ONE_MSG(LOG_INFO, "[UPNP]removing redirect rule port %hu %s", eport, protocol);
    return _upnp_delete_redir(eport, proto_atoi(protocol));
}

/* upnp_get_portmapping_number_of_entries()
 * TODO: improve this code */
    int
upnp_get_portmapping_number_of_entries()
{
    int n = 0, r = 0;
    unsigned short eport, iport;
    char protocol[4], iaddr[32], desc[64];
    int enabled;
    do {
        protocol[0] = '\0'; iaddr[0] = '\0'; desc[0] = '\0';
        r = upnp_get_redirection_infos_by_index(n, &eport, protocol, &iport,
                iaddr, sizeof(iaddr),
                desc, sizeof(desc), &enabled);
        n++;
    } while(r==0);
    return (n-1);
}

/* functions used to remove unused rules */
    struct rule_state *
get_upnp_rules_state_list(int max_rules_number_target)
{
    char ifname[IFNAMSIZ];
    int proto;
    unsigned short iport;
    struct rule_state * tmp;
    struct rule_state * list = 0;
    int i = 0;
    tmp = malloc(sizeof(struct rule_state));
    if(!tmp)
        return 0;
    while(get_redirect_rule_by_index(i, ifname, &tmp->eport, 0, 0,
                &iport, &proto, 0, 0,
                &tmp->packets, &tmp->bytes) >= 0)
    {
        tmp->proto = (short)proto;
        /* add tmp to list */
        tmp->next = list;
        list = tmp;
        /* prepare next iteration */
        i++;
        tmp = malloc(sizeof(struct rule_state));
        if(!tmp)
            break;
    }
    free(tmp);
    /* return empty list if not enough redirections */
    if(i<=max_rules_number_target)
        while(list)
        {
            tmp = list;
            list = tmp->next;
            free(tmp);
        }
    /* return list */
    return list;
}

    void
remove_unused_rules(struct rule_state * list)
{
    char ifname[IFNAMSIZ];
    unsigned short iport;
    struct rule_state * tmp;
    u_int64_t packets;
    u_int64_t bytes;
    int n = 0;
    while(list)
    {
        /* remove the rule if no traffic has used it */
        if(get_redirect_rule(ifname, list->eport, list->proto,
                    0, 0, &iport, 0, 0, &packets, &bytes) >= 0)
        {
            if(packets == list->packets && bytes == list->bytes)
            {
            	/* if delete entry form lease file use 1 */
                _upnp_delete_redir(list->eport, list->proto);
                n++;
            }
        }
        tmp = list;
        list = tmp->next;
        free(tmp);
    }
    if(n>0)
        SYSLOG(LOG_NOTICE, "removed %d unused rules", n);
}

/* return the time that the latest expired rule will expired */
unsigned int remove_expired_rules()
{
    int i;
    unsigned short eport, iport;
    char protocol[4], iaddr[32], desc[64];
    unsigned int leasetime=0, leaseduration=0, mini_dalta=0, timestamp=0, dalta=0;
    int enabled;

    for(i=0; i<upnp_get_portmapping_number_of_entries(); i++)
    {
        protocol[0] = '\0'; iaddr[0] = '\0'; desc[0] = '\0';
        upnp_get_redirection_infos_by_index(i, &eport, protocol, &iport,
                iaddr, sizeof(iaddr),
                desc, sizeof(desc), &enabled);
        if(get_redirect_rule_lease(eport, proto_atoi(protocol), &leasetime, &leaseduration))
            SYSLOG(LOG_INFO, "Can't find leasetime of the rule which external port=%d protocol=%s", eport, protocol);
        
        timestamp = get_uptime();
        if(leasetime != 0 && leasetime <= timestamp)
        {
            upnp_delete_redirection(eport, protocol);
#ifdef CONENAT
			{
			    FILE *fp = NULL;
			    char cnat_pro[8];
			    char cnat_ip[20];
			    char rule[256];
			    unsigned short cnat_eport, cnat_iport;
			    int cnat_id;
			    int find = 0;
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
	        }
#endif
        }
        else
        {
        	dalta = leasetime - timestamp;
        	if(mini_dalta == 0 && leasetime != 0)
        		mini_dalta = dalta;

        	if(leasetime != 0 && dalta < mini_dalta)
        	{
        		mini_dalta = dalta;
        	}
        }
    }
    if(mini_dalta > 0)
    	return mini_dalta;
    else
    	return 0;
}

/* stuff for miniupnpdctl */
#ifdef USE_MINIUPNPDCTL
    void
write_ruleset_details(int s)
{
    char ifname[IFNAMSIZ];
    int proto = 0;
    unsigned short eport, iport;
    char desc[64];
    char iaddr[32];
    u_int64_t packets;
    u_int64_t bytes;
    int i = 0;
    char buffer[256];
    int n;
    while(get_redirect_rule_by_index(i, ifname, &eport, iaddr, sizeof(iaddr),
                &iport, &proto, desc, sizeof(desc),
                &packets, &bytes) >= 0)
    {
        n = snprintf(buffer, sizeof(buffer), "%2d %s %s %hu->%s:%hu "
                "'%s' %llu %llu\n",
                i, ifname, proto==IPPROTO_TCP?"TCP":"UDP",
                eport, iaddr, iport, desc, packets, bytes);
        write(s, buffer, n);
        i++;
    }
}
#endif


/*
 * udp.c - handle upd connections
 *
 * Copyright (C) 1999 Brad M. Garcia <garsh@home.com>
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include "common.h"
#include "relay.h"
#include "cache.h"
#include "query.h"
#include "domnode.h"
#include "check.h"
#include "sc_project_defines.h"
#include "if_name.h"

#ifndef EXCLUDE_MASTER
#include "master.h"
#endif
#ifdef _RECURSION_
#include "dns.h"
#include "referral.h"
#endif
#include "nvram.h"
#include <stdarg.h>
#include <stdint.h>
#ifdef VLAN_IPTV
#include "ifgmap.h"
#endif

#define SYSTEM_BUF_SIZE 1024

#pragma pack(push, 1)
typedef struct 
	{
		uint16_t rr_pt;
		uint16_t rr_type;
		uint16_t rr_class;
		uint32_t rr_ttl;
		uint16_t rr_length;
	} RR_t;
#pragma pack(pop)

#include <stdarg.h>
static int print_console_func(const char *format, ...)
{
	if (access("/tmp/debug_dnrd_console", F_OK) == 0)
	{
#if 1
#define CONSOLE "/dev/console"
		static FILE* fp = NULL;
		va_list arg;
		if(fp == NULL)
		{
			fp = fopen(CONSOLE, "w");
			if(fp == NULL)
			{
				printf("can't open %s \n", CONSOLE);
				return 0;
			}
		}
		va_start(arg, format);
		vfprintf(fp, format, arg);
		va_end(arg);
#endif			
	}
	
	return 0;
}


int SYSTEM(const char *format, ...)
{
	char buf[SYSTEM_BUF_SIZE]="";
	va_list arg;

	va_start(arg, format);
	vsnprintf(buf,SYSTEM_BUF_SIZE, format, arg);
	va_end(arg);
	
	print_console_func("SYSTEM: %s\n", buf);
	
	system(buf);
	usleep(1);
	return 0;
}
#ifdef OPENDNS
static int trans_macaddr(OUT char * mac, IN char *mac_p);
static int get_macaddr_by_ip(OUT char *mac, IN char *ip);
static void get_deviceid_by_macaddr(IN char *macaddr, OUT char *deviceid);
static int trans_deviceid(IN char *deviceid, OUT char *deviceid_b);
#endif

#define HTONS_CHARS(n) (unsigned char)((n) >> 8), (unsigned char)(n)

static int udp_send(int sock, srvnode_t *srv, void *msg, int len)
{
    int	rc;
    time_t now = time(NULL);
    rc = sendto(sock, msg, len, 0,
		(const struct sockaddr *) &srv->addr,
		sizeof(struct sockaddr_in));

    if (rc != len) {
	log_msg(LOG_ERR, "sendto error: %s: ",
		inet_ntoa(srv->addr.sin_addr), strerror(errno));
	return (rc);
    }
    if ((srv->send_time == 0)) srv->send_time = now;
    srv->send_count++;
    return (rc);
}

unsigned short ttls[]={1,2,1,2,0,0};
int send2current(query_t *q, void *msg, const int len) {
    /* If we have domains associated with our servers, send it to the
       appropriate server as determined by srvr */
  domnode_t *d;
  assert(q != NULL);
  assert(q->domain != NULL);

  d = q->domain;
  q->client_time = time(NULL);
  q->ttl = ttls[q->try_count];/*set ttl for this time*/

  if(d->primary && q->try_count < TRY_SECOND_SERVER)
      q->srv = d->primary;
  else if(d->secondary && q->try_count >= TRY_SECOND_SERVER && q->try_count < TRY_THIRD_SERVER)/*SJ,if secondary don't exist,can't do this*/
      q->srv = d->secondary;
  else if(d->tertiary && q->try_count >= TRY_THIRD_SERVER)
      q->srv = d->tertiary;
    
  log_debug(2, "sending to: srv=%s, client_time: %lu, ttl: %d, try_count: %d, sock %d, msg%x ,len: %d",
	      q->srv?inet_ntoa(q->srv->addr.sin_addr):"NULL",
            q->client_time, q->ttl,q->try_count, q->sock, msg, len);
  if ((q->srv == NULL) || (udp_send(q->sock, q->srv, msg, len) != len)) {
      /*send query failed*/
      log_debug(2, "but failed....");
      return 0;
  }

  log_debug(2, "OK, let's wait for the response");
  return len;
}

#ifdef _RECURSION_
int send2referral(query_t *q, void *msg, const int len) {
    //domnode_t *d;
  assert(q != NULL);
  assert(q->referral != NULL);

  //d = q->domain;
  q->client_time = time(NULL);
  q->ttl = ttls[q->try_count];/*set ttl for this time*/
  q->srv = q->referral->next;
  //q->srv = d->primary;
  q->srv->addr.sin_port = htons(53);
  q->srv->addr.sin_family = AF_INET;
  q->recursion_count++;
  
  log_debug(2, "sending to: srv=%s, client_time: %lu, ttl: %d, try_count: %d",
	      q->srv?inet_ntoa(q->srv->addr.sin_addr):"NULL",
	      q->client_time, q->ttl,q->try_count);
  if ((q->srv == NULL) || (udp_send(q->sock, q->srv, msg, len) != len)) {
      /*send query failed*/
      log_debug(2, "but failed....");
      return 0;
  }

  log_debug(2, "OK, let's wait for the response");
  return len;
}
#endif

/*
 * put a short type data to a packet
 * INput:
 * 		val - host format; addr - a packet data's poiter
 * Return: void.
 */
void putshort(unsigned short val, char *addr)
{
	unsigned short tmp = htons(val);

	memcpy(addr, &tmp, 2);
	return;
}

/*
 * get a short type data from a packet
 * INput:
 * 		addr - a packet data's poiter
 * Return: host format data.
 */
unsigned short getshort(char *addr)
{
	unsigned short val;

	memcpy(&val, addr, 2);
	val=ntohs(val);
	return val;
}

/*
 * handle_udprequest()
 *
 * This function handles udp DNS requests by either replying to them (if we
 * know the correct reply via master, caching, etc.), or forwarding them to
 * an appropriate DNS server.
 */
  extern int opendns;
  extern char PC_table[];
query_t *udp_handle_request()
{
    unsigned           addr_len;
    int                len;
    const int          maxsize = UDP_MAXSIZE;
    static char        msg[UDP_MAXSIZE+4];
    struct sockaddr_in from_addr;
    int                fwd;
    domnode_t          *dptr;
    query_t *q, *prev;
#ifdef OPENDNS
	char macaddr[13]={0}, macaddr_p[18]={0};
	char deviceid[33]={0};
	const unsigned char clientid[11] = { HTONS_CHARS(4), HTONS_CHARS(15),
										'O', 'p', 'e', 'n', 'D', 'N', 'S' };
	const unsigned char fixed[11] = { 0, HTONS_CHARS(41), HTONS_CHARS(512),
									  0, 0, 0, 0, HTONS_CHARS(0) };
	unsigned char deviceid_b[8] = { 0x00, 0x00, 0x11, 0x11,
									0x11, 0x11, 0x11, 0x11};
	short int rdlen=0, add_rrs=0x01;
	char *buf=NULL, *pt=NULL;
#endif

    /* Read in the message */
    addr_len = sizeof(struct sockaddr_in);
    len = recvfrom(isock, msg, maxsize, 0,
		   (struct sockaddr *)&from_addr, &addr_len);

#ifdef OPENDNS

	if(1 == opendns)
	{
		char *pheader=msg+len, *p;

		if(!get_macaddr_by_ip(macaddr_p, inet_ntoa(from_addr.sin_addr)))
		{
			if(!trans_macaddr(macaddr, macaddr_p))
			{
				get_deviceid_by_macaddr(macaddr, deviceid);
				trans_deviceid(deviceid, deviceid_b);
			}
		}
		putshort(add_rrs, msg + 10);

		memcpy(msg+len, fixed, sizeof(fixed));
		len+=sizeof(fixed);
		memcpy(msg+len, clientid, sizeof(clientid));
		len+=sizeof(clientid);
		memcpy(msg+len, deviceid_b, sizeof(deviceid_b));
		len+=sizeof(deviceid_b);
		p = pheader + 9;
		rdlen = getshort(p);
		rdlen+=sizeof(clientid) + sizeof(deviceid_b);
		p = pheader + 9;
		putshort(rdlen, p);
	}
	free(buf);
#endif
    if (len < 0) {
	log_debug(1, "recvfrom error %s", strerror(errno));
	return NULL;
    }

    /* do some basic checking */
    if (check_query(msg, len) < 0) return NULL;

    /* Determine how query should be handled */
    if ((fwd = handle_query(&from_addr, msg, &len, &dptr)) < 0)
      return NULL; /* if its bogus, just ignore it */

    /* If we already know the answer, send it and we're done */
    if (fwd == 0) {
	if (sendto(isock, msg, len, 0, (const struct sockaddr *)&from_addr,
		   addr_len) != len) {
	    log_debug(1, "sendto error %s", strerror(errno));
	}
	return NULL;
    }


    /* dptr->current should never be NULL it is checked in handle_query */

    //    dnsquery_add(&from_addr, msg, len);
    // if (!send2current(dptr, msg, len)) {

    /* rewrite msg, get id and add to list*/

    if ((prev=query_add(dptr, dptr->current, &from_addr, msg, len)) == NULL){
      /* of some reason we could not get any new queries. we have to
	 drop this packet */
      return NULL;
    }
    q = prev->next;

    /*store the msg and len for sending query again*/
    memcpy(q->msg, msg, sizeof(msg));
    q->msg_len = len;

    if (send2current(q, msg, len) > 0) {
      /* add to query list etc etc */
      return q;
    } else {

      /* we couldn't send the query */
#ifndef EXCLUDE_MASTER
      int	packetlen;
      char	packet[maxsize+4];

      /*
       * If we couldn't send the packet to our DNS servers,
       * perhaps the `network is unreachable', we tell the
       * client that we are unable to process his request
       * now.  This will show a `No address (etc.) records
       * available for host' in nslookup.  With this the
       * client won't wait hang around till he gets his
       * timeout.
       * For this feature dnrd has to run on the gateway
       * machine.
       */

      if ((packetlen = master_dontknow(msg, len, packet)) > 0) {
	query_delete_next(prev);
	return NULL;
	if (sendto(isock, msg, len, 0, (const struct sockaddr *)&from_addr,
		   addr_len) != len) {
	  log_debug(1, "sendto error %s", strerror(errno));
	  return NULL;
	}
      }
#endif
    }
    return q;
}

/*
 * dnsrecv()							22OCT99wzk
 *
 * Abstract: A small wrapper for recv()/recvfrom() with output of an
 *           error message if needed.
 *
 * Returns:  A positove number indicating of the bytes received, -1 on a
 *           recvfrom error and 0 if the received message is too large.
 */
static int reply_recv(query_t *q, void *msg, int len)
{
    int	rc, fromlen;
    struct sockaddr_in from;

    fromlen = sizeof(struct sockaddr_in);
    rc = recvfrom(q->sock, msg, len, 0,
		  (struct sockaddr *) &from, &fromlen);

    if (rc == -1) {
	log_msg(LOG_ERR, "recvfrom error: %s",
		inet_ntoa(q->srv->addr.sin_addr));
	return (-1);
    }
    else if (rc > len) {
	log_msg(LOG_NOTICE, "packet too large: %s",
		inet_ntoa(q->srv->addr.sin_addr));
	return (0);
    }
    else if (memcmp(&from.sin_addr, &q->srv->addr.sin_addr,
		    sizeof(from.sin_addr)) != 0) {
	log_msg(LOG_WARNING, "unexpected server: %s",
		inet_ntoa(from.sin_addr));
	return (0);
    }

    return (rc);
}
#if defined(GUI_V14) || defined(AMPED)

#define ANSWER_IP_MAX 100
char answer_ip[ANSWER_IP_MAX][32];

/* get dns answer from reply pkts */
uint32_t get_answer_ip(char *data, int len, int answered_count)
{
	int ret = 0;
	RR_t dns_rr;
	char *pt = data;
	int i = 0, offset;
	int parsered_count = 0, got_answered_count = 0;
	uint32_t ret_ip = 0;
	/* prepare data, eat dns query part */
	while(*pt != 0)
		pt ++;
	pt = pt + 5;

	while(i + sizeof(dns_rr) < len)
	{
		memcpy(&dns_rr, pt, sizeof(dns_rr));
		parsered_count ++;
		/* the answer is ipv4 address */
		if(ntohs(dns_rr.rr_type) == 1)
		{
			if(i + sizeof(dns_rr) + 4 <= len)
			{
				pt = pt + sizeof(dns_rr);
				memcpy(&ret_ip, pt, 4);
				
				{
					struct in_addr ip_ans;
					ip_ans.s_addr = ret_ip;
					
					strcpy(&answer_ip[got_answered_count ++][0], inet_ntoa(ip_ans));
					print_console_func("get response %d: %s\n", answered_count, inet_ntoa(ip_ans));
					if (got_answered_count >= ANSWER_IP_MAX)
					{
						print_console_func("no enough space\n");
						break;	
					}
				}
				
				ret = 1;
				if (parsered_count >= answered_count)
				{
					print_console_func("get all response\n");
					break;	
				}
				
				pt += 4;
			}
			//break;
		}
		else
		{
			offset = ntohs(dns_rr.rr_length) + sizeof(dns_rr);
			if((i = i + offset) <= len)
			{
				/* Go to the next record */
				pt = pt + offset;
			}
		}
	}
	return ret;
}
#endif
/*
 * handle_udpreply()
 *
 * This function handles udp DNS requests by either replying to them (if we
 * know the correct reply via master, caching, etc.), or forwarding them to
 * an appropriate DNS server.
 *
 * Note that the mached query is prev->next and not prev.
 */
void udp_handle_reply(query_t *prev)
{
  //    const int          maxsize = 512; /* According to RFC 1035 */
    static char        msg[UDP_MAXSIZE+4];
    int                len;
    unsigned           addr_len;
    query_t *q = prev->next;
    int reply;
	int answerip_i = 0;
	int ret = 0;
	
    log_debug(3, "handling socket %i", q->sock);
    if ((len = reply_recv(q, msg, UDP_MAXSIZE)) < 0)
      {
	log_debug(1, "dnsrecv failed: %i", len);
	query_delete_next(prev);
	return; /* recv error */
      }

    /* do basic checking */
    if ((reply = check_reply(q->srv, msg, len, ((q->try_count+1) == ALL_SERVER_FAILED))) < 0) {
      log_debug(1, "check_reply failed");
#ifdef _RECURSION_
      q->recursion_count = 0;
#endif
      q->try_count++;
      if (q->try_count >= ALL_SERVER_FAILED) {
            query_delete_next(prev);
      }else {
            send2current(q,q->msg,q->msg_len);
    }
        return;
    }
#ifdef _RECURSION_
    else if ((reply == REPLY_REFERRAL)
             &&(q->client_flag & MASK_RD)) {
        char temp[128];
        
        if (q->recursion_count >= MAX_RECURSION_COUNT)
            goto delete;

        if (pickup_referral(msg, len,  temp, sizeof(temp))
            != PICKUP_OK)
            goto delete;

        if (!q->referral) /* no head */
            q->referral = init_srvlist();

        if (add_srv(q->referral, temp) == NULL)
            goto delete;

        if (send2referral(q, q->msg, q->msg_len) < 0)
            goto delete;
        
        return;
    delete:
        query_delete_next(prev);
        return;

    }
#endif

    if (opt_debug) {
	char buf[256];
	sprintf_cname(&msg[12], len-12, buf, 256);
	log_debug(3, "Received DNS reply for \"%s\"", buf);
    }

    dump_dnspacket("reply", msg, len);
    addr_len = sizeof(struct sockaddr_in);

    /* was this a dummy reactivate query? */
    if (q->domain != NULL) {
      /* no, lets cache the reply and send to client */
      cache_dnspacket(msg, len, q->srv);

      /* set the client qid */
      *((unsigned short *)msg) = q->client_qid;
      log_debug(3, "Forwarding the reply to the host %s",
		inet_ntoa(q->client.sin_addr));

	if (access("/tmp/debug_dnrd_console", F_OK) == 0)
	{
		print_console_func("dump response ip\n");
		memset(&answer_ip[0][0], 0, sizeof(answer_ip));
		get_answer_ip(&msg[12], len - 12, ntohs(((dnsheader_t *)msg)->ancount));
		for (answerip_i=0; answerip_i<ANSWER_IP_MAX; answerip_i++)
		{
			if (answer_ip[answerip_i][0])
			{
				print_console_func("dump response: %s\n", &answer_ip[answerip_i][0]);
			}
		}
		print_console_func("dump response ip done\n");
	}
	
#ifdef AMPED

	char buf[256]="";
	char *block_enable=nvram_safe_get("fw_block");
	if(len > 20 && *block_enable=='1')
	{
		sprintf_cname(&msg[12], len-12, buf, 256);
		char *type = nvram_safe_get("fw_block_keyword_type");
		char *pt= nvram_safe_get("fw_block_keyword");
		char *p=NULL;
		char keyword[64*6];
		char token=0x01;
		int i=0;
		struct in_addr ip_ans;
		char ip1[32] = "";
		
		memset(&answer_ip[0][0], 0, sizeof(answer_ip));
		//ip_ans.s_addr = get_answer_ip(&msg[12], len - 12, ntohs(((dnsheader_t *)msg)->ancount));
		ret = get_answer_ip(&msg[12], len - 12, ntohs(((dnsheader_t *)msg)->ancount));
		if(ret)
		{
			while(*pt!='\0')
			{
				i=0;
				while(*pt!=token && *pt!='\0')
					keyword[i++]=*pt++;
				keyword[i]='\0';

				if(*pt!='\0') pt++;

				if(strcmp(keyword,"w")==0 || strcmp(keyword,"ww")==0 || strcmp(keyword,"www")==0)
				{

					p=strstr(buf,"www.");
					if(p)
						p=p+4;
					else
						p=buf;
		
				}
				else
				{
					p=buf;
				}
				if(strstr(p,keyword))
				{
					if( *type == '1')//block sites on mac address
					{
						char *mac_list = nvram_safe_get("fw_block_keyword_mac_list");
						char mac[64]="";
						char filter[36]="";
						char word[1024];
						int i=0;
						strcpy(filter, "%*[^:]:%[^:]:%*s");
					    	get_filter_str(filter, ':', 0x02);
						while(*mac_list!='\0')
						{
							i=0;
							while(*mac_list!='\0' && *mac_list!=token)
							word[i++]=*mac_list++;
							word[i]='\0';
							if(*mac_list!=0) mac_list++;
							sscanf(word,filter,mac);
							
							for (answerip_i=0; answerip_i<ANSWER_IP_MAX; answerip_i++)
							{
                                                                if (answer_ip[answerip_i][0])
                                                                {
								SYSTEM("/usr/sbin/iptables -D KEY_BLOCK_HTTP -m mac --mac-source %s  -d %s -p tcp --dport 443 -j DROP",mac,&answer_ip[answerip_i][0]);
								SYSTEM("/usr/sbin/iptables -A KEY_BLOCK_HTTP -m mac --mac-source %s  -d %s -p tcp --dport 443 -j DROP",mac,&answer_ip[answerip_i][0]);
                                                                }
							}
						}
					}
					else//block sites on all users
					{
						for (answerip_i=0; answerip_i<ANSWER_IP_MAX; answerip_i++)
						{
                                                        if (answer_ip[answerip_i][0] )
                                                       {
							SYSTEM("/usr/sbin/iptables -D KEY_BLOCK_HTTP  -d %s -p tcp --dport 443 -j DROP",&answer_ip[answerip_i][0] );
							SYSTEM("/usr/sbin/iptables -A KEY_BLOCK_HTTP  -d %s -p tcp --dport 443 -j DROP",&answer_ip[answerip_i][0] );
                                                        }
						}
					}
				
				}
			}
		}
		
		
	}
#else
	char buf[256]="";
	char *state = NULL;
	char *block_enable=nvram_safe_get("fw_block");
	int group_sid = 1, group_eid, group_id;
	print_console_func("Bollen:%s------------1-------------\n", __FUNCTION__);
	if(len > 20)
	{
		sprintf_cname(&msg[12], len-12, buf, 256);

		if(strcmp(buf,"updates1.netgear.com")==0 || strcmp(buf,"http.updates1.netgear.com")==0)
		{
			state = nvram_safe_get("config_state");
			if( strcmp(state,"configured")==0 || strcmp(state,"blankstate")==0)
			{		
				/*
				char ip1[32]="",ip2[32]="";
				sprintf(ip1,"%d.%d.%d.%d",msg[len-20],msg[len-19],msg[len-18],msg[len-17]);
				sprintf(ip2,"%d.%d.%d.%d",msg[len-4],msg[len-3],msg[len-2],msg[len-1]);
							*/
				group_eid = ifgmap_get_if_num(IF_TYPE_GROUP);
#ifdef VLAN_IPTV
			    /* Now VLAN2-8 is only a bridge,needn't dnrd start them */
				group_eid = 1;
#endif			

#if defined(GUI_V14)
				struct in_addr ip_ans;
				char ip1[32] = "";
				//ip_ans.s_addr = get_answer_ip(&msg[12], len - 12, ntohs(((dnsheader_t *)msg)->ancount));
				memset(&answer_ip[0][0], 0, sizeof(answer_ip));
				ret = get_answer_ip(&msg[12], len - 12, ntohs(((dnsheader_t *)msg)->ancount));
				if(ret)
				{
					for (answerip_i=0; answerip_i<ANSWER_IP_MAX; answerip_i++)
					{
						memset(ip1, 0, sizeof(ip1));
						strcpy(ip1, &answer_ip[answerip_i][0]);
						
						if(ip1[0] && strcmp(ip1,"66.114.62.22")!=0 && 
							strcmp(ip1,"66.114.62.29")!=0 && 
							strcmp(ip1,"174.35.24.133")!=0 && 
							strcmp(ip1,"101.96.10.64")!=0)
						{
								for(group_id = group_sid; group_id <= group_eid; group_id ++)
							{
								{
									SYSTEM("/usr/sbin/iptables -t nat -D  PREROUTING -i group%d -d %s -p tcp --dport 80 -j RETURN", group_id,  ip1);
									SYSTEM("/usr/sbin/iptables -t nat -I  PREROUTING -i group%d -d %s -p tcp --dport 80 -j RETURN", group_id, ip1);
								}
							}
						}
					}
				}
#endif
			}
		
		}
		print_console_func("Bollen:%s------------2 block_enable:%s--buf:%s----------\n", __FUNCTION__,block_enable,buf);
		if(*block_enable=='1')
		{
			char *pt= nvram_safe_get("fw_https_block_keyword");
			char *p=NULL;
			char keyword[64*6];
			char token=0x01;
			int i=0;
			struct in_addr ip_ans;
			char ip2[32] = "";
			{
				while(*pt!='\0')
				{
					i=0;
					while(*pt!=token && *pt!='\0')
						keyword[i++]=*pt++;
					keyword[i]='\0';

					if(*pt!='\0') pt++;

					if(strcmp(keyword,"w")==0 || strcmp(keyword,"ww")==0 || strcmp(keyword,"www")==0)
					{

						p=strstr(buf,"www.");
						if(p)
							p=p+4;
						else
							p=buf;
			
					}
					else
					{
						p=buf;
					}
					print_console_func("%s---------3--p:%s, keyword:%s--------\n", __FUNCTION__, p,keyword);
					if(strstr(keyword, p))
					{
							memset(&answer_ip[0][0], 0, sizeof(answer_ip));
							ret = get_answer_ip(&msg[12], len - 12, ntohs(((dnsheader_t *)msg)->ancount));
							if (ret)
							{
								for (answerip_i=0; answerip_i<ANSWER_IP_MAX; answerip_i++)
								{
									memset(ip2, 0, sizeof(ip2));
									strcpy(ip2, &answer_ip[answerip_i][0]);
									if (ip2[0])
									{
										print_console_func("Bollen:%s--------4----ip:%s-----\n", __FUNCTION__,ip2);
										//SYSTEM("/usr/sbin/iptables -D KEY_BLOCK_HTTP  -d %s -p tcp --dport 443 -j DROP",ip2 );
										//SYSTEM("/usr/sbin/iptables -A KEY_BLOCK_HTTP  -d %s -p tcp --dport 443 -j DROP",ip2 );
										//SYSTEM("/usr/sbin/iptables -t nat -D PREROUTING  -d %s -p tcp --dport 443 -j DNAT --to-destination 192.168.1.1:443",ip2 );
										//SYSTEM("/usr/sbin/iptables -t nat -A PREROUTING  -d %s -p tcp --dport 443 -j DNAT --to-destination 192.168.1.1:443",ip2 );
										
										{
											SYSTEM("/usr/sbin/iptables -D KEY_BLOCK_HTTP  -d %s -p tcp --dport 443 -j BLOCK_HTTP", ip2);
											SYSTEM("/usr/sbin/iptables -A KEY_BLOCK_HTTP  -d %s -p tcp --dport 443 -j BLOCK_HTTP", ip2);
										}
									}
								}
							}
					}
				}
			}
		}//block_enable=='1'
	}
#endif
      if (sendto(isock, msg, len, 0,
		 (const struct sockaddr *)&q->client,
		 addr_len) != len) {
	log_debug(1, "sendto error %s", strerror(errno));
      }
	

    } else {
      log_debug(2, "We got a reactivation dummy reply. Cool!");
    }

    /* this server is obviously alive, we reset the counters */
    q->srv->send_time = 0;
    if (q->srv->inactive) log_debug(1, "Reactivating server %s",
				 inet_ntoa(q->srv->addr.sin_addr));
    q->srv->inactive = 0;
    /* remove query from list and destroy it */
    query_delete_next(prev);
}


/* send a dummy packet to a deactivated server to check if its back*/
int udp_send_dummy(srvnode_t *s) {
  static unsigned char dnsbuf[] = {
  /* HEADER */
    /* we send a lookup for localhost */
    /* will this work on a big endian system? */
    0x00, 0x00, /* ID */
    0x01, 0x00, /* QR|OC|AA|TC|RD -  RA|Z|RCODE  */
    0x00, 0x01, /* QDCOUNT */
    0x00, 0x00, /* ANCOUNT */
    0x00, 0x00, /* NSCOUNT */
    0x00, 0x00, /* ARCOUNT */

    9, 'l','o','c','a','l','h','o','s','t',0,  /* QNAME */
    0x00,0x01,   /* QTYPE A record */
    0x00,0x01   /* QCLASS: IN */

    /* in case you want to lookup root servers instead, use this: */
    /*    0x00,       /* QNAME:  empty */
    /*    0x00, 0x02, /* QTYPE:  a authorative name server */
    /*    0x00, 0x01  /* QCLASS: IN */
  };
  query_t *q;
  struct sockaddr_in srcaddr;

  /* should not happen */
  assert(s != NULL);

  if ((q=query_add(NULL, s, &srcaddr, dnsbuf, sizeof(dnsbuf))) != NULL) {
    int rc;
    q = q->next; /* query add returned the query 1 before in list */
    /* don't let those queries live too long */
    q->ttl = reactivate_interval;
    memset(&srcaddr, 0, sizeof(srcaddr));
    log_debug(2, "Sending dummy id=%i to %s", ((unsigned short *)dnsbuf)[0],
	      inet_ntoa(s->addr.sin_addr));
    /*  return dnssend(s, &dnsbuf, sizeof(dnsbuf)); */
    rc=udp_send(q->sock, s, dnsbuf, sizeof(dnsbuf));
    ((unsigned short *)dnsbuf)[0]++;
    return rc;
  }
  return -1;
}

#ifdef OPENDNS
//marc add for opendns(parental control)

#define DEVICE_ID_LEN 16
#define DEFAULT_DEVICE_ID "0000111111111111"
/*********************************
 * transform the the mac address from the formation AA:BB:CC:DD:EE:FF
 * assigned by mac_p to the formation AABBCCDDEEFF assigned by mac
 ********************************/
static int trans_macaddr(OUT char * mac, IN char *mac_p)
{
	if(!strcmp(mac_p, "") || 17 != strlen(mac_p))
		return (-1);

	*(mac+0)=*(mac_p+0);
	*(mac+1)=*(mac_p+1);
	*(mac+2)=*(mac_p+3);
	*(mac+3)=*(mac_p+4);
	*(mac+4)=*(mac_p+6);
	*(mac+5)=*(mac_p+7);
	*(mac+6)=*(mac_p+9);
	*(mac+7)=*(mac_p+10);
	*(mac+8)=*(mac_p+12);
	*(mac+9)=*(mac_p+13);
	*(mac+10)=*(mac_p+15);
	*(mac+11)=*(mac_p+16);
	*(mac+12)='\0';
	return 0;
}

/***********************************
 * get the mac address that is corresponding the specified ip address
 * from the arp buffer file "/proc/net/arp"
 **********************************/
static int get_macaddr_by_ip(OUT char *mac, IN char *ip)
{
	char buf[512];
	FILE *fp=NULL;
	char str_addr[20], str_type[20], str_flg[20], str_mac[20], str_mask[20], str_dev[20];
	int rc = 1;

	if(!(fp=fopen("/proc/net/arp", "r")))
		return (-1);

	while(fgets(buf, 512, fp))
	{
	      sscanf(buf, "%s %s %s %s %s %s", str_addr, str_type, str_flg, str_mac, str_mask, str_dev);
	      if(!strcasecmp(ip, str_addr))
	      {
	              strcpy(mac, str_mac);
				  rc = 0;
	              break;
	      }
	}
	if(fp)
	       fclose(fp);
	return rc;
}

static void get_deviceid_by_macaddr(IN char *macaddr, OUT char *deviceid)
{
	char default_deviceid[33]={0};
	//char *tab = nvram_get("PC_table"), *pt=NULL;
	//char *pt=NULL, *buf=NULL;
	char *pt=NULL;
	char buf[PC_TABLE_MAX_SIZE];

	strncpy(buf, PC_table,sizeof(buf));
	pt = strcasestr(buf, macaddr);
	if(pt)
	{
		memset(deviceid, 0, sizeof(deviceid));
		strncpy(deviceid, pt+strlen(macaddr)+1, DEVICE_ID_LEN);
	}
	pt = strstr(buf, "default");
	if(pt)
	{
		memset(default_deviceid, 0, sizeof(deviceid));
		strncpy(default_deviceid, pt+strlen("default")+1, DEVICE_ID_LEN);
	}
	//free(buf);
	if(strcmp(deviceid, ""))
		return ;
	if(strcmp(default_deviceid, ""))
	{
		strcpy(deviceid, default_deviceid);
		return;
	}
	strcpy(deviceid, DEFAULT_DEVICE_ID);   
}

/***********************************
 * transform the string of deviceid to the hexadecimal formation.
 * for instance, transform the string "1234567890abcdef" to the
 * hexadecimal digit 0x12 0x34 0x56 0x78 0x90 0xAB 0xCD 0xEF, and
 * copy this hex digit to the memory assigned by deviceid_b, then
 * the string have changed to binay code.
 **********************************/
static int trans_deviceid(IN char *deviceid, OUT char *deviceid_b)
{
	int i=0;
	if(DEVICE_ID_LEN != strlen(deviceid))
		return (-1);
	if(!strncmp(deviceid, DEFAULT_DEVICE_ID, DEVICE_ID_LEN))
		return 0;
	for(i=0; i<DEVICE_ID_LEN; i++)
	{
		switch (*(deviceid+i))
		{
			case '0':
				*(deviceid+i)=0x00;
				break;
			case '1':
				*(deviceid+i)=0x01;
				break;
			case '2':
				*(deviceid+i)=0x02;
				break;
			case '3':
				*(deviceid+i)=0x03;
				break;
			case '4':
				*(deviceid+i)=0x04;
				break;
			case '5':
				*(deviceid+i)=0x05;
				break;
			case '6':
				*(deviceid+i)=0x06;
				break;
			case '7':
				*(deviceid+i)=0x07;
				break;
			case '8':
				*(deviceid+i)=0x08;
				break;
			case '9':
				*(deviceid+i)=0x09;
				break;
			case 'a':
			case 'A':
				*(deviceid+i)=0x0A;
				break;
			case 'b':
			case 'B':
				*(deviceid+i)=0x0B;
				break;
			case 'c':
			case 'C':
				*(deviceid+i)=0x0C;
				break;
			case 'd':
			case 'D':
				*(deviceid+i)=0x0D;
				break;
			case 'e':
			case 'E':
				*(deviceid+i)=0x0E;
				break;
			case 'f':
			case 'F':
				*(deviceid+i)=0x0F;
				break;
			default :
				return (-1);
		}
	}
	for(i=0; i < 8; i++)
		*(deviceid_b+i)=((*(deviceid+i*2+0))<<4)|(*(deviceid+i*2+1));
	return 0;
}
#endif

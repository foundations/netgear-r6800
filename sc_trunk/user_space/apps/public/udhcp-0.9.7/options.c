/*
 * options.c -- DHCP server option packet tools
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "debug.h"
#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "leases.h"
#include "filenames.h"
#include "dhcpc.h"
#define ONE_OPTION_MAX_LEN 255
int route_count=0;       /* How many routes we have got in option 33 and 121 */
/* supported options are easily added here */
struct dhcp_option options[] = {
	/* name[10]	flags					code */
	{"subnet",	OPTION_IP | OPTION_REQ,			0x01},
	{"timezone",	OPTION_S32,				0x02},
	/* RFC 3443
		The Classless Static Routes option code MUST appear
		in the parameter request list prior to both the Router option
		code and the Static Routes option code, if present.
	*/
	{"clroute",OPTION_REQ |OPTION_LIST|OPTION_CLROUTE,   0x79},
	{"msclroute",OPTION_REQ |OPTION_LIST|OPTION_CLROUTE,   0xF9},
	{"router",	OPTION_IP | OPTION_LIST | OPTION_REQ,	0x03},
	{"timesvr",	OPTION_IP | OPTION_LIST,		0x04},
	{"namesvr",	OPTION_IP | OPTION_LIST,		0x05},
	{"dns",		OPTION_IP | OPTION_LIST | OPTION_REQ,	0x06},
	{"logsvr",	OPTION_IP | OPTION_LIST,		0x07},
	{"cookiesvr",	OPTION_IP | OPTION_LIST,		0x08},
	{"lprsvr",	OPTION_IP | OPTION_LIST,		0x09},
	{"hostname",	OPTION_STRING | OPTION_REQ,		0x0c},
	{"bootsize",	OPTION_U16,				0x0d},
	{"domain",	OPTION_STRING | OPTION_REQ,		0x0f},
	{"swapsvr",	OPTION_IP,				0x10},
	{"rootpath",	OPTION_STRING,				0x11},
	{"ipttl",	OPTION_U8,				0x17},
	{"mtu",		OPTION_U16,				0x1a},
	{"broadcast",	OPTION_IP | OPTION_REQ,			0x1c},
	{"ntpsrv",	OPTION_IP | OPTION_LIST,		0x2a},
	{"wins",	OPTION_IP | OPTION_LIST,		0x2c},
	{"requestip",	OPTION_IP,				0x32},
	{"lease",	OPTION_U32,				0x33},
	{"dhcptype",	OPTION_U8,				0x35},
	{"serverid",	OPTION_IP,				0x36},
	{"message",	OPTION_STRING,				0x38},
	{"tftp",	OPTION_STRING,				0x42},
	{"bootfile",	OPTION_STRING,				0x43},
#ifdef IPV6
    {"ip6_6rd", OPTION_LIST | OPTION_REQ,               0xd4},
#endif
    /* netgear Spec1.9
        option  33(0x21)    static route
        option 121(0x79)    classless route
    */
	{"stroute",OPTION_IP_PAIR | OPTION_LIST| OPTION_REQ, 0x21},
	{"",	    0x00,				0x00}
};

/* Lengths of the different option types */
int option_lengths[] = {
	[OPTION_IP] =		4,
	[OPTION_IP_PAIR] =	8,
	[OPTION_BOOLEAN] =	1,
	[OPTION_STRING] =	1,
	[OPTION_U8] =		1,
	[OPTION_U16] =		2,
	[OPTION_S16] =		2,
	[OPTION_U32] =		4,
	[OPTION_S32] =		4,
    [OPTION_CLROUTE] =  1,       /* dynamic length */
};


static char* get_net_addr( char* ip, char* mask)
{
        struct in_addr lip , lmask, subnet;

        inet_aton( ip, & lip);
        inet_aton( mask, & lmask) ;
        subnet.s_addr = ( lip.s_addr & lmask.s_addr);

        return (inet_ntoa( subnet));
}

static int get_net_prefix(  char* mask)
{
        struct in_addr  lmask;
        unsigned long ulmask;
        int prefix_len = 0;

        inet_aton( mask, & lmask) ;

        ulmask = ntohl(lmask.s_addr);

        for( prefix_len =0; prefix_len < 32 ; prefix_len ++)
        {
                if( ! (ulmask << prefix_len) )
                        return prefix_len;
        }

        return prefix_len;
}


/* get an option with bounds checking (warning, not aligned). */
unsigned char *get_option(struct dhcpMessage *packet, int code)
{
	int i, length;
	unsigned char *optionptr;
	int over = 0, done = 0, curr = OPTION_FIELD;

	optionptr = packet->options;
	i = 0;
	length = OPTION_LEN;
	while (!done) {
		if (i >= length) {
			LOG(LOG_WARNING, "bogus packet, option fields too long.");
			return NULL;
		}
		if (optionptr[i + OPT_CODE] == code) {
			if (i + 1 + optionptr[i + OPT_LEN] >= length) {
				LOG(LOG_WARNING, "bogus packet, option fields too long.");
				return NULL;
			}
			return optionptr + i + 2;
		}
		switch (optionptr[i + OPT_CODE]) {
		case DHCP_PADDING:
			i++;
			break;
		case DHCP_OPTION_OVER:
			if (i + 1 + optionptr[i + OPT_LEN] >= length) {
				LOG(LOG_WARNING, "bogus packet, option fields too long.");
				return NULL;
			}
			over = optionptr[i + 3];
			i += optionptr[OPT_LEN] + 2;
			break;
		case DHCP_END:
			if (curr == OPTION_FIELD && over & FILE_FIELD) {
				optionptr = packet->file;
				i = 0;
				length = 128;
				curr = FILE_FIELD;
			} else if (curr == FILE_FIELD && over & SNAME_FIELD) {
				optionptr = packet->sname;
				i = 0;
				length = 64;
				curr = SNAME_FIELD;
			} else done = 1;
			break;
		default:
			i += optionptr[OPT_LEN + i] + 2;
		}
	}
	return NULL;
}


/* return the position of the 'end' option (no bounds checking) */
int end_option(unsigned char *optionptr)
{
	int i = 0;

	while (optionptr[i] != DHCP_END) {
		if (optionptr[i] == DHCP_PADDING) i++;
		else i += optionptr[i + OPT_LEN] + 2;
	}
	return i;
}


/* add an option string to the options (an option string contains an option code,
 * length, then data) */
int add_option_string(unsigned char *optionptr, unsigned char *string)
{
	int end = end_option(optionptr);

	/* end position + string length + option code/length + end option */
	if (end + string[OPT_LEN] + 2 + 1 >= OPTION_LEN) {
		LOG(LOG_ERR, "Option 0x%02x did not fit into the packet!", string[OPT_CODE]);
		return 0;
	}
	DEBUG(LOG_INFO, "adding option 0x%02x", string[OPT_CODE]);
	memcpy(optionptr + end, string, string[OPT_LEN] + 2);
	optionptr[end + string[OPT_LEN] + 2] = DHCP_END;
	return string[OPT_LEN] + 2;
}


/* add a one to four byte option to a packet */
int add_simple_option(unsigned char *optionptr, unsigned char code, u_int32_t data)
{
	char length = 0;
	int i;
	unsigned char option[2 + 4];
	unsigned char *u8;
	u_int16_t *u16;
	u_int32_t *u32;
	u_int32_t aligned;

	u8 = (unsigned char *) &aligned;
	u16 = (u_int16_t *) u8;
	u32 = &aligned;

	for (i = 0; options[i].code; i++)
		if (options[i].code == code) {
			length = option_lengths[options[i].flags & TYPE_MASK];
		}

	if (!length) {
		DEBUG(LOG_ERR, "Could not add option 0x%02x", code);
		return 0;
	}

	option[OPT_CODE] = code;
	option[OPT_LEN] = length;

	switch (length) {
		case 1: *u8 =  data; break;
		case 2: *u16 = data; break;
		case 4: *u32 = data; break;
	}
	memcpy(option + 2, &aligned, length);
	return add_option_string(optionptr, option);
}


/* find option 'code' in opt_list */
struct option_set *find_option(struct option_set *opt_list, char code)
{
	while (opt_list && opt_list->data[OPT_CODE] < code)
		opt_list = opt_list->next;

	if (opt_list && opt_list->data[OPT_CODE] == code) return opt_list;
	else return NULL;
}


/* add an option to the opt_list */
void attach_option(struct option_set **opt_list, struct dhcp_option *option, char *buffer, int length)
{
	struct option_set *existing, *new, **curr;

	/* add it to an existing option */
	if ((existing = find_option(*opt_list, option->code))) {
		DEBUG(LOG_INFO, "Attaching option %s to existing member of list", option->name);
		if (option->flags & OPTION_LIST) {
			if (existing->data[OPT_LEN] + length <= 255) {
				existing->data = realloc(existing->data,
						existing->data[OPT_LEN] + length + 2);
				memcpy(existing->data + existing->data[OPT_LEN] + 2, buffer, length);
				existing->data[OPT_LEN] += length;
			} /* else, ignore the data, we could put this in a second option in the future */
		} /* else, ignore the new data */
	} else {
		DEBUG(LOG_INFO, "Attaching option %s to list", option->name);

		/* make a new option */
		new = malloc(sizeof(struct option_set));
		new->data = malloc(length + 2);
		new->data[OPT_CODE] = option->code;
		new->data[OPT_LEN] = length;
		memcpy(new->data + 2, buffer, length);

		curr = opt_list;
		while (*curr && (*curr)->data[OPT_CODE] < option->code)
			curr = &(*curr)->next;

		new->next = *curr;
		*curr = new;
	}
}

/* routing table */
typedef struct route_table_s{
	char dest[16];
	char gateway[16];
	char mask[16];
	int metric;
	char active[4];
	char interface[8];
	struct route_table_s *next;
} route_table_t;

#define BUFSZ PIPE_BUF
/*

Return value:
        0: ok
        -1: error
*/
int myPipe(char *command, char **output)
{
  FILE *fp;
  char buf[BUFSZ];
  int len;

  *output=malloc(1);
  strcpy(*output, "");
  if((fp=popen(command, "r"))==NULL)
     return -1;
  while((fgets(buf, BUFSZ, fp)) != NULL){
     len=strlen(*output)+strlen(buf);
     if((*output=realloc(*output, (sizeof(char) * (len+1))))==NULL)
	return -1;
      strcat(*output, buf);
  }
  pclose(fp);
  return 0;
}

#if 0
/* single route table. */
int do_route(route_table_t **route)
{
	char *output = NULL;
	char *pt = NULL;
	myPipe("/sbin/route -n",&output);
	pt=strstr(output,"Iface");

	if(!pt){
		*route=NULL;
	}else{
		char tmp[16];
		int len=strlen(output);

		route_table_t *now = NULL, *pre = NULL;

		while(*(pt++)!='\n');

		*route=malloc(sizeof(route_table_t));

		for(now=*route;(pt-output)<len;now=now->next){
		  pre=now;
		  sscanf(pt,"%s %s %s %s %d %s %s %s"
			 ,now->dest
			 ,now->gateway
			 ,now->mask
			 ,tmp
			 ,&now->metric
			 ,tmp
			 ,now->active
			 ,now->interface);
		  while(*(pt++)!='\n');
		  now->next=malloc(sizeof(route_table_t));
		}

		(now==*route)?(*route=NULL):(pre->next=NULL);
		free(now);
	}

	free(output);
	return 0;
}

int check_route_exist(char *dest, char *gw, char *mask, int metric, int invert)
{
	int ret = 0;
	route_table_t *now,*route_table,new_route;

    memset(&new_route, 0, sizeof(new_route));
    sprintf(new_route.dest, "%s", dest);
	sprintf(new_route.gateway, "%s", gw);
	sprintf(new_route.mask, "%s", mask);
	new_route.metric = metric;

	do_route(&route_table);

	for(now=route_table;now;now=now->next)
	{
		if(strcmp(new_route.dest, now->dest) == 0
		   && strcmp(new_route.gateway, now->gateway) == 0
		   && strcmp(new_route.mask, now->mask) == 0
		   && (invert?(metric != now->metric):(metric == now->metric)))
		{
			ret = 1;
			break;
		}
	}
	free(route_table);
	return ret;
}
#else
static unsigned int bits2mask(int mask_bit, char *str_mask)
{
	unsigned char mask[5];
	int j = 0, k = 0;
	int i = mask_bit>>3;

	memset(mask, 0, sizeof(mask));
	for(j=0;j<i;j++)
		mask[j]=0xff;
	k = j;
	i = mask_bit-(mask_bit>>3<<3);
	j = 7;
	while(i > 0)
	{
		mask[k] |= (1<<j);
		i--;
		j--;
	}
	sprintf(str_mask, "%u.%u.%u.%u", mask[0], mask[1], mask[2], mask[3]);
	return 0;
}
static void free_route_table(route_table_t *route_table)
{
    route_table_t *t, *n;

    for (t = route_table; t; t = n)
    {
        n = t->next;
        free(t);
    }
}
/* multiple route table. */
int do_route(route_table_t **route, int table_id)
{
	char *output = NULL;
	char *pt = NULL, *pt1 = NULL;
	route_table_t *now = NULL, *head = NULL, *last = NULL;
	char cmd_buf[100], metric[3];
	int i = 0; 
	if(table_id <= 0)
		sprintf(cmd_buf, "/usr/sbin/ip rout show");
	else
		sprintf(cmd_buf, "/usr/sbin/ip rout show table %d", table_id);
	myPipe(cmd_buf,&output);
	pt = output;

	while(pt && *pt!='\0')
	{
		if(!(pt1=strchr(pt, '\n')))
			break;
		*pt1 = '\0';
		now = malloc(sizeof(route_table_t));
		if(!now)
			continue;
		memset(now, 0, sizeof(route_table_t));
		strcpy(now->dest, "0.0.0.0");
		strcpy(now->mask, "255.255.255.255");
		strcpy(now->gateway, "0.0.0.0");
		strcpy(now->active, "Yes");
		now->metric = 0;
		now->next = NULL;

		pt1 = pt;
		if(pt1 && *pt1 != '\0' && isdigit(*pt1))
		{
			i = 0;
			while(pt1 && *pt1 != '\0' && *pt1 != ' ' && *pt1 != '/')
			{
				now->dest[i++] = *pt1;
				pt1++;
			}
			if(pt1 && *pt1 == '/')
			{
				pt1++;
				sscanf(pt1, "%d ", &i);
				bits2mask(i, now->mask);
			}
		}
		if((pt1 = strstr(pt, " via ")))
		{
			i = 0;
			pt1 = pt1 +sizeof(" via ")-1;
			if (strstr(pt, "default"))
			{
				strcpy(now->mask, "0.0.0.0");
			}
			while(pt1 && *pt1 != '\0' && *pt1 != ' ')
			{
				now->gateway[i++] = *pt1;
				pt1++;
			}
		}
		if((pt1 = strstr(pt, " dev ")))
		{
			i = 0;
			pt1 = pt1 +sizeof(" dev ")-1;
			while(pt1 && *pt1 != '\0' && *pt1 != ' ')
			{
				now->interface[i++] = *pt1;
				pt1++;
			}
		}
		if((pt1 = strstr(pt, " metric ")))
		{
			i = 0;
			memset(metric, 0, sizeof(metric));
			pt1 = pt1 +sizeof(" metric ")-1;
			sscanf(pt1, "%d", &now->metric);
		}
		pt1 = pt;
		while(pt && *pt != '\0')
			pt++;
		pt++;
		if(strstr(pt1, "blackhole "))
		{
			free(now);
			continue;
		}
		if(head == NULL){/*first one*/
			head = now;
			last = now;
		}
		else{
			last->next = now;
			last = now;
		}
	}
	*route = head;
	free(output);
	return 0;
}

int check_route_exist(char *dest, char *gw, char *mask, int metric, int invert, int table_id)
{
	int ret = 0;
	route_table_t *now,*route_table = NULL,new_route;

    memset(&new_route, 0, sizeof(new_route));
    sprintf(new_route.dest, "%s", dest);
	sprintf(new_route.gateway, "%s", gw);
	sprintf(new_route.mask, "%s", mask);
	new_route.metric = metric;

	do_route(&route_table, table_id);

	for(now=route_table;now;now=now->next)
	{
		if(strcmp(new_route.dest, now->dest) == 0
		   && strcmp(new_route.gateway, now->gateway) == 0
		   && strcmp(new_route.mask, now->mask) == 0
		   && (invert?(metric != now->metric):(metric == now->metric)))
		{
			ret = 1;
			break;
		}
	}
	free_route_table(route_table);
	return ret;
}

#endif

/* option33

    Code   Len         Destination 1           Router 1
    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
    |  33 |  n  |  d1 |  d2 |  d3 |  d4 |  r1 |  r2 |  r3 |  r4 |
    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
    Destination 2           Router 2
    +-----+-----+-----+-----+-----+-----+-----+-----+---
    |  d1 |  d2 |  d3 |  d4 |  r1 |  r2 |  r3 |  r4 | ...
    +-----+-----+-----+-----+-----+-----+-----+-----+---
 */
void sc_option33(struct dhcpMessage *packet)
{
    const int RECORD_LEN = 8;
    int  n = 0, i = 0;
    unsigned char *temp;
    char cmdbuf[256], dest_buf[64], gw_buf[64];
    struct in_addr dest, gw;
	int	long_option_len;

    if (packet == NULL)
        return;
   memset(&dest,0,sizeof(struct in_addr));
    memset(&gw,0,sizeof(struct in_addr));

    if ((temp = get_option(packet, DHCP_STATIC_ROUTE)) != NULL) {
    	/*If one option lenght > 255,server will separate the overflow bytes
    	  to another option 250.Luckly,we just need handle option 32 here.Pay
    	  attention when you add a option support if it may larger than 255 bytes
    	  For ex.option 32 len 256 separate to opt 32 with len 255 and opt 250 with
    	  len 1 */
    	  SC_CFPRINTF("got option 32\n");
    	if(temp[OPT_LEN -2] == ONE_OPTION_MAX_LEN)/*len is 255*/
    	{
    		SC_CFPRINTF("option 32 is over flow\n");
    		if(temp[ONE_OPTION_MAX_LEN] == DHCP_LONG_OPTION)/*next is option 250*/
    		{
    			SC_CFPRINTF("next is option 250\n");
    			long_option_len = temp[ONE_OPTION_MAX_LEN + OPT_LEN];
    			n = (temp[OPT_LEN -2] + long_option_len)/RECORD_LEN;
    		}
    		else
    		{
    			SC_CFPRINTF("impossible,next is not option 250,but option 32 is > 255\n");
    			return;
    		}
    	}
    	else
        {
        	n = temp[OPT_LEN -2] / RECORD_LEN;
        }
    }
    else/*no this option,should return*/
    {
    	return;
    }
/* when add option 33, we should also care whether we have already add 32 routes. */
    for(i = 0; (i < n) && (route_count < 32); i++) {
        /*  RFC2132: If multiple routes to the same destination are specified,
            they are listed in descending order of priority.

            So no matter there is a same destination, we should always add the
            static route. If we get a totally same static route that has existed,
            the "route add" command will find it and ignore it.        --- Joe
         */
        int ret = 0;

        if( (n <= ONE_OPTION_MAX_LEN / RECORD_LEN))/*haven't option 250*/
        {
        	dest.s_addr = *((uint32_t *)temp);
        	gw.s_addr = *((uint32_t *)(temp + 4));
        }
        else/*has option 250*/
        {
        	if(i == ONE_OPTION_MAX_LEN / RECORD_LEN)/*the 32th routing,separated one,i from 0*/
        	{
        		dest.s_addr = *((uint32_t *)temp);
        		/*skip option 250's code and len*/
        		gw.s_addr =	(temp[4]<<24) + (temp[5]<<16) + (temp[6]<<8) + (temp[9]);
        		temp += 2;
        		gw.s_addr = htonl(gw.s_addr);/*input network order for inet_ntoa*/
        	}
        	/* when i == ONE_OPTION_MAX_LEN / RECORD_LEN, it is the 32th routing,
          so we should not add the ones after it.  */
        	else if(i < ONE_OPTION_MAX_LEN / RECORD_LEN)
        	{
        		dest.s_addr = *((uint32_t *)temp);
        		gw.s_addr = *((uint32_t *)(temp + 4));
        	}
        }
        sprintf(dest_buf, "%s", inet_ntoa(dest));
        sprintf(gw_buf, "%s", inet_ntoa(gw));
		/* Added by static route ? */
		if(!check_route_exist(dest_buf, gw_buf, "255.255.255.255", 0, 1, client_config.rt))
		{
		    /* add route */
        	sprintf(cmdbuf, "/usr/sbin/ip route add to %s/32 via %s table %d\n", dest_buf, gw_buf,client_config.rt);
        	ret = system(cmdbuf);

            if (ret == 0)
            {
			    /* back up for del */
        	    sprintf(cmdbuf, "/bin/echo '/usr/sbin/ip route del to %s/32 via %s table %d' >> %s%s", dest_buf, gw_buf,client_config.rt , DHCPC_ROUTE_RELATED_OPTIONS_DEL,client_config.w);
        	    system(cmdbuf);
        	}
        }
        else
        {
            /* ignore it silently. */
            ret = 0;
        }
        temp += RECORD_LEN;
        if (ret == 0)
            route_count++;
        SC_CFPRINTF("route_count = %d\n", route_count);
    }
    return;
}
/*
    RFC3442
    if the DHCP server returns both a Classless Static Routes option and a Static Routes
    option, the DHCP client MUST ignore the Static Routes option.

    Code Len Destination 1    Router 1
    +-----+---+----+-----+----+----+----+----+----+
    | 121 | n | d1 | ... | dN | r1 | r2 | r3 | r4 |
    +-----+---+----+-----+----+----+----+----+----+
    Destination 2       Router 2
    +----+-----+----+----+----+----+----+
    | d1 | ... | dN | r1 | r2 | r3 | r4 |
    +----+-----+----+----+----+----+----+

    return
        0 received Classless Static Routes
 */
int sc_option_route_process(struct dhcpMessage *packet, int code)
{
    unsigned char *temp_op;
    unsigned char *temp;
    int len = 0;
    int mask_bitn, dest_n;
    struct in_addr dest;
    struct in_addr netmask;
    struct in_addr gw;
    int i;
    char cmdbuf[256], dest_buf[64], netmask_buf[64], gw_buf[64];
    unsigned char long_op_buf[512];
    int	long_option_len;

    if (packet == NULL)
        return -1;

    if ((temp_op = get_option(packet, code)) != NULL)
    {
    	/*If one option lenght > 255,server will separate the overflow bytes
    	  to another option 250.Luckly,we just need handle option 32 here.Pay
    	  attention when you add a option support if it may larger than 255 bytes
    	  For ex.option 32 len 256 separate to opt 32 with len 255 and opt 250 with
    	  len 1 */
    	SC_CFPRINTF("got option %d\n", code);
    	if(temp_op[OPT_LEN -2] == ONE_OPTION_MAX_LEN)/*len is 255*/
    	{
    		SC_CFPRINTF("option %d is over flow\n", code);
    		if(temp_op[ONE_OPTION_MAX_LEN] == DHCP_LONG_OPTION)/*next is option 250*/
    		{
    			long_option_len = temp_op[ONE_OPTION_MAX_LEN + OPT_LEN];
    			SC_CFPRINTF("next is option 250 with length %d\n", long_option_len);
    			/*merge opt 121 and opt 250 following to one buf*/
    			memcpy(long_op_buf, temp_op, ONE_OPTION_MAX_LEN);
    			memcpy(long_op_buf + ONE_OPTION_MAX_LEN, temp_op + ONE_OPTION_MAX_LEN
    								+ OPT_LEN + 1, long_option_len);
    			len = ONE_OPTION_MAX_LEN + long_option_len;
    			temp = long_op_buf;
    		}
    		else
    		{
    			SC_CFPRINTF("impossible,next is not option 250,but option 32 is > 255\n");
    			return -1;
    		}
    	}
    	else
    	{
        	len = temp_op[OPT_LEN -2];
        	temp = temp_op;
        	SC_CFPRINTF("option %d length = %d\n", code,len);
		}
    }
    else
    {
        return -1;
	}
/* we should only add 32 routes, so when route_count >31, we should ignore. */
    while(len > 0 && route_count < 32){
        int ret = 0;

        mask_bitn = *(temp++);
        /* The net mask must not greater than 32 */
        /* destination net and netmask */
        if(mask_bitn > 32)
            break;
        dest_n = mask_bitn / 8;
        if (mask_bitn % 8)
            dest_n++;

        if((len = len - (1 + dest_n + 4)) < 0)
            break;

        /* netmask.  default is 255.255.255.255 */
        netmask.s_addr=0xFFFFFFFF;
        for (i = 0; i < (32 - mask_bitn); i++){
            netmask.s_addr = netmask.s_addr << 1;
        }
        netmask.s_addr = htonl(netmask.s_addr);

        dest.s_addr = *((uint32_t *)temp);
        dest.s_addr = dest.s_addr & netmask.s_addr;
        temp += dest_n;

        /* route gateway */
        gw.s_addr = *((uint32_t *)temp);
        temp += 4;

        /* add route, metric = ? */
        sprintf(dest_buf, "%s", inet_ntoa(dest));
        sprintf(netmask_buf, "%s", inet_ntoa(netmask));
        sprintf(gw_buf, "%s", inet_ntoa(gw));
		/* Added by static route ? */
		if(!check_route_exist(dest_buf, gw_buf, netmask_buf, 0, 1, client_config.rt))
		{
	        char buf[32];

			sprintf(buf, "%s/%d"
			        , get_net_addr(dest_buf,netmask_buf)
			        , get_net_prefix(netmask_buf));

        	/* add route */
       		sprintf(cmdbuf, "/usr/sbin/ip route add to %s via %s table %d\n",
                        buf, gw_buf, client_config.rt);
        	ret = system(cmdbuf);

        	if (ret == 0)
        	{
        	    /* back up for del */
       		    sprintf(cmdbuf, "/bin/echo '/usr/sbin/ip route del to %s via %s table %d' >> %s%s",
                        buf, gw_buf, client_config.rt, DHCPC_ROUTE_RELATED_OPTIONS_DEL,client_config.w);
        	    system(cmdbuf);
        	}
        }
        else
        {
            /* ignore it silently. */
            ret = 0;
        }
        if (ret == 0)
            route_count++;
		SC_CFPRINTF("route_count = %d \n", route_count);
    }
    return 0;
}


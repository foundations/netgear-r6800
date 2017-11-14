/* script.c
 *
 * Functions to call the DHCP client notification scripts
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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <assert.h>

#include "options.h"
#include "dhcpd.h"
#include "dhcpc.h"
#include "packet.h"
#include "options.h"
#include "debug.h"
#include "filenames.h"
#include "nvram.h"

/* get a rough idea of how long an option will be (rounding up...) */
static int max_option_length[] = {
	[OPTION_IP] =		sizeof("255.255.255.255 "),
	[OPTION_IP_PAIR] =	sizeof("255.255.255.255 ") * 2,
	[OPTION_STRING] =	1,
	[OPTION_BOOLEAN] =	sizeof("yes "),
	[OPTION_U8] =		sizeof("255 "),
	[OPTION_U16] =		sizeof("65535 "),
	[OPTION_S16] =		sizeof("-32768 "),
	[OPTION_U32] =		sizeof("4294967295 "),
	[OPTION_S32] =		sizeof("-2147483684 "),
    [OPTION_CLROUTE] =  1,
};


static int upper_length(int length, struct dhcp_option *option)
{
	return max_option_length[option->flags & TYPE_MASK] *
	       (length / option_lengths[option->flags & TYPE_MASK]);
}


static int sprintip(char *dest, char *pre, unsigned char *ip) {
	return sprintf(dest, "%s%d.%d.%d.%d ", pre, ip[0], ip[1], ip[2], ip[3]);
}

/*
 * Netgear bing said that we need at most 3 DNS address and ignore the 4th DNS.
 */
#define MAX_DNS_NUM 3
/* Fill dest with the text of option 'option'. */
static void fill_options(char *dest, unsigned char *option, struct dhcp_option *type_p)
{
	int type, optlen;
	u_int16_t val_u16;
	int16_t val_s16;
	u_int32_t val_u32;
	int32_t val_s32;
	int len = option[OPT_LEN - 2];
    int num=0;

	dest += sprintf(dest, "%s=", type_p->name);

	type = type_p->flags & TYPE_MASK;
	optlen = option_lengths[type];
	for(;;) {
		switch (type) {
		case OPTION_IP_PAIR:
			dest += sprintip(dest, "", option);
			*(dest-1) = '/';
			dest += sprintip(dest, "", option+4);
            break;
		case OPTION_IP:	/* Works regardless of host byte order. */
            if(type_p->code == 0x06) {
                if(num < MAX_DNS_NUM) {
                    /* Add DNS env only if we do not have more than MAX_DNS_NUM DNS */
                    dest += sprintip(dest, "", option);
                    num++;
                }
            }
            else {
			    dest += sprintip(dest, "", option);
            }
 			break;
		case OPTION_BOOLEAN:
			dest += sprintf(dest, *option ? "yes " : "no ");
			break;
		case OPTION_U8:
			dest += sprintf(dest, "%u ", *option);
			break;
		case OPTION_U16:
			memcpy(&val_u16, option, 2);
			dest += sprintf(dest, "%u ", ntohs(val_u16));
			break;
		case OPTION_S16:
			memcpy(&val_s16, option, 2);
			dest += sprintf(dest, "%d ", ntohs(val_s16));
			break;
		case OPTION_U32:
			memcpy(&val_u32, option, 4);
			dest += sprintf(dest, "%lu ", (unsigned long) ntohl(val_u32));
			break;
		case OPTION_S32:
			memcpy(&val_s32, option, 4);
			dest += sprintf(dest, "%ld ", (long) ntohl(val_s32));
			break;
		case OPTION_STRING:
			memcpy(dest, option, len);
			dest[len] = '\0';
			return;	 /* Short circuit this case */
		}
		option += optlen;
		len -= optlen;
		if (len <= 0) break;
	}
}

#ifdef IPV6
static char *int_to_str(int value)
{
	static char str[100];	
	sprintf(str, "%d", value);
	return str;
}
int is_6rd_config_null(char *ipv6prefix, char *ipv6prefixlen, char *ipv4masklen, char *boarderip)
{
	int is_null = 1;
	if (ipv6prefix && ipv6prefixlen && ipv4masklen && boarderip && 
	ipv6prefix[0] && ipv6prefixlen[0] && ipv4masklen[0] && boarderip[0])
	{
		int v6prefixlen = atoi(ipv6prefixlen);		
		int v4masklen = atoi(ipv4masklen);
		
		if (v4masklen >= 0 && v4masklen < 32 && v6prefixlen > 0 && v6prefixlen < 128 &&
			((32-v4masklen) + v6prefixlen)<=128)
		{
			int v6strlen = strlen(ipv6prefix);
			int v4strlen = strlen(boarderip);
			if (v6strlen && v4strlen)
			{
				if ((boarderip[0] > '0') && (ipv6prefix[0] > '0'))
				{
					struct in_addr addr;
					if (inet_pton(AF_INET, boarderip, &addr) != 1) 
					{
						SC_CFPRINTF("invalid boarderip <%s>\n", boarderip);
					} else
					{
						struct in6_addr sin6;
						if (inet_pton(AF_INET6, ipv6prefix, &sin6) != 1) 
						{
							SC_CFPRINTF("invalid ipv6prefix <%s>\n", ipv6prefix);
						} else
						{
							is_null = 0;
						}
					}
				}
			}
		}
	}
	SC_CFPRINTF("current 6rd configuration %s OK: ipv6prefix %s, ipv6prefixlen %s, ipv4masklen %s, boarderip %s\n", 
				is_null?"NOT":"", 
				ipv6prefix, ipv6prefixlen, ipv4masklen, boarderip);
	return is_null;
}
#endif
 /* 
  * 6rd DHCPv4 Option
  *
  * 0                   1                   2                   3
  * 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  * |  OPTION_6RD   | option-length |  IPv4MaskLen  |  6rdPrefixLen |
  * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  * |                                                               |
  * |                           6rdPrefix                           |
  * |                          (16 octets)                          |
  * |                                                               |
  * |                                                               |
  * |                                                               |
  * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  * |                     6rdBRIPv4Address(es)                      |
  * .                                                               .
  * .                                                               .
  * .                                                               .
  * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  * option-code         OPTION_6RD (212)
  * option-length       The length of the DHCP option in octets (22
  *                     octets with one BR IPv4 address).
  * IPv4MaskLen         The number of high-order bits that are identical
  *                     across all CE IPv4 addresses within a given 6rd
  *                     domain.  This may be any value between 0 and 32.
  *                     Any value greater than 32 is invalid.
  * 6rdPrefixLen        The IPv6 prefix length of the SP's 6rd IPv6
  *                     prefix in number of bits.  For the purpose of
  *                     bounds checking by DHCP option processing, the
  *                     sum of (32 - IPv4MaskLen) + 6rdPrefixLen MUST be
  *                     less than or equal to 128.
  * 6rdBRIPv4Address    One or more IPv4 addresses of the 6rd Border
  *                     Relay(s) for a given 6rd domain.
  * 6rdPrefix           The service provider's 6rd IPv6 prefix
  *                     represented as a 16-octet IPv6 address.  The bits
  *                     in the prefix after the 6rdPrefixlen number of
  *                     bits are reserved and MUST be initialized to zero
  *                     by the sender and ignored by the receiver.
  */
#ifdef IPV6
/* Fill dest with the text of option 'option'. */
static int generate_ip6_6rd_ce_config(unsigned char *option)
{
    int length=0,ip4masklen=0,ip6prefixlen=0,brnum=0,i=0;
    struct in6_addr ip6prefix;
    struct in_addr *BRip4addr = NULL;
    FILE *fp;

    memset(&ip6prefix,0,sizeof(ip6prefix));

    length = option[OPT_LEN-2];
    if(length >= IP6_6RD_OPT_DATA_MINLEN /* at least one BR. */
    && (length + DHCP_OPT_HEADER_LEN)%4 == 0) /* 6RD option header length + data length = 4*N. */
    {
        ip4masklen = (unsigned char)(*option);
        ip6prefixlen = (unsigned char)(*(option+1));
        memcpy(&ip6prefix,option+2,sizeof(struct in6_addr));
        BRip4addr = (struct in_addr *)(option+2+sizeof(struct in6_addr));
        brnum = (length-2-16)/4;
    }
    else
    {
        LOG(LOG_ERR,"IPv6 6RD option data length invalid.");
        return 0;
    }

    fp=fopen(IP6_6RD_CE_CONFIG_FILE, "w+");
    if(fp==NULL)
    {
        LOG(LOG_ERR,"IPv6 6RD open config file failed.");
        return 0;
    }
    /* 6rdPrefixLen		The IPv6 prefix length of the SP's 6rd IPv6
	 *					prefix in number of bits.  For the purpose of
	 *					bounds checking by DHCP option processing, the
	 *					sum of (32 - IPv4MaskLen) + 6rdPrefixLen MUST be
	 *					less than or equal to 128.
	 */
    if((ip4masklen >= 0 && ip4masklen <= 32)
    && (ip6prefixlen > 0 && ip6prefixlen < 128)
    && ((32-ip4masklen) + ip6prefixlen)<=128
    && ip6prefix.s6_addr16[0] != 0 && BRip4addr->s_addr != 0 /* addr not NULL */)
    {
        char IP6DotHex[64] = {0};

#if 1	
	/* char *ipv6prefix, char *ipv6prefixlen, char *ipv4masklen, char *boarderip */
	if (is_6rd_config_null(nvram_get("ip6_custom_6rd_wan_ipaddr"), nvram_get("ip6_custom_6rd_wan_plen"), 
		nvram_get("ip6_custom_6rd_ip4masklen"), nvram_get("ip6_custom_6rd_wan_BRipaddr")))
	{
		SC_CFPRINTF("device 6rd configuration NOT ok, copy to nv\n");
		nvram_set("ip6_custom_6rd", "1");
		nvram_set("ip6_custom_6rd_ip4masklen", int_to_str(ip4masklen));
		nvram_set("ip6_custom_6rd_wan_plen", int_to_str(ip6prefixlen));
		inet_ntop(AF_INET6, (void *)(&ip6prefix), IP6DotHex, INET6_ADDRSTRLEN);
		nvram_set("ip6_custom_6rd_wan_ipaddr", IP6DotHex);
		for(i=0;i<brnum;i++)
		{
			if((BRip4addr+i)->s_addr != 0)
			{
				char IPdotDec[16] = {0};
				inet_ntop(AF_INET, (void *)(BRip4addr+i), IPdotDec, 16);
				nvram_set("ip6_custom_6rd_wan_BRipaddr", IPdotDec);
				break;
			}
		}
		nvram_commit();
    }
#else
        fprintf(fp,"IPv4MaskLen=%d\n",ip4masklen);
        fprintf(fp,"6rdPrefixLen=%d\n",ip6prefixlen);
        inet_ntop(AF_INET6, (void *)(&ip6prefix), IP6DotHex, INET6_ADDRSTRLEN);
        fprintf(fp,"6rdPrefix=%s\n",IP6DotHex);
        for(i=0;i<brnum;i++)
        {
            if((BRip4addr+i)->s_addr != 0)
            {
                char IPdotDec[16] = {0};
                inet_ntop(AF_INET, (void *)(BRip4addr+i), IPdotDec, 16);
                fprintf(fp,"6rdBRIPv4Address=%s\n",IPdotDec);
            }
        }
#endif        
    }
    else
        LOG(LOG_ERR,"IPv6 6RD option data invalid.");
    fclose(fp);

    return 1;
}
#endif

static char *find_env(const char *prefix, char *defaultstr)
{
	extern char **environ;
	char **ptr;
	const int len = strlen(prefix);

	for (ptr = environ; *ptr != NULL; ptr++) {
		if (strncmp(prefix, *ptr, len) == 0)
			return *ptr;
	}
	return defaultstr;
}

#define ADDON_OPTIONS 20   // if you add options, increase this const
/* put all the paramaters into an environment */
static char **fill_envp(struct dhcpMessage *packet)
{
	int num_options = 0;
	int i = 0, j = 0;
	char **envp;
	unsigned char *temp;
	char over = 0;
	char filename[]=DHCPC_ROUTE_RELATED_OPTIONS_DEL"XXXX";
#ifdef IPV6
    int set_ip6_6rd=0;
#endif

    sprintf(filename,"%s%s",DHCPC_ROUTE_RELATED_OPTIONS_DEL,client_config.w);
	if (packet == NULL)
		num_options = 0;
	else {
		for (i = 0; options[i].code; i++)
			if (get_option(packet, options[i].code))
				num_options++;
		if (packet->siaddr) num_options++;
		if ((temp = get_option(packet, DHCP_OPTION_OVER)))
			over = *temp;
		if (!(over & FILE_FIELD) && packet->file[0]) num_options++;
		if (!(over & SNAME_FIELD) && packet->sname[0]) num_options++;
	}

	j = 0;
	envp = malloc((num_options + ADDON_OPTIONS) * sizeof(char *));
	envp[j] = malloc(sizeof("interface=") + strlen(client_config.interface));
	sprintf(envp[j++], "interface=%s", client_config.interface);
	envp[j++] = find_env("PATH", "PATH=/bin:/usr/bin:/sbin:/usr/sbin");
	envp[j++] = find_env("HOME", "HOME=/");
	envp[j] = malloc(sizeof("w=") + strlen(client_config.w));
	sprintf(envp[j++], "w=%s", client_config.w);
	envp[j] = malloc(sizeof("info=") + strlen(client_config.info));
	sprintf(envp[j++], "info=%s", client_config.info);

	if (packet == NULL) {
		envp[j] = malloc(sizeof("route_related_options_del_file=") + strlen(filename));
		sprintf(envp[j++], "route_related_options_del_file=%s", filename);
		envp[j++] = NULL;
		return envp;
	}

	envp[j] = malloc(sizeof("ip=255.255.255.255"));
	sprintip(envp[j++], "ip=", (unsigned char *) &packet->yiaddr);
	for (i = 0; options[i].code; i++) {
		if ((temp = get_option(packet, options[i].code))) {
#ifdef IPV6
            if(options[i].code == DHCP_IP6_6RD) // 6rd don't update j,don't change original logic
                set_ip6_6rd = generate_ip6_6rd_ce_config(temp);
            else
            {
#endif
			envp[j] = malloc(upper_length(temp[OPT_LEN - 2], &options[i]) + strlen(options[i].name) + 2);
			fill_options(envp[j], temp, &options[i]);
			j++;
#ifdef IPV6
            }
#endif
		}
	}

#ifdef IPV6
    if(!set_ip6_6rd && access(IP6_6RD_CE_CONFIG_FILE, F_OK) == 0)
        unlink(IP6_6RD_CE_CONFIG_FILE);
#endif

	if (packet->siaddr) {
		envp[j] = malloc(sizeof("siaddr=255.255.255.255"));
		sprintip(envp[j++], "siaddr=", (unsigned char *) &packet->siaddr);
	}
	if (!(over & FILE_FIELD) && packet->file[0]) {
		/* watch out for invalid packets */
		packet->file[sizeof(packet->file) - 1] = '\0';
		envp[j] = malloc(sizeof("boot_file=") + strlen((char *)packet->file));
		sprintf(envp[j++], "boot_file=%s", packet->file);
	}
	if (!(over & SNAME_FIELD) && packet->sname[0]) {
		/* watch out for invalid packets */
		packet->sname[sizeof(packet->sname) - 1] = '\0';
		envp[j] = malloc(sizeof("sname=") + strlen((char *)packet->sname));
		sprintf(envp[j++], "sname=%s", packet->sname);
	}
	envp[j] = malloc(sizeof("route_related_options_del_file=") + strlen(filename));
	sprintf(envp[j++], "route_related_options_del_file=%s", filename);
	
	envp[j] = NULL;
	return envp;
}


/* Call a script with a par file and env vars */
void run_script(struct dhcpMessage *packet, const char *name)
{
	int pid;
	char **envp;

	if (client_config.script == NULL)
		return;

	/* call script */
	pid = fork();
	if (pid) {
		waitpid(pid, NULL, 0);
		return;
	} else if (pid == 0) {
		envp = fill_envp(packet);

		/* close fd's? */

		{
			int i=0;
			LOG(LOG_INFO, "==================");
			for (;envp[i]!=NULL;i++)
				LOG(LOG_INFO, "%s", envp[i]);
			LOG(LOG_INFO, "==================");
		}

		/* exec script */
		LOG(LOG_INFO, "execle'ing %s %s", client_config.script, name);
		execle(client_config.script, client_config.script,
		       name, NULL, envp);
		LOG(LOG_ERR, "script %s failed: %s",
		    client_config.script, strerror(errno));
		exit(1);
	}
}
/*
 * compare dhcp packet options.
 * If the two packet are equal, dhcp_packet_cmp returns 0.
 *
 * add this function for pass cdrouter test case:
 *
 * svn version -r1481 caused this issue.
 * fix mantis 0015120: When WAN connection is "DHCP", WAN port uptime is updated after 12 hours in router status page
 * We should not call renew/bound script if ip address is not changed
 *
 *
    Test cdrouter-109: Verify WAN side switches to new gateway after renumber
	Module: renum-dhcp.tcl
	Name: cdrouter_renumber_6
	Description:


		step 1. Change the default gateway that the DHCP server will assign
				to a new IPv4 address based on testvar wanIspNextIp
		step 2. Wait for DHCP lease to expire on WAN interface
		step 3. Verify WAN client sends DHCPREQUEST for same IP address
		step 4. Send DHCPACK that contains the new IP gateway
		step 5. Wait 5 seconds for CPE to update the default gateway
		step 6. Initiate TCP connection from LAN to WAN
		step 7. Verify TCP connection works using the new gateway

		NOTE: By default, this test case will wait 5 seconds after the
			  DHCP client has updated before checking that the new
			  gateway is working. This time interval can be adjusted
			  using the testvar dhcpGatewayCache.

			  testvar dhcpGatewayCache 5

			  --- Jacky Zhu
 */
int dhcp_packet_cmp(struct dhcpMessage *packet1,
		struct dhcpMessage *packet2)
{
	int i;

	for (i = 0; options[i].code; i++)
	{
		unsigned char *temp;
		char *tmp1 = NULL, *tmp2 = NULL;

		if ((temp = get_option(packet1, options[i].code)))
		{
			tmp1 = malloc(upper_length(temp[OPT_LEN - 2], &options[i])
					+ strlen(options[i].name) + 2);

			assert(tmp1);/* add assert if malloc fail. */
			fill_options(tmp1, temp, &options[i]);
		}
		if ((temp = get_option(packet2, options[i].code)))
		{
			tmp2 = malloc(upper_length(temp[OPT_LEN - 2], &options[i])
					+ strlen(options[i].name) + 2);

			assert(tmp2); /* add assert if malloc fail. */
			fill_options(tmp2, temp, &options[i]);
		}

		if (tmp1 && tmp2 && strcmp(tmp1, tmp2))
		{

			free(tmp1);
			free(tmp2);
			return 1;

		}
		else if ((tmp1==NULL && tmp2) || (tmp1 && tmp2 == NULL))
		{
			free(tmp1);
			free(tmp2);
			return 1;
		}

		free(tmp1);
		free(tmp2);

	}

	return 0;
}

/*
  APIs 
  "the same for each platform/projects"

*/  
    
#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <net/if.h>	
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
// sysinfo
#include <sys/sysinfo.h>
#include "nvram.h"
#include "sc_info.h"
#ifndef _SC_DEBUG_ENABLE_
#define _SC_DEBUG_ENABLE_
#endif
#include "sc_debug.h"
    
#define LAN_UPTIME "/tmp/lan_uptime"
#define WAN_UPTIME "/tmp/wan_uptime"
#define WLAN_UPTIME "/tmp/wlan_uptime"
#define GROUP_UPTIME "/tmp/group_uptime"
#define LAN_IF nvram_safe_get("lan_if")
#define WLAN_IF "wl0"
#define WAN_IF nvram_safe_get("wan_ifname")
#define GROUP_IF "group1"
    void
get_static_info(stats_info * ifinfo, int uptime) 
{
    struct interface eth;
    memset(&eth, 0, sizeof(eth));
    strcpy(eth.name, ifinfo->name);
    if_readlist_proc(&eth);
    ifinfo->RX_packets = eth.rx_packets;
    ifinfo->TX_packets = eth.tx_packets;
    ifinfo->RX_bytes=eth.rx_bytes;
    ifinfo->TX_bytes=eth.tx_bytes;
    ifinfo->RX_errors = eth.rx_errors;
    ifinfo->TX_errors = eth.tx_errors;
    if (uptime != 0)
    {
	ifinfo->RX_bps = (eth.rx_bytes / uptime);
	ifinfo->TX_bps = (eth.tx_bytes / uptime);
    }
    ifinfo->collisions = eth.collisions;
    ifinfo->uptime_min = uptime / 60;
    ifinfo->uptime_day = (ifinfo->uptime_min) / (60 * 24);
    ifinfo->uptime_hr = (ifinfo->uptime_min / 60) % 24;
    ifinfo->uptime_min %= 60;
    ifinfo->uptime_sec = (int) uptime % 60;
} 

int
get_sockfd() 
{
    int sockfd = -1;
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
	
    {
	perror("user: socket creating failed");
	return (-1);
    }
    return sockfd;
}


int check_interface_up(char *interface)
{
	int is_up = 0;
	
	if (interface)
	{
		struct ifreq ifr;
		int skfd = socket(AF_INET, SOCK_DGRAM, 0);
		
		if (skfd)
		{
			strcpy(ifr.ifr_name, interface);
			if (ioctl(skfd, SIOCGIFFLAGS, &ifr) >= 0)
			{
				if (ifr.ifr_flags & IFF_UP)
				{
					is_up = 1;
				}
			}
			close(skfd);
		}
	}
	if (access("/tmp/debug_check_interface_up", F_OK) == 0)
	{
		SC_CFPRINTF("interface %s is %s up\n", interface?:"?", is_up?"":"NOT");
	}
	return is_up;
}

    int
get_ip_addr(char *ifname, char *ipaddr) 
{
	struct ifreq ifr;
	struct sockaddr_in *saddr;
	int fd;
	int ret = -1;
	if (ifname && ipaddr)
	{
		if ((fd = get_sockfd()) >= 0)
		{
			strcpy(ifr.ifr_name, ifname);
			ifr.ifr_addr.sa_family = AF_INET;
			
			/* get ip address */ 
			if (ioctl(fd, SIOCGIFADDR, &ifr) >= 0)
			{
				saddr = (struct sockaddr_in *) &ifr.ifr_addr;
				strcpy(ipaddr, (char *) inet_ntoa(saddr->sin_addr));
				ret = 0;	
				SC_CFPRINTF("ifname %s ip %s\n", ifname, ipaddr);
			} else
			{
				SC_CFPRINTF("ioctl fail\n");
			}
			
			close(fd);
		}
	}
	return ret;
}


int sc_get_mask_if(char *ifname, char *mask)
{
	int ret = -1;
	struct ifreq ifr;
	int fd;
	struct sockaddr_in *saddr;
	if (ifname && mask)
	{
		if ((fd = get_sockfd()) >= 0)
		{
			strcpy(ifr.ifr_name, ifname);
			ifr.ifr_addr.sa_family = AF_INET;
			
			if (ioctl(fd, SIOCGIFNETMASK, &ifr) >= 0)
			{
				saddr = (struct sockaddr_in *)&ifr.ifr_addr;
				strcpy(mask, (char *)(inet_ntoa(saddr->sin_addr)));
				ret = 0;
			} else
			{
				SC_CFPRINTF("ioctl fail\n");
			}
			close(fd);
		}
	}
	return ret;
}


/*TODO: maybe we can get ip address directly from nvram */ 
    int
getIFInfo(if_info_t * if_info) 
{
    char *pt;
    struct ifreq ifr;
    struct sockaddr_in *saddr;
    int fd;
    int ret = 0;
    if ((fd = get_sockfd()) >= 0)
	
    {
	strcpy(ifr.ifr_name, if_info->ifname);
	ifr.ifr_addr.sa_family = AF_INET;
	
	    /* get ip address */ 
	    if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
	{
	    saddr = (struct sockaddr_in *) &ifr.ifr_addr;
	    strcpy(if_info->ipaddr, (char *) inet_ntoa(saddr->sin_addr));
	    
		/* for hide on demand ip */ 
		if (strcmp(if_info->ipaddr, "10.64.64.64") == 0)
		ret = -2;
	}
	else
	    ret = -1;
	if (!strncmp(if_info->ifname, "ppp", 3))
	    
	{
	    if (ioctl(fd, SIOCGIFDSTADDR, &ifr) == 0)
	    {
		saddr = (struct sockaddr_in *) &ifr.ifr_addr;
		strcpy(if_info->rmtaddr,
			(char *) inet_ntoa(saddr->sin_addr));
		
		    /* for hide on demand ip */ 
		    // if(strcmp(if_info->ipaddr,"10.64.64.64")==0)
		    // ret=-2; 
	    }
	    else
		ret = -1;
	}
	
	    /* get mac address */ 
	    if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0)
	{
	    pt = ifr.ifr_hwaddr.sa_data;
	    sprintf(if_info->mac,
		     "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx" , *pt,
		     *(pt + 1), *(pt + 2), *(pt + 3), *(pt + 4), *(pt + 5));
	    if_info->mac[17] = '\0';
	}
	else
	    ret = -1;
	
	    /* get netmask */ 
	    if (ioctl(fd, SIOCGIFNETMASK, &ifr) == 0)
	{
	    saddr = (struct sockaddr_in *) &ifr.ifr_addr;
	    strcpy(if_info->mask, (char *) inet_ntoa(saddr->sin_addr));
	}
	else
	    ret = -1;
	
	    /* get mtu */ 
	    if (ioctl(fd, SIOCGIFMTU, &ifr) == 0)
	{
	    if_info->mtu = ifr.ifr_mtu;
	}
	else
	    ret = -1;
	close(fd);
	return ret;
    }
    return -1;
}
long
sc_get_UpTime(void) 
{
    struct sysinfo info;
    sysinfo(&info);
    return info.uptime;
}
 /*TBD*/ int
sc_get_IF_stats(IF_TYPE_enum type, int if_id, stats_info * pstat) 
{
    char tmp[16];
    int uptime = 0;
    FILE * fp;
    if (type == IF_TYPE_WAN)
	
    {
	strcpy(pstat->name, WAN_IF);
	if ((fp = fopen(WAN_UPTIME, "r")) != NULL)
	    
	{
	    fread(tmp, 16, 1, fp);
	    sscanf(tmp, "%d", &uptime);
	    fclose(fp);
	    get_static_info(pstat, uptime);
	}
	
	else
	    get_static_info(pstat, 0);
    }
    
    else if (type == IF_TYPE_VLAN)
	
    {
	strcpy(pstat->name, LAN_IF);
	if ((fp = fopen(LAN_UPTIME, "r")) != NULL)
	    
	{
	    fread(tmp, 16, 1, fp);
	    sscanf(tmp, "%d", &uptime);
	    fclose(fp);
	    get_static_info(pstat, uptime);
	}
	
	else
	    get_static_info(pstat, 0);
    }
    
    else if (type == IF_TYPE_WLAN)
	
    {
	strcpy(pstat->name, WLAN_IF);
	if ((fp = fopen(WLAN_UPTIME, "r")) != NULL)
	    
	{
	    fread(tmp, 16, 1, fp);
	    sscanf(tmp, "%d", &uptime);
	    fclose(fp);
	    get_static_info(pstat, uptime);
	}
	
	else
	    get_static_info(pstat, 0);
    }
    
    else if (type == IF_TYPE_GROUP)
	
    {
	strcpy(pstat->name, GROUP_IF);
	if ((fp = fopen(GROUP_UPTIME, "r")) != NULL)
	    
	{
	    fread(tmp, 16, 1, fp);
	    sscanf(tmp, "%d", &uptime);
	    fclose(fp);
	    get_static_info(pstat, uptime);
	}
	
	else
	    get_static_info(pstat, 0);
    }
    return 0;
}
int
sc_get_serialnumber(char *buf) 
{
    if (buf == NULL)
	return -1;
    strcpy(buf, "DEFAULT_SERIAL");
    return 0;
}
int
sc_get_firmwareversion(char *buf) 
{
    FILE * fp;
    if (buf == NULL)
	return -1;
    fp = fopen("/etc/version", "r");
    if (fp == NULL)
	return 0;
    fgets(buf, 128, fp);
    fclose(fp);
    buf[0] = 'V';
    return 0;
}


int
sc_get_mac_if(char *ifname, char mac[6]) 
{
	int ret = -1;
	struct ifreq ifr;
	int fd;
	if ((fd = get_sockfd()) >= 0)
	{
		strcpy(ifr.ifr_name, ifname);
		ifr.ifr_addr.sa_family = AF_INET;
		if (ioctl(fd, SIOCGIFHWADDR, &ifr) >= 0)
		{
			memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
			ret = 0;
		}
		close(fd);
	}
	return ret;
}

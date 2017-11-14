#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>
#include <pwd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <dirent.h>
#include <sys/sysinfo.h>
#include <grp.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include "sc_util.h"
#include "if_name.h"
#include "nvram.h"
#include "misc.h"

extern char cache_file[];
extern time_t last_access_time;
extern char last_remote_ip[];
extern char remote_ip[];
extern char current_remote_ip[];
extern int do_ssl;

int sc_lockf(int fd, int shared, int wait)
{
	int ret;
	int op;

	if (fd < 0)
	{
		return 0;
	}

	if (shared)
		op = LOCK_SH;
	else
		op = LOCK_EX;
	if (!wait)
		op |= LOCK_NB;

	ret = flock(fd, op);

	return ret;
}

char *getIPAddress(char *ifname)
{
	struct ifreq ifr;
	struct sockaddr_in *saddr;
	int fd;
	char *address = NULL;

	fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (fd == -1)
	{
		perror("socket creating failed");
		return NULL;
	}

	strcpy(ifr.ifr_name, ifname);
	if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
	{
		saddr = (struct sockaddr_in *)&ifr.ifr_addr;
		address = strdup(inet_ntoa(saddr->sin_addr));
		close(fd);
		return address;
	} else
	{
		close(fd);
		return NULL;
	}
	if (address != NULL)
		free(address);
	return NULL;

}

char *getNetMask(char *ifname)
{
	struct ifreq ifr;
	struct sockaddr_in *saddr;
	int fd;
	char *address = NULL;

	fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (fd == -1)
	{
		perror("socket creating failed");
		return NULL;
	}

	strcpy(ifr.ifr_name, ifname);
	if (ioctl(fd, SIOCGIFNETMASK, &ifr) == 0)
	{
		saddr = (struct sockaddr_in *)&ifr.ifr_addr;
		address = strdup(inet_ntoa(saddr->sin_addr));
		close(fd);
		return address;
	} else
	{
		close(fd);
		return NULL;
	}
	if (address != NULL)
		free(address);
	return NULL;

}

int check_lan_guest()
{
#if 1
    // if access from lan, no ssl is needed! 
    // fix the bug: someone may access lan without NAT, which source ip is in different subnet.
    	
    return (do_ssl == 0);
#else
	/* Check if @remote_ip from LAN or WAN */
	unsigned long int lan_tmp, remote_tmp, mask_tmp;
#ifdef WIFI_AP
	unsigned long int wan_tmp, wan_mask_tmp;
	char *wan_ip = getIPAddress(LAN_LOG_IFNAME);
	char *wan_mask = getNetMask(LAN_LOG_IFNAME);
	wan_tmp = inet_network(wan_ip);
	wan_mask_tmp = inet_network(wan_mask);
#endif
	
	lan_tmp = inet_network(nvram_safe_get("lan_ipaddr"));
	mask_tmp = inet_network(nvram_safe_get("lan_netmask"));
	
	remote_tmp = inet_network(remote_ip);
#ifdef IPMAC
	nvram_bcm_set("ipmac_login_ip", remote_ip);	//get login_ip for ipmac_login_ip, we will use it at cgi
#endif
	nvram_bcm_set("lan_login_ip", remote_ip);	//get login ip for access control

#ifdef WIFI_AP
//      if(*nvram_safe_get(WIFI_AP_MODE) == '1')
	{
		if ((remote_tmp & wan_mask_tmp) == (wan_tmp & wan_mask_tmp))
			return 1;
	}
#endif
#ifdef OPEN_VPN
	/* when a mobile connect to DUT by OPENVPN(use ip 192.168.2.x), to let it can access gui */
	if (*nvram_safe_get("openvpn_enable") == '1')
	{
		unsigned long int mobilelan_tmp, mobilelan_mask_tmp;
		char *mobilelan_ip = getIPAddress("tun0");
		char *mobilelan_mask = getNetMask("tun0");

		if (mobilelan_ip && mobilelan_mask)
		{
			mobilelan_tmp = inet_network(mobilelan_ip);
			mobilelan_mask_tmp = inet_network(mobilelan_mask);

			if ((remote_tmp & mobilelan_mask_tmp) == (mobilelan_tmp & mobilelan_mask_tmp))
				return 1;
		}
	}
#endif
	/* When DUT power on, it only remote_ip with value ,so first check as follow */
	if ((remote_tmp & mask_tmp) == (lan_tmp & mask_tmp))
		return 1;
	else
	{
		/* When wan remote access DUT, lan access it also, it will popup
		 * warning message, but at this time, remote_ip is wan user ip,
		 * it will popup 403 forbid , url is illegal.
		 */
		if (strlen(current_remote_ip))
		{
			unsigned long int cur_remote_tmp;
			cur_remote_tmp = inet_network(current_remote_ip);
			if ((cur_remote_tmp & mask_tmp) == (lan_tmp & mask_tmp))
				return 1;
		}
		return 0;
	}
#endif	
}

/*Why add this func:
 * check_lan_guest will work error in this case:
 * A Client at lan side has login, when a wan side request 
 * comes in, it will be treated as a LAN side request(return 1),
 * since 
 * remote_tmp = inet_network(remote_ip);
 *  not get a truely current remote ip but the record of last accessed ip
 * 
 * we add this func to simply check request's side by always record the current
 * request's IP.*/ 
int check_current_request_side(void) 
{
	SC_CFPRINTF("current_remote_ip:%s\n", current_remote_ip);
	if (strlen(current_remote_ip))
	{
		unsigned long int cur_remote_tmp, lan_tmp, mask_tmp;
		cur_remote_tmp = inet_network(current_remote_ip);
		//lan_tmp = inet_network(nvram_bcm_safe_get("http_lan_ipaddr"));
		lan_tmp = inet_network(nvram_safe_get("lan_ipaddr"));
		//mask_tmp = inet_network(nvram_bcm_safe_get("http_lan_netmask"));
		mask_tmp = inet_network(nvram_safe_get("lan_netmask"));
		if ((cur_remote_tmp & mask_tmp) == (lan_tmp & mask_tmp))
		{
			return 1;
		}
	}
		
	return 0;

}

int log_debug(char *format, ...)
{
//#if 0
	va_list args;
	FILE *fp;

	fp = fopen("/tmp/http_debug", "a+");
	if (!fp)
		return -1;

	va_start(args, format);
	vfprintf(fp, format, args);
	va_end(args);
	fprintf(fp, "\n");
	fflush(fp);
	fclose(fp);

	return 0;
//#endif
}

/*
 * Check if @path exist in @paths[].
 * Return 1 ==> Yes, path exist in paths
 *        0 ==> No. Can not find path in paths
 *
 * NOTE: paths[] end with NULL
 */
int path_exist(char *path, char *paths[], char *method_str)
{
	int i;

	for (i = 0; paths[i]; i++)
	{
		if (strstr(path, paths[i]))
		{
			SC_CFPRINTF("not need auth since %s is part of %s\n", paths[i], path);
			return 1;
		}
	}
	
	SC_CFPRINTF("method_str: %s\n", method_str);
	if (method_str && *method_str == 'G')
	{
		/* For these .gif or .css of .js or .xml or .jpg  file, it will be used by other .htm file, and it's no need to request auth for these files.  */
		if (
				((strstr(path, ".gif") != NULL) || (strstr(path, ".css") != NULL) || (strstr(path, ".js") != NULL) || (strstr(path, ".xml") != NULL) || (strstr(path, ".png") != NULL) || (strstr(path, ".jpg") != NULL))
				&&
				((strstr(path,".htm") == NULL) && (strstr(path,"html") ==NULL) && (strstr(path,".asp")==NULL) && (strstr(path, "todo=") == NULL))
		   )
		{
			SC_CFPRINTF("path:%s\n", path);
			
			// if not auth in http, does this an attack?
			if (*nvram_safe_get("config_state") != 'b')
			{
				char path_tmp[1024];
				memset(path_tmp, 0, sizeof(path_tmp));
				strncpy(path_tmp, path, sizeof(path_tmp)-1);
				
				if (strncmp(path_tmp, "/setup.cgi?", strlen("/setup.cgi?")) == 0)
				{
					char *p = strstr(path_tmp, "next_file=");
					if (p)
					{
						char *p1 = strchr(p, '&');
						if (p1)
						{
							*p1 = 0;
							
							SC_CFPRINTF("want get %s\n", p);
							if ((strstr(p, ".gif") != NULL) || (strstr(p, ".css") != NULL) || (strstr(p, ".js") != NULL) || (strstr(p, ".xml") != NULL) || (strstr(p, ".png") != NULL) || (strstr(p, ".jpg") != NULL))
							{
								SC_CFPRINTF("gif/css/js/xml/png/jpg, pass auth_check\n");
							} else
							{
								SC_CFPRINTF("real want get not gif/css/js/xml/png/jpg, still need auth\n");
								return 0;
							}
						} else
						{
							SC_CFPRINTF("\n");	
						}
					} else
					{
						SC_CFPRINTF("\n");	
					}
				} else
				{
					char *p = strchr(path_tmp, '&');
					if (p)
					{
						*p = 0;
						
						SC_CFPRINTF("want get %s\n", path_tmp);
						if ((strstr(path_tmp, ".gif") != NULL) || (strstr(path_tmp, ".css") != NULL) || (strstr(path_tmp, ".js") != NULL) || (strstr(path_tmp, ".xml") != NULL) || (strstr(path_tmp, ".png") != NULL) || (strstr(path_tmp, ".jpg") != NULL))
						{
							SC_CFPRINTF("gif/css/js/xml/png/jpg, pass auth_check\n");
						} else
						{
							SC_CFPRINTF("real want get not gif/css/js/xml/png/jpg, still need auth\n");
							return 0;	
						}
					}
				}
			} else
			{
				SC_CFPRINTF("\n");	
			}
			
			SC_CFPRINTF("\n");	
			return 1;
		} else
		{
			SC_CFPRINTF("\n");	
		}
	} else
	{
		SC_CFPRINTF("not check on post\n");
	}
	
	SC_CFPRINTF("\n");	
	return 0;
}

#ifdef IP_ASSIGN_CHK
/* read timestamp from file, value unit is in second */
unsigned long long get_uptime(char *time_file)
{
	FILE *fp;
	unsigned long long uptime = 0;

	if ((fp = fopen(time_file, "r")) != NULL)
	{
		fscanf(fp, "%llu[^.]", &uptime);
		fclose(fp);
	}
	return uptime;
}
#endif

int need_fakepath(char *path)
{
	char *p = NULL;
	char tmp[MAXPATHLEN];
	int fakepath = 0, i;

	if (strstr(path, "multi_login.html"))	//should keep this
		fakepath = 0;
	else if (strstr(path, "/ HTTP") || strstr(path, ".htm"))
		fakepath = 1;
	else
	{
		i = sprintf(tmp, "/www");
		if ((p = strchr(path, '/')) != NULL)
		{
			while (*p != ' ' && *p != '?' && i < MAXPATHLEN - 1)
				tmp[i++] = *p++;
			tmp[i] = 0;
			SC_CFPRINTF("path=%s\n", tmp);
			if (access(tmp, F_OK) != 0)	/* if not exist, then fake path */
				fakepath = 1;
		}
	}
	SC_CFPRINTF("fakepath=%d\n", fakepath);
	return fakepath;
}

/*
 * Follow Netgear Spec V1.9 to send log
 */
void send_log(int login_success, char *remote_ip)
{
	unsigned long int lan_tmp, remote_tmp, mask_tmp;
	int from_lan;
	int event_base = 10;	/* See 7.3.2   Events to be logged of spec 1.9 event NO.10,11,12,13 */

	/* Check if @remote_ip from LAN or WAN */
	lan_tmp = inet_network(nvram_get("lan_ipaddr"));
	mask_tmp = inet_network(nvram_get("lan_netmask"));
	remote_tmp = inet_network(remote_ip);
	if ((remote_tmp & mask_tmp) == (lan_tmp & mask_tmp))
	{
		from_lan = 1;
	} else
	{
		from_lan = 0;
	}

	if (!from_lan)
	{
		event_base += 2;
	}
	if (login_success != LOGIN_SUCCESS)
	{
		event_base++;
	} else
	{
#ifdef IPMAC
		nvram_bcm_set("ipmac_login_ip", remote_ip);	//get login_ip for ipmac_login_ip, we will use it at cgi
#endif
		nvram_bcm_set("lan_login_ip", remote_ip);	//get login ip for access control
	}

	NETGEAR_SYSLOG("%02d 00[%s login%s] from source %s", event_base, from_lan ? "admin" : "remote", login_success == LOGIN_SUCCESS ? "" : " failure", remote_ip);
	return;
}


static int lock_fd = -1;
void lock_enter(void)
{
	if ((lock_fd = open("/var/lock/httpd.lock", O_RDWR | O_CREAT, 0644)) < 0)
	{
		return;
	}
	if (lockf(lock_fd, F_LOCK, 0) < 0)
	{
		close(lock_fd);
	}
	return;
}

void lock_leave(void)
{
	if (lock_fd >= 0)
	{
		lockf(lock_fd, F_ULOCK, 0);
		close(lock_fd);
		lock_fd = -1;
	}
}

void __restore_manager(void)
{
	FILE *fp;

	fp = fopen(cache_file, "rb");
	if (fp)
	{
		fread(&last_access_time, sizeof(last_access_time), 1, fp);
#ifdef HTTPD_IPV6_SUPPORT		
		fread(last_remote_ip, SC_IP6ADDR_LEN, 1, fp);
#else
		fread(last_remote_ip, SC_IPADDR_LEN, 1, fp);
#endif		
		fclose(fp);
		SC_CFPRINTF("remote_ip = <%s> last_remote_ip = <%s> last_access_time = <%d> \n", remote_ip, last_remote_ip, (int)last_access_time);
	}
}

void __save_manager(void)
{
	FILE *fp;

	fp = fopen(cache_file, "wb");
	if (fp)
	{
		fwrite(&last_access_time, sizeof(last_access_time), 1, fp);
#ifdef HTTPD_IPV6_SUPPORT			
		fwrite(last_remote_ip, SC_IP6ADDR_LEN, 1, fp);
#else				
		fwrite(last_remote_ip, SC_IPADDR_LEN, 1, fp);
#endif		
		fclose(fp);
		SC_CFPRINTF("remote_ip = <%s> last_remote_ip = <%s> last_access_time = <%d> cache_file:%s\n", remote_ip, last_remote_ip, (int)last_access_time, cache_file);
	}
}

inline void restore_manager(void)
{
	lock_enter();
	__restore_manager();
	lock_leave();
}

inline void save_manager(void)
{
	lock_enter();
	__save_manager();
	lock_leave();
}

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
#include "sc_usb.h"
#include "sc_util.h"
#include "misc.h"
#include "nvram.h"

extern char remote_ip[];
extern char *host; 
extern char *path;
extern char *authorization;
extern int web_timeout;

extern struct usb_user_t all_usb_user[];
extern int usb_user_count;
extern int is_usb_session;

extern void send_error(int s, char *title, char *extra_header, char *text);
extern int b64_decode(const char *str, unsigned char *space, int size);
extern void send_authenticate(char *realm);

static int http_admin_pwd_protect_enable(void)
{
    /* Check if @remote_ip from LAN or WAN */
    unsigned long int lan_tmp = inet_network(nvram_safe_get("lan_ipaddr"));
    unsigned long int mask_tmp = inet_network(nvram_safe_get("lan_netmask"));
    unsigned long int remote_tmp = inet_network(remote_ip);
    int from_wan;
    
    if( (remote_tmp & mask_tmp) != (lan_tmp & mask_tmp) ) {
        from_wan = 1;
    }
    else 
    {
        from_wan = 0;
    }
    
    /* Need check local and remote http whether */
	if((*nvram_safe_get("http_server_pwd_enable") == '1' && from_wan == 0)
	    || (*nvram_safe_get("http_server_wan_pwd_enable") == '1' && from_wan == 1))
	    return 1;
	else
	    return 0;
}

int is_usb_subdir_file(const char *dir)
{
    if(dir && *dir)
    {
    	if( strstr(dir, "shares/USB_Storage") 
            && (strlen(dir) > strlen("shares/USB_Storage/")))
            return 1;
        if((strstr(dir, "shares/") && strstr(dir, "_Drive"))
            && (strlen(dir) > strlen("shares/T_Drive/")))	                	        
            return 1;
    }
        
    return 0;
}

int is_symlink(char *buf)
{
	struct stat sb2;
	  
	if(lstat(buf, &sb2) < 0)
	    return 0;
	    
	if(S_ISLNK(sb2.st_mode))
	    return 1;
	else
	    return 0;  
}

int usb_subdir_is_symlink(char *buf)
{
    char *p = buf;
    int i = 0;
                
    while(*p)
    {
        p = strchr(p, '/');  
        
        if(p == NULL)
            return 0;
        /* shares/USB_Storage/ or shares/x_Drive/xxx/ ,so skip two */                    
        if( i > 1)
        {                           
            /* begin process 3nd, 4th ...  */
            *p = '\0';           
            if(is_symlink(buf))
                return  1;
            else
                *p = '/';
        }
        i++;
        p++;          
    }
    return 0;
}

int is_readable(const char *dir, const char *name)
{
 	FILE *fp = NULL;
 	char tmp[2048], dir_a[2048] = { 0, }, auth[64];
	static char buf[2048]={0};
 	struct usb_user_t *user;
	
	if(is_usb_subdir_file(dir))
	{
        /* If it is link, we will think it is  not read*/
        if(strlen(dir) > sizeof(buf))
            strncpy(buf, dir, sizeof(buf)-1);
        else
            strcpy(buf, dir);
        if(buf[strlen(buf)-1] == '/')
        {
            buf[strlen(buf)-1]='\0';
        }  
        if(is_symlink(buf))
        {
            return 0;
        }
        else
        {   
            /* fully path is not symlink, but the path directory will be symlink 
                such as shares/USB_Storage/tmp/ntp_start is a real file
                but shares/USB_Storage/tmp is symlink, so we need check the directory 
                path whether own symlink           
            */         
            if(usb_subdir_is_symlink(buf))
            {
                return 0;
            }         
        }
    } 
    memset(buf, 0, sizeof(buf));
 	user = usb_user_lookup(remote_ip);
 
 	if (*(dir + strlen(dir) - 1) != '/')
		snprintf(buf, sizeof(buf), "%s/%s/", dir, name);
	else
		snprintf(buf, sizeof(buf), "%s%s/", dir, name);

	if ((fp = fopen("/etc/usb_httpd.conf", "r")) != NULL)
	{
		while (fgets(tmp, 2048, fp) != NULL)
		{

			sscanf(tmp, "%s\t%s", dir_a, auth);
			SC_CFPRINTF("dir_a=<%s>(strlen(dir_a)=%d), auth=<%s>(strlen(auth)=%d)\n", dir_a, strlen(dir_a), auth, strlen(auth));
			SC_CFPRINTF("dir=<%s>(strlen(dir)=%d), admin=<admin>\n", buf, strlen(buf));
			SC_CFPRINTF("user->auth=<%d>, admin=<admin>\n", user->auth);
			if ((strlen(buf) >= strlen(dir_a) && strncmp(buf, dir_a, strlen(dir_a)) == 0) && strcmp(auth, "admin") == 0 && user->auth == USB_AUTH_GUEST)
			{
				fclose(fp);
				SC_CFPRINTF("<hit it! hahahahhaha>\n");
				return 0;
			}
		}
		fclose(fp);
	}

	return 1;

}

int check_valid_request(void)
{
	int valid_port;
	int remote_port = 0;
	char *p;

	if (check_lan_guest())
	{
		return 1;
	}

	p = strchr(host, ':');
	if (p)
		remote_port = atoi(p + 1);

	if (remote_port == 0)
		remote_port = 443;

	if (is_usb_session)
		valid_port = atoi(nvram_get("http_server_wan_port"));
	else
		valid_port = atoi(nvram_get("fw_remote_port"));

	if (remote_port != valid_port)
	{
		SC_CFPRINTF("invalid request: remote_port = %d, valid_port = %d\n", remote_port, valid_port);
		return 0;
	}

	return 1;
}

int usb_session_check(void)
{
	char *pt;

	pt = strstr(path, "/shares");
	if (pt != NULL && (pt[7] == '/' || pt[7] == ' ' || pt[7] == '\t' || pt[7] == '\r' || pt[7] == '\n'))
		return 1;
	else
		return 0;
}

void usb_user_load(void)
{
	FILE *fp;

	fp = fopen("/var/usb_http_user.cache", "rb");
	if (fp)
	{
		usb_user_count = fread(&all_usb_user, sizeof(all_usb_user[0]), MAX_USB_USER, fp);
		fclose(fp);
		SC_CFPRINTF("usb_user_count = %d \n", usb_user_count);
	}
}

void usb_user_save(void)
{
	FILE *fp;

	fp = fopen("/var/usb_http_user.cache", "wb");
	if (fp)
	{
		fwrite(&all_usb_user, sizeof(all_usb_user[0]), usb_user_count, fp);
		fclose(fp);
	}
}

struct usb_user_t *usb_user_lookup(const char *ip)
{
	int i;

	SC_CFPRINTF("lookup for <%s>\n", ip);
	for (i = 0; i < MAX_USB_USER; i++)
	{
		if (strcmp(all_usb_user[i].remote_ip, ip) == 0)
		{
			SC_CFPRINTF("find usb user %s@%d \n", ip, i);
			return &all_usb_user[i];
		}
	}
	return NULL;
}

struct usb_user_t *usb_user_add(const char *ip)
{
	int i;
	struct sysinfo info;
	struct usb_user_t *user = NULL;

	sysinfo(&info);

	/* TODO: add lock. */
	for (i = 0; i < MAX_USB_USER; i++)
	{
		/* pick up an empty entry or a timeout entry. */
		if (all_usb_user[i].remote_ip[0] == 0 || (info.uptime - all_usb_user[i].timestamp) > web_timeout)
		{
			SC_CFPRINTF("add usb user %s@%d \n", ip, i);
			user = &all_usb_user[i];
			break;
		}
	}
	if (user != NULL)
	{
		strcpy(user->remote_ip, ip);
		user->timestamp = info.uptime;
		user->auth = USB_AUTH_NONE;
		if (i == usb_user_count)
			usb_user_count++;
		usb_user_save();
	}
	return user;
}

void usb_auth_check(char *dirname)
{
	struct usb_user_t *user;
	struct sysinfo info;
	char *authpath = AUTH_FILE;
	FILE *fp;
	char buf[1024];
	char *pt, *savep;
	char authinfo[500];
	char *authpass;
	static char line[10000];
	int auth_flag = 0;
	char *cryp;
	char *authname = NULL, *name = NULL;
	int l, auth_res;

	if (authname || name)
	{
		/* let gcc463 not complain. */
	}

	sysinfo(&info);

	SC_CFPRINTF("dirname is <%s>\n", dirname);

	/* enter a critical session. */
	lock_enter();

	usb_user_load();
	user = usb_user_lookup(remote_ip);

	if (user == NULL && (user = usb_user_add(remote_ip)) == NULL)
	{
		send_error(403, "Forbidden", "", "The number of user has reached maximum value.");
	}

	/* check if this session need authentication or not. */
	if (strcmp(dirname, "shares/") != 0)
	{
		fp = fopen("/etc/usb_httpd.conf", "r");
		if (fp)
		{
			while (fgets(buf, sizeof(buf), fp) != NULL)
			{
				pt = strtok_r(buf, " \t", &savep);
				if (strstr(pt, dirname) != NULL || strstr(dirname, pt) != NULL)
				{
					pt = strtok_r(NULL, " \t", &savep);
					if (strcmp(pt, "admin") == 0)
					{
						auth_flag = 1;
						break;
					}
				}
			}
			fclose(fp);
		} else
		{
			auth_flag = 0;
		}
	}

	if(http_admin_pwd_protect_enable())
	    auth_flag = 1;

	if (auth_flag == 0)
	{
		if (user->auth == USB_AUTH_NONE)
		{
			user->auth = USB_NO_AUTH;
			if (check_lan_guest() == 0)
				NETGEAR_SYSLOG("20 00[USB remote access] from %s through HTTP", remote_ip);
		}
		user->timestamp = info.uptime;
		usb_user_save();
		goto _out;
	}
	auth_res = AUTH_OK;
	/* Does this request contain authorization info? */
	if (authorization == (char *)0)
	{
		auth_res = NO_AUTH;
		/* Nope, return a 401 Unauthorized. */
		goto _auth;
	}
	/* Basic authorization info? */
	if (strncmp(authorization, "Basic ", 6) != 0)
	{
		auth_res = NO_AUTH;
		goto _auth;
	}

	/* Decode it. */
	l = b64_decode(&(authorization[6]), (unsigned char *)authinfo, sizeof(authinfo) - 1);
	authinfo[l] = '\0';
	/* Split into user and password. */
	authpass = strchr(authinfo, ':');
	authname = authinfo;

	if (authpass == (char *)0)
	{
		/* No colon?  Bogus auth info. */
		goto _auth;
	}
	*authpass++ = '\0';

	/* Open the password file. */
	fp = fopen(authpath, "r");
	if (fp == (FILE *) 0)
	{
		/* The file exists but we can't open it?  Disallow access. */
#ifdef SYSLOG
		syslog(LOG_ERR, "%.80s auth file %.80s could not be opened - %m", ntoa(&client_addr), authpath);
#endif
		send_error(403, "Forbidden", "", "File is protected.");
	}

	/* Read it. */
	while (fgets(line, sizeof(line), fp) != (char *)0)
	{
		/* Nuke newline. */
		l = strlen(line);
		if (line[l - 1] == '\n')
			line[l - 1] = '\0';
		/* Split into user and encrypted password. */
		cryp = strchr(line, ':');
		if (cryp == (char *)0)
			continue;
		*cryp++ = '\0';
		name = line;
		/* Is this the right user? */
		if (strcmp(line, authinfo) == 0)
		{
			/* Yes. */
			(void)fclose(fp);
			/* So is the password right? */
			/* OK ! */
			if (strcmp(authpass, cryp) == 0)
			{
				if (user->auth == USB_AUTH_NONE || user->auth == USB_NO_AUTH || (user->auth == USB_AUTH_PASS && (info.uptime - user->timestamp) > web_timeout))
				{
					goto _auth;
				}
				if (user->auth == USB_AUTH_FAIL)
				{
					user->auth = USB_AUTH_PASS;
					if (check_lan_guest() == 0)
						NETGEAR_SYSLOG("20 00[USB remote access] from %s through HTTP", remote_ip);
				}
			} else
			{
				/* No. */
				SC_CFPRINTF("Login Failed - IP: %s\n", remote_ip);
				auth_res = AUTH_FAIL;
				goto _auth;
			}
			user->timestamp = info.uptime;
			usb_user_save();
			goto _out;
		}
	}

	/* Didn't find that user.  Access denied. */
	(void)fclose(fp);
	auth_res = AUTH_FAIL;

	if(http_admin_pwd_protect_enable())
		goto _auth;
	/* if come here, we consider this user is gurest */
	if (user->auth == USB_AUTH_NONE)
	{
		user->auth = USB_AUTH_FAIL;
		usb_user_save();
		send_authenticate(dirname);
		goto _out;
	} else
	{
		if (user->auth == USB_AUTH_GUEST && (info.uptime - user->timestamp) > web_timeout)
			goto _auth;
		user->auth = USB_AUTH_GUEST;
		user->timestamp = info.uptime;
		usb_user_save();
		goto _out;
	}

_auth:
	if (auth_res == AUTH_FAIL && check_lan_guest() == 0)
	{
		NETGEAR_SYSLOG("20 00[USB remote access rejected] from %s through HTTP", remote_ip);
	}
	user->auth = USB_AUTH_FAIL;
	usb_user_save();
	send_authenticate(dirname);
_out:
	lock_leave();
}

int sc_usb_mounted()
{
	int usb_mounted = 0;
	FILE *fp = fopen("/proc/mounts", "r");
	if (fp)
	{
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		fread(buf, sizeof(buf), 1, fp);
		SC_CFPRINTF("buf:%s\n", buf);
		if (strstr(buf, "/dev/sd") && strstr(buf, "mnt"))
		{
			usb_mounted = 1;
		}
		fclose(fp);	
	}
	return usb_mounted;
}





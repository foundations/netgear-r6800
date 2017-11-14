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
#include "sc_guest_management.h"
#include "sc_util.h"

extern int web_timeout;
extern time_t last_access_time;
extern char last_remote_ip[];

static struct guest_user_t all_guest_user[MAX_GUEST_USER];
static int guest_num = 0;
void guest_user_load(void)
{
	FILE *fp;

	fp = fopen("/var/guest_http_user.cache", "rb");
	if (fp)
	{
		guest_num = fread(&all_guest_user, sizeof(all_guest_user[0]), MAX_GUEST_USER, fp);
		fclose(fp);
		SC_CFPRINTF("guest_num = %d \n", guest_num);
	}
}

void guest_user_save(void)
{
	FILE *fp;

	fp = fopen("/var/guest_http_user.cache", "wb");
	if (fp)
	{
		fwrite(&all_guest_user, sizeof(all_guest_user[0]), guest_num, fp);
		fclose(fp);
	}
}

struct guest_user_t *guest_user_lookup(const char *ip)
{
	int i;

	SC_CFPRINTF("lookup for <%s>\n", ip);
	for (i = 0; i < MAX_GUEST_USER; i++)
	{
		if (strcmp(all_guest_user[i].guest_ip, ip) == 0)
		{
			SC_CFPRINTF("find guest user %s@%d \n", ip, i);
			return &all_guest_user[i];
		}
	}
	return NULL;
}

struct guest_user_t *guest_user_add(const char *ip)
{
	int i;
	struct sysinfo info;
	struct guest_user_t *user = NULL;

	sysinfo(&info);

	/* TODO: add lock. */
	for (i = 0; i < MAX_GUEST_USER; i++)
	{
		/* pick up an empty entry or a timeout entry. */
		if (all_guest_user[i].guest_ip[0] == 0 || (info.uptime - all_guest_user[i].guest_access_time) > web_timeout)
		{
			SC_CFPRINTF("add guest user %s@%d \n", ip, i);
			user = &all_guest_user[i];
			break;
		}
	}
	if (user != NULL)
	{
		strcpy(user->guest_ip, ip);
		user->guest_access_time = info.uptime;
		//user->auth = USB_AUTH_NONE;
		if (i == guest_num)
			guest_num++;
		guest_user_save();
	}
	return user;
}


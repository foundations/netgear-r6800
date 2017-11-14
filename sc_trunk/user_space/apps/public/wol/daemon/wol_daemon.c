#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "nvram.h"

#define BUFLEN 128

int main(void)
{
	int sleep_int = 60;
	int found = 0;
	FILE *fp;
	char clientaddr[16];
	char macaddr[18];
	char readline[BUFLEN];
	char ipaddr[16];
	char tmp1[8], tmp2[8];
	char cmd[64];

	pid_t pid;
	pid = fork();
	if(pid > 0)
		return 0;
	if(pid < 0)
		return -1;

	while(1)
	{

		if(*nvram_safe_get("apple_timemachine_on") == '1')
		{
			strcpy(clientaddr, nvram_safe_get("apple_ip_addr"));
			/* Couldn't get apple client address */
			if(*clientaddr == '0')
				continue;
			
			if(found == 0)
			{
				fp = fopen("/proc/net/arp", "r");
				if(fp == NULL)
					return -1;
				/* skip the first line */
				fgets(readline, sizeof(readline), fp);
				while(fgets(readline, sizeof(readline), fp) != NULL)
				{
					sscanf(readline, "%[^ ] %[^ ] %[^ ] %[^ ] ", ipaddr, tmp1, tmp2, macaddr);
					printf("find ip address from arp: %s\n", ipaddr);
					if(strcmp(clientaddr, ipaddr) == 0)
					{
						found = 1;
						break;
					}
				}
				fclose(fp);
			}

			if(found == 1)
			{
				sprintf(cmd, "/usr/sbin/wol %s", macaddr);
				system(cmd);
				found = 0;
			}

			sleep_int = 300;
		}

		else
		{
			found = 0;
			sleep_int = 60;
		}

		sleep(sleep_int);
	}
	return 0;
}


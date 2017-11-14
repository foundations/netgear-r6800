/*
 * Author: Paul.Russell@rustcorp.com.au and mneuling@radlogic.com.au
 *
 * Based on the ipchains code by Paul Russell and Michael Neuling
 *
 * (C) 2000-2002 by the netfilter coreteam <coreteam@netfilter.org>:
 * 		    Paul 'Rusty' Russell <rusty@rustcorp.com.au>
 * 		    Marc Boucher <marc+nf@mbsi.ca>
 * 		    James Morris <jmorris@intercode.com.au>
 * 		    Harald Welte <laforge@gnumonks.org>
 * 		    Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 *
 *	iptables -- IP firewall administration for kernels with
 *	firewall table (aimed for the 2.3 kernels)
 *
 *	See the accompanying manual page iptables(8) for information
 *	about proper usage of this program.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iptables.h>
#include "iptables-multi.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define SC_CFPRINTF(fmt, args...)    do{    FILE *fp=fopen("/dev/console", "a+"); if(fp) {fprintf(fp, "[%s::%s():%d] ", __FILE__, __FUNCTION__, __LINE__);fprintf(fp, fmt, ##args);fclose(fp);}}while(0)

static char *strnstr(const char *haystack, const char *needle, size_t haystacklen)
{
    char *p;
    size_t plen;
    size_t len = strlen(needle);

    if (*needle == '\0')    /* everything matches empty string */
    return (char*) haystack;

    plen = haystacklen;
    for (p = (char*) haystack; p != NULL; p = memchr(p + 1, *needle, plen-1)) {
        plen = haystacklen - (p - haystack);

        if (plen < len) return NULL;

        if (strncmp(p, needle, len) == 0)
            return (p);
        }
        return NULL;
}

#ifdef IPTABLES_MULTI
int
iptables_main(int argc, char *argv[])
#else
int
main(int argc, char *argv[])
#endif
{
	int ret;
	char *table = "filter";
	struct iptc_handle *handle = NULL;

	iptables_globals.program_name = "iptables";
	ret = xtables_init_all(&iptables_globals, NFPROTO_IPV4);
	if (ret < 0) {
		fprintf(stderr, "%s/%s Failed to initialize xtables\n",
				iptables_globals.program_name,
				iptables_globals.program_version);
				exit(1);
	}
#if defined(ALL_INCLUSIVE) || defined(NO_SHARED_LIBS)
	init_extensions();
#endif

#if 0
	ret = do_command(argc, argv, &table, &handle);
	if (ret) {
		ret = iptc_commit(handle);
		iptc_free(handle);
	}
#else
	if(argv[1] && strcmp(argv[1],"-C")==0){
		FILE *fp=NULL;
		char buf[512]="";
		char *margv[128];
		int margc=0;
		pid_t pid;

		if(argv[2]==NULL){
			printf("%s -C rules\n",argv[0]);
			return -1;
		}

		fp=fopen(argv[2],"r");
		ret = 1;
		while(fgets(buf,512,fp)){
			char *sp1, *sp2;
			if(strlen(buf)<10)
				continue;
//			puts(buf);
			if(((sp1=strstr(buf, "\""))!=NULL)&&((sp2=strstr(sp1+1, "\""))!=NULL)) {
				*(sp1++)=' ';
				*(sp2--)=' ';
				 while(sp1 && (sp1 <= sp2)) {
					  if((sp1=strnstr(sp1, " ", (int)(sp2 - sp1) + 1))!=NULL){
						*sp1='\1';
						sp1++;
					}
				}
			}
			margc=0;
			margv[margc++]=strtok(buf," ");
			while((margv[margc]=strtok(NULL," "))){
				int i=strlen(margv[margc]);
				if(strcmp(margv[margc], "\n")==0)
					continue;
				sp1=margv[margc];
				while((sp1=strstr(sp1, "\1"))!=NULL)
					*(sp1++)=' ';
//				printf("Ron == %s\n",margv[margc]);

				if(margv[margc][i-1]=='\n')
					margv[margc][i-1]='\0';
//				printf("<%s>\n",margv[margc]);
//				usleep(10000);
				margc++;
			};

			if(margv[0] && margv[0][0]=='#')
				continue;

			margv[0]=argv[0];
			/* fork to process this command */
			if((pid=fork())<0){
				perror("fork");
				exit(1);
			}else if(pid==0){
				ret = do_command(margc, margv, &table, &handle);
				if(ret) {
					ret = iptc_commit(handle);
					iptc_free(handle);
				}
				exit(!ret);
			}else{
				int status;
				waitpid(pid,&status,WUNTRACED);
				ret = 1;
			}
		}
		fclose(fp);

	}else{
		ret = do_command(argc, argv, &table, &handle);
		if (ret) {	
			ret = iptc_commit(handle);
			iptc_free(handle);
		}
	}
#endif

	if (!ret) {
		if (errno == EINVAL) {
			fprintf(stderr, "iptables: %s. "
					"Run `dmesg' for more information.\n",
				iptc_strerror(errno));
		} else {
			fprintf(stderr, "iptables: %s.\n",
				iptc_strerror(errno));
		}
		if (errno == EAGAIN) {
			exit(RESOURCE_PROBLEM);
		}
	}

	exit(!ret);
}

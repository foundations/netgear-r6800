/*
 * Copyright (C) 2012  Sercomm Corporation
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "nvram.h"

extern int readFileBin(char *path, char **data);
extern void writeFileBin(char *path, char *data, int len);

static int nvram_unset_func(const char* name,char *path)
{
	char *bufspace, *targetspace;
	int size;
	char *sp, *s;
	int found=0;

	if((size=readFileBin(path, &bufspace))>0)
	    targetspace=malloc(size);
	else
		return NVRAM_SUCCESS;

	sp=targetspace;
	if(size > 0) {
	   for (s = bufspace; *s; s++) {
		if (!strncmp(s, name, strlen(name)) && *(s+strlen(name))=='=') {
			found=1;
			while (*(++s));
		}
		else{
			while(*s) *(sp++)=*(s++);
	        	*(sp++)=END_SYMBOL;
		}
	    }

		free(bufspace);
	}
	if(!found){
		free(targetspace);
		return NVRAM_SUCCESS;
	}

	*(sp) = END_SYMBOL;

	writeFileBin(path, targetspace, (sp-targetspace)+1);

	free(targetspace);
	return NVRAM_SUCCESS;

}
#define nvram_unset(name) nvram_unset_func( name,NVRAM_TMP_PATH)
#define nvram_bcm_unset(name) nvram_unset_func( name,NVRAM_BCM_PATH)

void nvram_show_func(char *path)
{
	char *bufspace;
	int size;
	char *s,*pt;

	if((size=readFileBin(path, &bufspace))<0) {
	    printf("can't find nvram!!\n");
	    exit(1);
	}

	if(size > 0) {
	   for (s = bufspace; *s; s++) {
	   	for(pt=strchr(s,0x01);pt;pt=strchr(pt+1,0x01)) *pt=0x20;
		printf("%s\n",s);
		while(*(++s));
	   }

		free(bufspace);
	}

}
#define nvram_bcm_show() nvram_show_func(NVRAM_BCM_PATH)
#define nvram_show() nvram_show_func(NVRAM_TMP_PATH)


void usage()
{
	puts("\nUsage: \n\tnvram [show|bcmshow|get|set|bcmset|unset|bcmunset|add|init|commit] [name=value]\n");
	exit(0);
}

int main(int argc,char *argv[]){

	char *name;
	char *value;
	int readycloud_nvram = 0;
	if(!strcmp(argv[0],"readycloud_nvram"))
		readycloud_nvram = 1;
		
	if (argv[1]==NULL)
		usage();
	else if(!strcmp(argv[1],"show"))
		nvram_show();

	else if(!strcmp(argv[1],"bcmshow"))
		nvram_bcm_show();

	else if(!strcmp(argv[1],"set")){
		name=strtok(argv[2],"=");
		value=(strtok(NULL,"")?:"");
		if(!name) usage();
		nvram_set(name,value);
		nvram_commit();
	}else if(!strcmp(argv[1],"bcmset")){
		name=strtok(argv[2],"=");
		value=(strtok(NULL,"")?:"");
		if(!name) usage();
		nvram_bcm_set(name,value);
	}else if(!strcmp(argv[1],"bcmadd")){
		char tmp[4096];
		name=strtok(argv[2],"=");
		value=strtok(NULL,"");

		if(!name || !value) usage();
		sprintf(tmp,"%s%c%s",nvram_get(name),0x01,value);
		nvram_bcm_set(name,tmp);

	}else if(!strcmp(argv[1],"add")){
		char tmp[4096];
		name=strtok(argv[2],"=");
		value=strtok(NULL,"");

		if(!name || !value) usage();
		sprintf(tmp,"%s%c%s",nvram_get(name),0x01,value);
		nvram_set(name,tmp);
		nvram_commit();

	}else if(!strcmp(argv[1],"unset")){
		name=argv[2];
		if(!name) usage();
		nvram_unset(name);
		nvram_commit();
	}else if(!strcmp(argv[1],"bcmunset")){
		name=argv[2];
		if(!name) usage();
		nvram_bcm_unset(name);
	}else if(!strcmp(argv[1],"get")){
                char *p;
		name=argv[2];
		if(!name) usage();
                p = nvram_get(name);
                if(readycloud_nvram != 1)
                {
					if(p) {
						printf("%s=%s\n",name, p);
					}
					else {
						printf("%s do not exist!\n", name);
					}
				}else
				{                
					if(p) {
						printf("%s\n", p);
					}
					else {
						printf("%s do not exist!\n", name);
					}
				}
	}else if(!strcmp(argv[1],"init"))
			nvram_load();
	else if(!strcmp(argv[1],"commit"))
			nvram_commit();
	else
	{
		int idx;

		char fullpath[256];

		snprintf( fullpath, 256, "/tmp/nv/%s", argv[1]);

	
		if(!strcmp(argv[2],"show"))
		{
			nvram_show_func( fullpath );
			return 0;		
		}

		if( argc < 5)
			usage();

		idx = atoi ( argv[4] );

		if(!strcmp(argv[2],"get"))
		{
			printf("%s%s=%s\n", argv[3] , argv[4], nv_get(argv[1] , argv[3] ,idx)?:"");
			return 0;		
		}
		else if(!strcmp(argv[2],"unset"))
		{
			char name[256];
			snprintf( name, 256, "%s%s", argv[3],argv[4]);
			nvram_unset_func(name, fullpath);
			return 0;		
		}


		if( argc < 6)
			usage();
	
		if(!strcmp(argv[2],"set") )
		{
			nv_set(argv[1] , argv[3] ,idx, argv[5]);
		}
        
	}
	return 0;
}

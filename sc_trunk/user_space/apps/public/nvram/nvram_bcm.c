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
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
//#include <linux/mtd/mtd.h>
#include <sys/file.h>
#include "nvram.h"
#include "nvram_lock.h"

struct nv_entry{
	char *name;
	char *value;
	struct nv_entry *next;
};

static struct nv_entry *nv_list=NULL;

struct nv_entry *find_nv_entry(const char *name)
{
	struct nv_entry *p=nv_list;


	if(!name || !p)
		return NULL;

	while(p){
		if(strcmp(p->name,name)==0)
			break;
		p=p->next;
	}
	return p;
}
int add_nv_entry(const char *name,char *value)
{
	struct nv_entry *p;

	if(!value)
		return -1;

	p=find_nv_entry(name);

	if(!name){
		free(value);
		return -1;
	}
	// exist
	if(p){
		if(p->value)
			free(p->value);
		p->value = value;
	}else{
		struct nv_entry *new_entry;
		new_entry = malloc(sizeof(struct nv_entry));
		if(!new_entry){
			free(value);
			return -1;
		}
		new_entry->name = strdup(name);
		if(!new_entry->name){
			free(value);
			free(new_entry);
			return -1;
		}
		new_entry->value = value;
		new_entry->next=nv_list;
		nv_list = new_entry;
	}
	return 0;
}

static int readFileBin_unlock(char *path, char **data) {
	int total;
	int fd=0;
	if((fd=open(path, O_RDONLY)) < 0)
		return -1;
	total=lseek(fd,0,SEEK_END);
	lseek(fd,0,0);

	if((*data=malloc(total))==NULL){
		close(fd);
		return -1;
	}
	if(read(fd,*data,total)<0){
		free(*data);
		close(fd);
		return -1;
	}
	close(fd);
   	return total;
}

int readFileBin(char *path, char **data) {
	int lock;
	int rc;

	lock = nvram_lock(path);
	rc = readFileBin_unlock(path, data);
	nvram_unlock(lock);
	return rc;
}

static void writeFileBin_unlock(char *path, char *data, int len) {
   int fd;

	if((fd=open(path, O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR)) < 0)
   		return;
	write(fd, data, len);
	close(fd);
}


void writeFileBin(char *path, char *data, int len) {
	int lock;

	lock = nvram_lock(path);
	writeFileBin_unlock(path, data, len);
	nvram_unlock(lock);
	return;
}

int nvram_commit(){
//	printf("nv_bcm: commit\n");
	return NVRAM_SUCCESS;
}

char* nvram_get_func_unlock(const char *name,char *path)
{
	char *bufspace;
	int size;
	char *s,*sp;

	if((size=readFileBin_unlock(path, &bufspace))<0)
	{
		return NULL;
	}

	for (s = bufspace; *s; s++) {
		if (!strncmp(s, name, strlen(name)) && *(s+strlen(name))=='=') {
			sp=malloc(strlen(s)-strlen(name));
			memcpy(sp,(s+strlen(name)+1),(strlen(s)-strlen(name)));
			free(bufspace);
			add_nv_entry(name,sp);
			return sp;
		}
		while(*(++s));
	}
	free(bufspace);

	return NULL;
}

char* nvram_get_func(const char *name,char *path)
{
	char *value;
	int lock;

	lock = nvram_lock(path);
	value = nvram_get_func_unlock(name, path);
	nvram_unlock(lock);

	return value;
}


char* nvram_get(const char *name)
{
	char *pt;
	int mypid=0;
	mypid = getpid();

	if (mypid == 0)	
	{
		/* let gcc463 not complain. */
	}

	if((pt=nvram_get_func(name,NVRAM_BCM_PATH))==NULL){
			if((pt=nvram_get_func(name,NVRAM_DEFAULT))!=NULL)
				nvram_set(name,pt);
			else
			{
#if 0
				if( strcmp("wsc_proc_status", name) != 0
				  && strcmp("wsc_event", name) != 0
				  && strcmp("wsc_config_command", name) != 0
				  && strcmp("wsc_pbc_force", name) != 0)
#endif
	//				printf("nv_bcm: get %s = NULL\n", name);
//				sprintf(cmd,"/bin/echo \"pid=%d  nv_bcmget: %s=%s\" > /dev/console ",mypid,name,pt);
//				system(cmd);

				return NULL;
			}
	}

#if 0
	if( strcmp("wsc_proc_status", name) != 0
	  && strcmp("wsc_event", name) != 0
	  && strcmp("wsc_config_command", name) != 0
	  && strcmp("wsc_pbc_force", name) != 0)
#endif
//		printf("nv_bcm: get %s = %s\n", name, pt);

//				sprintf(cmd,"/bin/echo \"pid=%d  nv_bcmget: %s=%s\" > /dev/console ",mypid,name,pt);
//				system(cmd);

	// [bcm_nvram] s
	if ( pt && !strncmp( pt, "*DEL*", 5 ) ) // check if this is the deleted var
	{
		pt = NULL;
	}
	// [bcm_nvram] e


	return pt;
}

int nvram_set_unlock(const char* name,const char* value)
{
	char *bufspace, *targetspace;
	int size;
	char *sp, *s;
	int found=0;

	if((size=readFileBin_unlock(NVRAM_BCM_PATH, &bufspace))>0) {
	    targetspace=malloc(size+strlen(name)+strlen(value)+2);
	}
	else {
	    targetspace=malloc(strlen(name)+strlen(value)+2);
	}

	sp=targetspace;
	if(size > 0) {
	   for (s = bufspace; *s; s++) {
		if (!strncmp(s, name, strlen(name)) && *(s+strlen(name))=='=') {
			found=1;
  			strcpy(sp, name);
			sp+=strlen(name);
        		*(sp++) = '=';
       			strcpy(sp, value);
			sp+=strlen(value);
			while (*(++s));
		}
		while(*s) *(sp++)=*(s++);
	        *(sp++)=END_SYMBOL;
	    }

		free(bufspace);
	}
	if(!found){
		strcpy(sp, name);
		sp+=strlen(name);
        	*(sp++) = '=';
	        strcpy(sp, value);
		sp+=strlen(value);
	        *(sp++) = END_SYMBOL;
	}

	*(sp) = END_SYMBOL;

	writeFileBin_unlock(NVRAM_BCM_PATH, targetspace, (sp-targetspace)+1);
	free(targetspace);

	return NVRAM_SUCCESS;
}


int nvram_set(const char* name,const char* value)
{
	int lock;

//	printf("nv_bcm: set %s = %s\n", name, value);
	lock = nvram_lock(NVRAM_BCM_PATH);
	nvram_set_unlock(name, value);
	nvram_unlock(lock);
	return NVRAM_SUCCESS;
}


int nvram_unset(const char *name)
{
	int ret;

	/* nvram_unset just set a delete tag (*DEL*). No really deleted, to keep the hash table sequence*/
	ret = nvram_set( name, "*DEL*");

	return ret;
}

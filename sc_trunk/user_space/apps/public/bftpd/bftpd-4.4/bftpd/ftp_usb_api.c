
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "nvram.h"
//#include "rc.h"
#include "ftp_usb_api.h"

//void print_console(const char *format, ...);


SHARE_LIST_T * SHARE_LIST_malloc(void)
{
	SHARE_LIST_T * share_new;
	share_new = malloc ( sizeof(SHARE_LIST_T) );

	if( !share_new)
	{
		return NULL;
	}

	memset( share_new, 0 , sizeof(SHARE_LIST_T) );

	return share_new;
	


}

void SHARE_free( SHARE_T* share)
{

	if( share->shared_name) 
	{
		free(share->shared_name );
		share->shared_name = NULL;
	}
}




void SHARE_LIST_free( SHARE_LIST_T* share_list)
{
	SHARE_LIST_T* next = share_list->next;

	SHARE_free( &( share_list->s));

	if( next)
	{
		SHARE_LIST_free(next);
		free(next);
		share_list->next=NULL;
	}
}

void SHARE_LIST_print(SHARE_LIST_T* shares_in)
{
	SHARE_LIST_T* shares = shares_in;
	bftpd_log( "\n Shere List::\n");

	while( shares)
	{
		bftpd_log( "  Share Name=%s R=%d W=%d\n"
			, shares->s.shared_name?:""
			, shares->s.access_read
			, shares->s.access_write);
		shares=shares->next;
	}
}


SHARE_LIST_T* SHARE_parse( char* shared_name, char* read_auth, char *write_auth)
{
	SHARE_LIST_T* share_new = NULL;

	share_new = SHARE_LIST_malloc();

	if( share_new ==NULL)
		return NULL;


//shared_name
//print_console("sh nm=%s\n", pt);
	share_new->s.shared_name= strdup( shared_name );

//access_read
//print_console("R=%s\n", pt2);
	if(strcmp(read_auth, "admin") == 0)
		share_new->s.access_read= ACCESS_HTTP_USERNAME;
	else if(strcmp(read_auth, "nobody") == 0)
		share_new->s.access_read= ACCESS_GUEST;
	else
		share_new->s.access_read= ACCESS_NOTSHARE;


//access_write
//print_console("W=%s\n", pt);
	if(strcmp(write_auth, "admin") == 0)
		share_new->s.access_write= ACCESS_HTTP_USERNAME;
	else if(strcmp(write_auth, "nobody") == 0)
		share_new->s.access_write= ACCESS_GUEST;
	else
		share_new->s.access_write= ACCESS_NOTSHARE;

	return share_new;
	
}


SHARE_LIST_T* SHARELIST_parse( char* filename)
{

	SHARE_LIST_T * share =NULL;
	SHARE_LIST_T * share_end =NULL;
	SHARE_LIST_T * share_new =NULL;	
	FILE *fp = NULL;
	char tmp[2048], share_name[2048], auth_read[64], auth_write[64];

	if ((fp=fopen(USB_ADVANCED_CONF, "r"))!=NULL) {
		while(fgets(tmp,2048,fp) != NULL)
		{

			sscanf(tmp,"%s\t%s\t%s", share_name, auth_read, auth_write);
//print_console("  %d  pt=%s\n", *pt, pt);


			share_new = SHARE_parse( share_name, auth_read, auth_write);
	
			if( share_new == NULL) {
	//print_console("rc: SHARE_parse error\n");
				continue;
			}
	
			if( share == NULL)
			{
				share = share_new;
				share_end = share_new;
			}
			else
			{
				share_end ->next  = share_new;
				share_end = share_new;
			}
		
		}
		fclose(fp);
	}


	return share;
}

/* check if there is any files that has "ALL - no password" read access control 
   RETURN 
   		0: there is no such file
   		1: there is
*/
int all_file_readable_writable(void)
{
	FILE *fp = NULL;
	char tmp[2048], dir_a[2048]={0,}, read_auth[64], write_auth[64];
	int ret = 1;

	if ((fp=fopen(USB_ADVANCED_CONF, "r"))!=NULL) {
		while(fgets(tmp,2048,fp) != NULL)
		{

			sscanf(tmp,"%s\t%s\t%s", dir_a, read_auth, write_auth);
			if (strcmp(read_auth, "nobody")==0)
				is_file_readable = 1;
			if (strcmp(read_auth, "admin")==0 || strcmp(write_auth, "admin")==0) {
	    		bftpd_log_debug("<not all file readable and writable hit it! hahahahhaha>\n");
				ret = 0;
			}
		}
		fclose(fp);
	}
	
	if (ret == 1)
	    bftpd_log_debug("<all no need admin! hahahahhaha>\n");
	return ret;
	
}


int is_readable(const char *dir)
{
	SHARE_LIST_T *share_list_now = shared_list;
	char *shared_name = NULL;

	
	if(share_list_now == NULL)
		bftpd_log_debug("null share_list_now\n");

	while(share_list_now != NULL)
	{
		shared_name = share_list_now->s.shared_name;
		bftpd_log_debug("shared_name=%s,access_read = %d, access_write=%d\n", 			
		share_list_now->s.shared_name,
		share_list_now->s.access_read,
		share_list_now->s.access_write);
		bftpd_log_debug("dir=%s\n", 	dir);		
		if(((strlen(dir) >= strlen(shared_name) && strncmp(dir, shared_name, strlen(shared_name)) == 0)
			  || (strlen(dir) >= (strlen(shared_name)-1) && strncmp(dir, shared_name, strlen(shared_name)-1) == 0))
			&& share_list_now->s.access_read == ACCESS_HTTP_USERNAME
			&& is_admin_user == 0) {
	    		bftpd_log_debug("<is readable hit it! hahahahhaha>\n");
				return 0;
			}

		share_list_now = share_list_now->next;
	}

//		SD_LIST_free(sdl_now_head);
//		free(sdl_now_head);

	return 1;
	
}

int is_writable(const char *dir)
{
	SHARE_LIST_T *share_list_now = shared_list;
	char *shared_name = NULL;

	
	if(share_list_now == NULL)
		bftpd_log_debug("null share_list_now\n");

	while(share_list_now != NULL)
	{
		shared_name = share_list_now->s.shared_name;
		bftpd_log_debug("shared_name=%s,access_read = %d, access_write=%d\n", 			
		share_list_now->s.shared_name,
		share_list_now->s.access_read,
		share_list_now->s.access_write);
		bftpd_log("dir=%s\n", 	dir);		
		if((strlen(dir) >= strlen(shared_name) && strncmp(dir, shared_name, strlen(shared_name)) == 0)
			&& share_list_now->s.access_write == ACCESS_HTTP_USERNAME
			&& is_admin_user == 0) {
	    		bftpd_log_debug("<writable hit it! hahahahhaha>\n");
				return 0;
			}

		share_list_now = share_list_now->next;
	}

//		SD_LIST_free(sdl_now_head);
//		free(sdl_now_head);

	return 1;
	
}





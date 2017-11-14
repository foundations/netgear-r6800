#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "constant.h"
#include "PRO_file.h"
#include "utility.h"
#include "share.h"
#include "ftp.h"
#include "hd.h"

#ifdef NR65S
#include "volume.h"
#endif

#define BFTP_CONF_FILE "/etc/bftpd.conf"
#define	VSFTPD_CONF_FILE	"/etc/vsftpd.conf"
#define INETD_CONF_FILE "/etc/inetd.conf"
#define	ITUNE_CONF_FILE	"/etc/mt-daapd.conf"
#define	MEDIA_CONF_FILE	"/etc/media.conf"
#define WINAMP_CONF_FILE "/etc/ample.conf"
#define	REAL_MEDIA_CONF	"twonkyvision-mediaserver.ini"
#define	ETC_SERVICE		"/etc/services"

int ReadFTPConf(FTP_SERVER *pconf)
{
	char temp[16]={0};
	FILE *fh;

	fh=fopen(FSH_DS_CONF_FILE,"rt");
	if(!fh)
		return -1;
				
	temp[0]=0;
	PRO_GetStr("network","ftp_server",temp,16,fh);
	if(!strncmp(temp,"yes",3))
		pconf->ftp_server = 1;
	else 

		pconf->ftp_server = 0;
	pconf->ftp_change=0;
	temp[0]=0;
	
	PRO_GetStr("network","ftp_anony",temp,16,fh);
	if(!strncmp(temp,"yes",3))
		pconf->ftp_anon = 1;

	else 
		pconf->ftp_anon = 0;
	temp[0]=0;			
 
 	PRO_GetStr("network","ftp_lang",temp,16,fh);
	if(atoi(temp)>0)
		pconf->ftp_lang = atoi(temp);

	else 
		pconf->ftp_lang = 437;
	temp[0]=0;

 	PRO_GetInt("network","ftp_port",&pconf->ftp_port,fh);
	if(pconf->ftp_port!=21 && (pconf->ftp_port<1024 || pconf->ftp_port>65535))
		pconf->ftp_port = 21;	

 	PRO_GetInt("network","ftp_data_port",&pconf->ftp_data_port,fh);
	if(pconf->ftp_data_port!=20 && (pconf->ftp_data_port<1024 || pconf->ftp_data_port>65535))
		pconf->ftp_data_port = 20;		

	pconf->ftp_passive_ports_fix=0;
 	PRO_GetInt("network","fixed_passive_port",&pconf->ftp_passive_ports_fix,fh);

	pconf->passive_port_max=pconf->passive_port_min=0;
	PRO_GetInt("network","passive_port_min",&pconf->passive_port_min,fh);	
	PRO_GetInt("network","passive_port_max",&pconf->passive_port_max,fh);				
#ifdef _FTPS_
	pconf->ftp_ssl=0;
 	PRO_GetInt("network","ftp_ssl",&pconf->ftp_ssl,fh);
#endif	
	pconf->ftp_bandwidth=0;
 	PRO_GetInt("network","ftp_bandwidth",&pconf->ftp_bandwidth,fh);

	return 0;
}

int SaveFTPConf(FTP_SERVER *pconf)
{
#ifdef _FTPS_
	multi_items	all_items[10];
	char ftp_ssl[6]={0};
#else	
	multi_items	all_items[9];
#endif
	char ftp_enable[6]={0}, ftp_anon[6]={0}, ftp_lan[8]={0},ftp_port[12]={0},ftp_data_port[12]={0},ftp_bandwidth[8]={0};
	int ret;
	char fixed_passive_port[6]={0}, passive_port_min[12]={0}, passive_port_max[12]={0};
	
	strcpy(all_items[0].key,"ftp_server");
	if(pconf->ftp_server)
		strcpy(ftp_enable,"yes");
	else
		strcpy(ftp_enable,"no");	
	all_items[0].value=ftp_enable;

	strcpy(all_items[1].key,"ftp_anony");
	if(pconf->ftp_anon)
		strcpy(ftp_anon,"yes");
	else
		strcpy(ftp_anon,"no");	
	all_items[1].value=ftp_anon;

	strcpy(all_items[2].key,"ftp_lang");
	sprintf(ftp_lan,"%d",pconf->ftp_lang);
	all_items[2].value=ftp_lan;

	strcpy(all_items[3].key,"ftp_port");
	sprintf(ftp_port,"%d",pconf->ftp_port);
	all_items[3].value=ftp_port;

	strcpy(all_items[4].key,"ftp_data_port");
	sprintf(ftp_data_port,"%d",pconf->ftp_data_port);
	all_items[4].value=ftp_data_port;
		
	strcpy(all_items[5].key,"ftp_bandwidth");
	sprintf(ftp_bandwidth,"%d",pconf->ftp_bandwidth);
	all_items[5].value=ftp_bandwidth;	

	strcpy(all_items[6].key,"fixed_passive_port");
	sprintf(fixed_passive_port,"%d",pconf->ftp_passive_ports_fix);
	all_items[6].value=fixed_passive_port;
	
	strcpy(all_items[7].key,"passive_port_min");
	sprintf(passive_port_min,"%d",pconf->passive_port_min);
	all_items[7].value=passive_port_min;	
	
	strcpy(all_items[8].key,"passive_port_max");
	sprintf(passive_port_max,"%d",pconf->passive_port_max);
	all_items[8].value=passive_port_max;		
		
#ifdef _FTPS_
	strcpy(all_items[9].key,"ftp_ssl");
	sprintf(ftp_ssl,"%d",pconf->ftp_ssl);
	all_items[9].value=ftp_ssl;
#endif	

#ifdef _FTPS_
	ret=PRO_SetMultiItemStr("network",all_items,10, FSH_DS_CONF_FILE);
#else
	ret=PRO_SetMultiItemStr("network",all_items,9, FSH_DS_CONF_FILE);
#endif	
	if(ret)
		return -1;

	return ret;
}

int ResetFTP(void)
{
	char TmpFile[]="/tmp/tmpfile.XXXXXX",buf[128]={0};
	int anony=0,ret=0;
	FILE *fh,*fh1;
	int fd,find=0;
	FTP_SERVER conf;
	FILE * fh2;
	char guest[FSH_MaxEnableLen];
	int find0=0, find1=0, find2=0, find3=0, find4=0, find5=0;
	     
	ret=ReadFTPConf(&conf);
	if(ret)
		return(FSH_RESET_FTP_ERR);	
	if(strcmp(buf,"yes")==0)
		anony=1;
	else
		anony=0;
	fd=mkstemp(TmpFile);
	if(fd==-1)
		return FSH_CREATE_TMPFILE_ERR;
	close(fd);
	fh=fopen(ETC_SERVICE,"rt");
	if(!fh)
		return(FSH_FILE_OPEN_ERR);
	fh1=fopen(TmpFile,"wt");
	while(fgets(buf,sizeof(buf),fh)){
		if(strstr(buf,"ftp\t\t"))	{
			if(conf.ftp_port)
				fprintf(fh1,"ftp\t\t%d/tcp\n",conf.ftp_port);
		}
		else
			fputs(buf,fh1);		
	}
	fclose(fh);
	fclose(fh1);	
	sprintf(buf,"/bin/cp -f %s %s 2>/dev/null",TmpFile,ETC_SERVICE);
	ret=system(buf);
	
	if(ret && errno!=ECHILD){
		remove(TmpFile);		
		return(FSH_RESET_FTP_ERR);
	}
	remove(TmpFile);
	strcpy(TmpFile,"/tmp/tmpfile.XXXXXX");
	fd=mkstemp(TmpFile);
	if(fd>=0)
		close(fd);
#ifdef _FTPS_
	fh2 = fopen(FSH_DS_CONF_FILE, "rt");
	if (!fh2)
		return (FSH_FILE_OPEN_ERR);
	ret =
		PRO_GetStr("network", "guest_access", guest, FSH_MaxEnableLen, fh2);
	fclose(fh2);
	if (ret)
		return (FSH_RESET_GUEST_ERR);
	fh=fopen(VSFTPD_CONF_FILE,"rt");
	if(!fh)
		return(FSH_FILE_OPEN_ERR);
	fh1=fopen(TmpFile,"wt");
	while(fgets(buf,sizeof(buf),fh)){
		if(strstr(buf,"ssl_enable")){
			if(conf.ftp_ssl)
				fputs("ssl_enable=YES\n",fh1);
			else
				fputs("ssl_enable=NO\n",fh1);
		}
		else if(strstr(buf,"anonymous_enable"))	{
			if(conf.ftp_anon)
				fputs("anonymous_enable=YES\n",fh1);
			else
				fputs("anonymous_enable=NO\n",fh1);
		}
		else if(strstr(buf,"userlist_enable"))	{
			if(strcmp(guest,"yes")!=0)
				fputs("userlist_enable=YES\n",fh1);
			else
				fputs("userlist_enable=NO\n",fh1);
		}
		else if(strstr(buf,"listen_port"))	{
			find2=1;
			fprintf(fh1,"listen_port=%d\n",conf.ftp_port);
		}
		else if(strstr(buf,"ftp_data_port"))	{
			find3=1;
			fprintf(fh1,"ftp_data_port=%d\n",conf.ftp_data_port);
		}						

		else if(strstr(buf,"pasv_min_port"))	{
			find4=1;
			if(conf.ftp_passive_ports_fix)
				fprintf(fh1,"pasv_min_port=%d\n",conf.passive_port_min);
			else
				fprintf(fh1,"pasv_min_port=%d\n",5001);
		}	
		else if(strstr(buf,"pasv_max_port"))	{
			find5=1;
			if(conf.ftp_passive_ports_fix)
				fprintf(fh1,"pasv_max_port=%d\n",conf.passive_port_max);
			else
				fprintf(fh1,"pasv_max_port=%d\n",0);
		}		
					
		else if(strstr(buf,"anon_max_rate"))	{
			find0=1;
			fprintf(fh1,"anon_max_rate=%llu\n",((unsigned long long)conf.ftp_bandwidth)*1024);
		}	
		else if(strstr(buf,"local_max_rate"))	{
			find1=1;
			fprintf(fh1,"local_max_rate=%llu\n",((unsigned long long)conf.ftp_bandwidth)*1024);
		}				
		else
			fputs(buf,fh1);
	}
	if(!find0)
		fprintf(fh1,"anon_max_rate=%llu\n",((unsigned long long)conf.ftp_bandwidth)*1024);
	if(!find1)
		fprintf(fh1,"local_max_rate=%llu\n",((unsigned long long)conf.ftp_bandwidth)*1024);
	if(!find2)
		fprintf(fh1,"listen_port=%d\n",conf.ftp_port);
	if(!find3)
		fprintf(fh1,"ftp_data_port=%d\n",conf.ftp_data_port);	
		
	if(!find4){
		if(conf.ftp_passive_ports_fix)
			fprintf(fh1,"pasv_min_port=%d\n",conf.passive_port_min);
		else
			fprintf(fh1,"pasv_min_port=%d\n",5001);
	}	
	if(!find5){
		if(conf.ftp_passive_ports_fix)
			fprintf(fh1,"pasv_max_port=%d\n",conf.passive_port_max);
		else
			fprintf(fh1,"pasv_max_port=%d\n",0);
	}				
	fclose(fh);
	fclose(fh1);
	sprintf(buf,"/bin/cp -f %s %s 2>/dev/null",TmpFile,VSFTPD_CONF_FILE);
#else		
	fh=fopen(BFTP_CONF_FILE,"rt");
	if(!fh)
		return(FSH_FILE_OPEN_ERR);
	fh1=fopen(TmpFile,"wt");
	while(fgets(buf,sizeof(buf),fh)){
		if(strstr(buf,"ALIAS"))	{
			if(conf.ftp_anon)
				fputs("ALIAS=\"guest\"\n",fh1);
			else
				fputs("ALIAS=\"nobody\"\n",fh1);
			
		}
		else
			fputs(buf,fh1);		
	}
	fclose(fh);
	fclose(fh1);
	sprintf(buf,"/bin/cp -f %s %s 2>/dev/null",TmpFile,BFTP_CONF_FILE);
#endif	
	ret=system(buf);
	if(ret && errno!=ECHILD){
		remove(TmpFile);		
		return(FSH_RESET_FTP_ERR);
	}
	remove(TmpFile);
	strcpy(TmpFile,"/tmp/tmpfile.XXXXXX");
	fd=mkstemp(TmpFile);
	if(fd>=0)
		close(fd);	
	fh=fopen(INETD_CONF_FILE,"rt");
	if(!fh)
		return(FSH_FILE_OPEN_ERR);
	fh1=fopen(TmpFile,"wt");
	if (!fh1) {
		fclose(fh);
		return(FSH_FILE_OPEN_ERR);
	}
	while(fgets(buf,sizeof(buf),fh)){
		if(strstr(buf,"ftp")){
			if(conf.ftp_server)
#ifdef _FTPS_			
				fputs("ftp\tstream\ttcp\tnowait\troot\t/usr/sbin/vsftpd\n",fh1);
#else
				fputs("ftp\tstream\ttcp\tnowait\troot\t/usr/sbin/bftpd\n",fh1);
#endif				
			find=1;
		}

		else
			fputs(buf,fh1);
	}
	if(!find && conf.ftp_server)
#ifdef _FTPS_			
		fputs("ftp\tstream\ttcp\tnowait\troot\t/usr/sbin/vsftpd\n",fh1);
#else
		fputs("ftp\tstream\ttcp\tnowait\troot\t/usr/sbin/bftpd\n",fh1);
#endif	
	fclose(fh1);

	fclose(fh);
	sprintf(buf,"/bin/cp -f %s %s 2>/dev/null",TmpFile,INETD_CONF_FILE);
	system(buf);

	if(system(buf)){
		remove(TmpFile);
		return(FSH_RESET_FTP_ERR);
	}
	remove(TmpFile);
	chmod(INETD_CONF_FILE,0644);
	system("/bin/killall inetd 2>/dev/null");
	system("/bin/killall bftpd 2>/dev/null");
	if(fork()==0){
		setsid();
	//	for(i=0;i<64;i++)
	//		close(i);
		if(fork()>0)
			exit(0);
		chdir("/");
		umask(0);
		signal(SIGCHLD, SIG_DFL);
		sleep(1);
		execl("/etc/rc.d/rc.xinetd","rc.xinetd",NULL);
		exit(0);
	}	

	return(FSH_OK);
}

int ReadiTuneConf(ITUNE *pConf)
{
	char temp[16]={0},hostname[64]={0};
	FILE *fh;
	
	memset(pConf, '\0', sizeof(UPNP_AV));
	fh=fopen(FSH_DS_CONF_FILE,"rt");
	if(!fh)
		return -1;	
	PRO_GetStr("media","itune",temp,16,fh);
	if(!strncmp(temp,"yes",3))
		pConf->itune = 1;
	else
		pConf->itune = 0;
	PRO_GetStr("media","itune_name",pConf->itune_name,MAX_UPNP_NAME_LEN,fh);	
	PRO_GetStr("media","itune_path",pConf->itune_path,MAX_AV_PATH_LEN,fh);
	PRO_GetStr("network","disk_server_name",hostname,64,fh);	
	if(!strlen(pConf->itune_name))
		sprintf(pConf->itune_name, "%s_iTunes",hostname);

	fclose(fh);
	return 0;
}

int SaveiTuneConf(ITUNE *pConf)
{
	multi_items	all_items[3];
	int ret=0;
	
	strcpy(all_items[0].key,"itune");
	if (pConf->itune)
		all_items[0].value="yes";
	else
		all_items[0].value="no";

	strcpy(all_items[1].key,"itune_name");
	all_items[1].value=pConf->itune_name;

	strcpy(all_items[2].key,"itune_path");
	all_items[2].value=pConf->itune_path;
	
	PRO_SetStr("media", "itune", all_items[0].value, FSH_DS_CONF_FILE);
	ret=PRO_SetMultiItemStr("media",all_items,3, FSH_DS_CONF_FILE);
	if(ret){
		return -1;
	}
    return 0;
}

int ReadBroadConf(BROAD *pConf)
{
	char temp[16]={0},hostname[64]={0};
	FILE *fh;
	
	memset(pConf, '\0', sizeof(UPNP_AV));
	fh=fopen(FSH_DS_CONF_FILE,"rt");
	if(!fh)
		return -1;	
	PRO_GetStr("media","music_broad",temp,16,fh);
	if(!strncmp(temp,"yes",3))
		pConf->broad = 1;
	else
		pConf->broad = 0;
	PRO_GetStr("media","broad_name",pConf->broad_name,MAX_UPNP_NAME_LEN,fh);	
	PRO_GetInt("media","broad_port",&pConf->broad_port,fh);		
	PRO_GetStr("media","broad_path",pConf->broad_path,MAX_AV_PATH_LEN,fh);
	PRO_GetStr("media","play_method",pConf->play_method,12,fh);
	PRO_GetStr("network","disk_server_name",hostname,64,fh);	
	fclose(fh);
	if(!pConf->broad_port)
		pConf->broad_port=1234;
	if(!strlen(pConf->broad_name))
		sprintf(pConf->broad_name, "%s_MP3",hostname);
		
	return 0;
}

int SaveBroadConf(BROAD *pConf)
{
	multi_items	all_items[5];
	int ret=0;
	char port_str[12]={0};
	
	strcpy(all_items[0].key,"music_broad");
	if (pConf->broad)
		all_items[0].value="yes";
	else
		all_items[0].value="no";

	strcpy(all_items[1].key,"broad_name");
	all_items[1].value=pConf->broad_name;

	strcpy(all_items[2].key,"broad_path");
	all_items[2].value=pConf->broad_path;

	strcpy(all_items[3].key,"broad_port");
	sprintf(port_str, "%d",pConf->broad_port);
	all_items[3].value=port_str;

	strcpy(all_items[4].key,"play_method");
	all_items[4].value=pConf->play_method;
			
	PRO_SetStr("media", "broad", all_items[0].value, FSH_DS_CONF_FILE);
	ret=PRO_SetMultiItemStr("media",all_items,5, FSH_DS_CONF_FILE);
	if(ret){
		return -1;
	}
    return 0;
}

int ReadUPnPAVConf(UPNP_AV *pConf)
{
	int num=0, no_any_dir=1;
	char temp[16]={0},key_str[32]={0},hostname[64]={0};
	FILE *fh;
//	int nValidNum[DSK_NUM]={0}, nInvalidNum[DSK_NUM]={0};
	
	memset(pConf, '\0', sizeof(UPNP_AV));
	fh=fopen(FSH_DS_CONF_FILE,"rt");
	if(!fh)
		return -1;	
	PRO_GetStr("media","upnp_av",temp,16,fh);
	if(!strncmp(temp,"yes",3))
		pConf->upnp_av = 1;
	else
		pConf->upnp_av = 0;
	PRO_GetStr("media","upnp_name",pConf->upnp_name,MAX_UPNP_NAME_LEN,fh);	
	PRO_GetStr("network","disk_server_name",hostname,64,fh);	
	if(!strlen(pConf->upnp_name))
		sprintf(pConf->upnp_name, "%s_Media",hostname);
	temp[0]=0;
	PRO_GetStr("media","scan_interval",temp, sizeof(temp),fh);
	pConf->content_scan=-1;
	PRO_GetInt("media","content_scan",&pConf->content_scan,fh);
	pConf->scan_interval=strtoul(temp,NULL,10);
	for(;num<FOLDER_NUM;num++){
		sprintf(key_str, "upnp_av_folder_%d", num+1);
		PRO_GetInt("media",key_str,&(pConf->upnp_av_folder[num]),fh);
		sprintf(key_str, "upnp_av_share_%d", num+1);
		PRO_GetStr("media",key_str,pConf->upnp_av_share[num],FSH_MaxShareLen,fh);
		sprintf(key_str, "upnp_av_path_%d", num+1);
		PRO_GetStr("media",key_str,pConf->upnp_av_path[num],MAX_AV_PATH_LEN,fh);
		if(strlen(pConf->upnp_av_path[num]))
			no_any_dir=0;
		pConf->content_type[num]=0;
		sprintf(key_str, "content_type_%d", num+1);
		PRO_GetInt("media",key_str,&pConf->content_type[num],fh);		
	}	
	pConf->code_page=437;
	PRO_GetInt("network","code_page",&pConf->code_page,fh);
	PRO_GetInt("network","port",&pConf->device_port,fh);
	PRO_GetStr("network","version",pConf->fw_version,sizeof(pConf->fw_version)-1,fh);
#if 0			
	
	if(no_any_dir){
		DSK_DetectSysDisk(nValidNum,nInvalidNum);
		if(nValidNum[FIRST_DSK]){
			sprintf(pConf->upnp_av_path[0], "/%s 1", FSH_HDD_SHARE);
			pConf->upnp_av_folder[0]=1;
		}
		if(nValidNum[SECOND_DSK]){
			if(pConf->upnp_av_folder[0]){
				sprintf(pConf->upnp_av_path[1], "/%s 2", FSH_HDD_SHARE);
				pConf->upnp_av_folder[1]=1;				
			}
			else{
				sprintf(pConf->upnp_av_path[0], "/%s 2", FSH_HDD_SHARE);
				pConf->upnp_av_folder[0]=1;
			}
		}		
		if(nValidNum[USB_DISK_1]){
			sprintf(pConf->upnp_av_path[0], "/%s", FSH_HDD_SHARE);
			pConf->upnp_av_folder[0]=1;
		}			
	}
#endif	
	fclose(fh);
	fh=fopen(FSH_DEF_DS_CONF_FILE,"rt");
	if(fh){
		PRO_GetStr("network","version",pConf->fw_version,sizeof(pConf->fw_version)-1,fh);
		fclose(fh);
	}
	return 0;
}

void ResetUPnPAV2Def(void)
{
	FILE *fp=NULL;
	int ret=0;
	int i=0;
	char tmp_str[312]={0};
#define	MAX_ITEMS_SYS	24		
	multi_items item[MAX_ITEMS_SYS];
	char *media_keys[]={
			"upnp_av",		 
			"upnp_name",
			"scan_interval",
			"upnp_av_folder_1",
			"upnp_av_share_1",
			"upnp_av_path_1",
			"upnp_av_folder_2",
			"upnp_av_share_2",
			"upnp_av_path_2",
			"upnp_av_folder_3",
			"upnp_av_share_3",
			"upnp_av_path_3",
			"upnp_av_folder_4",
			"upnp_av_share_4",
			"upnp_av_path_4"						
	};
	fp=fopen(FSH_DEF_DS_CONF_FILE,"r");
	if(!fp)
		return;		

	for(i=0;i< sizeof(media_keys)/sizeof(char *);i++){
		 strcpy(item[i].key,media_keys[i]);
		 ret=PRO_GetStr("media",item[i].key,tmp_str,sizeof(tmp_str)-1,fp);	
		  if(!ret && strlen(tmp_str))
		  	item[i].value=strdup(tmp_str);
		else
		  	item[i].value="";
	}
	PRO_SetMultiItemStr("media",item,sizeof(media_keys)/sizeof(char *),FSH_DS_CONF_FILE);
	for(i=0;i< sizeof(media_keys)/sizeof(char *);i++){
		if(strlen(item[i].value)){
			free(item[i].value);
			item[i].value=NULL;
		}
	}	
	fclose(fp);
	system("/etc/rc.d/twonkymedia.sh stop >/dev/null 2>&1");
	return;
}

int SaveUPnPAVConf(UPNP_AV *pConf)
{
	multi_items	all_items[4+4*FOLDER_NUM];
	int ret=0,num=0,i=0;
	char scan_interval_str[12]={0}, av_folder_str[FOLDER_NUM][4]={{0}}, content_type_str[FOLDER_NUM][4]={{0}};
	char content_scan[12]={0};
	
	strcpy(all_items[0].key,"upnp_av");
	if (pConf->upnp_av)
		all_items[0].value="yes";
	else
		all_items[0].value="no";

	strcpy(all_items[1].key,"upnp_name");
	all_items[1].value=pConf->upnp_name;

	strcpy(all_items[2].key,"scan_interval");
	sprintf(scan_interval_str, "%lu",pConf->scan_interval);
	all_items[2].value=scan_interval_str;
	
	strcpy(all_items[3].key,"content_scan");
	sprintf(content_scan, "%d",pConf->content_scan);
	all_items[3].value=content_scan;	
	i=3;
	for(;num<FOLDER_NUM;num++){
		sprintf(all_items[++i].key, "upnp_av_folder_%d", num+1);
		sprintf(av_folder_str[num], "%d",pConf->upnp_av_folder[num]);
		all_items[i].value=av_folder_str[num];
	
		sprintf(all_items[++i].key, "upnp_av_share_%d", num+1);
		all_items[i].value=pConf->upnp_av_share[num];
	
		sprintf(all_items[++i].key, "upnp_av_path_%d", num+1);
		all_items[i].value=pConf->upnp_av_path[num];
		
		sprintf(all_items[++i].key, "content_type_%d", num+1);
		sprintf(content_type_str[num], "%d",pConf->content_type[num]);
		all_items[i].value=content_type_str[num];
	}
	PRO_SetStr("media", "upnp_av", all_items[0].value, FSH_DS_CONF_FILE);
	ret=PRO_SetMultiItemStr("media",all_items,4+4*FOLDER_NUM, FSH_DS_CONF_FILE);
	if(ret){
		return -1;
	}
    return 0;
}

int ResetItune(void)
{
	char TmpFile[]="/tmp/tmpfile.XXXXXX",buf[1024]={0};
	FILE *fh,*fh1;
	int fd,ret=0;
	sh_info s_info;
	ITUNE	itune_conf;
	char sh_name[FSH_MaxShareLen+1]={0}, subfolder[FSH_MaxDirLen+1]={0};
	
	ret=ReadiTuneConf(&itune_conf);
	if(ret)
		return ret;
	system("/bin/killall mt-daapd 2>/dev/null");
	sleep(2);
	if(itune_conf.itune==0)
		return 0;
	ret=StripShareAndFolder(itune_conf.itune_path, sh_name, subfolder, 0);
	if(ret){//-2: Folder doesn't exist.
		if(ret==-2)
			PRO_SetStr("media","itune","no",FSH_DS_CONF_FILE);
		return -1;
	}

	strcpy(s_info.name, sh_name);
	ret=GetShareInfo(&s_info);
	if(ret){
		PRO_SetStr("media","itune","no",FSH_DS_CONF_FILE);
		return ret;
	}	
	fh=fopen(ITUNE_CONF_FILE,"rt");
	if(!fh)
		return -1;
	fd=mkstemp(TmpFile);
	if(fd>0)
		close(fd);		
	fh1=fopen(TmpFile,"wt");
	while(fgets(buf,sizeof(buf),fh)){
		if(strstr(buf,"mp3_dir")==buf){
			fprintf(fh1,"mp3_dir\t\t"FSH_MNT_PREFIX"/"FSH_DATA_NAME"%s/%s\n",s_info.hard_index,s_info.dir,subfolder);
		}
		else if(strstr(buf,"port")==buf){
			fprintf(fh1,"port\t\t%s\n","3689");
		}
		
		else if(strstr(buf,"servername")==buf){
			fprintf(fh1,"servername\t\t%s\n",itune_conf.itune_name);
		}
		else
			fputs(buf,fh1);		
	}
	fclose(fh);
	fclose(fh1);
	sprintf(buf,"/bin/cp -f %s %s 2>/dev/null",TmpFile,ITUNE_CONF_FILE);
	ret=system(buf);
	if(ret){
		remove(TmpFile);		
		return -1;
	}
	remove(TmpFile);	
	chmod(ITUNE_CONF_FILE,0644);
	remove("/tmp/songs.gdb");
#if 0
	if(fork()==0){
		setsid();
		for(i=0;i<64;i++)
			close(i);
		if(fork()>0)
			exit(0);
		chdir("/");
		umask(0);
		signal(SIGCLD,SIG_IGN);
		sleep(1);
		execl("/usr/sbin/mt-daapd","mt-daapd",NULL);
		exit(0);
	}
#else
	system("/usr/sbin/mt-daapd &>/dev/null");		
#endif
	return(FSH_OK);
}

int StripShareAndFolder(char *pSrc, char *pShare, char *pDir, int checkonly)
{
	char *p_1=NULL, *p_2=NULL, *ch=NULL;
	char *val=NULL, dir[312]={0}, tmp_str[600]={0};
	sh_info info;
	int ret=0;
		
	val=pSrc;				
	while(*(val+1)=='/')
		val++;
	strcpy(dir,val);
	ch=strstr(dir,"//");
	while(ch){
		*ch=0;
		strcat(dir,(ch+1));
		ch=strstr(dir,"//");
	}
	p_1=strchr(dir, '/');
	if(!p_1){
		return -1;
	}
	p_1++;
	p_2=strchr(p_1, '/');
	if(p_2){
		*p_2=0;
		p_2++;	
	}

	strcpy(info.name, p_1);
	ret=GetShareInfo(&info);
	if(ret)
		return -1;
	if(p_2)
		sprintf(tmp_str, FSH_MNT_PREFIX"/data%s/%s",info.hard_index,info.dir,p_2);
	else
		sprintf(tmp_str, FSH_MNT_PREFIX"/data%s",info.hard_index,info.dir);
	if(access(tmp_str, F_OK))
		return -2;
	if(!checkonly){
		strcpy(pShare, p_1);
		if(p_2)
			sprintf(pDir,"/%s",p_2);
		else
			pDir[0]=0;
	}
	return 0;
}					

int IsFolderReadonly(char *pPath)
{
	FILE *fp=NULL;
	char tmp[128];
	
	if(!pPath)
		return 0;
	fp=fopen("/proc/mounts","r");
	if(!fp)
		return 0;
	while(fgets(tmp,sizeof(tmp),fp)){
		if(strstr(tmp,pPath) && strstr(tmp, "ntfs") && strstr(tmp, "ro,")){
			fclose(fp);
			tmp[0]=0;
			return 1;
		}
	}
	fclose(fp);
	tmp[0]=0;
	return 0;	
}

#ifdef __SERCOMM_MEDIA_SERVER__
void StopUPnPDaemon(UPNP_AV *pAVConf, int del_db)
{
	FILE *fp=NULL;
	int pid=-1, i=0;
#ifdef __SERCOMM_MEDIA_SERVER__
	int nValidNum[DSK_NUM]={0}, nInvalidNum[DSK_NUM]={0};
	UPNP_AV	av_conf, *pAV=NULL;
	char sh_name[FSH_MaxShareLen+1]={0}, subfolder[FSH_MaxDirLen+1]={0}, buf[128]={0};
	int ret=0;

	if(pAVConf)
		pAV=pAVConf;
	else{
		ret=ReadUPnPAVConf(&av_conf);
		if(ret)
			goto stop_process;
		pAV=&av_conf;
	}
	for(;i<FOLDER_NUM;i++){
		if(pAV->upnp_av_folder[i]==0)
			continue;
		ret=StripShareAndFolder(pAV->upnp_av_path[i], sh_name, subfolder, 0);
		if(ret)
			continue;
		if(strlen(sh_name) && (strstr(sh_name,FSH_HARD_SHARE)==sh_name || strstr(sh_name,FSH_FLASH_SHARE)==sh_name))
			del_db=1;
	}	
	if(del_db){
		DSK_DetectSysDisk(nValidNum,nInvalidNum);
		if(nValidNum[FIRST_DSK]==1){
			sprintf(buf,FSH_MNT_PREFIX"/conf/media.db/media_server.db",FIRST_DSK+1);
			remove(buf);
			sprintf(buf,FSH_MNT_PREFIX"/conf/media.db/db_scan.ok",FIRST_DSK+1);
			remove(buf);
		}
		else if(nValidNum[SECOND_DSK]==1){		
			sprintf(buf,FSH_MNT_PREFIX"/conf/media.db/media_server.db",SECOND_DSK+1);
			remove(buf);		
			sprintf(buf,FSH_MNT_PREFIX"/conf/media.db/db_scan.ok",SECOND_DSK+1);
			remove(buf);			
		}
#ifdef NR65S		
		else if(nValidNum[DSK_VOL]==1){		
			sprintf(buf,FSH_MNT_PREFIX"/conf/media.db/media_server.db",DSK_VOL+1);
			remove(buf);		
			sprintf(buf,FSH_MNT_PREFIX"/conf/media.db/db_scan.ok",DSK_VOL+1);
			remove(buf);			
		}
#endif				
	}	

stop_process:		
#endif	
	fp=fopen(MEDIA_SERVER_PID,"rt");
	if(fp){
		fscanf(fp,"%d", &pid);	
		fclose(fp);
	}
	if(pid>0){
		kill(pid, SIGTERM);	
		sleep(4);
	}	
	return;
}

void ScanUPnPAVServer(void)
{
	FILE *fp=NULL;
	int pid=-1;
	
	fp=fopen(MEDIA_SCAN_PID,"rt");
	if(fp){
		fscanf(fp,"%d", &pid);	
		fclose(fp);
	}
	if(pid>0)
		kill(pid, SIGHUP);		
	return;
}

void ReloadUPnPAVServerIP(void)
{
	FILE *fp=NULL;
	int pid=-1;
	char device_port[8]="80";
	
	fp=fopen(MEDIA_SERVER_PID,"rt");
	if(fp){
		fscanf(fp,"%d", &pid);	
		fclose(fp);
	}

	fp=fopen(FSH_DS_CONF_FILE,"rt");
	if(fp){
		PRO_GetStr("network","port",device_port,6,fp);
		fclose(fp);
		PRO_SetStr(NULL,"device_port",device_port,MEDIA_CONF_FILE);	
	}
	if(pid>0)
		kill(pid, SIGHUP);		
	return;
}
#endif

void RemoveDBOKFlag(void)
{
	int disk_index=0;
	char buf[256]={0};
	
	disk_index=FirstAvaibleVolume();
	if(disk_index<0)
		return;	
	sprintf(buf,FSH_MNT_PREFIX"/conf/media.db/db_scan.ok",disk_index);
	if(!access(buf, F_OK))
		remove(buf);
	return;
}

int ResetUPnPAV(int fast_scan)
{
	char buf[(MAX_AV_PATH_LEN+FSH_MaxDirLen+64)*FOLDER_NUM]={0};
	char upnp_av_path[FSH_MaxDirLen*2+64]={0},db_file[256]={0},db_scan_ok[256]={0},content_type[FOLDER_NUM][8]={"+A|", "+M|","+P|","+V|"};
	int ret=0,i=0,len=0;
	sh_info s_info;
	int nValidNum[DSK_NUM]={0}, nInvalidNum[DSK_NUM]={0},hard_index=-1;
	UPNP_AV	av_conf;
	char scan_time_str[12]={0};
	char sh_name[FSH_MaxShareLen+1]={0}, subfolder[FSH_MaxDirLen+1]={0};
	char db_path[FSH_MaxDirLen+64]={0},code_page_str[8]={0};
	char device_port_str[10]={0};
	int db_ok=0;
	char fw_version[24]="V", *p=NULL;
	
	ret=ReadUPnPAVConf(&av_conf);
	if(ret)
		return ret;
#ifdef __SERCOMM_MEDIA_SERVER__		
	if(fast_scan)
		StopUPnPDaemon(&av_conf, 0);
	else	
		StopUPnPDaemon(&av_conf, 1);
#else
	system("/etc/rc.d/twonkymedia.sh stop >/dev/null 2>&1");
#endif
	sleep(4);
	if(av_conf.upnp_av==0)
		return 0;
	for(;i<FOLDER_NUM;i++){
		if(av_conf.upnp_av_folder[i]==0)
			continue;
		ret=StripShareAndFolder(av_conf.upnp_av_path[i], sh_name, subfolder, 0);
		if(ret)
			continue;
		strcpy(s_info.name, sh_name);
		ret=GetShareInfo(&s_info);
		if(ret){
			continue;
		}	
		if(hard_index==-1){
			sprintf(upnp_av_path,FSH_MNT_PREFIX"/data%s",s_info.hard_index, s_info.dir);
			len=strlen(upnp_av_path);
			if(len && upnp_av_path[len-1]=='/')
				upnp_av_path[len-1]=0;
			
			if(len && !IsFolderReadonly(upnp_av_path)){
				hard_index=s_info.hard_index;
				strcpy(db_path, s_info.dir);
			}
		}
		if(strlen(subfolder)>1)
			sprintf(upnp_av_path,"%s"FSH_MNT_PREFIX"/data%s/%s",content_type[av_conf.content_type[i]], s_info.hard_index, s_info.dir,subfolder+1);
		else
			sprintf(upnp_av_path,"%s"FSH_MNT_PREFIX"/data%s",content_type[av_conf.content_type[i]],s_info.hard_index, s_info.dir);
		if(strlen(buf)){
			strcat(buf,",");	
			strcat(buf, upnp_av_path);
		}
		else
			strcat(buf, upnp_av_path);
	}
	
	if(strlen(buf)){
		ret=PRO_SetStr(NULL,"contentdir",buf,MEDIA_CONF_FILE);
	}
	PRO_SetStr(NULL,"friendlyname",av_conf.upnp_name,MEDIA_CONF_FILE);
	if(av_conf.content_scan==-1)
		sprintf(scan_time_str, "%d", -1);
	else
		sprintf(scan_time_str, "%lu", av_conf.scan_interval);
	
#ifdef __SERCOMM_MEDIA_SERVER__
	PRO_SetStr(NULL,"scan_interval",scan_time_str,MEDIA_CONF_FILE);
#else
	PRO_SetStr(NULL,"scantime",scan_time_str,MEDIA_CONF_FILE);
#endif	

	sprintf(code_page_str, "%d", av_conf.code_page);
	PRO_SetStr(NULL,"codepage",code_page_str,MEDIA_CONF_FILE);
	strcat(fw_version,av_conf.fw_version);
	p=strrchr(fw_version,'.');
	if(p)
		*p='R';	
	PRO_SetStr(NULL,"modelnumber",fw_version,MEDIA_CONF_FILE);
	PRO_SetStr(NULL,"serialnumber",av_conf.fw_version,MEDIA_CONF_FILE);
	sprintf(device_port_str, "%d", av_conf.device_port);
	PRO_SetStr(NULL,"device_port",device_port_str,MEDIA_CONF_FILE);
	DSK_DetectSysDisk(nValidNum,nInvalidNum);
	if(nValidNum[FIRST_DSK]==1){
#ifdef __SERCOMM_MEDIA_SERVER__
		sprintf(buf,FSH_MNT_PREFIX"/conf/media.db",FIRST_DSK+1);
		PRO_SetStr(NULL,"dbfile",buf,MEDIA_CONF_FILE);	
		sprintf(db_scan_ok,"%s/%s",buf, "db_scan.ok");
		if(!access(db_scan_ok, F_OK))
			db_ok=1;
#else		
		sprintf(buf,FSH_MNT_PREFIX"/conf/twonkymedia.db",FIRST_DSK+1);
		PRO_SetStr(NULL,"dbdir",buf,MEDIA_CONF_FILE);	
#endif		
		
		//sprintf(db_file,"/bin/rm -rf %s",buf);	
		//system(db_file);		
#ifndef __SERCOMM_MEDIA_SERVER__		
		sprintf(buf,FSH_MNT_PREFIX"/conf/twonkymedia.db/cache",FIRST_DSK+1);
		PRO_SetStr(NULL,"cachedir",buf,MEDIA_CONF_FILE);
#endif			
	}
	else if(nValidNum[SECOND_DSK]==1){
#ifdef __SERCOMM_MEDIA_SERVER__
		sprintf(buf,FSH_MNT_PREFIX"/conf/media.db",SECOND_DSK+1);
		PRO_SetStr(NULL,"dbfile",buf,MEDIA_CONF_FILE);	
		sprintf(db_scan_ok,"%s/%s",buf, "db_scan.ok");
		if(!access(db_scan_ok, F_OK))
			db_ok=1;		
#else		
		sprintf(buf,FSH_MNT_PREFIX"/conf/twonkymedia.db",SECOND_DSK+1);
		PRO_SetStr(NULL,"dbdir",buf,MEDIA_CONF_FILE);	
#endif		
		//sprintf(db_file,"/bin/rm -rf %s",buf);	
		//system(db_file);		
#ifndef __SERCOMM_MEDIA_SERVER__		
		sprintf(buf,FSH_MNT_PREFIX"/conf/twonkymedia.db/cache",SECOND_DSK+1);
		PRO_SetStr(NULL,"cachedir",buf,MEDIA_CONF_FILE);
#endif					
	}

#ifdef NR65S
	else if(nValidNum[DSK_VOL]==1){
#ifdef __SERCOMM_MEDIA_SERVER__
		sprintf(buf,FSH_MNT_PREFIX"/conf/media.db",DSK_VOL+1);
		PRO_SetStr(NULL,"dbfile",buf,MEDIA_CONF_FILE);	
		sprintf(db_scan_ok,"%s/%s",buf, "db_scan.ok");
		if(!access(db_scan_ok, F_OK))
			db_ok=1;		
#else		
		sprintf(buf,FSH_MNT_PREFIX"/conf/twonkymedia.db",DSK_VOL+1);
		PRO_SetStr(NULL,"dbdir",buf,MEDIA_CONF_FILE);	
#endif		
		//sprintf(db_file,"/bin/rm -rf %s",buf);	
		//system(db_file);		
#ifndef __SERCOMM_MEDIA_SERVER__		
		sprintf(buf,FSH_MNT_PREFIX"/conf/twonkymedia.db/cache",DSK_VOL+1);
		PRO_SetStr(NULL,"cachedir",buf,MEDIA_CONF_FILE);
#endif					
	}
#endif
	else if(nValidNum[USB_DISK_1] || nValidNum[USB_DISK_2]){
#ifdef __SERCOMM_MEDIA_SERVER__
		if(hard_index>=0)
			sprintf(buf,FSH_MNT_PREFIX"/data%s/media.db",hard_index, db_path);
		else
			sprintf(buf,"/tmp/media.db");
		PRO_SetStr(NULL,"dbfile",buf,MEDIA_CONF_FILE);	
		remove(buf);
#else		
		if(hard_index>=0)
			sprintf(buf,FSH_MNT_PREFIX"/data%s/twonkymedia.db",hard_index, db_path);
		else
			sprintf(buf,"/tmp/twonkymedia.db");
		PRO_SetStr(NULL,"dbdir",buf,MEDIA_CONF_FILE);	
#endif			
		sprintf(db_file,"/bin/rm -rf %s",buf);	
		system(db_file);	
#ifndef __SERCOMM_MEDIA_SERVER__			
		if(hard_index>=0)
			sprintf(buf,FSH_MNT_PREFIX"/data%s/twonkymedia.db/cache",hard_index, db_path);
		else
			sprintf(buf,"/tmp/twonkymedia.db/cache");
		PRO_SetStr(NULL,"cachedir",buf,MEDIA_CONF_FILE);	
#endif				
	}	


	sleep(4);
#ifndef __SERCOMM_MEDIA_SERVER__	
	sprintf(buf,"/usr/sbin/iconv -f UTF-8 -t UTF-16 %s > /etc/%s", MEDIA_CONF_FILE,REAL_MEDIA_CONF);
	ret=system(buf);
	if(ret){
		sprintf(buf,"/bin/cp -af /usr/local/%s /etc/%s >/dev/null 2>&1",REAL_MEDIA_CONF,REAL_MEDIA_CONF);
		system(buf);
	}
#endif	

#ifndef _MS3500_
	db_ok=0;
#endif
	if(fork()==0){
		setsid();
		for(i=0;i<64;i++)
			close(i);
		if(fork()>0)
			exit(0);
		umask(0);
#ifdef __SERCOMM_MEDIA_SERVER__
	#ifdef _MS3500_
		if(fast_scan && db_ok)
			execl("/etc/rc.d/mediaserver.sh","mediaserver.sh","start","fast",NULL);
		else
			execl("/etc/rc.d/mediaserver.sh","mediaserver.sh","start",NULL);
	#else
		execl("/etc/rc.d/mediaserver.sh","mediaserver.sh","start",NULL);
	#endif
#else
		execl("/etc/rc.d/twonkymedia.sh","twonkymedia.sh","start",NULL);
#endif		
		exit(0);
	}
	signal(SIGCLD,SIG_IGN);
	return(FSH_OK);
}

int ResetBroadcast(void)
{
	char buf[1024]={0};
	char device_ip[16];
	FILE *fh=NULL;
#if 1
	FILE *fh1=NULL;
	int fd=0, find=0;
	char TmpFile[]="/tmp/tmpfile.XXXXXX";
#endif	
	int ret=0;
	sh_info s_info;
	BROAD	broad_conf;
	char sh_name[FSH_MaxShareLen+1]={0}, subfolder[FSH_MaxDirLen+1]={0};
	
	ret=ReadBroadConf(&broad_conf);
	if(ret)
		return ret;
	system("/bin/killall ample 2>/dev/null");
	sleep(2);
//	if(broad_conf.broad==0)
//		return 0;
	ret=StripShareAndFolder(broad_conf.broad_path, sh_name, subfolder, 0);
	if(ret){//-2: Folder doesn't exist.
		if(ret==-2)
			PRO_SetStr("media","music_broad","no",FSH_DS_CONF_FILE);
		return -1;	
	}
	strcpy(s_info.name, sh_name);
	ret=GetShareInfo(&s_info);
	if(ret){
		PRO_SetStr("media","music_broad","no",FSH_DS_CONF_FILE);
		return ret;
	}	
	fh=fopen(FSH_DS_CONF_FILE,"rt");
	if(!fh) 
		return(FSH_FILE_OPEN_ERR);
	ret=PRO_GetStr("network","ip_addr",device_ip,sizeof(device_ip),fh);
	fclose(fh);	
	
	sprintf(buf,FSH_MNT_PREFIX"/"FSH_DATA_NAME"%s/%s",s_info.hard_index,s_info.dir,subfolder);	
	PRO_SetStr(NULL,"mp3path",buf,WINAMP_CONF_FILE);
	PRO_SetInt(NULL,"port",broad_conf.broad_port,WINAMP_CONF_FILE);
	if(!strcmp(broad_conf.play_method,"order"))
		PRO_SetStr(NULL,"order","true",WINAMP_CONF_FILE);	
	else
		PRO_SetStr(NULL,"order","false",WINAMP_CONF_FILE);	
	PRO_SetStr(NULL,"servername",broad_conf.broad_name,WINAMP_CONF_FILE);
	PRO_SetStr(NULL,"serveraddress",device_ip,WINAMP_CONF_FILE);
	
#if 0	
	if(fork()==0){
		setsid();
		for(i=0;i<64;i++)
			close(i);
		if(fork()>0)
			exit(0);
		chdir("/");
		umask(0);
		signal(SIGCLD,SIG_IGN);
		sleep(1);
		execl("/usr/sbin/ample","ample",NULL);
		exit(0);
	}	
#else
//Update /etc/services
	remove(TmpFile);
	strcpy(TmpFile,"/tmp/tmpfile.XXXXXX");
	fd=mkstemp(TmpFile);
	if(fd>=0)
		close(fd);	
	fh=fopen(ETC_SERVICE,"rt");
	if(!fh)
		return(FSH_FILE_OPEN_ERR);
	fh1=fopen(TmpFile,"wt");
	if (!fh1) {
		fclose(fh);
		return(FSH_FILE_OPEN_ERR);
	}
	while(fgets(buf,sizeof(buf),fh)){
		if(strstr(buf,"ample")==buf && broad_conf.broad){
			fprintf(fh1,"ample\t%d/tcp\n",broad_conf.broad_port);
			find=1;
		}
		else
			fputs(buf,fh1);
	}
	if(!find && broad_conf.broad)
		fprintf(fh1,"ample\t%d/tcp\n",broad_conf.broad_port);
	fclose(fh1);
	fclose(fh);
	sprintf(buf,"/bin/cp -f %s %s 2>/dev/null",TmpFile,ETC_SERVICE);
	system(buf);
	if(system(buf)){
		remove(TmpFile);
		return(FSH_RESET_FTP_ERR);
	}
	remove(TmpFile);

//Update /etc/inetd.conf
	strcpy(TmpFile,"/tmp/tmpfile.XXXXXX");
	fd=mkstemp(TmpFile);
	if(fd>=0)
		close(fd);	
	fh=fopen(INETD_CONF_FILE,"rt");
	if(!fh)
		return(FSH_FILE_OPEN_ERR);
	fh1=fopen(TmpFile,"wt");
	if (!fh1) {
		fclose(fh);
		return(FSH_FILE_OPEN_ERR);
	}
	while(fgets(buf,sizeof(buf),fh)){
		if(strstr(buf,"ample")){
			if(broad_conf.broad)
				fputs("ample\tstream\ttcp\tnowait\troot\t/usr/sbin/ample\n",fh1);
			find=1;
		}
		else
			fputs(buf,fh1);
	}
	if(!find && broad_conf.broad)
		fputs("ample\tstream\ttcp\tnowait\troot\t/usr/sbin/ample\n",fh1);
	fclose(fh1);
	fclose(fh);
	sprintf(buf,"/bin/cp -f %s %s 2>/dev/null",TmpFile,INETD_CONF_FILE);
	system(buf);
	if(system(buf)){
		remove(TmpFile);
		return(FSH_RESET_FTP_ERR);
	}
	remove(TmpFile);
	chmod(INETD_CONF_FILE,0644);
	system("/bin/killall inetd 2>/dev/null");
	if(fork()==0){
		setsid();
		if(fork()>0)
			exit(0);
		chdir("/");
		umask(0);
		signal(SIGCHLD, SIG_DFL);
		sleep(1);
		execl("/etc/rc.d/rc.xinetd","rc.xinetd",NULL);
		exit(0);
	}	
#endif

	return(FSH_OK);
}

int StopMediaProcess(int hard_index, char *sh_prefix, int disable)//1-HDD,2-FLASH
{
	char share_name[20],itune_enable[6]={0},broad_enable[6]={0};
	sh_info s_info;
	FILE *fh=NULL;
	int ret=0,itune=0,broad=0,upnp_av=0,ccode=0,i=0;
	UPNP_AV	av_conf;
	char sh_name[FSH_MaxShareLen+1]={0}, subfolder[FSH_MaxDirLen+1]={0};
	char tmp_path[312]={0};
	
	fh=fopen(FSH_DS_CONF_FILE,"rt");
	if(!fh) 
		return 0;
	PRO_GetStr("media","itune",itune_enable,FSH_MaxEnableLen,fh);
	if(!strcmp(itune_enable,"yes")){
		share_name[0]=0;
		ccode=PRO_GetStr("media","itune_path",tmp_path,sizeof(tmp_path),fh);
		ccode=StripShareAndFolder(tmp_path, share_name, subfolder, 0);
		if(!ccode){
			strcpy(s_info.name, share_name);
			ccode=GetShareInfo(&s_info);
			if(!ccode){
				if(s_info.hard_index==hard_index){
					if(hard_index==1){
						system("/bin/killall mt-daapd 2>/dev/null");
						itune=1;	
						ret|=1;
					}
					else if(sh_prefix==NULL || !strncmp(share_name,sh_prefix,strlen(sh_prefix))){
						system("/bin/killall mt-daapd 2>/dev/null");
						itune=1;
						ret|=1;
					}
				}
				sleep(2);			
			}
			else{
				system("/bin/killall mt-daapd 2>/dev/null");
				sleep(2);
				itune=1;		
			}
		}
		else{
			system("/bin/killall mt-daapd 2>/dev/null");
			sleep(2);
			itune=1;		
		}		
	}	
	PRO_GetStr("media","music_broad",broad_enable,FSH_MaxEnableLen,fh);
	if(!strcmp(broad_enable,"yes")){
		share_name[0]=0;
		ccode=PRO_GetStr("media","broad_path",tmp_path,sizeof(tmp_path),fh);
		ccode=StripShareAndFolder(tmp_path, share_name, subfolder, 0);
		if(!ccode){
			strcpy(s_info.name, share_name);
			ccode=GetShareInfo(&s_info);
			if(!ccode){
				if(s_info.hard_index==hard_index){
					if(hard_index==1){
						system("/bin/killall ample 2>/dev/null");
						broad=1;
						ret|=4;
					}
					else if(sh_prefix==NULL || !strncmp(share_name,sh_prefix,strlen(sh_prefix))){
						system("/bin/killall ample 2>/dev/null");
						broad=1;
						ret|=4;
					}
				}
				sleep(2);			
			}
			else{
				system("/bin/killall ample 2>/dev/null");
				sleep(2);	
				broad=1;		
			}
		}
		else{
			system("/bin/killall ample 2>/dev/null");
			sleep(2);	
			broad=1;		
		}
	}		
	fclose(fh);
	
	ccode=ReadUPnPAVConf(&av_conf);
	if(ccode)
		goto out;
	if(av_conf.upnp_av==1){
		for(;i<FOLDER_NUM;i++){
			if(av_conf.upnp_av_folder[i]==0)
				continue;
			ccode=StripShareAndFolder(av_conf.upnp_av_path[i], sh_name, subfolder, 0);
			if(ccode)
				continue;
			strcpy(s_info.name, sh_name);
			ccode=GetShareInfo(&s_info);
			if(ccode){
				continue;
			}	
			if(s_info.hard_index==hard_index){
			#ifdef __SERCOMM_MEDIA_SERVER__
				StopUPnPDaemon(&av_conf, 1);
			#else
				system("/etc/rc.d/twonkymedia.sh stop >/dev/null 2>&1");
			#endif	
				upnp_av=1;
				ret|=8;				
			}
			else
				upnp_av=0;
		}	
	}
out:	
	if(itune && disable){
		PRO_SetStr1("media","itune","no",FSH_DS_CONF_FILE);
	}
	if(broad && disable){
		PRO_SetStr1("media","music_broad","no",FSH_DS_CONF_FILE);				
	}
	if(upnp_av && disable){
		PRO_SetStr1("media","upnp_av","no",FSH_DS_CONF_FILE);				
	}	
	return ret;
}

void StartMediaProcess(int num)
{
	sleep(6);
	if(num & 1)
		ResetItune();
	if(num & 4)
		ResetBroadcast();
	if(num & 8)
		ResetUPnPAV(0);		
}

void StopAllMedia(void)
{
	system("/bin/killall ample 2>/dev/null");
	system("/bin/killall mt-daapd 2>/dev/null");
#ifdef __SERCOMM_MEDIA_SERVER__
	StopUPnPDaemon(NULL, 1);
#else
	system("/etc/rc.d/twonkymedia.sh stop >/dev/null 2>&1");
#endif	
}

void StopAllMedia1(void)
{
	system("/bin/killall ample 2>/dev/null");
	system("/bin/killall mt-daapd 2>/dev/null");
#ifdef __SERCOMM_MEDIA_SERVER__
	StopUPnPDaemon(NULL, 0);
#else
	system("/etc/rc.d/twonkymedia.sh stop >/dev/null 2>&1");
#endif	
}

#ifdef __WEB_STATION__
#define HTTPD_CONF_S		"/usr/local/apache2/conf/httpd.conf"
#define HTTPD_CONF			"/etc/web_station/httpd.conf"
#define MYSQLD_CONF			"/etc/web_station/my.cnf"
#define PHP_INI				"/etc/web_station/php.ini"

int ReadWebStationConf(WebStationConf *pConf)
{
	FILE *fh=NULL;
	char tmp_str[6]={0};
	int ret=0;
	
	pConf->httpd_port=8080;
	pConf->mysqld_port=3306;
	fh = fopen(FSH_DS_CONF_FILE, "rt");
	if (!fh)
		return -1;
	ret=PRO_GetStr("web_station", "root_path", pConf->root_path, MAX_WEB_STATION_PATH_LEN, fh);		
	ret=PRO_GetStr("web_station", "httpd_enable", tmp_str, 5, fh);	
	if(!ret && !strcmp(tmp_str, "yes"))
		pConf->httpd_enable=1;
		
	ret=PRO_GetInt("web_station", "httpd_port", &pConf->httpd_port,fh);		
		
	ret=PRO_GetStr("web_station", "mysqld_enable", tmp_str, 5, fh);	
	if(!ret && !strcmp(tmp_str, "yes"))
		pConf->mysqld_enable=1;
		
	ret=PRO_GetStr("web_station", "mysqld_remote", tmp_str, 5, fh);	
	if(!ret && !strcmp(tmp_str, "yes"))
		pConf->mysqld_remote=1;		

	ret=PRO_GetInt("web_station", "mysqld_port", &pConf->mysqld_port,fh);		
	fclose(fh);
	return 0;
}

int SaveWebStationConf(WebStationConf *pConf)
{
	int ret=0;
	multi_items	all_items[6];
	char httpd_port[6]={0}, mysqld_port[6]={0};

	strcpy(all_items[0].key,"root_path");
	all_items[0].value=pConf->root_path;
		
	strcpy(all_items[1].key,"httpd_enable");
	if (pConf->httpd_enable)
		all_items[1].value="yes";
	else
		all_items[1].value="no";

	strcpy(all_items[2].key,"httpd_port");
	sprintf(httpd_port,"%d", pConf->httpd_port);
	all_items[2].value=httpd_port;
	
	strcpy(all_items[3].key,"mysqld_enable");
	if (pConf->mysqld_enable)
		all_items[3].value="yes";
	else
		all_items[3].value="no";

	strcpy(all_items[4].key,"mysqld_port");
	sprintf(mysqld_port,"%d", pConf->mysqld_port);
	all_items[4].value=mysqld_port;
	
	strcpy(all_items[5].key,"mysqld_remote");
	if (pConf->mysqld_remote)
		all_items[5].value="yes";
	else
		all_items[5].value="no";
	
	PRO_SetStr("web_station", "httpd_enable", all_items[0].value, FSH_DS_CONF_FILE);
	ret=PRO_SetMultiItemStr("web_station",all_items,6, FSH_DS_CONF_FILE);
	if(ret){
		return -1;
	}
    return 0;
}



int ResetWebStationConf(WebStationConf *pConf)
{
    FILE *fp = NULL;
    FILE *fp_new = NULL;
    char temp[256] = { 0 };
    int flag = 0;
    char root_dir[1024] = { 0 };
	int ret=0,fd=0;
	char sh_name[FSH_MaxShareLen+1] = {0}, subfolder[FSH_MaxDirLen+1] = {0};
	char TmpFile[]="/tmp/tmpfile.XXXXXX";
	sh_info s_info;
	
	ret = StripShareAndFolder(pConf->root_path, sh_name, subfolder, 0);
	if (ret == -1) {
		return -1;
	}
	strcpy(s_info.name, sh_name);
	ret = GetShareInfo(&s_info);
	if(ret)
		return -1;
	sprintf(root_dir, FSH_MNT_PREFIX"/"FSH_DATA_NAME"%s/%s", s_info.hard_index, s_info.dir, subfolder);
	sprintf(temp, FSH_MNT_PREFIX"/conf/tmp", s_info.hard_index);
	if(access(temp, F_OK))
		mkdir(temp, 0777);
	chmod(temp, 0777);
    // Reset httpd.conf
    fp = fopen(HTTPD_CONF_S, "rt");
    if(!fp) {
		return -1;
    }
    memset(temp, '\0', sizeof(temp));
    fp_new = fopen(HTTPD_CONF, "w");
    if(!fp_new)
    	return -1;
    while(fgets(temp, sizeof(temp), fp)) {
		if (strstr(temp, "Listen")) {
		    fprintf(fp_new, "Listen %d\n", pConf->httpd_port);
		}
		else if (strstr(temp, "DocumentRoot")) {
		    fprintf(fp_new, "DocumentRoot %s/www\n", root_dir);
		}
		else if (strstr(temp, "ErrorLog")) {
		    fprintf(fp_new, "ErrorLog %s/logs/error_log\n", root_dir);
		}
		else if (strstr(temp, "CustomLog")) {
		    fprintf(fp_new, "#CustomLog %s/logs/access_log common\n", root_dir);
		}
		else
		    fprintf(fp_new, "%s", temp);
		memset(temp, '\0', sizeof(temp));
    }
    fflush(fp_new);
    fclose(fp_new);
    fclose(fp);
    // reset my.cnf
    fp = fopen(MYSQLD_CONF, "rt");
    if (!fp)
		return -1;
    temp[0]=0;
	fd=mkstemp(TmpFile);
	if(fd<0){
		fclose(fp);
		return -1;
	}
	close(fd);    
    fp_new = fopen(TmpFile, "w");
    if(!fp_new){
    	remove(TmpFile);
 		fclose(fp);
		return -1;   	
    }
    while(fgets(temp, sizeof(temp), fp)) {
    	if(temp[0]=='[' && flag)
    		break;
		if (strstr(temp, "[mysqld]")) {
		    flag = 1;
		}
		if (flag && strstr(temp, "port")) {
		    fprintf(fp_new, "port = %d\n", pConf->mysqld_port);
		} else if (flag && strstr(temp, "skip-networking")) {
		    fprintf(fp_new, "%s-networking\n", pConf->mysqld_remote?"#skip":"skip");
		} else if (flag && strstr(temp, "datadir")) {
		    fprintf(fp_new, "datadir = %s/mysql_db\n", root_dir);
		} else if (flag && strstr(temp, "tmpdir")) {
		    fprintf(fp_new, "tmpdir = "FSH_MNT_PREFIX"/conf/tmp\n", s_info.hard_index);
		} else
		    fprintf(fp_new, "%s", temp);
    }
    fclose(fp_new);
    fclose(fp);
	sprintf(temp,"/bin/cp -f %s %s 2>/dev/null",TmpFile,MYSQLD_CONF);
	system(temp);
	remove(TmpFile);
    // reset php.ini
    fp = fopen(PHP_INI, "rt");
    if (!fp) {
		return -1;
    }
    temp[0]=0;
	strcpy(TmpFile, "/tmp/tmpfile.XXXXXX");
	fd=mkstemp(TmpFile);
	if(fd<0){
		fclose(fp);
		return -1;
	}
	close(fd);    
    fp_new = fopen(TmpFile, "w");
    if(!fp_new){
    	remove(TmpFile);
 		fclose(fp);
		return -1;   	
    }
    while (fgets(temp, sizeof(temp), fp)) {
		if (strstr(temp, "error_log")) {
		    fprintf(fp_new, "error_log = %s/logs/php_log\n", root_dir);
		}
		else if (strstr(temp, "upload_tmp_dir")) {
		    fprintf(fp_new, "upload_tmp_dir = "FSH_MNT_PREFIX"/conf/tmp\n", s_info.hard_index);
		}
		else if (strstr(temp, "session.save_path")) {
		    fprintf(fp_new, "session.save_path = "FSH_MNT_PREFIX"/conf/tmp\n", s_info.hard_index);
		}
		else if (strstr(temp, "soap.wsdl_cache_dir")) {
		    fprintf(fp_new, "soap.wsdl_cache_dir = "FSH_MNT_PREFIX"/conf/tmp\n", s_info.hard_index);
		}
		else
		    fprintf(fp_new, "%s", temp);
    }
    fclose(fp_new);
    fclose(fp);
	sprintf(temp,"/bin/cp -f %s %s 2>/dev/null",TmpFile,PHP_INI);
	system(temp);
	remove(TmpFile);
    return 0;
}
#endif

#ifdef _PHOTO_ALBUM_
int ReadPhotoAlbumConf(ALBUM_CONF *pConf)
{
	FILE *fh=NULL;
	char tmp_str[6]={0};
	int ret=0;
	
	pConf->album_enable=0;
	fh = fopen(FSH_DS_CONF_FILE, "rt");
	if (!fh)
		return -1;
		
	ret=PRO_GetStr("photo_album", "album_enable", tmp_str, 5, fh);	
	if(!ret && !strcmp(tmp_str, "yes"))
		pConf->album_enable=1;
	ret=PRO_GetStr("photo_album", "album_path", pConf->album_path, sizeof(pConf->album_path)-1,fh);		
	if(ret){
		fclose(fh);
		return -1;
	}
	ret=PRO_GetStr("photo_album", "album_back_music", pConf->background_music_path, sizeof(pConf->background_music_path)-1,fh);		
	if(ret){
		fclose(fh);
		return -1;
	}	
	fclose(fh);
	return 0;
}

int SavePhotoAlbumConf(ALBUM_CONF *pConf)
{
	int ret=0;
	multi_items	all_items[3];
	
	strcpy(all_items[0].key,"album_enable");
	if (pConf->album_enable)
		all_items[0].value="yes";
	else
		all_items[0].value="no";

	strcpy(all_items[1].key,"album_path");
	all_items[1].value=pConf->album_path;

	strcpy(all_items[2].key,"album_back_music");
	all_items[2].value=pConf->background_music_path;
	
	PRO_SetStr("photo_album", "album_enable", all_items[0].value, FSH_DS_CONF_FILE);
	ret=PRO_SetMultiItemStr("photo_album",all_items,3, FSH_DS_CONF_FILE);
	if(ret){
		return -1;
	}
    return 0;
}

#define	PHOTO_DIR_LINK	"/etc/photo_album"
#define	BACK_MUSIC_LINK	"/etc/background.mp3"

void ResetPhotoAlbum(ALBUM_CONF *pConf)
{
	char tmp_str[1024]={0}, full_path[1024]={0};
	char *p1=NULL, *p2=NULL;
	sh_info s_info;
	int ret=0;
	
	strcpy(tmp_str, pConf->album_path);
	p1=strchr(tmp_str, '/');
	if(!p1)
		return;
	p1++;
	p2=strchr(p1, '/');
	if(p2){
		*p2=0;
		p2++;	
	}
	else
		p2="";
	strcpy(s_info.name, p1);
	ret=GetShareInfo(&s_info);
	if(ret){
		return;
	}		
	if(strlen(p2))
		sprintf(full_path,FSH_MNT_PREFIX"/data%s%s",s_info.hard_index, s_info.dir,p2);
	else
		sprintf(full_path,FSH_MNT_PREFIX"/data%s",s_info.hard_index, s_info.dir);
	remove(PHOTO_DIR_LINK);
	symlink(full_path, PHOTO_DIR_LINK);
	
	strcpy(tmp_str, pConf->background_music_path);
	p1=strchr(tmp_str, '/');
	if(!p1)
		return;
	p1++;
	p2=strchr(p1, '/');
	if(p2){
		*p2=0;
		p2++;	
	}
	else
		p2="";
	strcpy(s_info.name, p1);
	ret=GetShareInfo(&s_info);
	if(ret){
		return;
	}		
	if(strlen(p2))
		sprintf(full_path,FSH_MNT_PREFIX"/data%s%s",s_info.hard_index, s_info.dir,p2);
	else
		sprintf(full_path,FSH_MNT_PREFIX"/data%s",s_info.hard_index, s_info.dir);
	remove(BACK_MUSIC_LINK);
	symlink(full_path, BACK_MUSIC_LINK);	
	
	return;
}
#endif


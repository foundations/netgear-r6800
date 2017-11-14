#ifndef _FTP_H_
#define _FTP_H_
#include "constant.h"

#ifdef __SERCOMM_MEDIA_SERVER__
#define	MEDIA_SERVER_PID	"/var/run/media_server.pid"
#define	MEDIA_SCAN_PID		"/var/run/media_scan.pid"
#endif

#define MAX_UPNP_NAME_LEN	32
#define MAX_AV_PATH_LEN		256
#define	FOLDER_NUM			4
typedef struct upnp_av{
	int		upnp_av;
	char	upnp_name[MAX_UPNP_NAME_LEN+1];
	int		upnp_av_folder[FOLDER_NUM];
	char	upnp_av_share[FOLDER_NUM][FSH_MaxShareLen+1];
	char	upnp_av_path[FOLDER_NUM][MAX_AV_PATH_LEN+1];	
	int 	content_type[FOLDER_NUM];
	int		content_scan;
	int		code_page;
	int		device_port;
	char	fw_version[12];
	unsigned long scan_interval;
}UPNP_AV;

typedef struct itune{
	int		itune;
	char	itune_name[MAX_UPNP_NAME_LEN+1];
	char	itune_path[MAX_AV_PATH_LEN+1];	
}ITUNE;

typedef struct broad{
	int		broad;
	char	broad_name[MAX_UPNP_NAME_LEN+1];
	int		broad_port;
	char	broad_path[MAX_AV_PATH_LEN+1];	
	char	play_method[12];		
}BROAD;

#ifdef _PHOTO_ALBUM_
typedef struct album_conf
{
	int album_enable;
	char album_path[256];
	char background_music_path[256];
}ALBUM_CONF;

int ReadPhotoAlbumConf(ALBUM_CONF *pConf);
int SavePhotoAlbumConf(ALBUM_CONF *pConf);
void ResetPhotoAlbum(ALBUM_CONF *pConf);
#endif

typedef struct ftp_conf{
    int ftp_server;
    int ftp_change;
    int ftp_anon;
    int ftp_lang;
    int ftp_port;
    int ftp_data_port;
    int ftp_passive_ports_fix;	//0-Default, 1-Spedified
    int passive_port_min;
    int passive_port_max;
#ifdef _FTPS_    
    int ftp_ssl;
#endif    
	int ftp_bandwidth;//Unit: KB/s
}FTP_SERVER;

int ReadFTPConf(FTP_SERVER *pconf);
int SaveFTPConf(FTP_SERVER *pconf);
 int ResetFTP(void);
int ResetBroadcast(void);
int ResetUPnPAV(int fast_scan);
int ResetItune(void);
int StopMediaProcess(int hard_index, char *sh_prefix, int disable);
void StartMediaProcess(int num);
int ReadiTuneConf(ITUNE *pConf);
int SaveiTuneConf(ITUNE *pConf);

int ReadBroadConf(BROAD *pConf);
int SaveBroadConf(BROAD *pConf);

int ReadUPnPAVConf(UPNP_AV *pConf);
int SaveUPnPAVConf(UPNP_AV *pConf);
int StripShareAndFolder(char *pSrc, char *pShare, char *pDir, int checkonly);
void ResetUPnPAV2Def(void);
void StopAllMedia(void);
void StopAllMedia1(void);
#ifdef __SERCOMM_MEDIA_SERVER__
void StopUPnPDaemon(UPNP_AV *pAVConf, int del_db);
void ScanUPnPAVServer(void);
void ReloadUPnPAVServerIP(void);
#endif

#ifdef __WEB_STATION__
#define	MAX_WEB_STATION_PATH_LEN	255
typedef struct WebStationConf {
	char root_path[MAX_WEB_STATION_PATH_LEN+1];
	int httpd_enable;
	int httpd_port;
	int mysqld_enable;
	int mysqld_remote;
	int mysqld_port;
} WebStationConf;

int ReadWebStationConf(WebStationConf *pConf);
int SaveWebStationConf(WebStationConf *pConf);
int ResetWebStationConf(WebStationConf *pConf);
#endif
void RemoveDBOKFlag(void);
#endif

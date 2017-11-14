#ifndef __LIBDOWN_H__
#define __LIBDOWN_H__

//#include "constant.h"
//#include "cgi_err.h"
#include <time.h>

#define FSH_MaxUserLen      256 // need follow USB samba : mike zhou
#define	MAX_URL_LEN			256
#define	MAX_SH_LEN			21
#define	MAX_SUB_DIR_LEN		128
#define	MAX_FILENAME_LEN	256
#define	MAX_ENABLE_LEN		4
#define	MAX_ACCOUNT_LEN		64
#define	MAX_PASS_LEN		64
#define	MAX_RESULT_LEN		128
#define	MAX_FIN_SIZE_LEN	32
#define	MAX_SPEED_LEN		24
#define MAX_DIR_LEN         243   // NTFS only support 243, so we use min

#define TASK_PID_FOLDER		"/var/run/down"
#define	MANAGER_PID			"/var/run/down_manager.pid"
#define TASK_STATUS_FOLDER	"/var/tmp"

#define	DOWN_FILE_SUFFIX	"downloading"

#define	MY_DOWNLOAD_FOLDER		FSH_MNTHD1_CONF"/downloads"

#define	TORRENT_FOLDER_HDD		MY_DOWNLOAD_FOLDER"/torrent"
#define	TORRENT_FOLDER_FLASH	MY_DOWNLOAD_FOLDER"/torrent"
#define	MAX_DOWN_JOB_NUM		100

/* Mike add define */
#define GD_FTP_HTTP_DL       0
#define GD_BT_DL             1
#define GD_EMULE_DL          2

#define ERR_UNKNOW          -1
#define ERR_EXIST           -2
#define ERR_MAX_JOBS        -3
#define ERR_ILLEGAL_PATH    -4
#define ERR_BAD_TORRENT     -5
#define ERR_DOWN_LARGE_FILE -6
#define ERR_DOWN_NO_SPACE   -7
#define ERR_EXIST_FILE      -8
#define ERR_BT_INCOMPATIBLE -9

/* Mike add tmp */
#define FSH_MNT_PREFIX          "/tmp/mnt/shares"
#define	PROC_MOUNT	            "/proc/mounts"
#define FSH_DS_CONF_FILE "/mnt/shares/U/netgeardownload.conf"
#define DOWNLOAD_FOLDER  "/mnt/shares/%s/netgear_downloader"
#define TORRENT_FOLDER   "/mnt/shares/%s/netgear_downloader/torrent"
#define CONFIG_FOLDER    "/mnt/shares/%s/netgear_downloader/conf"
/*****

[URL]
share_name=
subdirectory=
bt_file=
file_name=
auth_enable=
account=
password=
total_size=
finish_size=
result=
task_status=
speed=
*******/
#define	TASK_WAIT	    0
#define	TASK_STOPPED	1
#define	TASK_FINISHED	2
#define	TASK_FAILED		3
#define	TASK_TRANSFER	4
#define	TASK_CONNECT	5
#define	TASK_CONNECTED	6
#define	TASK_HASHING	7

typedef struct down_task{
	char owner[FSH_MaxUserLen+1];
	int proto;		//0-FTP/HTTP, 1-BT
	int priority;		//0-Low, 1-Normal, 2-High
	char url[MAX_URL_LEN+1];
	char sh_name[MAX_SH_LEN+1];
	char sub_dir[MAX_SUB_DIR_LEN+1];
	char auth_enable[MAX_ENABLE_LEN+1];
	char account[MAX_ACCOUNT_LEN+1];
	char pass[MAX_PASS_LEN+1];

	char start_time[32];
	//char finish_time[32];

	int start_port;
	int end_port;
	int upload_speed;	//KB/s
	int down_speed;	//KB/s
	int seed_hour;	//Hours
	int task_start;
	time_t create_time;	//Time when task was created.
	time_t finish_time;
	struct down_task *next;
}down_task;

typedef struct down_status{
	char file_name[MAX_FILENAME_LEN+1];
	char total_size[MAX_FIN_SIZE_LEN+1];
	char finish_size[MAX_FIN_SIZE_LEN+1];
	char finish_percent[10];
	char result[MAX_RESULT_LEN+1];
	char down_speed[MAX_SPEED_LEN+1];
	char up_speed[MAX_SPEED_LEN+1];
	char announce[MAX_URL_LEN+1];
	char create_date[64];
	int peer_number;
	int seeder_number;
	int	 task_status;//0-Ongoing, 1-Stopped, 2-Finished, 3-Failed
}down_status;

typedef struct sys_download{
	int down_enable;
	int ftp_mode;//0-Passive, 1-Port
	int threads;//1~10
	int max_task;//30
	int max_run;//1~5
	int max_http_run;//1~10
	int down_schl;
	int down_day;//0~7
	int start_hour;//1~12
	int start_min;//0~59
	int stop_hour;//1~12
	int stop_min;//0~59
	unsigned long max_speed;
}sys_download;
//Functions for data backup

#ifdef __cplusplus
extern "C" {
#endif
int CheckDownManagerEnabled(void);
int ModTaskPrio(char *pURL, int order);//order: 1-UP, 0-Down
int GetDownTaskStatus(down_status *pTask, char *pUrl);//Non-encoded URL
int DOWN_ModTaskPriority(char *pURL, int task_priority);
int ReadDownInfo(sys_download *pInfo);
int SaveDownInfo(sys_download *pInfo);
int ResetDownSchl(sys_download *pInfo);
int TimeReady(sys_download *pInfo);
int GetDownTaskList(down_task **,int *);
int AddDownTask(down_task *);
int DelDownTask(char *pURL);
void FreeDownTask(down_task *);
int IsDownTaskExist(char *pURL);
int IsDownTaskDoing(char *pURL);
void NotifyManager(void);
void StopAllTasks();
int StopTask(char *pURL);
void ReloadDownTask(char *pURL);
void StopManager(void);
void ReloadAllTasks(void);
void StopUSBTask(char *pPrefix);
void RunTask(down_task *pTask, unsigned int listen_port);
int NumOfDownTasks(void);
int NumofBTRunTasks();
int NumofHTTPFTPRunTasks();
int MaxTasks(void);
int MaxRunTasks(void);
void StopDownTask(char *pURL);
int CheckStopTask(char *pURL);
int GetManagerPid();
int DOWN_ModTaskStatus(char *pURL, int task_status);
int DOWN_ModTaskStatusFile(char *pURL);
int GetDownTaskInfo(down_task *pTask, char *pUrl);
int String2HexStr(char *pSrc, char *pDest);
int HexStr2String(char *pSrc, char *pDest);
int user_acc_share(char *user,char *share);
void CheckPath(char *prefix, char *path);
int DownTorrentFile(down_task *pTask, char *pFileName);
int UpdateDownTask(down_task *pTask);
down_task * TaskofList(down_task *pList, char *pURL);
int BT_SetStr(char *pKey, char *pVal, char *pFileName);
int CopyFile(char *pSrc, char *pDest);
int RecordStartFinTime(char *pURL, int job_status);//job_status: 0-start, 1-finish
void SortTaskListByPriority(down_task *pTask);
int DelAllFinishedTask();
int Set_task_start_flag(char *pURL,char *value);
int reAddDownTask(char *pURL);
int CheckHDDSpace(char *pURL, char *share_name);
int DelDownTaskTorrentCache(char *pURL, int proto);
void SortTaskListByFinishTime(down_task *pTask);
void create_download_dir(void);
void check_download_dir(void);
#ifdef _LOG_DOWN_TASK_
int LogDownTaskActions(char *pURL, int log_status);
#endif
#ifdef __cplusplus
}
#endif

#endif

#define VERSION_FILE "info.dat"
#define LOCAL_VERSIOIN_FILE     "/tmp/LastestVersion"

#ifdef AMPED_EXTENDER
#define IMAGE_SERVER1 "fwampedserver.com/RE2600M/"
#else
#define IMAGE_SERVER1 "fwampedserver.com/RTA2600R2/"
#endif
#define NEW_VERSION             "/etc/new_version"

#define IMG_FILE "img.bin"
#define LOCAL_IMG               "/tmp/img"
#define LOCAL_IMG_MD5           "/tmp/img_md5"
#define IMG_SIZE_LIMIT           1024*1024*10

#define AUTO_UP_NO_NEED_UPGRADE     0
#define AUTO_UP_NEED_UPGRADE_FW     (1<<0)


#define S_FAILED          (-1)
#define S_OK              0

/*
info format:
,"version":"1.43"
,"date":"Thu Jul 23 17:37:09 CST 2015"
,"md5":"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
*/
#define VERSION_LEN    100
#define DATE_LEN       100
#define MD5_LEN           40

typedef struct downpara_s
{
    char version[VERSION_LEN];         /* version of image    */
    char date[DATE_LEN];    		/* date of image   */
    char md5[MD5_LEN];                  /* md5 value of image  */
}downpara_t;


#define SWAP16(x)   (((x&0xFF) << 8) | ((x&0xFF00) >> 8))

int download_file(char *server_str, char *r_file, char *l_file);
int get_down_para(char *file, downpara_t *para_p);
int check_fw_size(char *cpFileName, downpara_t *para_p);
int check_fw_file(char *cpFileName, downpara_t *para_p);
int download_file_from_servers(char *r_file, char *l_file);
int need_upgrade(char *local_version_file) ;


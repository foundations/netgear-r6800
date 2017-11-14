#ifndef _AUTO_UP_H_
#define _AUTO_UP_H_

#include "nvram_var.h"

//remote path and file name
/////16M flash server path/////
//#define _SJ_TEST_
#ifdef _SJ_TEST_
#define IMAGE_SERVER1 "172.21.8.33/%s/ww/"
#define IMAGE_SERVER2 "172.21.8.33/%s/ww/"
#define IMAGE_SERVER3 "172.21.8.33/%s/ww/"
#else

#ifdef FLASH_16M
#ifdef ANNEXB
#define IMAGE_SERVER1 "updates1.netgear.com/%s/de/"
#define IMAGE_SERVER2 "updates2.netgear.com/%s/de/"
#define IMAGE_SERVER3 "updates3.netgear.com/%s/de/"
#else
#ifdef US_ONLY
#define IMAGE_SERVER1 "updates1.netgear.com/%s/us/"
#define IMAGE_SERVER2 "updates2.netgear.com/%s/us/"
#define IMAGE_SERVER3 "updates3.netgear.com/%s/us/"
#else
#define IMAGE_SERVER1 "updates1.netgear.com/%s/ww/"
#define IMAGE_SERVER2 "updates2.netgear.com/%s/ww/"
#define IMAGE_SERVER3 "updates3.netgear.com/%s/ww/"
#endif//US_ONLY
#endif//ANNEXB
/////8M flash server path/////
#else
#ifdef ANNEXB
#define IMAGE_SERVER1 "updates1.netgear.com/%s/de/"
#define IMAGE_SERVER2 "updates2.netgear.com/%s/de/"
#define IMAGE_SERVER3 "updates3.netgear.com/%s/de/"
#else
#ifdef US_ONLY
#define IMAGE_SERVER1 "updates1.netgear.com/%s/us/"
#define IMAGE_SERVER2 "updates2.netgear.com/%s/us/"
#define IMAGE_SERVER3 "updates3.netgear.com/%s/us/"
#else
#define IMAGE_SERVER1 "updates1.netgear.com/%s/ww/"
#define IMAGE_SERVER2 "updates2.netgear.com/%s/ww/"
#define IMAGE_SERVER3 "updates3.netgear.com/%s/ww/"
#endif//US_ONLY
#endif//ANNEXB
#endif//FLASH_16M
#endif//_SJ_TEST_

//SUPPORT FOR ABSOLUTE PATH
#ifdef _SJ_TEST
#define SERVER1_ABS "172.21.22.44"
#define SERVER2_ABS "172.21.22.44"
#define SERVER3_ABS "172.21.22.44"
#else
#define SERVER1_ABS "updates1.netgear.com"
#define SERVER2_ABS "updates2.netgear.com"
#define SERVER3_ABS "updates3.netgear.com"
#endif

#ifdef _MAJOR_UPDATE_
/*we use Major1 as usual. 
Normally, version level n1 would never be incremented*/
#define MAJOR_NAME   "Major2"
#else
#define MAJOR_NAME   "Major1"
#endif
#define VERSION_FILE_STATIC "fileinfo.txt"
#ifdef AUTOUPG_FILEINFO_IN_NV
#define VERSION_FILE (nvram_safe_get(AUTOUPG_FILEINFO_IN_NV))
#else
#define VERSION_FILE "fileinfo.txt"
#endif
#define STRTB_FILE	 "stringtable.dat"

//local path and file name
#define NEW_VERSION             "/etc/new_version"
#define LOCAL_VERSIOIN_FILE     "/tmp/LastestVersion"
#define LOCAL_STRTB_FILE	    "/tmp/stringtable.dat"
#define LOCAL_IMG               "/tmp/img"
#define LOCAL_IMG_MD5           "/tmp/img_md5"
#define LOCAL_GUI_TMP            "/tmp/gui_tmp"
#define LOCAL_GUI               "/tmp/gui"
#define LOCAL_GUI_MD5           "/tmp/gui_md5"
#define LOCAL_GUI_DIR           "/tmp/gui_dir"

#define S_FAILED          (-1)
#define S_OK              0

#define VER_BUF_LEN       9
#define ALERT_BUF_LEN     500
#define CMD_BUF_LEN       500
//#define MODULE_NAME_LEN   50
#define MAJOR_NAME_LEN    100
#define R_FILENAME_LEN    150
#define SRV_NAME_LEN      100
#define MD5_LEN           40

#define NEXT_PAGE              "NEXT_PAGE"

#define AUTO_UP_NO_NEED_UPGRADE     0
#define AUTO_UP_NEED_UPGRADE_FW     (1<<0)
#define AUTO_UP_NEED_UPGRADE_GUI    (1<<1)


typedef struct downpara_s
{
    char major[MAJOR_NAME_LEN];         /* Section tag name    */
    char remote_file[R_FILENAME_LEN];   /* filename of image   */
    char md5[MD5_LEN];                  /* md5 value of image  */
    char server_name[SRV_NAME_LEN];     /* Not used.. -- Argon */
    unsigned int img_size;              /* size of image       */
}downpara_t;

int get_down_opt(char *file,downpara_t *para_p);
int download_file(char *server,char *r_file,char *l_file);
int get_down_para(char *file, downpara_t *para_p, char *section);
int check_fw_file(char *cpFileName, downpara_t *para_p);
int download_file_from_servers(char *r_file, char *l_file);
int printf_release_notes(char *buf, int max_buf_len);
int need_upgrade(char *local_version_file);
void get_language_section(char *section, int current);
int check_gui_file(char *cpFileName, downpara_t *para_p);
int gui_is_newer(downpara_t *para_p, char *mtd);
#ifdef USE_JEFFS2_FS
int get_gui_file(char *fsFile, char *local_gui_file);
#endif
#endif /* _AUTO_UP_H_ */

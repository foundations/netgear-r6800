#ifndef _SC_LANGUAGE_H_
#define _SC_LANGUAGE_H_
#include "upgrade_flash.h"
#define LANGUAGE_ID_ENGLISH     "ENU"
#define MAX_LANGUAGE_FILE_SIZE  (128*1024)
#define TEMP_LANG_FILE          "/tmp/lang.pkg"

#define DEFAULT_LANGUAGE "English"

struct lang_info_s {
    char id[4];         /* 3-Charactor language ID        */
    int version[4];     /* Version of this language file  */
    int len;            /* Length of zipped language file */
};
#define LANGUAGE_HEADER_LEN          128

int decompress_language(char *mtd);
int get_language_info(char *mtd, struct lang_info_s *lang_s);
char *get_language_from_id(char *lang_id);
char *get_id_from_language(char *language);
void create_language_file(char *lang);
int print_language_mtd_info(char *buf);
int gw_language_update(char *file);
int language_file_is_correct(char *file);
char **get_http_accept_language_list(void);
char *language_is_in_flash(char *language);
#endif /* _SC_LANGUAGE_H_ */

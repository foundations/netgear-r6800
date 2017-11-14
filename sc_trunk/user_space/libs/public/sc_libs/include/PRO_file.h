#include <stdio.h>

#ifndef OK
#define OK   0
#endif

#define MAX_LEN   356
#define MAX_KEY_LEN   64
#define MS_MSG_F  "/share/message.txt"
typedef char Char128[128];

int PRO_2sect(char *sect, FILE *fp);
int PRO_GetStr(char *sect, char *key, char *val, int size, FILE* fp);
int PRO_GetInt(char *sect, char *key, int *val, FILE *fp);
int PRO_GetMsgByIndex(char *sect, int start, int num, Char128 *msg, char *fn);

#ifndef _3DES_H
#define _3DES_H

#ifdef __cplusplus
extern "C" {
#endif

#define ENCRYPT 1
#define DECRYPT 0
#define DESLEN 8
#define DES3_KEY_LEN 24

void DES3_E_D(char output[DESLEN],char input[DESLEN],char type,char SubKey[3][16][6]);
void SetKey(char SKey[16][6],const char Key[DESLEN]);


#ifdef __cplusplus
}
#endif


#endif

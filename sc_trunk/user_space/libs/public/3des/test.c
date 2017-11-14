#include <assert.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include "3des.h"
#include "base64.h"



enum {
	PWD_T,
	VER_T
};

typedef struct{
	unsigned long type;
	unsigned long len;
	unsigned char value[0];
}TLV;


int
encrypt_pwd (char *pwd, char *enc_pwd)
{
//	char *str = "abcdefg";
	unsigned char array_a[128+1];
	unsigned char array_b[128+1];
	unsigned char *ret_en, *ret_de;
	int len;
    TLV *entry = NULL;
    unsigned long random;
    int i;
    int value_len = 128*3/4 - 8;
    int len_out, len_dst = 128 +1;
    char *str = pwd;
    
    len = strlen(str);
    memset(array_a, 0, sizeof(array_a));
    entry = (TLV *)array_a;
    
    entry->type = PWD_T;
    entry->len = len;
    memcpy(entry->value, str, len);
    

	srand(time(0));
	
	printf("len = %d\n", len);
	while (len < value_len)
	{
		random = rand();
//		printf("value_len = %d, len =%d, value_len-len = %d, random = %d, random =%ld\n", value_len, len, value_len - len, random,random);
		len += snprintf((entry->value + len), value_len - len, "%ld", random);
//		printf("len = %d\n", len);
	}
	printf("entry->value = %s, len = %d, strlen()=%d\n", entry->value, len, strlen(entry->value));
	
	
	printf("array_a:");
	for( i = 0; i < 128*3/4; i++)
		printf("%2x ", array_a[i]);
	printf("\n");

   	des3_main(array_a, 128*3/4, array_b, &len_out, ENCRYPT);

	printf("after 3des  encrypt len_out = %d\n", len_out);

	
	printf("\narray_b:");
	for( i = 0; i < 128*3/4; i++)
		printf("%2x ", array_b[i]);
	printf("\n");

	base64_encode(array_a, &len_dst, array_b, len_out);
	printf("after base64 encode ret_en = %s, ret_len = %d, len_dst = %d\n", array_a, strlen(array_a), len_dst);
	memcpy(enc_pwd, array_a, len_dst);
	
	return len_dst;

}

int
decrypt_pwd (char *pwd, char *dec_pwd)
{
//	char *str = "abcdefg";
	unsigned char array_a[128+1];
	unsigned char array_b[128+1];
	unsigned char *ret_en, *ret_de;
	int len;
    TLV *entry = NULL;
    int i;
    int value_len = 128*3/4 - 8;
    int len_out, len_dst;
    char *str = pwd;
    
    memset(array_a, 0, sizeof(array_a));
    memcpy(array_a, pwd, strlen(pwd));
	memset(array_b, 0, sizeof(array_b));
	len_dst = 96 + 1; /* 96 is the largest return len */
	base64_decode(array_b, &len_dst, array_a, strlen(array_a));
	printf("base64 decode len_dst =%d\n", len_dst);
	
	printf("ret_de:");
	for( i = 0; i < 128*3/4; i++)
		printf("%2x ", array_b[i]);
	printf("\n");

	des3_main(array_b, 128*3/4, array_a, &len_out, DECRYPT);
	printf("after 3des  decrypt len_out = %d\n", len_out);
	printf("array_a:");
	for( i = 0; i < 128*3/4; i++)
		printf("%2x ", array_a[i]);
	printf("\n");
	
	entry = (TLV *)array_a;
	
	if ( entry->type == PWD_T )
	{
		memcpy(dec_pwd, entry->value, entry->len);
		*(dec_pwd + entry->len) = 0;
	}
	
	
	printf("type = %s, len = %d, dec_pwd = %s\n", (entry->type == PWD_T)?"PWD":"Unknow", entry->len, dec_pwd);
	return ;	
	
}

int main (int argc, char *argv[])
{
	int type;
	char pwd[128+1];
	char result_pwd[128+1];
	int check = 0;
    int opt;
	
	memset(pwd, 0, 129);
	memset(result_pwd, 0, 129);
	
    while((opt=getopt(argc,argv,"e:d:"))!=-1)
    {
        switch(opt)
        {
        case 'e':
            type=ENCRYPT;check+=2;
    		strcpy(pwd, optarg);
            break;
        case 'd':
            type=DECRYPT;check+=1;
    		strcpy(pwd, optarg);
            break;
        case '?':
            printf("unknown option:%c\n",optopt);break;
        case ':':
            printf("option need a value\n");break;
        }
    }
    
    
    if(check != 1 && check != 2)
    {
    	printf("format error!\n\
        format:test -e(for encryption) -d(for decryption)\n\
             -e and -d can't be chosen either!\n");
        exit(1);
    }
    printf("will do something for %s\n", pwd);
    
    if (type == ENCRYPT)
    	encrypt_pwd( pwd, result_pwd);
    else if (type == DECRYPT)
    	decrypt_pwd( pwd, result_pwd);
    
    printf("%s <%s>\n <%s>\n", (type == ENCRYPT)?"ENCRYPT":"DECRYPT", pwd, result_pwd);
}


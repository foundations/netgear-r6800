#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include"3des.h"

char SubKey[3][16][6];
const char Key[24]={'D','e','s','=','0','6','2','6','B','y','S','i','e','g','f','r','i','e','d','.','S','B','U','3'};
//int main(int argc,char *argv[])
int des3_main(char * in, int len_in, char *out, int *len_out, int type)
{
    int loops,n,m;
    char *tmpin,*tmpout;


    SetKey(SubKey[0],Key);
    SetKey(SubKey[1],Key+8);
    SetKey(SubKey[2],Key+16);

//    printf("-----input len is %d-----\n", len_in);
    int slen;
    if(type==ENCRYPT){
    	if(len_in%8 != 0)
        	slen = 8-(len_in%8);
        else
        	slen = 0;
        tmpin = malloc(len_in+slen);
        tmpout = malloc(len_in+slen);
        if(slen != 0)
        	memset(tmpin+len_in,slen,slen);
    }
    else if(type==DECRYPT){
        tmpin = malloc(len_in);
        tmpout = malloc(len_in);
    }
    
    memcpy(tmpin, in, len_in);

    if(type==ENCRYPT)
        loops = (len_in+slen)/8;
    else if(type==DECRYPT)
        loops = len_in/8;
    for(n=0,m=0;n<loops;n++,m+=8){
        DES3_E_D(tmpout+m,tmpin+m,type,SubKey);
    }


    if(type==DECRYPT){
        int a = (int)tmpout[len_in-1];
       	memcpy(out, tmpout, len_in - a);
       	*len_out = len_in - a;
    }
    else if(type==ENCRYPT)
    {
       	memcpy(out, tmpout, len_in + slen);
       	*len_out = len_in + slen;
    }
    free(tmpin);
    free(tmpout);
//    printf("3DES success\n");
}

#if 0
int main(int argc,char *argv[])
{
    int loops,n,m;
    struct stat fileA;      //file attribution
    int opt;
    char inputfile[255],outputfile[255];
    FILE *filein, *fileout;                 //input and output file names
    char *tmpin,*tmpout;
    //char tmpout[8];
    int data_rest;
    int len;
    char type;
    int check=0;

    while((opt=getopt(argc,argv,"i:o:ed"))!=-1)
    {
        switch(opt)
        {
        case 'i':
            strcpy(inputfile,optarg);check+=8;break;
        case 'o':
            strcpy(outputfile,optarg);check+=4;break;
        case 'e':
            type=ENCRYPT;check+=2;break;
        case 'd':
            type=DECRYPT;check+=1;break;
        case '?':
            printf("unknown option:%c\n",optopt);break;
        case ':':
            printf("option need a value\n");break;
        }
    }
    if(check!=13&&check!=14)
    {
        printf("format error!\n\
        format:3des -i inputfilename -o outputfilename\
             -e(for encryption) -d(for decryption)\n\
             -e and -d can't be chosen either!\n");
        exit(1);
    }

    filein=fopen(inputfile,"r");
    if(filein==NULL)
    {
        printf("open input file failed\n");
        exit(1);
    }

    SetKey(SubKey[0],Key);
    SetKey(SubKey[1],Key+8);
    SetKey(SubKey[2],Key+16);

    if((stat(inputfile,&fileA))<0){
        printf("get file size failed!\n");
        exit(1);
    }
    printf("-----filesize is %d-----\n",fileA.st_size);
    int slen;
    if(type==ENCRYPT){
        slen = 8-(fileA.st_size%8);
        tmpin = malloc(fileA.st_size+slen);
        tmpout = malloc(fileA.st_size+slen);
        memset(tmpin+fileA.st_size,slen,slen);
    }
    else if(type==DECRYPT){
        tmpin = malloc(fileA.st_size);
        tmpout = malloc(fileA.st_size);
    }
    if((fread(tmpin,1,fileA.st_size,filein))!=fileA.st_size){
        printf("read file error!\n");
        exit(1);
    }
    fclose(filein);
    if(type==ENCRYPT)
        loops = (fileA.st_size+slen)/8;
    else if(type==DECRYPT)
        loops = fileA.st_size/8;
    for(n=0,m=0;n<loops;n++,m+=8){
        DES3_E_D(tmpout+m,tmpin+m,type,SubKey);
    }
    fileout=fopen(outputfile,"w");
    if(fileout==NULL)
    {
        printf("open output file failed\n");
        exit(1);
    }
    if(type==DECRYPT){
        int a = (int)tmpout[fileA.st_size-1];
        fwrite(tmpout,1,fileA.st_size-a,fileout);
    }
    else if(type==ENCRYPT)
        fwrite(tmpout,1,fileA.st_size+slen,fileout);
    fclose(fileout);
    free(tmpin);
    free(tmpout);
    printf("3DES success\n");
}
#endif


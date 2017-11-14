#include"3des.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static int descount;
static char KeyArray[3][16][6];

// Permuted Choice 1 (PC-1)
const static char PC1[56] =
{
57, 49, 41, 33, 25, 17,  9,
 1, 58, 50, 42, 34, 26, 18,
10,  2, 59, 51, 43, 35, 27,
19, 11,  3, 60, 52, 44, 36,
63, 55, 47, 39, 31, 23, 15,
 7, 62, 54, 46, 38, 30, 22,
14,  6, 61, 53, 45, 37, 29,
21, 13,  5, 28, 20, 12,  4
};

// Left shift hops for keys
const static char LSHIFT[16] =
{
1, 1, 2, 2, 2, 2, 2, 2,
1, 2, 2, 2, 2, 2, 2, 1
};

// Permuted Choice 2 (PC-2)
const static char PC2[48] =
{
14, 17, 11, 24,  1,  5,
 3, 28, 15,  6, 21, 10,
23, 19, 12,  4, 26,  8,
16,  7, 27, 20, 13,  2,
41, 52, 31, 37, 47, 55,
30, 40, 51, 45, 33, 48,
44, 49, 39, 56, 34, 53,
46, 42, 50, 36, 29, 32
};


// Initial Permutation (IP)
const static char IP[64] =
{
58, 50, 42, 34, 26, 18, 10, 2,
60, 52, 44, 36, 28, 20, 12, 4,
62, 54, 46, 38, 30, 22, 14, 6,
64, 56, 48, 40, 32, 24, 16, 8,
57, 49, 41, 33, 25, 17,  9, 1,
59, 51, 43, 35, 27, 19, 11, 3,
61, 53, 45, 37, 29, 21, 13, 5,
63, 55, 47, 39, 31, 23, 15, 7
};

// Expansion (from 32 bits to 48 bits)
static const char ETO48[48] =
{
32,  1,  2,  3,  4,  5,
 4,  5,  6,  7,  8,  9,
 8,  9, 10, 11, 12, 13,
12, 13, 14, 15, 16, 17,
16, 17, 18, 19, 20, 21,
20, 21, 22, 23, 24, 25,
24, 25, 26, 27, 28, 29,
28, 29, 30, 31, 32,  1
};

// The famous S-boxes
const static char S_BOX[8][4][16] =
{
{
   // S1
   {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
   { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
   { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
   {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
},
{
   // S2
   {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
   { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
   { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
   {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}
},
{
   // S3
   {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
   {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
   {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
   { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
},
{
   // S4
   { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
   {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
   {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
   { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}
},
{
   // S5
   { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
   {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
   { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
   {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}
},
{
   // S6
   {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
   {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
   { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
   { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
},
{
   // S7
   { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
   {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
   { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
   { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}
},
{
   // S8
   {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
   { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
   { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
   { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
}
};

// Permutation P
const static char P_Table[32] =
{
16,  7, 20, 21,
29, 12, 28, 17,
 1, 15, 23, 26,
 5, 18, 31, 10,
 2,  8, 24, 14,
32, 27,  3,  9,
19, 13, 30,  6,
22, 11,  4, 25
};

// Final Permutation
const static char IPR[64] =
{
40, 8, 48, 16, 56, 24, 64, 32,
39, 7, 47, 15, 55, 23, 63, 31,
38, 6, 46, 14, 54, 22, 62, 30,
37, 5, 45, 13, 53, 21, 61, 29,
36, 4, 44, 12, 52, 20, 60, 28,
35, 3, 43, 11, 51, 19, 59, 27,
34, 2, 42, 10, 50, 18, 58, 26,
33, 1, 41,  9, 49, 17, 57, 25
};

//void DES3_E_D(char output[DESLEN],char input[DESLEN],char type);
static inline void DES(char strin[DESLEN],char strout[DESLEN],char type);
//void SetKey(const char Key[DESLEN]);                              //set key
static void DesRun(char out[DESLEN], char in[DESLEN], char type);   //DES encrypt and decrypt

static void des_data(char in[4], const char Ki[6]);
static void des_s_box(char out[4], const char in[6]);
static void transform(char *out, char *in, const char *table, int len); //change sequence
static void transformb(char *Out, char *In, const char *Table, int len);
static void xor(char *in1, const char *in2, int len);
static void lshift(char *in, int len, int loop);                //left shift
static void bytetobit(char *out, const char *in, int bits);     // convert bytes into bits
static void bittobyte(char *out, const char *in, int bits);     // convert bits into bytes

void DES3_E_D(char output[DESLEN],char input[DESLEN],char type,char SKey[3][16][6])
{
    memcpy(KeyArray,SKey,sizeof(KeyArray));
    char tmp1[DESLEN],tmp2[DESLEN];
    if(ENCRYPT==type)
    {
        descount=0;
        DES(tmp1,input,ENCRYPT);
        DES(tmp2,tmp1,DECRYPT);
        DES(output,tmp2,ENCRYPT);
    }
    else
    {
        descount=0;
        DES(tmp1,input,DECRYPT);
        DES(tmp2,tmp1,ENCRYPT);
        DES(output,tmp2,DECRYPT);
    }
}

inline void DES(char strout[DESLEN],char strin[DESLEN],char type)
{
    DesRun(strout,strin,type);
}

void des_data(char in[4], const char Ki[6])
{
    static char MR[6];
    char tmp_1[4];
    transform(MR, in, ETO48, 48);
    xor(MR, Ki, 6);
    des_s_box(tmp_1, MR);
    transform(in, tmp_1, P_Table, 32);
}

void des_s_box(char out[4], const char in[6])
{

    out[0]=S_BOX[0][((in[0]&0x80)>>6)|((in[0]&0x04)>>2)][(in[0]&0x78)>>3];
    out[0]=out[0]<<4;
    out[0]|=S_BOX[1][(in[0]&0x02)|((in[1]&0x10)>>4)][((in[0]&0x01)<<3)|((in[1]&0xE0)>>5)];
    out[1]=S_BOX[2][((in[1]&0x08)>>2)|((in[2]&0x40)>>6)][((in[1]&0x07)<<1)|((in[2]&0x80)>>7)];
    out[1]=out[1]<<4;
    out[1]|=S_BOX[3][((in[2]&0x20)>>4)|(in[2]&0x01)][(in[2]&0x1E)>>1];
    out[2]=S_BOX[4][((in[3]&0x80)>>6)|((in[3]&0x04)>>2)][(in[3]&0x78)>>3];
    out[2]=out[2]<<4;
    out[2]|=S_BOX[5][(in[3]&0x02)|((in[4]&0x10)>>4)][((in[3]&0x01)<<3)|((in[4]&0xE0)>>5)];
    out[3]=S_BOX[6][((in[4]&0x08)>>2)|((in[5]&0x40)>>6)][((in[4]&0x07)<<1)|((in[5]&0x80)>>7)];
    out[3]=out[3]<<4;
    out[3]|=S_BOX[7][((in[5]&0x20)>>4)|(in[5]&0x01)][(in[5]&0x1E)>>1];
}

void transform(char *out, char *in, const char *table, int len){
    int i,j,k1,k2;
    memset(out,0,len>>3);
    for(i=0;i<len;i++){
        j = i&7;
        k1 = (table[i]-1)>>3;
        k2 = (table[i]-1)&7;
        if((k2-j)>=0)
            out[i>>3]|=((in[k1]&(0x80>>k2))<<(k2-j));
        else
            out[i>>3]|=((in[k1]&(0x80>>k2))>>(j-k2));
    }
}

void transformb(char *Out, char *In, const char *Table, int len){
    int i;
    static char tmp[256];

    for(i=0; i<len; i++)
    {
        tmp[i] = In[ Table[i]-1 ];
    }
    memcpy(Out, tmp, len);
}

void xor(char *in1, const char *in2, int len)
{
    int i;
    for(i=0; i<len; i++)
    {
        in1[i] ^= in2[i];
    }
}

void lshift(char *in, int len, int loop)
{
    static char tmp[256];
    memcpy(tmp, in, loop);
    memcpy(in, in+loop, len-loop);
    memcpy(in+len-loop, tmp,loop);
}

void bytetobit(char *out, const char *in, int bits)
{
    int i;
    for(i=0; i<bits; i++)
            out[i] = (in[i>>3]>>(7-(i&7))) & 1;
}

void bittobyte(char *out, const char *in, int bits)
{
    int i;
    memset(out, 0, bits/8);
    for(i=0; i<bits; i++)
        out[i>>3] |= in[i]<<(7-(i&7));
}

void SetKey(char SKey[16][6],const char Key[DESLEN])
{
    int i;
    char tmp_2[64],tmp_0[7];
    static char K[64], *KL = &K[0], *KR = &K[28];
    bytetobit(tmp_2, Key, 64);
    transformb(K, tmp_2, PC1, 56);
    for(i=0; i<16; i++)
    {
        lshift(KL, 28, LSHIFT[i]);
        lshift(KR, 28, LSHIFT[i]);
        bittobyte(tmp_0,K,56);
        transform(SKey[i], tmp_0, PC2, 48);
    }
}

void DesRun(char out[DESLEN], char in[DESLEN], char type)
{
    int i;
    static char M[8], tmp[4], *Li = &M[0], *Ri = &M[4];
    if(descount==0){
        transform(M, in, IP, 64);
    }
    if(type==ENCRYPT)
    {
        for(i=0; i<16; i++)
        {
            memcpy(tmp, Ri, 4);
            des_data(Ri, KeyArray[descount][i]);
            xor(Ri, Li, 4);
            memcpy(Li, tmp, 4);
            if(i==15){
                memcpy(tmp,Ri,4);
                memcpy(Ri,Li,4);
                memcpy(Li,tmp,4);
            }
        }
    }
    else
    {
        for(i=15; i>=0; i--)
        {
            if(i==15){
                memcpy(tmp,Li,4);
                memcpy(Li,Ri,4);
                memcpy(Ri,tmp,4);
            }
            memcpy(tmp, Li, 4);
            des_data(Li, KeyArray[2-descount][i]);
            xor(Li, Ri, 4);
            memcpy(Ri, tmp, 4);
        }
    }

    if(descount==2){
        transform(out, M, IPR, 64);
    }
    descount++;
}


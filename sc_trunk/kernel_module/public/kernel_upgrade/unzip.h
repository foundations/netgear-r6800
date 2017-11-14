// UNZIP.H - Unzip code bastardized from Erwins hack of gzip

#ifndef __UNZIP_H
#define __UNZIP_H
#include <sc_project_defines.h>
typedef unsigned char  uch;
typedef unsigned short ush;
typedef unsigned long  ulg;
typedef short INT16;
typedef unsigned short WORD;
typedef long INT32;
typedef unsigned long DWORD;
typedef char CHAR;
typedef unsigned char BYTE;
typedef void VOID;
#define PACKED          __attribute__((packed))
typedef void *voidp;
#define memzero(s, n)     memset ((voidp)(s), 0, (n))



typedef struct __attribute__((packed)) {
    DWORD      dwSignature;  // 0
    WORD          wVersion;  // 4
    WORD   wGeneralPurpose;  // 6
    WORD           wMethod;  // 8
    WORD             wTime;  // 10
    WORD             wDate;  // 12
    DWORD            dwCrc;  // 14
    DWORD       dwCompSize;  // 18
    DWORD     dwUncompSize;  // 22
    WORD   wFilenameLength;  // 26
    WORD      wExtraLength;  // 28
    BYTE         bFilename;  // 30
} ZIPHEADER;

/* Huffman code lookup table entry--this entry is four bytes for machines
   that have 16-bit pointers (e.g. PC's in the small or medium model).
   Valid extra bits are 0..13.  e == 15 is EOB (end of block), e == 16
   means that v is a literal, 16 < e < 32 means that v is a pointer to
   the next table, which codes e - 16 bits, and lastly e == 99 indicates
   an unused code.  If a code with e == 99 is looked up, this implies an
   error in the data. */
struct huft {
  uch e;                /* number of extra bits or operation */
  uch b;                /* number of bits in this code or subcode */
  union {
    ush n;              /* literal, length base, or distance base */
    struct huft *t;     /* pointer to next level of table */
  } v;
};


// Procedure prototypes
#ifdef SUPPORT_MULTIPAL_MTD_BLOCK
int unzip(int zipfile_fd);
#else
int unzip(int zipfile_fd, int mtd_fd, unsigned long shift);
#endif
int inflate(void);
int inflate_block(int* e);
int inflate_dynamic(void);
int inflate_fixed(void);
int inflate_stored(void);
int inflate_codes(struct huft* tl, struct huft* td, int bl, int bd);
int huft_free(struct huft *t);
int huft_build(unsigned *b, unsigned n, unsigned s, ush *d, ush *e, struct huft **t, int *m);
ulg updcrc(uch *s,unsigned n);
void write_buffer(voidp buf, unsigned int);
void flush_window(void);
int get_byte(void);

#endif // __UNZIP_H



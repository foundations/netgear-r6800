// Unzip.c - Open Unzip, command line compatible with ZipRisc

#include "unzip.h"
#ifndef SUPPORT_MULTIPAL_MTD_BLOCK
#include <linux/mtd/mtd.h>
#endif


#ifdef USE_MTK_NAND_MTD_WRITE
/* englarge win size to flash erase size */
#define UNZIP_BLOCK_SIZE      (256*1024)
#define WSIZE (UNZIP_BLOCK_SIZE/2)	// win size
#define BLOCKSIZE (128*1024)
#else
#define UNZIP_BLOCK_SIZE      (64*1024)
#define WSIZE (UNZIP_BLOCK_SIZE/2)	// window size
#define BLOCKSIZE (16*1024)	// window size
#endif

unsigned int outcnt;           	// bytes in output buffer
uch window[UNZIP_BLOCK_SIZE];
#ifdef USE_MTK_NAND_MTD_WRITE
uch check_buf[WSIZE*2]; /* user buf + oob data */
#else
uch check_buf[WSIZE];
#endif
unsigned inptr;				// index of next byte to be processed in inbuf
int test = 0;         		// test .gz file integrity
static unsigned long bytes_out = 0;            	// number of output bytes
ulg m_ulgCRC;

//////////////////////////////////////////////////////////////////////////
int file_fd;
#ifndef SUPPORT_MULTIPAL_MTD_BLOCK
int flash_fd;
unsigned long shift_bytes;
static loff_t cur_offset = 0;
#endif
#define show_info(format, args...)    do {printk("<0>"); printk(format, ## args);}while(0)

#define xchgs(x) (WORD)((((WORD)((WORD)x)&0x0ff)<<8)|(((WORD)((WORD)x)&0x0ff00)>>8))
#define xchgl(x) (DWORD)((((DWORD)x)<<24)|((((DWORD)x)<<8)&0x00ff0000)|((((DWORD)x)>>8)&0x0000ff00)|(((DWORD)x)>>24))

#define ZIP_SIGNATURE 0x504B0304
#define ZIP_METHOD 0x0008

#define MALLOC(x)   kmalloc((x), GFP_KERNEL)
#define FREE        kfree
#define EOF         -1

///////////////////////////////////////////////////////////////////////////

/* ========================================================================
 * Table of CRC-32's of all single-byte values (made by makecrc.c)
 */
ulg crc_32_tab[] = {
                       0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
                       0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
                       0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
                       0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
                       0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
                       0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
                       0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
                       0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
                       0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
                       0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
                       0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
                       0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
                       0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
                       0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
                       0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
                       0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
                       0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
                       0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
                       0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
                       0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
                       0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
                       0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
                       0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
                       0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
                       0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
                       0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
                       0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
                       0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
                       0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
                       0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
                       0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
                       0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
                       0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
                       0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
                       0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
                       0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
                       0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
                       0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
                       0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
                       0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
                       0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
                       0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
                       0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
                       0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
                       0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
                       0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
                       0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
                       0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
                       0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
                       0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
                       0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
                       0x2d02ef8dL
                   };

/* ===========================================================================
 * Run a set of bytes through the crc shift register.  If s is a NULL
 * pointer, then initialize the crc shift register contents instead.
 * Return the current crc in either case.
 */
ulg updcrc(uch *s, unsigned n)
//uch *s;                 /* pointer to bytes to pump through */
//unsigned n;             /* number of bytes in s[] */
{
    register ulg c;         /* temporary variable */

    static ulg crc = (ulg)0xffffffffL; /* shift register contents */

    if (s == NULL)
    {
        c = 0xffffffffL;
    }
    else
    {
        c = crc;
        if (n)
            do
            {
                c = crc_32_tab[((int)c ^ (*s++)) & 0xff] ^ (c >> 8);
            }
            while (--n);
    }
    crc = c;
    return c ^ 0xffffffffL;       /* (instead of ~c for 64-bit machines) */
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/* inflate.c -- Not copyrighted 1992 by Mark Adler
   version c10p1, 10 January 1993 */

/* You can do whatever you like with this source file, though I would
   prefer that if you modify it and redistribute it that you include
   comments to that effect with your name and the date.  Thank you.
   [The history has been moved to the file ChangeLog.]
 */

/*
   Inflate deflated (PKZIP's method 8 compressed) data.  The compression
   method searches for as much of the current string of bytes (up to a
   length of 258) in the previous 32K bytes.  If it doesn't find any
   matches (of at least length 3), it codes the next byte.  Otherwise, it
   codes the length of the matched string and its distance backwards from
   the current position.  There is a single Huffman code that codes both
   single bytes (called "literals") and match lengths.  A second Huffman
   code codes the distance information, which follows a length code.  Each
   length or distance code actually represents a base value and a number
   of "extra" (sometimes zero) bits to get to add to the base value.  At
   the end of each deflated block is a special end-of-block (EOB) literal/
   length code.  The decoding process is basically: get a literal/length
   code; if EOB then done; if a literal, emit the decoded byte; if a
   length then get the distance and emit the referred-to bytes from the
   sliding window of previously emitted data.

   There are (currently) three kinds of inflate blocks: stored, fixed, and
   dynamic.  The compressor deals with some chunk of data at a time, and
   decides which method to use on a chunk-by-chunk basis.  A chunk might
   typically be 32K or 64K.  If the chunk is uncompressible, then the
   "stored" method is used.  In this case, the bytes are simply stored as
   is, eight bits per byte, with none of the above coding.  The bytes are
   preceded by a count, since there is no longer an EOB code.

   If the data is compressible, then either the fixed or dynamic methods
   are used.  In the dynamic method, the compressed data is preceded by
   an encoding of the literal/length and distance Huffman codes that are
   to be used to decode this block.  The representation is itself Huffman
   coded, and so is preceded by a description of that code.  These code
   descriptions take up a little space, and so for small blocks, there is
   a predefined set of codes, called the fixed codes.  The fixed method is
   used if the block codes up smaller that way (usually for quite small
   chunks), otherwise the dynamic method is used.  In the latter case, the
   codes are customized to the probabilities in the current block, and so
   can code it much better than the pre-determined fixed codes.

   The Huffman codes themselves are decoded using a mutli-level table
   lookup, in order to maximize the speed of decoding plus the speed of
   building the decoding tables.  See the comments below that precede the
   lbits and dbits tuning parameters.
 */


/*
   Notes beyond the 1.93a appnote.txt:

   1. Distance pointers never point before the beginning of the output
      stream.
   2. Distance pointers can point back across blocks, up to 32k away.
   3. There is an implied maximum of 7 bits for the bit length table and
      15 bits for the actual data.
   4. If only one code exists, then it is encoded using one bit.  (Zero
      would be more efficient, but perhaps a little confusing.)  If two
      codes exist, they are coded using one bit each (0 and 1).
   5. There is no way of sending zero distance codes--a dummy must be
      sent if there are none.  (History: a pre 2.0 version of PKZIP would
      store blocks with no distance codes, but this was discovered to be
      too harsh a criterion.)  Valid only for 1.93a.  2.04c does allow
      zero distance codes, which is sent as one code of zero bits in
      length.
   6. There are up to 286 literal/length codes.  Code 256 represents the
      end-of-block.  Note however that the static length tree defines
      288 codes just to fill out the Huffman codes.  Codes 286 and 287
      cannot be used though, since there is no length base or extra bits
      defined for them.  Similarly, there are up to 30 distance codes.
      However, static trees define 32 codes (all 5 bits) to fill out the
      Huffman codes, but the last two had better not show up in the data.
   7. Unzip can check dynamic Huffman blocks for complete code sets.
      The exception is that a single code would not be complete (see #4).
   8. The five bits following the block type is really the number of
      literal codes sent minus 257.
   9. Length codes 8,16,16 are interpreted as 13 length codes of 8 bits
      (1+6+6).  Therefore, to output three times the length, you output
      three codes (1+1+1), whereas to output four times the same length,
      you only need two codes (1+3).  Hmm.
  10. In the tree reconstruction algorithm, Code = Code + Increment
      only if BitLength(i) is not zero.  (Pretty obvious.)
  11. Correction: 4 Bits: # of Bit Length codes - 4     (4 - 19)
  12. Note: length code 284 can represent 227-258, but length code 285
      really is 258.  The last length deserves its own, short code
      since it gets used a lot in very redundant files.  The length
      258 is special since 258 - 3 (the min match length) is 255.
  13. The literal/length and distance code bit lengths are read as a
      single stream of lengths.  It is possible (and advantageous) for
      a repeat code (16, 17, or 18) to go across the boundary between
      the two sets of lengths.
 */

#define slide window

/* The inflate algorithm uses a sliding 32K byte window on the uncompressed
   stream to find repeated byte strings.  This is implemented here as a
]   circular buffer.  The index is updated simply by incrementing and then
   and'ing with 0x7fff (32K-1). */
/* It is left to other modules to supply the 32K area.  It is assumed
   to be usable as if it were declared "uch slide[32768];" or as just
   "uch *slide;" and then malloc'ed in the latter case.  The definition
   must be in unzip.h, included above. */
/* unsigned wp;             current position in slide */
#define wp outcnt
#define flush_output(w) (wp=(w),flush_window())

/* Tables for deflate from PKZIP's appnote.txt. */
static unsigned border[] = {    /* Order of the bit length code lengths */
                               16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
static ush cplens[] = {         /* Copy lengths for literal codes 257..285 */
                          3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
                          35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
/* note: see note #13 above about the 258 in this list. */
static ush cplext[] = {         /* Extra bits for literal codes 257..285 */
                          0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
                          3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 99, 99}; /* 99==invalid */
static ush cpdist[] = {         /* Copy offsets for distance codes 0..29 */
                          1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
                          257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
                          8193, 12289, 16385, 24577};
static ush cpdext[] = {         /* Extra bits for distance codes */
                          0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
                          7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
                          12, 12, 13, 13};


/* Macros for inflate() bit peeking and grabbing.
   The usage is:

        NEEDBITS(j)
        x = b & mask_bits[j];
        DUMPBITS(j)

   where NEEDBITS makes sure that b has at least j bits in it, and
   DUMPBITS removes the bits from b.  The macros use the variable k
   for the number of bits in b.  Normally, b and k are register
   variables for speed, and are initialized at the beginning of a
   routine that uses these macros from a global bit buffer and count.

   If we assume that EOB will be the longest code, then we will never
   ask for bits with NEEDBITS that are beyond the end of the stream.
   So, NEEDBITS should not read any more bytes than are needed to
   meet the request.  Then no bytes need to be "returned" to the buffer
   at the end of the last block.

   However, this assumption is not true for fixed blocks--the EOB code
   is 7 bits, but the other literal/length codes can be 8 or 9 bits.
   (The EOB code is shorter than other codes because fixed blocks are
   generally short.  So, while a block always has an EOB, many other
   literal/length codes have a significantly lower probability of
   showing up at all.)  However, by making the first table have a
   lookup of seven bits, the EOB code will be found in that first
   lookup, and so will not require that too many bits be pulled from
   the stream.
 */

ulg bb;                         /* bit buffer */
unsigned bk;                    /* bits in bit buffer */

ush mask_bits[] = {
                      0x0000,
                      0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
                      0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
                  };

#define NEXTBYTE()  (uch)get_byte()
#define NEEDBITS(n) {while(k<(n)){b|=((ulg)NEXTBYTE())<<k;k+=8;}}
#define DUMPBITS(n) {b>>=(n);k-=(n);}

/*
   Huffman code decoding is performed using a multi-level table lookup.
   The fastest way to decode is to simply build a lookup table whose
   size is determined by the longest code.  However, the time it takes
   to build this table can also be a factor if the data being decoded
   is not very long.  The most common codes are necessarily the
   shortest codes, so those codes dominate the decoding time, and hence
   the speed.  The idea is you can have a shorter table that decodes the
   shorter, more probable codes, and then point to subsidiary tables for
   the longer codes.  The time it costs to decode the longer codes is
   then traded against the time it takes to make longer tables.

   This results of this trade are in the variables lbits and dbits
   below.  lbits is the number of bits the first level table for literal/
   length codes can decode in one step, and dbits is the same thing for
   the distance codes.  Subsequent tables are also less than or equal to
   those sizes.  These values may be adjusted either when all of the
   codes are shorter than that, in which case the longest code length in
   bits is used, or when the shortest code is *longer* than the requested
   table size, in which case the length of the shortest code in bits is
   used.

   There are two different values for the two tables, since they code a
   different number of possibilities each.  The literal/length table
   codes 286 possible values, or in a flat code, a little over eight
   bits.  The distance table codes 30 possible values, or a little less
   than five bits, flat.  The optimum values for speed end up being
   about one bit more than those, so lbits is 8+1 and dbits is 5+1.
   The optimum values may differ though from machine to machine, and
   possibly even between compilers.  Your mileage may vary.
 */

int lbits = 9;          /* bits in base literal/length lookup table */
int dbits = 6;          /* bits in base distance lookup table */

/* If BMAX needs to be larger than 16, then h and x[] should be ulg. */
#define BMAX 16         /* maximum bit length of any code (16 for explode) */
#define N_MAX 288       /* maximum number of codes in any set */

unsigned hufts;         /* track memory usage */


//+****************************************************************************
/* Given a list of code lengths and a maximum table size, make a set of
   tables to decode that set of codes.  Return zero on success, one if
   the given code set is incomplete (the tables are still built in this
   case), two if the input is invalid (all zero length codes or an
   oversubscribed set of lengths), and three if not enough memory. */
//unsigned *b;            /* code lengths in bits (all assumed <= BMAX) */
//unsigned n;             /* number of codes (assumed <= N_MAX) */
//unsigned s;             /* number of simple-valued codes (0..s-1) */
//ush *d;                 /* list of base values for non-simple codes */
//ush *e;                 /* list of extra bits for non-simple codes */
//struct huft **t;        /* result: starting table */
//int *m;                 /* maximum lookup bits, returns actual */
int huft_build(unsigned *b, unsigned n, unsigned s, ush *d, ush *e, struct huft **t, int *m)
{
    unsigned a;                   /* counter for codes of length k */
    unsigned c[BMAX + 1];           /* bit length count table */
    unsigned f;                   /* i repeats in table every f entries */
    int g;                        /* maximum code length */
    int h;                        /* table level */
    register unsigned i;          /* counter, current code */
    register unsigned j;          /* counter */
    register int k;               /* number of bits in current code */
    int l;                        /* bits per table (returned in m) */
    register unsigned *p;         /* pointer into c[], b[], or v[] */
    register struct huft *q;      /* points to current table */
    struct huft r;                /* table entry for structure assignment */
    struct huft *u[BMAX];         /* table stack */
    unsigned v[N_MAX];            /* values in order of bit length */
    register int w;               /* bits before this table == (l * h) */
    unsigned x[BMAX + 1];           /* bit offsets, then code stack */
    unsigned *xp;                 /* pointer into x */
    int y;                        /* number of dummy codes added */
    unsigned z;                   /* number of entries in current table */

    /* Generate counts for each bit length */
    memzero(c, sizeof(c));
    p = b;
    i = n;
    do
    {
        c[*p]++;                    /* assume all entries <= BMAX */
        p++;                      /* Can't combine with above line (Solaris bug) */
    }
    while (--i);
    if (c[0] == n)                /* null input--all zero length codes */
    {
        *t = (struct huft *)NULL;
        *m = 0;
        return 0;
    }

    /* Find minimum and maximum length, bound *m by those */
    l = *m;
    for (j = 1; j <= BMAX; j++)
        if (c[j])
            break;
    k = j;                        /* minimum code length */
    if ((unsigned)l < j)
        l = j;
    for (i = BMAX; i; i--)
        if (c[i])
            break;
    g = i;                        /* maximum code length */
    if ((unsigned)l > i)
        l = i;
    *m = l;

    /* Adjust last length count to fill out codes, if needed */
    for (y = 1 << j; j < i; j++, y <<= 1)
        if ((y -= c[j]) < 0)
            return 2;                 /* bad input: more codes than bits */
    if ((y -= c[i]) < 0)
        return 2;
    c[i] += y;


    /* Generate starting offsets into the value table for each length */
    x[1] = j = 0;
    p = c + 1;
    xp = x + 2;
    while (--i)
    {                 /* note that i == g from above */
        *xp++ = (j += *p++);
    }

    /* Make a table of values in order of bit lengths */
    p = b;
    i = 0;
    do
    {
        if ((j = *p++) != 0)
            v[x[j]++] = i;
    }
    while (++i < n);
    n = x[g];

    /* Generate the Huffman codes and for each, make the table entries */
    x[0] = i = 0;                 /* first Huffman code is zero */
    p = v;                        /* grab values in bit order */
    h = -1;                       /* no tables yet--level -1 */
    w = -l;                       /* bits decoded == (l * h) */
    u[0] = (struct huft *)NULL;   /* just to keep compilers happy */
    q = (struct huft *)NULL;      /* ditto */
    z = 0;                        /* ditto */

    /* go through the bit lengths (k already is bits in shortest code) */
    for (; k <= g; k++)
    {
        a = c[k];
        while (a--)
        {
            /* here i is the Huffman code of length k bits for value *p */
            /* make tables up to required level */
            while (k > w + l)
            {
                h++;
                w += l;                 /* previous table always l bits */

                /* compute minimum size table less than or equal to l bits */
                z = (z = g - w) > (unsigned)l ? l : z;  /* upper limit on table size */
                if ((f = 1 << (j = k - w)) > a + 1)     /* try a k-w bit table */
                {                       /* too few codes for k-w bit table */
                    f -= a + 1;           /* deduct codes from patterns left */
                    xp = c + k;

                    if (j < z)
                        while (++j < z)       /* try smaller tables up to z bits */
                        {
                            if ((f <<= 1) <= *++xp)
                                break;            /* enough codes to use up j bits */
                            f -= *xp;           /* else deduct codes from patterns */
                        }
                }
                z = 1 << j;             /* table entries for j-bit table */

                /* allocate and link in new table */
                if ((q = (struct huft *)MALLOC((z + 1) * sizeof(struct huft))) ==
                        (struct huft *)NULL)
                {
                    if (h)
                        huft_free(u[0]);
                    return 3;             /* not enough memory */
                }
                hufts += z + 1;         /* track memory usage */
                *t = q + 1;             /* link to list for huft_free() */
                *(t = &(q->v.t)) = (struct huft *)NULL;
                u[h] = ++q;             /* table starts after link */

                /* connect to last table, if there is one */
                if (h)
                {
                    x[h] = i;             /* save pattern for backing up */
                    r.b = (uch)l;         /* bits to dump before this table */
                    r.e = (uch)(16 + j);  /* bits in this table */
                    r.v.t = q;            /* pointer to this table */
                    j = i >> (w - l);     /* (get around Turbo C bug) */
                    u[h - 1][j] = r;        /* connect to last table */
                }
            }

            /* set up table entry in r */
            r.b = (uch)(k - w);
            if (p >= v + n)
                r.e = 99;               /* out of values--invalid code */
            else if (*p < s)
            {
                r.e = (uch)(*p < 256 ? 16 : 15);    /* 256 is end-of-block code */
                r.v.n = (ush)(*p);             /* simple code is just the value */
                p++;                           /* one compiler does not like *p++ */
            }
            else
            {
                r.e = (uch)e[*p - s];   /* non-simple--look up in lists */
                r.v.n = d[*p++ - s];
            }

            /* fill code-like entries with r */
            f = 1 << (k - w);
            for (j = i >> w; j < z; j += f)
                q[j] = r;

            /* backwards increment the k-bit code i */
            for (j = 1 << (k - 1); i & j; j >>= 1)
                i ^= j;
            i ^= j;

            /* backup over finished tables */
            while ((i & ((1 << w) - 1)) != x[h])
            {
                h--;                    /* don't need to update q */
                w -= l;
            }
        }
    }

    /* Return true (1) if we were given an incomplete table */
    return y != 0 && g != 1;
}


//+****************************************************************************
/* Free the malloc'ed tables built by huft_build(), which makes a linked
   list of the tables it made, with the links in a dummy first entry of
   each table. */
int huft_free(struct huft *t)
{
    register struct huft *p, *q;

    /* Go through linked list, freeing from the malloced (t[-1]) address. */
    p = t;
    while (p != (struct huft *)NULL)
    {
        q = (--p)->v.t;
        FREE((char*)p);
        p = q;
    }
    return 0;
}

int inflate_stored(void)
/* "decompress" an inflated type 0 (stored) block. */
{
    unsigned n;           /* number of bytes in block */
    unsigned w;           /* current window position */
    register ulg b;       /* bit buffer */
    register unsigned k;  /* number of bits in bit buffer */


    /* make local copies of globals */
    b = bb;                       /* initialize bit buffer */
    k = bk;
    w = wp;                       /* initialize window position */


    /* go to byte boundary */
    n = k & 7;
    DUMPBITS(n);


    /* get the length and its complement */
    NEEDBITS(16)
    n = ((unsigned)b & 0xffff);
    DUMPBITS(16)
    NEEDBITS(16)
    if (n != (unsigned)((~b) & 0xffff))
        return 1;                   /* error in compressed data */
    DUMPBITS(16)


    /* read and output the compressed data */
    while (n--)
    {
        NEEDBITS(8)
        slide[w++] = (uch)b;
        if (w == WSIZE)
        {
            flush_output(w);
            w = 0;
        }
        DUMPBITS(8)
    }


    /* restore the globals from the locals */
    wp = w;                       /* restore global window pointer */
    bb = b;                       /* restore global bit buffer */
    bk = k;
    return 0;
}

//+****************************************************************************
/* inflate (decompress) the codes in a deflated (compressed) block.
   Return an error code or zero if it all goes ok. */
int inflate_codes(struct huft* tl, struct huft* td, int bl, int bd)
{
    register unsigned e;  /* table entry flag/number of extra bits */
    unsigned n, d;        /* length and index for copy */
    unsigned w;           /* current window position */
    struct huft *t;       /* pointer to table entry */
    unsigned ml, md;      /* masks for bl and bd bits */
    register ulg b;       /* bit buffer */
    register unsigned k;  /* number of bits in bit buffer */


    /* make local copies of globals */
    b = bb;                       /* initialize bit buffer */
    k = bk;
    w = wp;                       /* initialize window position */

    /* inflate the coded data */
    ml = mask_bits[bl];           /* precompute masks for speed */
    md = mask_bits[bd];
    for (;;)                      /* do until end of block */
    {
        NEEDBITS((unsigned)bl)
        if ((e = (t = tl + ((unsigned)b & ml))->e) > 16)
            do
            {
                if (e == 99)
                    return 1;
                DUMPBITS(t->b)
                e -= 16;
                NEEDBITS(e)
            }
            while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
        DUMPBITS(t->b)
        if (e == 16)                /* then it's a literal */
        {
            slide[w++] = (uch)t->v.n;
            if (w == WSIZE)
            {
                flush_output(w);
                w = 0;
            }
        }
        else                        /* it's an EOB or a length */
        {
            /* exit if end of block */
            if (e == 15)
                break;

            /* get length of block to copy */
            NEEDBITS(e)
            n = t->v.n + ((unsigned)b & mask_bits[e]);
            DUMPBITS(e);

            /* decode distance of block to copy */
            NEEDBITS((unsigned)bd)
            if ((e = (t = td + ((unsigned)b & md))->e) > 16)
                do
                {
                    if (e == 99)
                        return 1;
                    DUMPBITS(t->b)
                    e -= 16;
                    NEEDBITS(e)
                }
                while ((e = (t = t->v.t + ((unsigned)b & mask_bits[e]))->e) > 16);
            DUMPBITS(t->b)
            NEEDBITS(e)
            d = w - t->v.n - ((unsigned)b & mask_bits[e]);
            DUMPBITS(e)

            /* do the copy */
            do
            {
                n -= (e = (e = WSIZE - ((d &= WSIZE - 1) > w ? d : w)) > n ? n : e);
#if !defined(NOMEMCPY) && !defined(DEBUG)

                if (w - d >= e)         /* (this test assumes unsigned comparison) */
                {
                    memcpy(slide + w, slide + d, e);
                    w += e;
                    d += e;
                }
                else                      /* do it slow to avoid memcpy() overlap */
#endif /* !NOMEMCPY */

                    do
                    {
                        slide[w++] = slide[d++];
                    }
                    while (--e);
                if (w == WSIZE)
                {
                    flush_output(w);
                    w = 0;
                }
            }
            while (n);
        }
    }

    /* restore the globals from the locals */
    wp = w;                       /* restore global window pointer */
    bb = b;                       /* restore global bit buffer */
    bk = k;

    /* done */
    return 0;
}


//+****************************************************************************
/* decompress an inflated type 2 (dynamic Huffman codes) block. */
int inflate_dynamic(void)
{
    int i;                /* temporary variables */
    unsigned j;
    unsigned l;           /* last length */
    unsigned m;           /* mask for bit lengths table */
    unsigned n;           /* number of lengths to get */
    struct huft *tl;      /* literal/length code table */
    struct huft *td;      /* distance code table */
    int bl;               /* lookup bits for tl */
    int bd;               /* lookup bits for td */
    unsigned nb;          /* number of bit length codes */
    unsigned nl;          /* number of literal/length codes */
    unsigned nd;          /* number of distance codes */
#ifdef PKZIP_BUG_WORKAROUND

    unsigned ll[288 + 32];  /* literal/length and distance code lengths */
#else

    unsigned ll[286 + 30];  /* literal/length and distance code lengths */
#endif

    register ulg b;       /* bit buffer */
    register unsigned k;  /* number of bits in bit buffer */

    /* make local bit buffer */
    b = bb;
    k = bk;

    /* read in table lengths */
    NEEDBITS(5)
    nl = 257 + ((unsigned)b & 0x1f);      /* number of literal/length codes */
    DUMPBITS(5)
    NEEDBITS(5)
    nd = 1 + ((unsigned)b & 0x1f);        /* number of distance codes */
    DUMPBITS(5)
    NEEDBITS(4)
    nb = 4 + ((unsigned)b & 0xf);         /* number of bit length codes */
    DUMPBITS(4)
#ifdef PKZIP_BUG_WORKAROUND
    if (nl > 288 || nd > 32)
#else

    if (nl > 286 || nd > 30)
#endif

        return 1;                   /* bad lengths */

    /* read in bit-length-code lengths */
    for (j = 0; j < nb; j++)
    {
        NEEDBITS(3)
        ll[border[j]] = (unsigned)b & 7;
        DUMPBITS(3)
    }
    for (; j < 19; j++)
        ll[border[j]] = 0;

    /* build decoding table for trees--single level, 7 bit lookup */
    bl = 7;
    if ((i = huft_build(ll, 19, 19, NULL, NULL, &tl, &bl)) != 0)
    {
        if (i == 1)
            huft_free(tl);
        return i;                   /* incomplete code set */
    }

    if (tl == NULL)
        return 2;
    /* read in literal and distance code lengths */
    n = nl + nd;
    m = mask_bits[bl];
    i = l = 0;
    while ((unsigned)i < n)
    {
        NEEDBITS((unsigned)bl)
        j = (td = tl + ((unsigned)b & m))->b;
        DUMPBITS(j)
        j = td->v.n;
        if (j < 16)                 /* length of code in bits (0..15) */
            ll[i++] = l = j;          /* save last length in l */
        else if (j == 16)           /* repeat last length 3 to 6 times */
        {
            NEEDBITS(2)
            j = 3 + ((unsigned)b & 3);
            DUMPBITS(2)
            if ((unsigned)i + j > n)
                return 1;
            while (j--)
                ll[i++] = l;
        }
        else if (j == 17)           /* 3 to 10 zero length codes */
        {
            NEEDBITS(3)
            j = 3 + ((unsigned)b & 7);
            DUMPBITS(3)
            if ((unsigned)i + j > n)
                return 1;
            while (j--)
                ll[i++] = 0;
            l = 0;
        }
        else                        /* j == 18: 11 to 138 zero length codes */
        {
            NEEDBITS(7)
            j = 11 + ((unsigned)b & 0x7f);
            DUMPBITS(7)
            if ((unsigned)i + j > n)
                return 1;
            while (j--)
                ll[i++] = 0;
            l = 0;
        }
    }

    /* free decoding table for trees */
    huft_free(tl);

    /* restore the global bit buffer */
    bb = b;
    bk = k;

    /* build the decoding tables for literal/length and distance codes */
    bl = lbits;
    if ((i = huft_build(ll, nl, 257, cplens, cplext, &tl, &bl)) != 0)
    {
        if (i == 1)
        {
            huft_free(tl);
        }
        return i;                   /* incomplete code set */
    }
    bd = dbits;
    if ((i = huft_build(ll + nl, nd, 0, cpdist, cpdext, &td, &bd)) != 0)
    {
        if (i == 1)
#ifdef PKZIP_BUG_WORKAROUND
        {
            i = 0;
        }
#else
        {
            huft_free(td);
        }
        huft_free(tl);
        return i;                   /* incomplete code set */
#endif

    }

    /* decompress until an end-of-block code */
    if (inflate_codes(tl, td, bl, bd))
        return 1;

    /* free the decoding tables, return */
    huft_free(tl);
    huft_free(td);
    return 0;
}
int inflate_fixed(void)
/* decompress an inflated type 1 (fixed Huffman codes) block.  We should
   either replace this with a custom decoder, or at least precompute the
   Huffman tables. */
{
    int i;                /* temporary variable */
    struct huft *tl;      /* literal/length code table */
    struct huft *td;      /* distance code table */
    int bl;               /* lookup bits for tl */
    int bd;               /* lookup bits for td */
    unsigned l[288];      /* length list for huft_build */

    /* set up literal table */
    for (i = 0; i < 144; i++)
        l[i] = 8;
    for (; i < 256; i++)
        l[i] = 9;
    for (; i < 280; i++)
        l[i] = 7;
    for (; i < 288; i++)          /* make a complete, but wrong code set */
        l[i] = 8;
    bl = 7;
    if ((i = huft_build(l, 288, 257, cplens, cplext, &tl, &bl)) != 0)
        return i;


    /* set up distance table */
    for (i = 0; i < 30; i++)      /* make an incomplete code set */
        l[i] = 5;
    bd = 5;
    if ((i = huft_build(l, 30, 0, cpdist, cpdext, &td, &bd)) > 1)
    {
        huft_free(tl);

        return i;
    }


    /* decompress until an end-of-block code */
    if (inflate_codes(tl, td, bl, bd))
        return 1;


    /* free the decoding tables, return */
    huft_free(tl);
    huft_free(td);
    return 0;
}


//+****************************************************************************
/* decompress an inflated block */
int inflate_block(int* e)
{
    unsigned t;           /* block type */
    register ulg b;       /* bit buffer */
    register unsigned k;  /* number of bits in bit buffer */

    /* make local bit buffer */
    b = bb;
    k = bk;

    /* read in last block bit */
    NEEDBITS(1)
    *e = (int)b & 1;
    DUMPBITS(1)

    /* read in block type */
    NEEDBITS(2)
    t = (unsigned)b & 3;
    DUMPBITS(2)

    /* restore the global bit buffer */
    bb = b;
    bk = k;

    /* inflate that block type */
    if (t == 2)
        return inflate_dynamic();
    if (t == 0)
        return inflate_stored();
    if (t == 1)
        return inflate_fixed();

    /* bad block type */
    return 2;

}


//+****************************************************************************
/* decompress an inflated entry */
int inflate(void)
{
    int e;                /* last block flag */
    int r;                /* result code */
    unsigned h;           /* maximum struct huft's malloc'ed */

    /* initialize window, bit buffer */
    wp = 0;
    bk = 0;
    bb = 0;

    /* decompress until the last block */
    h = 0;
    do
    {
        hufts = 0;
        if ((r = inflate_block(&e)) != 0)
            return r;
        if (hufts > h)
            h = hufts;
    }
    while (!e);

    /* Undo too much lookahead. The next read will be byte aligned so we
     * can discard unused bits in the last meaningful byte.
     */
    while (bk >= 8)
    {
        bk -= 8;
        inptr--;
    }
    show_info("inflate wp=%d\n", wp);

    /* flush out slide */
    flush_output(wp);

    /* return success */
    return 0;

}


//+****************************************************************************
int get_byte(void)
{
    int byKar;
    int ret;
    unsigned char c;

    ret = sys_read(file_fd, &c, sizeof(c));
    if(!ret) {
        return EOF;
    }
    byKar=c;
    return byKar;
}

//+****************************************************************************
void flush_window(void)
{

    if (outcnt == 0)
        return ;

    m_ulgCRC = updcrc(window, outcnt);

    if (!test)
    {
#ifdef SUPPORT_MULTIPAL_MTD_BLOCK
        write_buffer( (char *)window, outcnt);
#else
	 /*devide into 16K*2*/
        if(outcnt > BLOCKSIZE)
        {
            write_buffer( (char *)window, BLOCKSIZE);
            bytes_out += BLOCKSIZE;
            write_buffer( (char *)(window+BLOCKSIZE), outcnt-BLOCKSIZE);
            bytes_out += (ulg)(outcnt-BLOCKSIZE);
        }
        else
        {
            write_buffer( (char *)window, outcnt);
            bytes_out += (ulg)outcnt;
        }
#endif
    }
#ifndef SUPPORT_MULTIPAL_MTD_BLOCK
   else
#endif
   {
	 bytes_out += (ulg)outcnt;
   }
    outcnt = 0;

}

/*
 * Read data from FLASH @fd to @buf. Length is @len
 *  0 ==> Read success, data in @buf
 * -1 ==> Read Fail. Do not use data in @buf
 */
#define MAX_RETRY_TIMES     5
static int read_from_flash(int fd, char *buf, int len) {
    int readLen=0, retry=0, ret=0;

    while(readLen < len) {
        ret = sys_read(fd, buf+readLen, len-readLen);
        if(ret < 0) {
            show_info("sys_read error: ret = %d\n", ret);
            break;
        }
        readLen += ret;
        retry++;
        if(retry > MAX_RETRY_TIMES) {
            /* We retry so many times! */
            break;
        }
    }
    /* Seek back */
    sys_lseek(fd, -readLen, SEEK_CUR);
    if(ret < 0 || retry > MAX_RETRY_TIMES) {
        if(readLen) {
            /*
             * It means we reach the end of FLASH MTD if we can not read any data from flash.
             * In most case, it should be normal, no need to print error message.
             * (zip file bigger then FLASH MTD and no need to write the tail data to MTD)
             */
            show_info("Read from FLASH error!\n");
        }
        return -1;
    }
    else {
        return 0;
    }
}

/*
 * cnt = WSIZE = (UNZIP_BLOCK_SIZE/2) = 32768
 * erver time write 32K data
 */
#ifdef SUPPORT_MULTIPAL_MTD_BLOCK
#ifdef USE_MTK_NAND_MTD_WRITE
static int
mtd_unlock_init(struct t_mtdinfo *mtd)
{
    struct erase_info_user mtdLockInfo;
    mtd->w = 0;
    mtd->e = 0;

	if (sys_ioctl(mtd->fd, MEMGETINFO, &mtd->mtdInfo)) {
		printk("Could not get MTD device info from %s\n", mtd->mtd_name);
	}

	mtdLockInfo.start = 0;
	mtdLockInfo.length = mtd->mtdInfo.size;

	if (sys_ioctl(mtd->fd, MEMUNLOCK, &mtdLockInfo)) {
		return 0;
	}
	
	return 0;
}
#endif

void write_buffer(voidp buf, unsigned int cnt)
{
    int ret = -1;
	int curr_fd = -1;
	int i = 0;
	unsigned long shift_bytes;
	unsigned long mtd_end;
#ifdef USE_MTK_NAND_MTD_WRITE
    int oobblock;
    struct t_mtdinfo *mtd = NULL;
	struct erase_info_user mtdEraseInfo;
    int bad_block = 0;
#else
	unsigned long force_write_cnt;
#endif

	for (i=0; i<upg_mtd_cnt; i++) {

		shift_bytes = upg_mtdinfo[i].offset;
		mtd_end = upg_mtdinfo[i].offset + upg_mtdinfo[i].size;

		if( (bytes_out >= shift_bytes && bytes_out < mtd_end)
				|| (bytes_out < shift_bytes && bytes_out + cnt > shift_bytes)) /* has data writes to mtd */
		{
			curr_fd = upg_mtdinfo[i].fd;
			break;
		}
	}

	if (curr_fd < 0 || i >= upg_mtd_cnt) { // for fast discard
		return;
	}

#ifdef USE_MTK_NAND_MTD_WRITE
    mtd = &upg_mtdinfo[i];

    oobblock = mtd->mtdInfo.oobblock;

    /* check bad block */
    do
    {
        struct mtd_oob_buf oob;
        unsigned char oobd[mtd->mtdInfo.oobsize];
        oob.start = (u_int32_t)mtd->e;
        oob.length = mtd->mtdInfo.oobsize;
        oob.ptr = oobd;

        if (sys_ioctl(curr_fd, MEMREADOOB, &oob) != 0) {
            printk("Reading %s OOB failed: %lx\n   ", mtd->mtd_name, mtd->e);
        }
        else if (oobd[0] != (unsigned char)0xff) { //CONFIG_BAD_BLOCK_POS
            printk("skip bad block %lx\n   ", mtd->e);
            if (sys_lseek(curr_fd, mtd->mtdInfo.erasesize, SEEK_CUR) == -1) {
                printk("lseek failed.\n");
            }
            mtd->e += mtd->mtdInfo.erasesize;
            mtd->w += mtd->mtdInfo.erasesize;
            
            bad_block = 1;
        }
        else
        {
            bad_block = 0;
        }
    } while(bad_block);


    mtd->w += cnt;

#endif

    /* Just in the middle of output buffer */
    if(shift_bytes > bytes_out && shift_bytes < bytes_out + cnt) {
        /*
         * FIXME: Do not have chance to test the code in this if() block. --Argon
         */
        int off = shift_bytes - bytes_out;
        voidp start = (char *)buf + off;
        int len = cnt - off;

        ret = sys_write(curr_fd, start, len);
        if(ret != len) {
            show_info("write_buffer error: ret=%d\n", ret);
        }
        show_info("\033[33mWrite FLASH: [0x%08lX ==> 0x%08lX]\033[0m\n", shift_bytes, shift_bytes+len-1);
    }
    else {
        if(!read_from_flash(curr_fd, check_buf, cnt) && !memcmp(buf, check_buf, cnt)) {
            /* Just move FILE point if data in buf and FLASH same */
            sys_lseek(curr_fd, cnt, SEEK_CUR);
            show_info("\033[32mSkip  FLASH: [0x%08lX ==> 0x%08lX]\033[0m\n", bytes_out, bytes_out+cnt-1);

#ifdef USE_MTK_NAND_MTD_WRITE
            /* do not erase this block */
            mtd->e += cnt;
#endif
        }
        else {
#ifdef USE_MTK_NAND_MTD_WRITE
            /* need to erase the next block before writing data to it */
            while (mtd->w > mtd->e) {
                mtdEraseInfo.start = mtd->e;
                mtdEraseInfo.length = mtd->mtdInfo.erasesize;

                /* erase the chunk */
                if (sys_ioctl (curr_fd,MEMERASE,&mtdEraseInfo) < 0) {
                    printk("Erasing mtd failed: %s\n", mtd->mtd_name);
                }
                mtd->e += mtd->mtdInfo.erasesize;
            }

            // oob align
            memcpy(check_buf, buf, cnt);

			cnt = ((cnt + (oobblock-1))/oobblock)*oobblock;	

            ret = sys_write(curr_fd, check_buf, cnt);
#else
            ret = sys_write(curr_fd, buf, cnt);
#endif
            if(ret != cnt) {
                if(ret == -ENOSPC) {
                    show_info("\033[34mDiscard  it: [0x%08lX ==> 0x%08lX]\033[0m\n", bytes_out, bytes_out+cnt-1);
                }
                else {
                    show_info("write_buffer error: ret=%d\n", ret);
                }
            }
            else {
                show_info("\033[33mWrite FLASH: [0x%08lX ==> 0x%08lX]\033[0m\n", bytes_out, bytes_out+cnt-1);
            }
        }
    }

#ifndef USE_MTK_NAND_MTD_WRITE
	force_write_cnt = (upg_mtdinfo[i].size >= 0x20000) 
		? 0x20000 : upg_mtdinfo[i].size;

	if(bytes_out >= mtd_end - force_write_cnt) { // force writes to last block
		sys_lseek(curr_fd, -cnt, SEEK_CUR);
		sys_write(curr_fd, buf, cnt);
		show_info("\033[33mExtra Write FLASH: [0x%08lX ==> 0x%08lX]\033[0m\n", bytes_out, bytes_out+cnt-1);
	}
#endif


    return ;
}
#else
/*
 * cnt = WSIZE = (UNZIP_BLOCK_SIZE/2) = 32768
 * erver time write 32K data
 */
void write_buffer(voidp buf, unsigned int cnt)
{
    int ret = -1;
    /* no need write to FLASH */
    if(shift_bytes >= bytes_out + cnt ) {
        return;
    }
    /*met bad block, seek to next 16K*/
    while(sys_ioctl(flash_fd, MEMGETBADBLOCK, &cur_offset)>0)
    {
        show_info("bad block 0x%08lX\n", bytes_out );
        sys_lseek(flash_fd, BLOCKSIZE, SEEK_CUR);
        cur_offset += BLOCKSIZE;
    }

    /* Just in the middle of output buffer */
    if(shift_bytes > bytes_out && shift_bytes < bytes_out + cnt) {
        /*
         * FIXME: Do not have chance to test the code in this if() block. --Argon
         */
        int off = shift_bytes - bytes_out;
        voidp start = (char *)buf + off;
        int len = cnt - off;

        ret = sys_write(flash_fd, start, len);
        cur_offset += BLOCKSIZE;
        if(ret != len) {
            show_info("write_buffer error: ret=%d\n", ret);
        }
        show_info("\033[33mWrite FLASH: [0x%08lX ==> 0x%08lX]\033[0m\n", shift_bytes, shift_bytes+len-1);
    }
    else {
        if(!read_from_flash(flash_fd, check_buf, cnt) && !memcmp(buf, check_buf, cnt)) {
            /* Just move FILE point if data in buf and FLASH same */
            sys_lseek(flash_fd, cnt, SEEK_CUR);
            cur_offset += BLOCKSIZE;
            show_info("\033[32mSkip  FLASH: [0x%08lX ==> 0x%08lX]\033[0m\n", bytes_out, bytes_out+cnt-1);
        }
        else {
            ret = sys_write(flash_fd, buf, cnt);
            cur_offset += BLOCKSIZE;//seek to next 16K, do not care if write failed
            if(ret != cnt) {
                if(ret == -ENOSPC) {
                    show_info("\033[34mDiscard  it: [0x%08lX ==> 0x%08lX]\033[0m\n", bytes_out, bytes_out+cnt-1);
                }
                else {
                    show_info("write_buffer error: ret=%d\n", ret);
                }
            }
            else {
                show_info("\033[33mWrite FLASH: [0x%08lX ==> 0x%08lX]\033[0m\n", bytes_out, bytes_out+cnt-1);
            }
        }
    }

    return ;
}
#endif

/*
 * unzip() -- unzip image file pointed by 'zipfile_fd' to FLASH 'mtd_fd',
 *            jump over the first 'shift' bytes unziped size
 */
#ifdef SUPPORT_MULTIPAL_MTD_BLOCK
int unzip(int zipfile_fd)
#else
int unzip(int zipfile_fd, int mtd_fd, unsigned long shift)
#endif
{

    ZIPHEADER zh, *pzh;
    int ret;

    file_fd = zipfile_fd;
#ifndef SUPPORT_MULTIPAL_MTD_BLOCK
    flash_fd = mtd_fd;
    shift_bytes = shift;
#endif
#ifdef USE_MTK_NAND_MTD_WRITE
    memset(check_buf, 0xff, sizeof(check_buf));
#endif

    updcrc(NULL, 0);

    /* Read ZIP header */
    pzh = &zh;
    sys_read(zipfile_fd, (char *)pzh, sizeof(zh));


    show_info("unzip<%d>, <%d>\n", __LINE__, ntohs(xchgs(pzh->dwCompSize)));

    show_info("unzip<%d>, <%x><%x>, <%x><%x>\n",
              __LINE__, ntohl(pzh->dwSignature), ZIP_SIGNATURE,
              ntohs(xchgs(pzh->wMethod)), ZIP_METHOD);
    if ((ntohl(pzh->dwSignature) != ZIP_SIGNATURE) || (ntohs(xchgs(pzh->wMethod)) != ZIP_METHOD))
    {
        show_info("wrong file1<%d>\n", __LINE__);
        return -1;
    }
    /* Seek to the beginning of compress data */
    sys_lseek(zipfile_fd, ntohs(xchgs(pzh->wFilenameLength)) + ntohs(xchgs(pzh->wExtraLength)) - 1, SEEK_CUR);

    /* Start to decompress */
    ret = inflate();

#define MEGA   (1024*1024)
    show_info("total write 0x%08lx Bytes ( %d.%d MB ).\n", bytes_out, (int)(bytes_out/MEGA), (int)((bytes_out%MEGA)*10/MEGA));
    return (ret);
}



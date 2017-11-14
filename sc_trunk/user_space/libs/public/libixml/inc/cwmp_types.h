#ifndef _CWMP_TYPES_H_
#define _CWMP_TYPES_H_


#include <stdio.h>
#ifndef CHAR
typedef char CHAR;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef INT16
typedef short INT16;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef INT32
typedef int INT32;
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef BOOL
typedef unsigned char BOOL;
#endif

#ifndef VOID
typedef void VOID;
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef COUNTOF
#define COUNTOF(array) (sizeof (array) / sizeof (*(array)))
#endif

#ifndef T_SUCCESS
#define T_SUCCESS 0
#endif

#ifndef T_FAILURE
#define T_FAILURE -1
#endif


typedef struct obj_uniid
{
    char domain[4];
    unsigned short serial;
}OBJECT_Unify_Idt;



#endif /* _CWMP_TYPES_H_ */

#ifndef _SC_DEBUG_H_
#define _SC_DEBUG_H_
/*
 * We define some debug marco to help print debug message to stdout and file
 * with file name, function name and line number as prefix.
 *
 * Q: How can I use this file?
 * A: Enable _SC_DEBUG_ENABLE_ before include this file.
 *    You can use -D_SC_DEBUG_ENABLE_ in Makefile and then rebuild your code.
 *    If your *.c file do not contain debug macro, please do not include this file. 
 *
 * NOTE:
 *      start with SC_ means print file, function and line as prefix
 *      start with SC_X means do NOT print any prefix
 */

#ifdef _SC_DEBUG_ENABLE_

#ifndef __KERNEL__
/* kernel do not have this header file */
#include <stdio.h>
#include <string.h>
#endif

#if 0
/*
 * -1 mean need init
 *  1  mean need print debug message
 *  0  mean jump debug message
 */
static int sc_debug_enable = -1;

/*
 * If debug_file is null str, we will print all debug message.
 * Or we just print debug message which is in debug_file list.
 */
static char *debug_file[]={
	//"sfr_detect.c",
	//"start_apclient.c",
	NULL
};

#define CHECK_DEBUG()									\
{														\
	if(sc_debug_enable ==0)								\
		break;											\
	else if(sc_debug_enable == -1)						\
	{													\
		int i;												\
														\
		if(debug_file[0] == NULL)							\
			sc_debug_enable = 1;						\
		else												\
		{												\
			for(i = 0; debug_file[i] != NULL; i++)			\
			{											\
				if(strstr(__FILE__,debug_file[i]) != NULL)		\
				{										\
					sc_debug_enable = 1;				\
					break;								\
				}										\
			}											\
			if(debug_file[i] == NULL)						\
			{											\
				sc_debug_enable = 0;					\
				break;									\
			}											\
		}												\
	}													\
}														
#else
#define CHECK_DEBUG()
#endif

/* print debug message to stdout with prefix */
#define SC_PRINTF(fmt, args...)    do{   CHECK_DEBUG();    printf("[%s::%s():%d] ", __FILE__, __FUNCTION__, __LINE__),printf(fmt, ##args);}while(0)
/* print debug message to /var/glb_dbg with prefix */
#define SC_FPRINTF(fmt, args...)    do{   CHECK_DEBUG();    FILE *fp=fopen("/var/glb_dbg", "a+"); if(fp) {fprintf(fp, "[%s::%s():%d] ", __FILE__, __FUNCTION__, __LINE__);fprintf(fp, fmt, ##args);fclose(fp);}}while(0)

/* print debug message to stdout without prefix */
#define SC_XPRINTF(fmt, args...)    do{   CHECK_DEBUG();    printf(fmt, ##args);}while(0)
/* print debug message to /var/glb_dbg without prefix */
#define SC_XFPRINTF(fmt, args...)    do{   CHECK_DEBUG();    FILE *fp=fopen("/var/glb_dbg", "a+"); if(fp) {fprintf(fp, fmt, ##args);fclose(fp);}}while(0)

/* Kernel debug marco */
#define SC_PRINTK(fmt, args...)    do{   CHECK_DEBUG();    printk("<0>[%s::%s():%d] ", __FILE__, __FUNCTION__, __LINE__),printk(fmt, ##args);}while(0)
#define SC_XPRINTK(fmt, args...)    do{   CHECK_DEBUG();    printk(fmt, ##args);}while(0)

/*print message to console*/
#define SC_CFPRINTF(fmt, args...)    do{   CHECK_DEBUG();    FILE *fp=fopen("/dev/console", "a+"); if(fp) {fprintf(fp, "[%s::%s():%d] ", __FILE__, __FUNCTION__, __LINE__);fprintf(fp, fmt, ##args);fclose(fp);}}while(0)
#define SC_XCFPRINTF(fmt, args...)    do{  CHECK_DEBUG();   FILE *fp=fopen("/dev/console", "a+"); if(fp) {fprintf(fp, fmt, ##args);fclose(fp);}}while(0)

#else

#define SC_PRINTF(fmt, args...)
#define SC_FPRINTF(fmt, args...)

#define SC_XPRINTF(fmt, args...)
#define SC_XFPRINTF(fmt, args...)

#define SC_PRINTK(fmt, args...)
#define SC_XPRINTK(fmt, args...)

#define SC_CFPRINTF(fmt, args...)
#define SC_XCFPRINTF(fmt, args...)

#endif /* _SC_DEBUG_ENABLE_ */


#endif /* _SC_DEBUG_H_ */


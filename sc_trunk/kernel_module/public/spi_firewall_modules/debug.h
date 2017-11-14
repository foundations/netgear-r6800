#ifndef __DEBUG_H__
#define __DEBUG_H__

//#define DEBUG_SPI_DOS

#ifdef DEBUG_SPI_DOS
#define DEBUG	printk
#else
#define DEBUG(...)
#endif

#endif

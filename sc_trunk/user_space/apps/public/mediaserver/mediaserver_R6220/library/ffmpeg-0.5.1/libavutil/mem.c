/*
 * default memory allocator for libavutil
 * Copyright (c) 2002 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file libavutil/mem.c
 * default memory allocator for libavutil
 */

#include "config.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#if HAVE_MALLOC_H
#include <malloc.h>
#endif

#include "mem.h"

/* here we can use OS-dependent allocation functions */
#undef free
#undef malloc
#undef realloc

#ifdef _MEM_TRACE_

#if 0
#undef fprintf
#include <stdio.h>
#define mt_debug(fmt, args...) do { \
	FILE *fp = fopen("/dev/console", "w"); \
	if (fp){fprintf(fp, "<%s-%d>: ", __FUNCTION__, __LINE__);fprintf(fp, fmt, ##args);fclose(fp);}}while(0)
#else
#define mt_debug(fmt, args...) do{}while(0)
#endif

typedef struct mt_node {
	void * ptr;
	int size;
	struct mt_node *next;
} MT_NODE;

#define MULTIPLIER		-1664117991L
#define MT_HASH_SIZE 	256
#define MT_SIZE_BIGBLK      (128000)

static MT_NODE * mt_hash_table[MT_HASH_SIZE];
static int mt_size = 0;
static int mt_thresh[3] = {4000000, 7000000, 8000000};

static int mt_hashfn(unsigned long key)
{
	int i;
	unsigned long hashcode;
	unsigned char * bytes = (unsigned char*)&key;

	hashcode = 0;
	for ( i = 0; i < sizeof(unsigned long) ; i++ ) {
		hashcode = hashcode * MULTIPLIER + bytes[i];
	}
	return (hashcode&(MT_HASH_SIZE-1));
}

static void mt_free(void *ptr)
{
	MT_NODE *node, **h, *prev;
	
	if ( !ptr ) {
		mt_debug("invalid memory address.\n");
		return;
	}
	h = &mt_hash_table[mt_hashfn((unsigned long)ptr)];
	prev = node = *h;
	while ( node ) {
		if ( node->ptr == ptr ) {
			break;
		}
		prev = node;
		node = node->next;
	}
	if ( node ) {
		if (node == *h) {
			*h = node->next;
		} else {
			prev->next = node->next;
		}
		mt_size -= node->size;
		mt_debug("free %d, current used is %d\n", node->size, mt_size);
		free(node);
	} else {
		mt_debug("failed to find memory trace for address: %p \n", ptr);
	}
	free(ptr);
}

static void* mt_malloc(int size)
{
	MT_NODE * new_node, **h;
	void * ptr = NULL;

	if ((mt_size + size) > mt_thresh[2]) {
		mt_debug("fail to alloc more(%d), for current used is %d\n", size, mt_size);
		return NULL;
	}

	if ((mt_size + size) > mt_thresh[1] && size > MT_SIZE_BIGBLK) {
		mt_debug("fail to alloc more(%d), for current used is %d\n", size, mt_size);
		return NULL;
	}
#if HAVE_POSIX_MEMALIGN
	if (size)
		posix_memalign(&ptr, 16, size);
#else
	ptr = malloc(size);
#endif
	if ( !ptr ) {
		if (size) mt_debug("out of mem\n");
		return NULL;
	}
	new_node = malloc(sizeof(MT_NODE));
	if ( !new_node ) {
		mt_debug("out of mem\n");
		free(ptr);
		return NULL;
	}
	new_node->ptr = ptr;
	new_node->size = size;
	h = &mt_hash_table[mt_hashfn((unsigned long)ptr)];
	new_node->next = *h;
	*h = new_node;
	mt_size += size;
	mt_debug("new allocated %d, current used is %d,\n", size, mt_size);
	return ptr;
}
static void* mt_realloc(void *ptr, int size)
{
	MT_NODE *node, **h, *prev;
	MT_NODE * new_node;
	void * new_ptr;
	int diff;

    if (!size) {
        mt_free(ptr);
        return NULL;
    }

	if (!ptr)
		return mt_malloc(size);

	h = &mt_hash_table[mt_hashfn((unsigned long)ptr)];
	prev = node = *h;
	while ( node ) {
		if ( node->ptr == ptr ) {
			break;
		}
		prev = node;
		node = node->next;
	}

	if (node)
		diff = size - node->size;
	else
		diff = size;

	if ((mt_size + diff) > mt_thresh[2]) {
		mt_debug("fail to alloc more(%d), for current used is %d\n", size, mt_size);
		return NULL;
	}

	if ((mt_size + diff) > mt_thresh[1] && diff > MT_SIZE_BIGBLK) {
		mt_debug("fail to alloc more(%d), for current used is %d\n", size, mt_size);
		return NULL;
	}

	new_ptr = realloc(ptr, size);
	if ( !new_ptr ) {
		return NULL;
	}

	if ( node ) {
		// rm it from link.
		if (node == *h) {
			*h = node->next;
		} else {
			prev->next = node->next;
		}
		new_node = node;
	}  else {
		// should not happen.
		new_node = malloc(sizeof(MT_NODE));
		if (!new_node)
			return new_ptr; // fail to trace
	}

	new_node->ptr = new_ptr;
	new_node->size = size;
	h = &mt_hash_table[mt_hashfn((unsigned long)new_ptr)];
	new_node->next = *h;
	*h = new_node;
	mt_size += diff;
	mt_debug("new allocated %d, current used is %d,\n", diff, mt_size);
	return new_ptr;
}

void av_flush_mt(void)
{
	MT_NODE *node, *next;
	int i;
	
	for (i = 0; i < MT_HASH_SIZE; i++) {
		for (node = mt_hash_table[i]; node; node = next) {
			next = node->next;
			mt_size -= node->size;
			free(node->ptr);
			free(node);
		}
		mt_hash_table[i] = NULL;
	}
	if (mt_size != 0) {
		mt_debug("error: after flush, mt_size = %d but should be 0\n");
		mt_size = 0;
	}
}

int av_get_mt_thresh(int level)
{
    if (level < 0 || level > 3)
        return 0;
    return mt_thresh[level];
}

void av_set_mt_thresh(int level, int size)
{
    if (level >= 0 && level < 3)
        mt_thresh[level] = size;
    mt_debug("mt_thresh%d set to %d\n", level, mt_thresh[level]);
}

// must be 'av_' in name
int av_get_mt_size(void)
{
	return (mt_size);
}
#endif

/* You can redefine av_malloc and av_free in your project to use your
   memory allocator. You do not need to suppress this file because the
   linker will do it automatically. */

void *av_malloc(unsigned int size)
{
    void *ptr = NULL;
#if CONFIG_MEMALIGN_HACK
    long diff;
#endif

    /* let's disallow possible ambiguous cases */
    if(size > (INT_MAX-16) )
        return NULL;

#ifdef _MEM_TRACE_
	ptr = mt_malloc(size);
#else // MT
#if CONFIG_MEMALIGN_HACK
    ptr = malloc(size+16);
    if(!ptr)
        return ptr;
    diff= ((-(long)ptr - 1)&15) + 1;
    ptr = (char*)ptr + diff;
    ((char*)ptr)[-1]= diff;
#elif HAVE_POSIX_MEMALIGN
    if (posix_memalign(&ptr,16,size))
        ptr = NULL;
#elif HAVE_MEMALIGN
    ptr = memalign(16,size);
    /* Why 64?
       Indeed, we should align it:
         on 4 for 386
         on 16 for 486
         on 32 for 586, PPro - K6-III
         on 64 for K7 (maybe for P3 too).
       Because L1 and L2 caches are aligned on those values.
       But I don't want to code such logic here!
     */
     /* Why 16?
        Because some CPUs need alignment, for example SSE2 on P4, & most RISC CPUs
        it will just trigger an exception and the unaligned load will be done in the
        exception handler or it will just segfault (SSE2 on P4).
        Why not larger? Because I did not see a difference in benchmarks ...
     */
     /* benchmarks with P3
        memalign(64)+1          3071,3051,3032
        memalign(64)+2          3051,3032,3041
        memalign(64)+4          2911,2896,2915
        memalign(64)+8          2545,2554,2550
        memalign(64)+16         2543,2572,2563
        memalign(64)+32         2546,2545,2571
        memalign(64)+64         2570,2533,2558

        BTW, malloc seems to do 8-byte alignment by default here.
     */
#else
    ptr = malloc(size);
#endif
#endif // MT
    return ptr;
}

void *av_realloc(void *ptr, unsigned int size)
{
#if CONFIG_MEMALIGN_HACK
    int diff;
#endif

    /* let's disallow possible ambiguous cases */
    if(size > (INT_MAX-16) )
        return NULL;

#ifdef _MEM_TRACE_
	return mt_realloc(ptr, size + !size);
#else // MT
#if CONFIG_MEMALIGN_HACK
    //FIXME this isn't aligned correctly, though it probably isn't needed
    if(!ptr) return av_malloc(size);
    diff= ((char*)ptr)[-1];
    return (char*)realloc((char*)ptr - diff, size + diff) + diff;
#else
    return realloc(ptr, size);
#endif
#endif // MT
}

void av_free(void *ptr)
{
    /* XXX: this test should not be needed on most libcs */
#ifdef _MEM_TRACE_
	mt_free(ptr);
#else // MT
    if (ptr)
#if CONFIG_MEMALIGN_HACK
        free((char*)ptr - ((char*)ptr)[-1]);
#else
        free(ptr);
#endif
#endif //MT
}

void av_freep(void *arg)
{
    void **ptr= (void**)arg;
    av_free(*ptr);
    *ptr = NULL;
}

void *av_mallocz(unsigned int size)
{
    void *ptr = av_malloc(size);
    if (ptr)
        memset(ptr, 0, size);
    return ptr;
}

char *av_strdup(const char *s)
{
    char *ptr= NULL;
    if(s){
        int len = strlen(s) + 1;
        ptr = av_malloc(len);
        if (ptr)
            memcpy(ptr, s, len);
    }
    return ptr;
}


/*
 * referral.c - pickup a referral authoritative server.
 *
 * Copyright (C) 2011 sErCoMm
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "referral.h"

//#define _DEBUG_

#ifdef _DEBUG_ 
#define DPRINTF printf
#else
#define DPRINTF(...)
#endif

#define RRFIXEDSZ		10

static int length_dotted(const unsigned char *data, int offset)
{
    int original = offset;
    int l;

    if (!data)
        return -1;

    while ((l = data[offset++])) {

        if ((l & 0xc0) == (0xc0)) {
            offset++;
            break;
        }

        offset += l;
    }
    
    return offset - original;
}

static int length_question(unsigned char *message, int offset)
{
    int i;

    i = length_dotted(message, offset);
    if (i < 0)
        return i;

    return i + 4;
}

static int decode_dotted(const unsigned char *data, int offset,
				  char *dest, int maxlen)
{
    int l;
    int measure = 1;
    int total = 0;
    int used = 0;

    if (!data)
        return -1;

    while ((l=data[offset++])) {
        if (measure)
            total++;
        if ((l & 0xc0) == (0xc0)) {
            if (measure)
                total++;
            /* compressed item, redirect */
            offset = ((l & 0x3f) << 8) | data[offset];
            measure = 0;
            continue;
        }

        if ((used + l + 1) >= maxlen)
            return -1;

        memcpy(dest + used, data + offset, l);
        offset += l;
        used += l;
        if (measure)
            total += l;

        if (data[offset] != 0)
            dest[used++] = '.';
        else
            dest[used++] = '\0';
    }

    /* The null byte must be counted too */
    if (measure) {
        total++;
    }


    return total;
}

static int decode_nameserver(unsigned char *message, int offset, char *nameserver, int len, char *host)
{
    int i;
    char temp[256];
    short ns = 0;
    int original = offset;
    
    /* we don't care domain or root, etc */
    i = decode_dotted(message, offset, temp, sizeof(temp));

    if (i < 0)
        return i;
    ns = (message[offset + i] << 8) | message[offset + i + 1];
    
    i += RRFIXEDSZ;
    
    offset += i;
    
    
    
    if (ns == 2)
    {
    /* we care name server */
        i = decode_dotted(message, offset, temp, sizeof(temp));
        if (i < 0)
            return i;

        strncpy(nameserver, temp, len);
        offset += i;
    }
    else if (ns == 1)
    {
        strncpy(nameserver, temp, len);
        sprintf(host,"%d.%d.%d.%d",message[offset],message[offset+1],message[offset+2],message[offset+3]);
        offset += 4;
    }
    else
    {
        i = length_dotted(message, offset);
        if (i < 0)
            return i;
        offset += i;
    }
    
    return offset - original;
   
}


#define MAX_SERVERS 16

int pickup_referral(void *msg, int msg_len, char *server, int len)
{
    int offset = 12;
    int j, i, k = 0;
    short qrnr, annr, nsnr, rrnr;
    char *servers[MAX_SERVERS]={0};
    char temp[128]={0};
    int ret = PICKUP_FAIL;
    
    if (!server || !msg) return PICKUP_FAIL;

    qrnr = ntohs(((short int *)msg)[2]);
    annr = ntohs(((short int *)msg)[3]);
    nsnr = ntohs(((short int *)msg)[4]);
    rrnr = ntohs(((short int *)msg)[5]);
    DPRINTF("parse question %d, servers %d, rrs %d\n", qrnr, nsnr, rrnr);
    /* answer record should be empty.*/
    if (annr != 0) return PICKUP_FAIL;
    
    for (j = 0; j < qrnr; j++) {
        DPRINTF("Skipping question %d at %d\n", j+1, offset);
        i = length_question(msg, offset);
        DPRINTF("Length of question %d is %d\n", j+1, i);
        if (i < 0)
            goto error;
        offset += i;
        if (offset >= msg_len)
            goto error;
        
    }
    DPRINTF("Decoding name server at pos %d\n", offset);


    for (j = 0; j < nsnr; j++)
    {
        char host[64];
        
        DPRINTF("Decoding serverlist %d at %d\n", j+1, offset);
        
        i = decode_nameserver(msg, offset, temp, sizeof(temp), host);
        if (i<0) {
            DPRINTF("failed decode %d\n", i);
            goto error;
        }
        DPRINTF("record server %s\n", temp);

        if (temp[0] && (k<MAX_SERVERS)) servers[k++] = strdup(temp);
        
        offset += i;
        if (offset >= msg_len)
            goto error;
    }

    for (j = 0; j < rrnr; j++)
    {
        int l;
        DPRINTF("Decoding resource record %d at %d\n", j+1, offset);
        char host[64];
        
        i = decode_nameserver(msg, offset, temp, sizeof(temp), host);

        DPRINTF("server %s => %s\n", temp, host);
        if (i<0) {
            DPRINTF("failed decode %d\n", i);
            goto error;
        }

        /*ignore none server => ip */
        if (!temp[0] || !host[0])
            goto next;
        
 
        for (l = 0; l < k; l++)
            if (strcmp(servers[l],temp) == 0)
                break;

        if (l < k) /* found first one*/
        {
            DPRINTF("will use %s\n", host);
            strncpy(server, host, len);
            ret = PICKUP_OK;
            break;
        }
    next:
        offset += i; 
        if (offset >= msg_len)
            goto error;
    }
    
 error:
    for (k = 0; k < MAX_SERVERS; k++)
        if (servers[k])
            free(servers[k]);
        else
            break;
    
    return ret;
}

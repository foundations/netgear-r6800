/* $Id: getifstats.c,v 1.5 2007/12/20 11:53:05 nanard Exp $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006,2007 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include "../getifstats.h"
#include "../config.h"
//Sercomm
long get_uptime()
{
    struct sysinfo info;
    sysinfo(&info);
    return (info.uptime);
}

/*
   Copy from CGI
   direation value for
upstream: 0
downstream: 1
Return Value: bps
 */
int get_adsl_datarate(int direction)
{
    char tmp[200];
    FILE *fp;
    int datarate =0;

    if( access("/tmp/adsl/state",F_OK))  {
        /* DSL not SHOWTIME */
        return 0;
    }

    if((fp=fopen("/tmp/adsl/state","r")) == NULL)
        return 0;

    fread(tmp,128,1,fp);
    fclose(fp);

    if(strstr(tmp,"Showtime") == NULL)
        return 0;

    if(direction == 0)
    {
        fp = fopen("/tmp/adsl/upstream", "r");
        if(fp){
            fread(tmp, 32, 1, fp);
            fclose(fp);
            datarate = (atoi(tmp));	
        }
    }
    else
    {
        fp = fopen("/tmp/adsl/downstream", "r");
        if(fp){
            fread(tmp, 32, 1, fp);
            fclose(fp);
            datarate = (atoi(tmp));	
        }
    }


    return datarate;
}

    int
getifstats(const char * ifname, struct ifdata * data)
{
    FILE *f;
    char line[512];
    char * p;
    int i;
    int r = -1;

#ifdef ENABLE_GETIFSTATS_CACHING
    static time_t cache_timestamp = 0;
    static struct ifdata cache_data;
    time_t current_time;
#endif
    data->upstream_bitrate = get_adsl_datarate(0);
    data->downstream_bitrate = get_adsl_datarate(1);
    data->opackets = 0;
    data->ipackets = 0;
    data->obytes = 0;
    data->ibytes = 0;
#ifdef ENABLE_GETIFSTATS_CACHING
    current_time = time(NULL);
    if(current_time == ((time_t)-1)) {
        SYSLOG(LOG_ERR, "getifstats() : time() error : %m");
    } else {
        if(current_time < cache_timestamp + GETIFSTATS_CACHING_DURATION) {
            memcpy(data, &cache_data, sizeof(struct ifdata));
            return 0;
        }
    }
#endif
    f = fopen("/proc/net/dev", "r");
    if(!f) {
        SYSLOG(LOG_ERR, "getifstats() : cannot open /proc/net/dev : %m");
        return -1;
    }
    /* discard the two header lines */
    fgets(line, sizeof(line), f);
    fgets(line, sizeof(line), f);
    while(fgets(line, sizeof(line), f)) {
        p = line;
        while(*p==' ') p++;
        i = 0;
        while(ifname[i] == *p) {
            p++; i++;
        }
        /* TODO : how to handle aliases ? */
        if(ifname[i] || *p != ':')
            continue;
        p++;
        while(*p==' ') p++;
        data->ibytes = strtoul(p, &p, 0);
        while(*p==' ') p++;
        data->ipackets = strtoul(p, &p, 0);
        /* skip 6 columns */
        for(i=6; i>0 && *p!='\0'; i--) {
            while(*p==' ') p++;
            while(*p!=' ' && *p) p++;
        }
        while(*p==' ') p++;
        data->obytes = strtoul(p, &p, 0);
        while(*p==' ') p++;
        data->opackets = strtoul(p, &p, 0);
        r = 0;
        break;
    }
    fclose(f);
#ifdef ENABLE_GETIFSTATS_CACHING
    if(r==0 && current_time!=((time_t)-1)) {
        cache_timestamp = current_time;
        memcpy(&cache_data, data, sizeof(struct ifdata));
    }
#endif
    return r;
}


#ifndef __LIBAPCLI_SERVICE__
#define __LIBAPCLI_SERVICE__

int apcli_service_stop(int which_band);
int apcli_service_start(int which_band);

int apcli_wan_up(int which_band, char *ipaddr);
int apcli_wan_up_nowait(int which_band, char *ipaddr);

#endif

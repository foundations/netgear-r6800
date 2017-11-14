#ifndef __LIBAPCLI_LINK__
#define __LIBAPCLI_LINK__

int apcli_link_start_interface(int which_band);
int apcli_link_stop_interface(int which_band);
int apcli_link_ok(int which_band, int wait_time);

enum
{
	APCLI_MONITOR_TO_DOWN,
	APCLI_MONITOR_TO_UP,	
};

#define APCLI_LINK_MONITOR_2G "apcli_link_monitor_2g"
#define APCLI_LINK_MONITOR_5G "apcli_link_monitor_5g"

#define APCLI_LINK_MONITOR_2G_RUNNING APCLI_DIR"/link_monitor_2g_running"
#define APCLI_LINK_MONITOR_5G_RUNNING APCLI_DIR"/link_monitor_5g_running"

int apcli_start_apcli_link_monitor(int which_band, int init_state);
int apcli_stop_apcli_link_monitor(int which_band);

char *apcli_link_get_link_info_file(int which_band);
char *apcli_get_apcli_link_monitor_running_flag_file(int which_band);

#endif



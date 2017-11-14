#ifndef __LIBAPCLI_NVRAM__
#define __LIBAPCLI_NVRAM__

enum apcli_band
{
	APCLI_BAND_MIN,
	APCLI_BAND_2G,
	APCLI_BAND_5G,
	APCLI_BAND_2G5G,
	APCLI_BAND_MAX,
};

#define APCLI_DIR "/tmp/apcli"
#define APCLI_SCAN_DIR "/tmp/apcli"
#define APCLI_SCANNING APCLI_SCAN_DIR"/scanning"
#define APCLI_PARSE_MORE_DEBUG APCLI_DIR"/more_dbg"

#define APCLI_DNSHJ_PROC "/proc/net/dns_hijack_apcli"

int apcli_nvram_band_correct(int which_band);

char *apcli_nvram_get(int which_band, char *name);
int apcli_nvram_set(int which_band, char *name, char *value);

char *apcli_interface_apcli_get(int which_band);
char *apcli_interface_wlan_get(int which_band);
char *apcli_interface_group_get(int which_band);
char *apcli_band_str_get(int which_band);

/* must add \ before special characters like $, so that mtk driver can take it. */
char *apcli_specialchar_update(char *orignal_key, int is_key);

int SYSTEM(const char *format, ...);
int myPipe(char *command, char **output);

int apcli_group_has_eth();
int apcli_group_has_interface(char *interface);

int apcli_set_linkmonitor_not_found_ap(int which_band);
int apcli_clear_linkmonitor_not_found_ap(int which_band);
int apcli_get_linkmonitor_not_found_ap(int which_band);

int apcli_get_current_bw(int which_band, char *interface);
#endif

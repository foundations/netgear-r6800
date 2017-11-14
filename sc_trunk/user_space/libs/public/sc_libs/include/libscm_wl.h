#ifndef _LIBWL_H_
#define _LIBWL_H_

#define WSC_STATUS_FILE "/tmp/wscd_status"

/* wps_proc_status value meanings */
typedef enum {
	WPS_INIT = 0,
	WPS_ASSOCIATED, //now it is waiting meanning
	WPS_OK,
	WPS_TIMEOUT,
	WPS_MSG_ERR,
	WPS_SENDM2,
	WPS_SENDM7,
	WPS_MSGDONE,
	WPS_PBCOVERLAP,
	WPS_FIND_PBC_AP,
	WPS_ASSOCIATING
} EWPS_STATE;

#ifdef RTL_WIFI
#define RTL_WIFI_ASSOC_LIST_FILE1 "/proc/wlan0/sta_info"
#define RTL_WIFI_ASSOC_LIST_FILE2 "/proc/wlan0-va0/sta_info"
#define RTL_WIFI_ASSOC_LIST_FILE3 "/proc/wlan0-va1/sta_info"
#define RTL_WIFI_ASSOC_LIST_FILE4 "/proc/wlan0-va2/sta_info"
#endif

#if defined(MTK_WIFI)
#include "oid.h"
#ifndef MTK_SDK5000
#include "wps.h"
#endif

int getWscProfile(char *interface, WSC_PROFILE *wsc_profile);
unsigned int getAPPIN(char *interface);
int getWscStatus(char *interface);
unsigned char getSetupLock(char *interface);
unsigned char getRecomWidth(char *interface);
int getCurrentWscProfile(char *interface, WSC_CONFIGURED_VALUE *data, int len);
char *getWscStatusStr(int status);
void GenerateUUID();
#endif


struct wlan_info {
    char w_mac_e[18];
    int wlan_iface; /* 0: 2.4G, 1: 5G, -1: invalid and stands for the last entry */
    int ap_index; /* 0: main SSID, 1,2,3: guest SSIDs */
	char rssi; /* RSSI */
	unsigned int signal; /* signal strength, calculated by RSSI */

	char RSSI[8]; /* RSSIs per antenna */
	unsigned int link_rate;
	unsigned long tx_rate;
	unsigned long rx_rate;
	unsigned int idle;
	unsigned int assoc_time;
	char phy_mode;
};
void get_assoc_wlan_mac_iface(struct wlan_info *wlan_info_t);

int get_wl_device_iface(struct wlan_info *wl_dev, int num);
EWPS_STATE get_wsc_status();
void get_all_assoc_mac(char *assoc_wl_mac, int max_entry);

#ifdef MTK_WIFI
int mtk_wsc_profile_to_nvram(int wifi_dev);
int mtk_wsc_process(int wifi_dev);
#endif

void wps_led_off(void);
void wps_led_idle(void);
void wps_led_active(void);
void wps_led_error(void);
void wps_led_aplock(void);
void wps_led_error_sleep_idle(int max);

enum{
	WSC_DISABLED = 0,
	WSC_24G_MODE,
	WSC_5G_MODE,
	WSC_DUAL_BAND
};
int get_sc_wps_mode(void);
int ap_oid_query_info(unsigned long OidQueryCode, int socket_id, char *DeviceName, void *ptr, unsigned long PtrLength);
void GetWifiDriverVersion(int band, char *buf);
#ifdef MTK_BSP
int set_ralink_wifi_mac(char *name, char *mac);
#endif
#endif /* _LIBWL_H_ */

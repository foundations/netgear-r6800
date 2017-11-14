#ifndef __LIBAPCLI_SCAN__
#define __LIBAPCLI_SCAN__


#define APCLI_MAX_AP 100


enum 
{
	APCLI_IN_MONITOR,
	APCLI_IN_CGI,
	APCLI_IN_RC,
	APCLI_IN_RC_DEBUG,
};

struct ap_info
{
	int index;
	char ssid[33];	
	char bssid[18];
	int channel;
	int have_extch;
	int is_above_extch;
	char auth[30];
	char encry[30];
	int signal; /* 0-100 */
	int is_wps_supported;
	int is_infra;
	char mode[10];
} ;

/* if not ready, need wait longer time so that apcli_scan will do the up interface work. */
int apcli_is_ready_for_scan(int which_band);

/* scan function */
int apcli_scan_init(int which_band, int request_from); // scan in cgi or apcli monitor, if need scan in other cgi type or monitor, just add more type
int apcli_scan(int which_band, int request_from, char *ssid); // do the scan 
int apcli_scan_clean(int which_band, int request_from); // delete the temp file

void apcli_scan_sync_remote_ap_channel(int which_band, int request_from, int *is_found, int *is_updated);

/* result function */
struct ap_info *apcli_get_apclist(int which_band, int request_from);
struct ap_info *apcli_match_ap(struct ap_info *ap, char *ssid, char* bssid);
int apcli_list_ap(struct ap_info *APs);

int apcli_mark_no_rescan_in_wizard(int which_band);
int apcli_no_rescan_in_wizard(int which_band);
int apcli_clear_no_rescan_in_wizard(int which_band);

#endif

#ifndef _FLASH_H_
#define _FLASH_H_
#include "sc_project_defines.h"

/*sc:
char* sc_get_pin( char* buf, int len);

	Get the {PIN code} string to the buf
	The length of {PIN code} should be 8

Parameters:
	buf: return buf
	len: buf length (include the '\0' char)

Return:
	return buf if success, NULL if fail

*/
#ifdef PROJECT_D7000
#define BUF_LEN_PIN 10	// length of pin =8, +1 ('\0')
#else
#define BUF_LEN_PIN 13	// length of pin =12, +1 ('\0')
#endif
char* sc_get_pin( char* buf, int len);



/*sc:
char* sc_get_csn( char* buf, int len);

	Get the {csn} string to the buf
	The max length of {sn/serial no} is 42

Parameters:
	buf: return buf
	len: buf length (include the '\0' char)

Return:
	return buf if success, NULL if fail

*/
#define BUF_LEN_CSN 43
char* sc_get_csn( char* buf, int len);

/*sc:
char* sc_get_sn( char* buf, int len);

	Get the {sn/serial no} string to the buf
	The max length of {sn/serial no} is 13

Parameters:
	buf: return buf
	len: buf length (include the '\0' char)

Return:
	return buf if success, NULL if fail

*/
#ifdef PROJECT_D7000
#define BUF_LEN_SN 14
#else
#define BUF_LEN_SN 43
#endif
char* sc_get_sn( char* buf, int len);

/*sc:
char* sc_get_mac( char* buf, int len);

	Get the mac string to the buf
	The max length of {sn/serial no} is 6

Parameters:
	buf: return buf, return mac format: 00:c0:02:11:22:33
	len: buf length (include the '\0' char)

Return:
	return buf if success, NULL if fail

*/
#define BUF_LEN_MAC 17
char* sc_get_mac( char* buf, int len);

/*sc:
int sc_get_mac_if( char *ifname, char mac[6]);

	Get the {mac addr} of the interface {ifname}
	The length of {mac} is 6

Parameters:
	ifname: interface name
	mac: returned mac address, length=6

Return:
	return 0 if success, -1 if fail

*/
int sc_get_mac_if( char *ifname, char mac[6]);

int sc_get_updown_if( char *ifname);

/*sc:
int sc_get_pid(char* buf, int len);

	Get the {PID date} from flash to buf
	The length of {PID date} is 70 // include "sErCoMm"

Parameters:
	buf: return buf
	len: buf length

Return:
	return 0 if success, -1 if fail

*/
int sc_get_pid(char* buf, int len);


struct region_related_s {
    unsigned int region;
    char *wiz_language;
    char *wifi_region;
    char *fw_time_zone;
    char *name;

    /*Add more related values for ADSL gateway*/
    char *encap;
    char *vpi;
    char *vci;
    char *wan_mode;
    char *mtu;
};

struct pid_related_s{
	unsigned int pid;
	char *name;
};

/*
 * Get DUT Region related struct point
 * Return NULL if error
 */
struct region_related_s *sc_get_region(void);

/* get domain from flash to buf
 * return 0 if success; return -1 if fail
 */
#define BUF_LEN_DOMAIN 3
char* sc_get_domain(char* buf, int len);

/* get Language ID from flash to buf
 * return 0 if success; return -1 if fail
 */
#ifdef PROJECT_D7000
#define BUF_LEN_LANG_ID 3
#else
#define BUF_LEN_LANG_ID 5
#endif
char* sc_get_country(char* buf, int len);

/* get PCBA SN from flash to buf
 * return 0 if success; return -1 if fail
 */
#define BUF_LEN_PCBA_SN 17
char* sc_get_pcba_sn(char* buf, int len);

#define BUF_LEN_SSID 21
char* sc_get_ssid(char* buf, int len);
char* sc_get_def_ssid( char* buf, int len);

#define BUF_LEN_WPAKEY 65
char* sc_get_phrase(char* buf, int len);
char* sc_get_def_wpakey( char* buf, int len);

#define BUF_LEN_PRODUCT_ID 5
#if 0
char *sc_get_product_id(char *buf, int len);
int sc_set_product_id(char *buf);
#endif
char *sc_get_model_id(char *buf, int len);
int sc_set_model_id(char *buf);

#ifdef USE_CHK_CAL
#define BUF_LEN_CAL 6
int sc_get_cal(char* buf, int len);
#endif

/* open mtd block */
int sc_set_close();

/* close mtd block */
int sc_set_open();

/* assign MAC to device
 * return 1 if success; return -1 if fail
 */
int sc_set_mac(char *buf);

/* assign CSN to device
 * return 1 if success; return -1 if fail
 */
int sc_set_csn(char *buf);
/* assign PIN to device
 * return 1 if success; return -1 if fail
 */
int sc_set_pin(char *buf);

/* assign domain to device
 * return 1 if success; return -1 if fail
 */
int sc_set_domain(char *buf);

/* assign language ID to device
 * return 1 if success; return -1 if fail
 */
int sc_set_country(char *buf);

/* assign PCBA SN to device
 * return 1 if success; return -1 if fail
 */
int sc_set_pcba_sn(char *buf);

int sc_set_ssid(char *buf);
int sc_set_phrase(char *buf);
int sc_set_wpakey(char *buf);

#ifdef MT_CODE
//int sc_set_mt_fw_mode(char *buf);
#endif

char *sc_get_flash_region(char *buf);

int sc_set_remotescfgmgrmagic(char *buf);
int sc_clear_remotescfgmgmagic();
int sc_check_remotescfgmgrmagic();
int region_is_NA();
#ifdef PROJECT_D7000
/* assign REGION to device
 * return 1 if success; return -1 if fail
 */
int sc_set_region(char *buf);
char *sc_get_burning_time(char* buf, int len);
int sc_set_burning_time(char *buf);
char *sc_get_downloadmode(char* buf, int len);
int sc_set_downloadmode(char *buf);
#endif

int set_wireless_time_zone(char * language,char *wireless_apply,char *schedule_apply);
#endif /*_LIBCOMM_SC_H_*/

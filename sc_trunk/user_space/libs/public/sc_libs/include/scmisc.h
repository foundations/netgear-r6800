#ifndef _SCMISC_H_
#define _SCMISC_H_
char *get_filter_str(char *buf, char token1, char token2);

/*
 * tuple struct for convert from name to value or from value to name
 */
struct tuple_s {
    char *name;
    char *value;
};

char *get_tuple_val(char *name, struct tuple_s *tuples);
char *get_tuple_name(char *value, struct tuple_s *tuples);
int myPipe(char *command, char **output);
int COMMAND(const char *format, ...);
void get_all_assoc_wired_mac(char *assoc_wired_mac, int max_len);
int region_isNA(void);
int region_isPR_RU(void);
int region_isRU(void);
int region_isGR(void);
int region_isPR(void);
int region_isPE(void);
int region_supMIII(void);
int l2tp_supported();
char *get_region(void);
int need_run_download_stringtable(void);

int vlan_hidden_Intranet(void);

int process_exist(char *name);

int modelinfo_get_product_name(char *buff, int len);
int modelinfo_get_5g_support_level(void);
char *modelinfo_get_autoupg_productname();
void modelinfo_set_autoupg_fileinfo();
int modelinfo_get_fbwifi_show(void);
int modelinfo_get_ipv6_show(void);
int modelinfo_get_is_useGeneric(void);
int modelinfo_get_usePrefix(void);
int modelinfo_get_gband_use_256qam(void);
int modelinfo_get_pnpx_hw_id(char *buff, int len);
int modelinfo_get_guest_led_replace_usb_led();

enum
{
	SC_5G_SUPPORT_11AC=0,
	SC_5G_SUPPORT_11N_ONLY
};

#endif /* _SCMISC_H_ */


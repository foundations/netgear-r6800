
#ifndef __SC_WIFI_H_
#  define __SC_WIFI_H_
 /*WLAN*/ struct wlan_assoc_dev_s
{
    char mac[18];
    char ip[16];
    struct wlan_assoc_dev_s *p_next;
};

typedef struct wlan_assoc_info_s
{
    int total_assoc;
    struct wlan_assoc_dev_s *devs;
} wlan_assoc_info_t;


/* wifi information */
int sc_get_wlan_assoc_info(wlan_assoc_info_t * p_wlan_assoc_info);
void sc_free_wlan_assoc_info(wlan_assoc_info_t * p_wlan_assoc_info);


#endif

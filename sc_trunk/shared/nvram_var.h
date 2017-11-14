#ifndef _NVRAM_VAR_H_
#define _NVRAM_VAR_H_

#ifndef __KERNEL__ //due to some macros below would conflict with MTK driver
//----------------------------------
// nvram for interface group
//----------------------------------

#define IFGROUP_MAP         "group_map"
#define IFGROUP_MODE        "group_mode"
#define IFGROUP_DEFAULT     "group_default"

#define IFGROUP_NUM         "groupn"
#define WLAN_NUM            "wifi_ssidn"
#define VOICE_NUM           "phonen"

#define WAN_BASE            "WAN"
#define WLAN_BASE           "SSID"
#define VLAN_BASE           "VLAN"
#define VOICE_BASE          "Phone"

#define L2_DSL				"DSL"
#define L2_VDSL				"VDSL"
#define L2_ETH				"Ethernet"
#define L2_DONGLE_3G                       "dongle_3g"
#define AUTO_WAN	"auto"
#define DSL_WAN		"dsl"
#define ETH_WAN		"eth"
#define DONGLE_3G_WAN   "dongle_3g"

#define IFGROUP_IP          "lan_ipaddr"
#define IFGROUP_MASK        "lan_netmask"
#define IFGROUP_BADDR       "lan_bipaddr"    //broadcast address
#define IFGROUP_IF          "lan_if"

#define IFMAP_VLAN          "vlan_if"
#define IFMAP_WLAN          "wifi_if"

#define DEFUALT_PVC_NUM     "default_pvc_num"

//----------------------------------
// nvram for vlan
//----------------------------------

#define  VLAN_OPT           "vlan_on"    // enable, disable
#define  VLAN_MODE          "vlan_mode" //  0: port-based vlan, 1:tag-based vlan
#define  VLAN_MODE_PORT     0
#define  VLAN_MODE_TAG      1
#define  PORTN              "portn"          //total number of physical ports
#define  VLAN_NUM           "vlann"
#define  VLAN_MAP           "vlan_map"      // Argon: FIXME
#define  VLAN_MAP_PORT      "vlan_map"
#define  VLAN_MAP_TAG       "tags"

//----------------------------------
// nvram for wan
//----------------------------------
// Common
#define PVC_NUM             "pvcn"
#define WAN_PROTO           "wan_mode"
#define WAN_IFNAME          "wan_ifname"
#define WAN_IPTYPE          "wan_iptype"

#define WAN_MODE_DHCP       "dhcpc"
#define WAN_MODE_PPPOE      "pppoe"
#define WAN_MODE_PPTP       "pptpc"
#define WAN_MODE_IPOA       "ipoa"
#define WAN_MODE_PPPOA      "pppoa"
#define WAN_MODE_FIXIP      "ip"
#define WAN_MODE_IPOA       "ipoa"
#define WAN_MODE_BPA        "bpa"
#define WAN_MODE_L2TP       "l2tp"
#define WAN_MODE_BRIDGE		"bridge"
#define WAN_MODE_HSDPA		"hsdpa"

/*START------>VALUE USED IN "RUS_WIZ" FEATURE*/
#define BRS_RUS_TMP_WAN_MODE		"brs_rus_tmp_wan_mode"
#define BRS_RUS_WIZ_MODE				"brs_rus_wiz_mode"		//auto or manual
#define BRS_RUS_CITY_IDX				"brs_rus_city_index"
#define BRS_RUS_ISP_IDX				"brs_rus_isp_index"
#define BRS_RUS_CONN_TYPE_IDX		"brs_rus_conn_type_index"
#define BRS_RUS_MAC_SPOOF_FLAG		"brs_rus_mac_spoof_flag"
#define BRS_RUS_VPN_SERV_ADDR		"brs_rus_vpn_server_address"
#define BRS_RUS_PPPOE_SERV_NAME		"brs_rus_pppoe_service_name"
#define BRS_RUS_IP					"brs_rus_ip_addr"
#define BRS_RUS_NETMASK				"brs_rus_netmask"
#define BRS_RUS_GATEWAY				"brs_rus_gateway"
#define BRS_RUS_PRI_DNS				"brs_rus_pri_dns"
#define BRS_RUS_SEC_DNS				"brs_rus_sec_dns"
/*END------>VALUE USED IN "RUS_WIZ" FEATURE*/

#define VPI                 "wan_vpi"
#define VCI                 "wan_vci"
#define ENCAP               "wan_encap"

// settings value range
#define NO_VALUE            (-1)
#define VPI_MIN             0
#define VPI_MAX             255
#define VCI_MIN             31
#define VCI_MAX             63353
#define ENCAP_MIN           0             // LLC
#define ENCAP_MAX           1             // VC

// ATM_QOS
#define ATM_QOS             "atm_qos"       // ubr, cbr, vbr, vbrrt
#define ATM_PCR             "atm_pcr"
#define ATM_SCR             "atm_scr"
#define ATM_MBS             "atm_mbs"

#define WAN_MAC             "wan_macaddr"
#define WAN_MAC_DEFAULT     "wan_macaddr_def"
#define WAN_MTU             "wan_mtu"
#define WAN_DEFAULT_MAC     "wan_def_mac"
#define WAN_MIN_MTU         616          /* Follow wnr2000v2 */
// DNS
#define FIXED_DNS_MODE      "wan_fix_dns"
#define WAN_DOMAIN          "wan_domain"
#define DNS1                "wan_dns1"
#define DNS2                "wan_dns2"
#ifdef AMPED
#define DNS3				"wan_dns3"
#endif

// IP
#define WAN_FIX_IP_ADDR     "fix_ipaddr"
#define WAN_FIX_IP_MASK     "fix_netmask"
#define WAN_FIX_IP_GW       "fix_gateway"

// DHCPC
#define HOST_NAME           "wan_account"

// PPPoE
#define PPPOE_USER          "pppoe_username"
#define PPPOE_PW            "pppoe_password"
#define PPPOE_IPADDR        "pppoe_ipaddr"
#define PPPOE_IDLE          "pppoe_idle"
#define PPPOE_SERVICE       "pppoe_service"
#define PPPOE_SID           "pppoe_sid"
#define PPPOE_SRV_ADDR      "pppoe_srv_addr"
#define PPPOE_FIX_IP        "pppoe_fix_ip"
#define PPPOE_FIXIP_MASK    "pppoe_subnet_mask"

// PPPoA
#define PPPOA_USER          "pppoa_username"
#define PPPOA_PW            "pppoa_password"
#define PPPOA_IPADDR        "pppoa_ipaddr"
#define PPPOA_IDLE          "pppoa_idle"

// IPOA
#define IPOA_IPADDR         "ipoa_ipaddr"
#define IPOA_MASK           "ipoa_netmask"
#define IPOA_GW             "ipoa_gateway"

#define IPOA_MODE           "ipoa_mode"

#define WAN_IF              "wan_ifname"
#define WAN_IF_ETH          "wan_ifname_eth"
#define WAN_IF_MAX          24
#define WAN_DOD             "wan_dod"      // dial-on-demand
#define WAN_SCAN_PORT_OPT   "fw_spi"     // scan port option
//IPV6
#define WAN_ACTYPE	    "wan_wanAcType"
#define WAN_ACTYPE2	    "wan_wanAcType2"
#define WAN_ACTYPE3	    "wan_wanAcType3"

#define WAN_TRAFFIC_TYPE "wan_traffic_type"
#define WAN_DSLMODE "wan_dslmode"

// PPTP
#define PPTPC_USER           "pptpc_user"
#define PPTPC_PASSWORD       "pptpc_password"
#define PPTPC_FIX_IP         "pptpc_fix_ip"
#define PPTPC_FIXIP_MASK     "pptpc_subnet_mask"
#define PPTPC_SERVER         "pptpc_server"
#define PPTPC_GWIP           "pptpc_gateway"
#define PPTPC_CONNID         "pptpc_connid"
#define PPTPC_IDLETIME       "pptpc_idletime"
#define PPTPC_DNSTYPE        "wan_fix_dns"
#define PPTPC_DNS1           "wan_dns1"
#define PPTPC_DNS2           "wan_dns2"
#define PPTPC_MACTYPE        "pptpc_mactype"
#define PPTPC_MACADDRESS     "pptpc_macaddress"

// L2TP
#define L2TP_USER           "l2tp_user"
#define L2TP_PASSWORD       "l2tp_password"
#define L2TP_FIX_IP         "l2tp_fix_ip"
#define L2TP_FIXIP_MASK     "l2tp_subnet_mask"
#define L2TP_SERVER         "l2tp_server"
#define L2TP_GWIP           "l2tp_gateway"
#define L2TP_IDLETIME       "l2tp_idletime"
#define L2TP_DNSTYPE		   "wan_fix_dns"
#define L2TP_DNS1           "wan_dns1"
#define L2TP_DNS2           "wan_dns2"

//----------------------------------
// nvram for firewall
//----------------------------------

#define FW_DISABLE_OPT      "fw_disable"
#define FW_NAT_OPT          "fw_nat"

#define FW_REMOTE_OPT       "fw_remote"      // remove control
#define FW_REMOTE_WAN       "fw_remote_wan"  // remove control for this wan
#define FW_REMOTE_TYPE      "fw_remote_type"
#define FW_REMOTE_PORT      "fw_remote_port"
#define FW_REMOTE_HOST      "fw_remote_single"

#define FW_REMOTE_RSTART    "fw_remote_range_start"
#define FW_REMOTE_REND      "fw_remote_range_end"

#define FW_INBOUND_RULES    "fw_in_rules"
#define FW_OUTBOUND_RULES   "fw_out_rules"

#define FW_RESP_PING_OPT    "fw_response_ping"

#define FW_DMZ_OPT          "fw_dmz_enable"
#define FW_DMZ_SERV         "fw_dmz"

#define FW_SERVICES         "fw_services"
#define FW_SERVICES_DEF     "fw_services_def"
#define FW_SERVICE_BLOCK    "fw_service_block"

// Block site options
#define BLOCK_NEVER         0
#define BLOCK_ALWAYS        1
#define BLOCK_PER_SCHEDULE  2

#define FW_IMPORT           "fw_import"
#define FW_BLOCKSITE_OPT    "fw_block"
#define FW_BLOCKSITE_TRUST_OPT  "fw_block_trust_enable"
#define FW_BLOCKSITE_TRUST_HOST "fw_block_trust"
#define FW_BLOCKSITE_KEYWORDS   "fw_block_keyword"

#define FW_PORT_FORWARD_RULES	"port_forward_rules"
#define FW_PORT_TRIGGER_RULES	"port_trigger_rules"
#define FW_PORT_TRIGGER_TIMEOUT	"port_trigger_timeout"
//----------------------------------
// nvram for voip / sip
//----------------------------------

#define VOIP_IP_OLD         "voip_ip_old"
#define SIP_LOCAL_PORT      "sip_local_port"
#define SIP_RTP_PORT        "sip_rtp_port"

//----------------------------------
// nvram for firewall
//----------------------------------
#define LOG_ASITES          "log_asites"
#define LOG_BLOCK           "log_block"
#define LOG_LOGIN           "log_login"
#define LOG_OPERATION       "log_gateway"
#define LOG_DOS             "log_dos"
#define LOG_FWD             "log_fwd"
#define LOG_WLAN            "log_wlan"
#define LOG_USB				"log_usb"
#define LOG_3G				"log_3g"
#define LOG_VPN				"log_vpn"
#define LOG_TYPE            "log_type"
#define LOG_REMOTE_IP       "log_remote_ip"
#define FW_TIME_ZONE        "fw_time_zone"
// ------------------------------------------------------------
//    debug
// ------------------------------------------------------------

#define RC_DBG_OPT          "debug_rc"
#define RC_TRACE_OPT        "trace_rc"

// ------------------------------------------------------------
//    TR069
// ------------------------------------------------------------

#define CWMPD_OPT           "cwmp_enable"
#define CWMPD_PORT          "cwmp_cpeport"

// ------------------------------------------------------------
//    dhcp server
// ------------------------------------------------------------

#define DHCP_SERVER_OPT     "dhcp_server_enable"
#define DHCP_START_IP       "dhcp_start_ip"
#define DHCP_END_IP         "dhcp_end_ip"
#define DHCP_RESERVED       "dhcp_reserved"
#define DHCP_LEASETIME      "dhcp_lease_time"

// ------------------------------------------------------------
//    telnet server
// ------------------------------------------------------------

#define TELNET_SERVER_OPT   "telnet_active_status"

// ------------------------------------------------------------
//    upnp
// ------------------------------------------------------------

#define UPNP_OPT            "upnp_enable"

// ------------------------------------------------------------
//    http server
// ------------------------------------------------------------
#define HTTP_TIMEOUT        "http_timeout"
#define WIZ_LANGUAGE        "wiz_language"
#define WIZ_LANGUAGE2       "wiz_language2"
#define AUTO_LANG_CHG 		"auto_lang_chg"
#define ENABLE_GUISTRINGTABLE "enable_guistringtable"
#define LANG_USER_SELECT        "lang_user_select"
// ------------------------------------------------------------
//    ddns
// ------------------------------------------------------------
#define DDNS_OPT            "ddns_enable"
#define DDNS_SERVER         "ddns_service_provider"
#define DDNS_USERNAME       "ddns_user_name"
#define DDNS_PW             "ddns_password"
#define DDNS_HOSTNAME       "ddns_host_name"
#define DDNS_WILDCARDS      "ddns_use_wildcards"
#define DDNS_3322_USERNAME  "ddns_3322_user_name"
#define DDNS_3322_PW        "ddns_3322_password"
#define DDNS_3322_HOSTNAME  "ddns_3322_host_name"
#define DDNS_3322_WILDCARDS "ddns_3322_use_wildcards"
#define DDNS_PEANUTHULL_USERNAME  "ddns_peanuthull_user_name"
#define DDNS_PEANUTHULL_PW        "ddns_peanuthull_password"
#define DDNS_WAN            "ddns_wan_num"
#define DDNS_NOIP_USERNAME  "ddns_noip_user_name"
#define DDNS_NOIP_PW        "ddns_noip_password"
#define DDNS_NOIP_HOSTNAME  "ddns_noip_host_name"

// ------------------------------------------------------------
//    wifi
// ------------------------------------------------------------
#define WIFI_SUPER_WIFI_ENABLE        "wifi_super_wifi_enable"
#define WIFI_SUPER_WIFI_ENABLE1        "wifi_super_wifi_enable1"
#define WIFI_POWERPERCENT        "wifi_powerpercent"
#define WIFI_POWERPERCENT1        "wifi_powerpercent1"
#define WIFI_PRESENT        "wifi_present"
#define WIFI_MAC            "wifi_mac"
#define WIFI_SSID           "wifi_ssid"
#define WIFI_PSK            "wifi_psk_pwd"
# if 1 //JHOSHUA
	#define WIFI_AP_MODE            "wifi_ap_mode"
	#define WIFI_AP_ASSIGN          "wifi_ap_assign"
	#define WIFI_AP_DNS_ASSIGN      "wifi_ap_dns_assign"
	#define WIFI_AP_IP              "wifi_ap_ip"
	#define WIFI_AP_NETMASK         "wifi_ap_netmask"
	#define WIFI_AP_GATEWAY         "wifi_ap_gateway"
	#define WIFI_AP_DNS1            "wifi_ap_dns1"
	#define WIFI_AP_DNS2            "wifi_ap_dns2"
	#define WIFI_AP_NAME            "wifi_ap_name"
#endif
// ------------------------------------------------------------
//    Interface Path Mapping
// ------------------------------------------------------------
#define IFPATH_MAP_PERSENT  "ifmap_present" // 0: not have if path mapping.
// 1: have if path mapping.
#define IFPATH_MAP_OPT      "ifmap_enable"  // enable: enable if path mapping.
// disable: disable if path mapping.

#define IFPATH_MAP_DEF_WAN  "ifmap_dft"     // default wan for path mapping
#define IFPATH_MAP_LAN      "ifmap_lan"
#define IFPATH_MAP_SSID     "ifmap_wifi"

// ------------------------------------------------------------
//    QoS Configuration
// ------------------------------------------------------------

enum {
    QOS_CATEGORY_APP,
    QOS_CATEGORY_GAME,
    QOS_CATEGORY_MAC,
    QOS_CATEGORY_MAX,
    QOS_CATEGORY_PORT
};

#define WAN_QOS_ENABLE      "wan_qos_enable"
#define APPLY_QOS_RULES     "apply_qos_rules"
#define PREDEF_QOS_RULES    "predef_qos_rules"
#define PREDEF_QOS_RULES2   "predef_qos_rules2"

#define QOS_APP_RULES       "qos_app_rules"
#define QOS_GAME_RULES      "qos_game_rules"
#define APPLY_APP_RULES     "apply_app_rules"
#define APPLY_GAME_RULES    "apply_game_rules"
#define APPLY_MAC_RULES     "apply_mac_rules"
#define APPLY_PORT_RULES    "apply_port_rules"
#define APPLY_APP_SEQ       "apply_app_seq"
#define APPLY_GAME_SEQ      "apply_game_seq"
#define APPLY_MAC_SEQ       "apply_mac_seq"
#define APPLY_PORT_SEQ      "apply_port_seq"

#define QOS_SPRIO_TABLE_TID "qos_sprio_table_"
#define QOS_SPRIO_TABLE_INFO    "qos_sprio_table_info"
#define QOS_TOS_MODE        "qos_tos_mode"
#define QOS_TOS_ENABLE      "qos_tos_enable"
#define WMM_ENABLE          "wmm_enable"
#define QOS_8021Q_ENABLE    "qos_8021q_enable"
#define QOS_OPT             "qos_enable"
#define BWCONTROL_ENABLE    "bwcontrol_enable"

#define QOS_OPT_DFT         0       // default qos option
#define QN                  "qn"
#define QN_DFT              1               // default number of queue (bands)
#define QN_MAX              "qn_max"
#define QOS_DFT_QUEUE       "qos_dft_queue"
#define QOS_DFT_QUEUE_DFT   1
#define QU_LEN_DFT          100     // default queue length
#define QOS_BW_ALL          "qos_bw_all"
#define QOS_BW_RATE         "qos_bw_rate"
#define QOS_BW_RATIO        "qos_bw_ratio"
#define QOS_OPT_BORROW      "qos_opt_borrow"
#define QOS_OPT_BORROW_DFT  1
#define QOS_OUT_DSCP        "qos_out_dscp"
#define QOS_OUT_8021P       "qos_out_8021p"
#define QOS_BAND_DROP       "qos_band_drop"
#define QOS_ICMP_OPT        "qos_icmp_enable"
#define QOS_ICMP_BAND       "qos_icmp_queue"
#define QOS_IP_PRIO_TABLE   "qos_ip_prio_table"
#define QOS_UPLINK_RATE     "qos_uplink_rate"
#define QOS_RATE_UNIT       "qos_rate_unit"
#define QOS_ACTUAL_RATE     "get_actual_up_rate"
#define QOS_BW_SEL          "qos_bw_sel"

#define QOS_TV_ENABLE       "qos_tv_enable" // turbo video
#define QOS_TV_BANDWIDTH    "qos_tv_bandwidth"
#define QOS_TV_TRUST_IPADDR "qos_tv_trust_ipaddr"
#define QOS_TV_BUTTONSEL    "qos_tv_buttonsel"

// ------------------------------------------------------------
//    Bezeq requre generate SSID and PSK
// ------------------------------------------------------------
#define AUTO_EZSETUP        "ezsetup"

// ------------------------------------------------------------
//    Static Routes Configuration
// ------------------------------------------------------------
#define     LAN_STATIC_ROUTE_NAME   "rt_static_route"
#define     WAN_STATIC_ROUTE_NAME   "rt_wan_static_route"
// ------------------------------------------------------------
//    RIP Configuration
// ------------------------------------------------------------
#define RT_RIP_VERSION          "rt_rip_version"
#define RT_RIP_DIRECTION        "rt_rip_direction"

// ------------------------------------------------------------
//    Wireless Main SSID Configuration  2.4G
// ------------------------------------------------------------
/* wireless Basic Setting */
#define WIFI_SSID               "wifi_ssid"
#define WIFI_REGION             "wifi_region"
#define WIFI_CHANNEL            "wifi_channel"
#define WIFI_MODE               "wifi_dot11_mode"
#define WIFI_IFACE_ENABLE       "wifi_if_on"
#define WIFI_SSID_BROADCASE     "wifi_broadcast_ssid"
#define WIFI_NETWORK_ISOLATION  "wifi_dot11_iso"
#define WIFI_SECURITY_TYPE      "wifi_wep_on"
#define WIFI_BAND               "wifi_band"
/* wireless access */
#define WIFI_ACCESS_ENABLE  "wifi_access_control"
#define WIFI_ACCESS_LIST    "wifi_access_list"

/* wireless wep */
#define WIFI_WEP_AUTH_TYPE  "wifi_auth_type"
#define WIFI_WEP_KEY_LEN    "wifi_key_len"
#define WIFI_WEP_KEY_INDEX  "wifi_def_key"
#define WIFI_WEP_KEY_1      "wifi_key1"
#define WIFI_WEP_KEY_2      "wifi_key2"
#define WIFI_WEP_KEY_3      "wifi_key3"
#define WIFI_WEP_KEY_4      "wifi_key4"

/* wpa-psk/wpa2-psk */
#define WIFI_PSK_PWD        "wifi_psk_pwd"
/* 802.1x */
#define WIFI_RADDIUS_SERVER     "wifi_radius_server"
#define WIFI_RADDIUS_PORT       "wifi_radius_port"
#define WIFI_RADDIUS_KEY        "wifi_radius_login_key"
#define WIFI_RADDIUS_REKEY_SEC  "wifi_radius_rekey_secs"
/* wds */
#define WIFI_WDS_ENABLE         "wifi_wds"
#define WIFI_WDS_APMODE         "wifi_wds_apmode"
#define WIFI_WDS_5G_ENABLE      "wifi_wds1"
#define WIFI_WDS_5G_APMODE      "wifi_wds_apmode1"
#define WIFI_WDS_BRIDGE_MAC_0   "wifi_wds_bridge0mac"
#define WIFI_WDS_BRIDGE_MAC_1   "wifi_wds_bridge1mac"
#define WIFI_WDS_BRIDGE_MAC_2   "wifi_wds_bridge2mac"
#define WIFI_WDS_BRIDGE_MAC_3   "wifi_wds_bridge3mac"
#define WIFI_WDS_BRIDGE_MAC_4   "wifi_wds_bridge4mac"
#define WIFI_WDS_REPEATER_MAC_1 "wifi_wds_repeater1mac"
#define WIFI_WDS_REPEATER_MAC_2 "wifi_wds_repeater2mac"
#define WIFI_WDS_REPEATER_MAC_3 "wifi_wds_repeater3mac"
#define WIFI_WDS_REPEATER_MAC_4 "wifi_wds_repeater4mac"
#define WIFI_WDS_DIS_ASSO       "wifi_wds_dis_asso"
#define WIFI_WDS_DIS_ASSO1      "wifi_wds_dis_asso1"
#define WIFI_WDS_REPEATER_IP_1  "wifi_repeaterip1"
#define WIFI_WDS_REPEATER_IP_2  "wifi_repeaterip2"
#define WIFI_WDS_REPEATER_IP_3  "wifi_repeaterip3"
#define WIFI_WDS_REPEATER_IP_4  "wifi_repeaterip4"

/* wps config status*/
#define WIFI_WPS_MODE           "wps_mode"
#define WIFI_WPS_CONFIG_STATE   "wps_config_state"
#define WIFI_FRAG_THRE          "wifi_frag_thre"
#define WIFI_RTS_THRE           "wifi_rts_thre"
#define WIFI_PREAMBLE_MODE      "wifi_enable_shortpreamble"
#define WIFI_WPS_STA_MAC        "wps_sta_mac"
#define WIFI_WPS_PIN_ENABLE     "wps_pin_enable"
#define WIFI_WPS_PIN            "wps_device_pin"

// ------------------------------------------------------------
//    Wireless Main SSID Configuration  5G
// ------------------------------------------------------------
/* wireless Basic Setting */
#define WIFI_SSID1               "wifi_ssid1"
#define WIFI_REGION1             "wifi_region1"
#define WIFI_CHANNEL1            "wifi_channel1"
#define WIFI_MODE1               "wifi_dot11_mode1"
#define WIFI_IFACE_ENABLE1       "wifi_if_on1"
#define WIFI_SSID_BROADCASE1     "wifi_broadcast_ssid1"
#define WIFI_NETWORK_ISOLATION1  "wifi_dot11_iso1"
#define WIFI_SECURITY_TYPE1      "wifi_wep_on1"
#define WIFI_BAND1               "wifi_band1"
/* wireless access */
#define WIFI_ACCESS_ENABLE1  "wifi_access_control1"
#define WIFI_ACCESS_LIST1    "wifi_access_list1"

/* wireless wep */
#define WIFI_WEP_AUTH_TYPE1  "wifi_auth_type1"
#define WIFI_WEP_KEY_LEN1    "wifi_key_len1"
#define WIFI_WEP_KEY_INDEX1  "wifi_def_key1"
#define WIFI_WEP_KEY_11      "wifi_key11"
#define WIFI_WEP_KEY_21      "wifi_key21"
#define WIFI_WEP_KEY_31      "wifi_key31"
#define WIFI_WEP_KEY_41      "wifi_key41"

/* wpa-psk/wpa2-psk */
#define WIFI_PSK_PWD1        "wifi_psk_pwd1"
/* 802.1x */
#define WIFI_RADDIUS_SERVER1     "wifi_radius_server1"
#define WIFI_RADDIUS_PORT1       "wifi_radius_port1"
#define WIFI_RADDIUS_KEY1        "wifi_radius_login_key1"
#define WIFI_RADDIUS_REKEY_SEC1  "wifi_radius_rekey_secs1"
/* wds */
#define WIFI_WDS_ENABLE1         "wifi_wds1"
#define WIFI_WDS_APMODE1         "wifi_wds_apmode1"
#define WIFI_WDS_BRIDGE_MAC_01   "wifi_wds_bridge0mac1"
#define WIFI_WDS_BRIDGE_MAC_11   "wifi_wds_bridge1mac1"
#define WIFI_WDS_BRIDGE_MAC_21   "wifi_wds_bridge2mac1"
#define WIFI_WDS_BRIDGE_MAC_31   "wifi_wds_bridge3mac1"
#define WIFI_WDS_BRIDGE_MAC_41   "wifi_wds_bridge4mac1"
#define WIFI_WDS_REPEATER_MAC_11 "wifi_wds_repeater1mac1"
#define WIFI_WDS_REPEATER_MAC_21 "wifi_wds_repeater2mac1"
#define WIFI_WDS_REPEATER_MAC_31 "wifi_wds_repeater3mac1"
#define WIFI_WDS_REPEATER_MAC_41 "wifi_wds_repeater4mac1"
#define WIFI_WDS_DIS_ASSO1      "wifi_wds_dis_asso1"
#define WIFI_WDS_REPEATER_IP_11  "wifi_repeaterip11"
#define WIFI_WDS_REPEATER_IP_21  "wifi_repeaterip21"
#define WIFI_WDS_REPEATER_IP_31  "wifi_repeaterip31"
#define WIFI_WDS_REPEATER_IP_41  "wifi_repeaterip41"
/* 5G Hz*/
#define WIFI_WDS_5G_ENABLE      "wifi_wds1"
#define WIFI_WDS_5G_APMODE      "wifi_wds_apmode1"

/* wps config status*/
#define WIFI_FRAG_THRE1          "wifi_frag_thre1"
#define WIFI_RTS_THRE1           "wifi_rts_thre1"
#define WIFI_PREAMBLE_MODE1      "wifi_enable_shortpreamble1"
#define WIFI_WPS_STA_MAC1        "wps_sta_mac1"
/* 5G wps config state */
#define WIFI_5G_WPS_MODE        "wps_mode1"

/* wifi schedule */
#define WIFI_SCHEDULE_ON1         "wifi_schedule_on1"
#define WIFI_SCHEDULE_RULE1       "wifi_schedule_rule1"

/* NETGEAR badge LED */
#define WIFI_BADGE_LED_ENABLE  "wifi_badge_led_enable"
// ------------------------------------------------------------
//    Wireless Guest SSID Configuration
// ------------------------------------------------------------
/* MBSSID basic setting */
#define WIFI_SSID2              "wifi_ssid2"
#define WIFI_IFACE_ENABLE2       "wifi_if_on2"
#define WIFI_BROADCAST_SSID2    "wifi_broadcast_ssid2"
#define WIFI_ALLOE_GUEST2       "wifi_allow_guest2"
#define WIFI_NETWORK_ISOLATION2  "wifi_dot11_iso2"
#define WIFI_SECUTIRY_TYPE2     "wifi_wep_on2"

#define WIFI_SSID3              "wifi_ssid3"
#define WIFI_IFACE_ENABLE3       "wifi_if_on3"
#define WIFI_BROADCAST_SSID3    "wifi_broadcast_ssid3"
#define WIFI_ALLOE_GUEST3       "wifi_allow_guest3"
#define WIFI_NETWORK_ISOLATION3  "wifi_dot11_iso3"
#define WIFI_SECUTIRY_TYPE3     "wifi_wep_on3"

#define WIFI_SSID4              "wifi_ssid4"
#define WIFI_IFACE_ENABLE4       "wifi_if_on4"
#define WIFI_BROADCAST_SSID4    "wifi_broadcast_ssid4"
#define WIFI_ALLOE_GUEST4       "wifi_allow_guest4"
#define WIFI_NETWORK_ISOLATION4  "wifi_dot11_iso4"
#define WIFI_SECUTIRY_TYPE4     "wifi_wep_on4"

#define WIFI_SSID5              "wifi_ssid5"
#define WIFI_IFACE_ENABLE5       "wifi_if_on5"
#define WIFI_BROADCAST_SSID5    "wifi_broadcast_ssid5"
#define WIFI_ALLOE_GUEST5       "wifi_allow_guest5"
#define WIFI_NETWORK_ISOLATION5  "wifi_dot11_iso5"
#define WIFI_SECUTIRY_TYPE5     "wifi_wep_on5"

/* MBSSID wep */
#define WIFI_WEP_AUTH_TYPE2     "wifi_auth_type2"
#define WIFI_WEP_KEY_LEN2       "wifi_key_len2"
#define WIFI_WEP_KEY_INDEX2     "wifi_def_key2"
#define WIFI_WEP_KEY_12         "wifi_key12"
#define WIFI_WEP_KEY_22         "wifi_key22"
#define WIFI_WEP_KEY_32         "wifi_key32"
#define WIFI_WEP_KEY_42         "wifi_key42"

#define WIFI_WEP_AUTH_TYPE3     "wifi_auth_type3"
#define WIFI_WEP_KEY_LEN3       "wifi_key_len3"
#define WIFI_WEP_KEY_INDEX3     "wifi_def_key3"
#define WIFI_WEP_KEY_13         "wifi_key13"
#define WIFI_WEP_KEY_23         "wifi_key23"
#define WIFI_WEP_KEY_33         "wifi_key33"
#define WIFI_WEP_KEY_43         "wifi_key43"

#define WIFI_WEP_AUTH_TYPE4     "wifi_auth_type4"
#define WIFI_WEP_KEY_LEN4       "wifi_key_len4"
#define WIFI_WEP_KEY_INDEX4     "wifi_def_key4"
#define WIFI_WEP_KEY_14         "wifi_key14"
#define WIFI_WEP_KEY_24         "wifi_key24"
#define WIFI_WEP_KEY_34         "wifi_key34"
#define WIFI_WEP_KEY_44         "wifi_key44"

#define WIFI_WEP_AUTH_TYPE5     "wifi_auth_type5"
#define WIFI_WEP_KEY_LEN5       "wifi_key_len5"
#define WIFI_WEP_KEY_INDEX5     "wifi_def_key5"
#define WIFI_WEP_KEY_15         "wifi_key15"
#define WIFI_WEP_KEY_25         "wifi_key25"
#define WIFI_WEP_KEY_35         "wifi_key35"
#define WIFI_WEP_KEY_45         "wifi_key45"
/* MBSSID wpa */
#define WIFI_PSK_PWD2           "wifi_psk_pwd2"
#define WIFI_PSK_PWD3           "wifi_psk_pwd3"
#define WIFI_PSK_PWD4           "wifi_psk_pwd4"
#define WIFI_PSK_PWD5           "wifi_psk_pwd5"

#define WIFI_PSK_PWD12           "wifi_psk_pwd12"

/* 802.1x */
#define WIFI_RADDIUS_SERVER12     "wifi_radius_server12"
#define WIFI_RADDIUS_PORT12       "wifi_radius_port12"
#define WIFI_RADDIUS_KEY12        "wifi_radius_login_key12"
#define WIFI_RADDIUS_REKEY_SEC12  "wifi_radius_rekey_secs12"

/* ssid 3 */
#define WIFI_SSID13              "wifi_ssid13"
#define WIFI_IFACE_ENABLE13       "wifi_if_on13"
#define WIFI_BROADCAST_SSID13    "wifi_broadcast_ssid13"
#define WIFI_ALLOE_GUEST13       "wifi_allow_guest13"
#define WIFI_NETWORK_ISOLATION13  "wifi_dot11_iso13"
#define WIFI_SECURITY_TYPE13     "wifi_wep_on13"

/* MBSSID wep */
#define WIFI_WEP_AUTH_TYPE13     "wifi_auth_type13"
#define WIFI_WEP_KEY_LEN13       "wifi_key_len13"
#define WIFI_WEP_KEY_INDEX13     "wifi_def_key13"
#define WIFI_WEP_KEY_113         "wifi_key113"
#define WIFI_WEP_KEY_213         "wifi_key213"
#define WIFI_WEP_KEY_313         "wifi_key313"
#define WIFI_WEP_KEY_413         "wifi_key413"

/* MBSSID wpa */
#define WIFI_PSK_PWD13           "wifi_psk_pwd13"

/* 802.1x */
#define WIFI_RADDIUS_SERVER13     "wifi_radius_server13"
#define WIFI_RADDIUS_PORT13       "wifi_radius_port13"
#define WIFI_RADDIUS_KEY13        "wifi_radius_login_key13"
#define WIFI_RADDIUS_REKEY_SEC13  "wifi_radius_rekey_secs13"

/* ssid 4 */
#define WIFI_SSID14              "wifi_ssid14"
#define WIFI_IFACE_ENABLE14       "wifi_if_on14"
#define WIFI_BROADCAST_SSID14    "wifi_broadcast_ssid14"
#define WIFI_ALLOE_GUEST14       "wifi_allow_guest14"
#define WIFI_NETWORK_ISOLATION14  "wifi_dot11_iso14"
#define WIFI_SECURITY_TYPE14     "wifi_wep_on14"

/* MBSSID wep */
#define WIFI_WEP_AUTH_TYPE14     "wifi_auth_type14"
#define WIFI_WEP_KEY_LEN14       "wifi_key_len14"
#define WIFI_WEP_KEY_INDEX14     "wifi_def_key14"
#define WIFI_WEP_KEY_114         "wifi_key114"
#define WIFI_WEP_KEY_214         "wifi_key214"
#define WIFI_WEP_KEY_314         "wifi_key314"
#define WIFI_WEP_KEY_414         "wifi_key414"

/* MBSSID wpa */
#define WIFI_PSK_PWD14           "wifi_psk_pwd14"

/* 802.1x */
#define WIFI_RADDIUS_SERVER14     "wifi_radius_server14"
#define WIFI_RADDIUS_PORT14       "wifi_radius_port14"
#define WIFI_RADDIUS_KEY14        "wifi_radius_login_key14"
#define WIFI_RADDIUS_REKEY_SEC14  "wifi_radius_rekey_secs14"

/* AP mode */
#define WIFI_AP_MODE            "wifi_ap_mode"
#define WIFI_AP_NAME            "wifi_ap_name"
/* tmp nvram parameter */
#define WAN_PROTO_LAST			"wan_mode_last"

/* separator token */
#define ENTRY_TOKEN             (0x01)
#define ITEM_TOKEN              (0x02)

/* traffic meter */
#define TM_LIMIT_VOLUMN						"tm_limit_volumn"
#define	TM_LIMIT_DIR						"tm_limit_dir"
#define	TM_LIMIT_BY							"tm_limit_by"
#define TM_ENABLE							"tm_enable"
#define TM_ACTION_INTERNET					"tm_action_internet"

#define IGMP_OPT                            "enable_igmp"

/* others */
#define FIRST_PG_IS_WIZ         "firstpageiswiz"

#define GENIE_WIZARD_RESULT     "checking_result"

// ------------------------------------------------------------
//    3G
// ------------------------------------------------------------
#define ADSLWAN_MODE_BAK      "adslwan_mode_backup"
#define WAN_CONN_METHOD       "wan_conn_method"
#define DETECT_METHOD         "detect_method"
#define DETECT_DNS_HOSTNAME   "detect_dns_hostname"
#define DETECT_PING_IP        "detect_ping_ip"
#define DETECT_RETRY_NUM      "detect_retry_num"


#define DETECT_RETRY_INTERVAL  "detect_retry_interval"
#define DETECT_RESUME_TIME     "detect_resume_time"
#define DETECT_HW_LINK         "detect_hw_link"
#define DETECT_HWLINK_FAILTIME "detect_hwlink_failtime"

#define STR_HSDPA		"hsdpa"


#define WAN_CONN_METHOD_ADSLONLY '1'
#define WAN_CONN_METHOD_3GONLY   '2'
#define WAN_CONN_METHOD_FAILOVER '3'

#define DETECT_METHOD_DNS_NTGR   '1'
#define DETECT_METHOD_DNS_HOST   '2'
#define DETECT_METHOD_PING_IP    '3'

#define WL_RADIO_CTRL_TYPE       "wl_radio_ctrl_type"
#define HSDPA_IDLETIME           "hsdpa_idletime"

#define HSDPA_WIZARDING          "hsdpa_wizarding"
#define WIFI_BUTTON_CTL          "wifi_button_ctl"
#define _3G_RADIO_STATUS_        "3g_radio_status"

/*Here is the SMS NV*/
#define _SMS_NEW_MSG				"sms_new_msg"
#define _SMS_DELETE_INDEX_STR		"sms_new_index_str"

#endif							//__KERNEL_

#define IPV6IPV4PPP_ONE_SESSION	"ipv6ipv4ppp_one_session"
#define IPV6_PPP_STATELESS		"ipv6_ppp_stateless"
#define IPV6_DHCPC_NO_NA			"ipv6_dhcpc_no_na"

#define MTK_DISABLE_EDCCA "wifi_disable_edcca"
#define AUTOUPG_FILEINFO_IN_NV "autoupg_fileinfo_in_nv"

/* APCLI */
#define APCLI_BOOSTBAND_ENABLE "apcli_boostband_enable" // 0/1
#define APCLI_ETH_BIND_5G "apcli_eth_bind_5g" // 0/1
	
#define APCLI_REMOTEAP_CHANNEL			"apcli_remoteap_channel" 
#define APCLI_REMOTEAP_HASEXTCHAN		"apcli_remoteap_hasextchan" 
#define APCLI_REMOTEAP_ABOVEEXTCHAN	"apcli_remoteap_aboveextchan" 

#define APCLI_REMOTEAP_SSID			"apcli_remoteap_ssid" 
#define APCLI_REMOTEAP_BSSID		"apcli_remoteap_bssid" 
#define APCLI_REMOTEAP_AUTH			"apcli_remoteap_auth" 
#define APCLI_REMOTEAP_ENCRY		"apcli_remoteap_encry" 
#define APCLI_REMOTEAP_WPAKEY		"apcli_remoteap_wpakey"
#define APCLI_REMOTEAP_WEPKEY		"apcli_remoteap_wepkey" 
#define APCLI_REMOTEAP_WEPKEYIDX	"apcli_remoteap_wepkeyidx" 
#define APCLI_REMOTEAP_WEPKEYTYPE	"apcli_remoteap_wepkeytype"

#define APCLI_MACCLONE_ENABLE		"apcli_macclone_enable"
#define APCLI_MACCLONE_MAC			"apcli_macclone_mac"

/* lan_bridge_enable define now device operate in apcli mode, or, normal ethernet router mode */
#define LAN_BRIDGE_ENABLE			"lan_bridge_enable" 
#define APCLI_REMOTEAP_BAND			"apcli_remoteap_band"     // 2G 5G 

#define APCLI_ENABLE					"apcli_enable" 
#define APCLI_ACT_AS_WAN			"apcli_act_as_wan" 
#define APCLI_WAN_MODE				"apcli_wan_mode" 		// fix or dynamic 
#define APCLI_WAN_DNS_MODE			"apcli_wan_dns_mode" 		// fix or dynamic 
#define APCLI_WAN_FIXIPADDR			"apcli_wan_fixipaddr" 
#define APCLI_WAN_FIXMASK			"apcli_wan_fixmask" 
#define APCLI_WAN_FIXGW				"apcli_wan_fixgw" 
#define APCLI_WAN_FIXDNS1			"apcli_wan_fixdns1" 
#define APCLI_WAN_FIXDNS2			"apcli_wan_fixdns2" 

#define APCLI_REPEATER_DHCP_SERVER  "192.168.1.1"
#define APCLI_REPEATER_DHCP_NETMASK "255.255.255.0"
#define APCLI_REPEATER_POOL_START   "192.168.1.2"
#define APCLI_REPEATER_POOL_END     "192.168.1.200"
#define APCLI_REPEATER_LAN_BCASTADDR  "192.168.1.255"
#define APCLI_REPEATER_POOL_LEASE   30
#define APCLI_DNSHJ_DISABLE "apcli_dnshj_disable"

#define WAN_PREFERENCE "wan_preference"

/* DSL wizard log */
#define DSL_FTP_UN      "ftp"
#define DSL_FTP_PWD     "ftp"
#define DSL_FTP_IP      "60.248.155.55"
#define FTP_LOG_PATH    "D7000/"
#define DSL_LOG_PATH    "DSL_log_path"

#define AMPED_REPEATER_STP_OFF "amped_repeater_stp_off"
#define AMPED_REPEATER_DHCP_STATE "amped_repeater_dhcp_state" // 0 - auto, 1 - client, 2 - server, 3 - disable 

#define AMPED_REPEATER_DHCP_STATE_AUTO        "0"
#define AMPED_REPEATER_DHCP_STATE_CLIENT      "1"
#define AMPED_REPEATER_DHCP_STATE_SERVER     "2"
#define AMPED_REPEATER_DHCP_STATE_DISABLE     "3"



#endif                          // _NVRAM_VAR_H_


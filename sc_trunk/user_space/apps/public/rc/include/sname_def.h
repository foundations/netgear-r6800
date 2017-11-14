
#ifndef _SNAME_DEF_H_
#define _SNAME_DEF_H_


/*
	nvram wan
*/
#define CAT_WAN                     "wan"

// id=1
#define WANDEVINS                   "wandevins_"

// id=idWanDev
#define LINK_UP_NV                     "linkup_"		// 1: showtime, others: idle
#define ATMARPD_RUN                 "atmarpd_run_"
#define DSL_INIT                    "dsl_init_"   // dsl driver init flag

#define WANCONINS                   "wanconins_"
#define WAN_ACTYPE_NV               "wan_actype_"  // DSL/Ethernet

// id=idWanDev.idWanCon             
#define DEF_MAC                     "default_mac_"			// default mac addr of a WANCon
#define VPI_NV                         "vpi_"						// vpi of a WANCon
#define VCI_NV                         "vci_"						// vci of a WANCon
#define WANPPPINS                   "wancon_pppins_"
#define WANIPINS                    "wancon_ipins_"
#define WANBRINS                    "wancon_brins_"
#define WANTOTALINS					"wancon_totalins_"
#define WAN_L2IF                    "wan_l2if_" //name of nas device, alway be nasX or nasX.y
#define WAN_L2IF_REAL               "wan_l2if_real_" //name of nas device, alway be nasX
#define PPPOE_PROXY                 "poe_proxy_"
#define POEPROXY_CHECK              "poeproxy_check_"
#define WAN_L2IF_INS                    "wan_l2if_ins_" //instance of same L2 interface
#define PVC_IF                    	"pvc_if_" //name of pvc

// for AnnexB
#define WAN_VLAN_ENABLE             "wan_vlan_enable_" 
#define WAN_VLAN_ID                	"wan_vlan_id_"
#define WAN_VLAN_PRI                	"wan_vlan_pri_"

// id=idWan                         
#define WAN_ID                      "wan_id_"
#define BOOTED                      "booted_"			// boot flag for a WAN L3 conn
#define UPTIME                      "uptime_"			// uptime of a WAN L3 conn
#define PPPD_CMD                    "pppd_cmd_"		// pppd cmd line of a WAN L3 conn
#define CONN_TYPE                   "conntype_"		// used for EoA
#define ADDR_TYPE                   "addrtype_"		// used for EoA
#define DNS1_NV                        "dns1_"		// wan info, dns1
#define DNS2_NV                        "dns2_"		// wan info, dns2
#define DNS3_NV                     "dns3_"
#define GW                          "gw_"		// wan info, gateway
#define LEASE_T                     "lease_" 	// wan info, dhcp lease
#define DHCP_SER                    "dhcpser_"  // wan info, dhcp server
#define DEMAND_UP                   "demand_up_"	// 1 if pppd demand,
#define DEMAND_LOCAL                "demand_local_"	// local addr
#define DEMAND_REMOTE               "demand_remote_"	// remote addr
#define WAN_IP_OLD                  "wan_ip_old_"
#define WAN_LinkType                "wan_linktype_"
#define WAN_IF_NV                      "wanif_"	// wan info, the real wan interface in used, it may be nasX or pppX.
#define WAN_NETMASK_OLD             "wan_netmask_"
#define WAN_GROUP_ID                "group_id_"		// group of wan belong to
#define WAN_MODE_NV					"wan_mode_"
#define WAN_ENABLE_NV				"wan_enable_"
// id = vpi +vci
#define ATM                         "atm_"		// flag for bcm atmctl
#define ETH_LINK_UP                 "eth_link_up"
#define PTM                         "ptm_"		// flag for bcm atmctl

/*
	nvram wifi
*/
#define CAT_WIFI "wifi"
#define WIFI_PRESENT "wifi_present"
#define NV_WIFI_GROUP "wifi_gourp_"


/*
	nvram lan
*/
#define CAT_LAN "lan"
#define LAN_IP_OLD "lan_ip_old_"
#define LAN_NETMASK_OLD "lan_netmask_old_"
#define LAN_UPTIME "lan_uptime"
#define LAN_IFLIST "lan_iflist"
#define LAN_MODE_OLD  "lan_mode"
#define LAN_AP_ASSIGN "ap_assign"
#define LAN_AP_WAN "ap_wan_num"


/*
	nvram vlan
*/
#define CAT_VLAN "vlan"

// index =1 
#define VLAN_MODE "vlan_mode"	 // 0: disable , 1: port-based, 2: tag based
#define VLAN_MODE_PORT 0
#define VLAN_MODE_TAG 1
#define VLAN_IFLIST "viflist_"

// index = vlan id (1~ 4)
#define TAG_ID "tag_"		// tag id for tag vlan
#define GROUP_ID "group_"		// group id for tag vlan

/*
	nvram firewall
*/
#define CAT_FIREWALL "firewall"
#define FW_WAN_EVENT  "wan_event_"
#define FW_IM_PAR "im_" // idx= idGroup, 1: rule is set, 0: rule is not set
#define FW_DOS_PAR "dos_" // old dos rule
#define FW_DNS_RELAY_PAR "dns_relay_"
#define FW_USB_HTTP_PAR "usb_http_"
#define FW_USB_FTP_PAR "usb_ftp_"
#define FW_DMZ_PAR  "dmz_"
#define FW_REMOTE_PAR "remote_"
#define FW_RESPONSE_PING_PAR "response_ping_"
#define FW_MASQ_PAR "masq_"
#define FW_DISABLED_PAR "fw_disabled_"
#define FW_INBOUND_PAR "fw_inbound_"
#define FW_OUTBOUND_PAR "fw_outbound_"
#define FW_CWMP_PAR "cwmp_"
#define FW_CNAPT_PAR   "cnapt_"
#define FW_CNAPT_RACE_PAR "cnapt_race_"
#define FW_NATLIMIT_PAR "natlimit_"
#define FW_HAIRPIN_PAR  "hairpin_"
#define FW_CUDP_PAR "cudp_"
#define FW_CDMZ_PAR "cdmz_"
#define FW_UPNP_PAR "upnp_"
#define FW_ACC_LAN_PAR "acc_lan_"
#define FW_PT_PAR "pt_"
#define FW_RIP_PAR "rip_"
#define FW_TELNET_PAR "telnet_"
#define FW_SETUPWIZARD_PAR "setupwizard_"
#define FW_DNRD_LAN_PAR "dnrd_lan_"
#define FW_QOS_PAR      "qos_wanif_"
#define FW_NONAT_PAR "nonat_"
#define FW_PORT_FORWARD    "port_forward_"
// cnapt
#define CNAPT_LANIP    "cnapt_lanip_"
#define CNAPT_LANMASK  "cnapt_lanmask_"
#define CNAPT_WANIP    "cnapt_wanip_"

#define CAT_L2BR "l2br"
#define L2BRINS "l2brins_"
#define L2BRNAME "l2brname_"

// ------------------------------------------------------------
//     QoS Configuration
// ------------------------------------------------------------

 #define QOS_ENABLE "qos_enable"
 #define QOS_APP_RULES "qos_app_rules"
 #define QOS_GAME_RULES "qos_game_rules"
 #define APPLY_APP_RULES "apply_app_rules"
 #define APPLY_GAME_RULES "apply_game_rules"
 #define APPLY_MAC_RULES "apply_mac_rules"
 #define APPLY_LANID_RULES "apply_port_rules"
 #define QOS_SPRIO_TABLE_TID "qos_sprio_table_"
 #define QOS_SPRIO_TABLE_INFO "qos_sprio_table_info"
 #define QOS_TOS_MODE    "qos_tos_mode"
 #define QOS_TOS_ENABLE "qos_tos_enable"
 #define QOS_WMM_ENABLE "qos_wmm_enable"
 #define QOS_8021Q_ENABLE "qos_8021q_enable"
 #define QOS_OPT_DFT     0               // default qos option
 #define QN      "qn"
 #define QN_DFT  1               // default number of queue (bands)
 #define QN_MAX  "qn_max"
 #define QOS_DFT_QUEUE "qos_dft_queue"
 #define QOS_DFT_QUEUE_DFT 1
 #define QU_LEN_DFT      100             // default queue length
 #define QOS_BW_ALL      "qos_bw_all"
 #define QOS_BW_RATE     "qos_bw_rate"
 #define QOS_BW_RATIO  "qos_bw_ratio"
 #define QOS_OPT_BORROW  "qos_opt_borrow"
 #define QOS_OPT_BORROW_DFT  1
 #define QOS_OUT_DSCP  "qos_out_dscp"
 #define QOS_OUT_8021P  "qos_out_8021p"
 #define QOS_BAND_DROP  "qos_band_drop"
 #define QOS_ICMP_OPT  "qos_icmp_enable"
 #define QOS_ICMP_BAND "qos_icmp_queue"
 #define QOS_IP_PRIO_TABLE "qos_ip_prio_table"
 #define QOS_UPLINK_RATE "qos_uplink_rate"
 #define QOS_UPLINK_UNIT "qos_rate_unit"
 #define QOS_BW_MANUAL "qos_bwcontrol_enable"
 #define QOS_FUNCTION "qos_function_map"



/*
   	DDNS 
 */
#define DDNS_ENABLE         DDNS_ENABLED
#define DDNS_USE_WILDCARDS  "ddns_use_wildcards"
#define DDNS_WAN_NUM	    "ddns_wan_num"
#define DDNS_USER_NAME      DDNS_USERNAME
#define DDNS_HOST_NAME      DDNS_HOSTNAME
/*
	Remote Access
*/
#define REMOTE_ENABLE "fw_remote"
#define REMOTE_TYPE "fw_remote_type"
#define REMOTE_WAN "fw_remote_wan"
#define REMOTE_WANL3 "fw_remote_wanl3id"
#define REMOTE_PORT "fw_remote_port"
#define REMOTE_SINGLE "fw_remote_single"
#define REMOTE_RANGE_START "fw_remote_range_start"
#define REMOTE_RANGE_END "fw_remote_range_end"


// ------------------------------------------------------------
//     VOIP Configuration
// ------------------------------------------------------------

#define CAT_VOIP                        "voip"
#define PROV_ACTIVE_STATUS              "prov_active_status"
#define PROV_TFTP_DOMAIN                "prov_tftp_domain"
#define PROV_TFTP_DEFAULT_PORT          "prov_tftp_default_port"
#define PROV_TFTP_PRIMARY_PORT          "prov_tftp_primary_port"
#define PROV_TFTP_SECONDARY_PORT        "prov_tftp_secondary_port"
#define PROV_CFG_FILENAME               "prov_cfg_filename"
#define PROV_RC_KEY                     "prov_rc_key"
#define PROV_PERIOD_INTEVAL             "prov_period_inteval"

#define SIP_LINE1_PROXY_ENABLE                "sip_line1_proxy_enable"
#define SIP_LINE1_PROXY                       "sip_line1_proxy"
#define SIP_LINE1_PROXY_PORT                  "sip_line1_proxy_port"
#define SIP_LINE1_REG_SERVER                  "sip_line1_reg_server"
#define SIP_LINE1_REG_PORT                    "sip_line1_reg_port"
#define SIP_LOCAL_PORT                  "sip_local_port"
#define SIP_RTP_PORT                    "sip_rtp_port"
//#define SIP_CTRL_PORT                   "sip_ctrl_port"

//#define SIP_OUTBOUND_SERVER_ENABLE      "sip_outbound_server_enable"
//#define SIP_OUTBOUND_SERVER             "sip_outbound_server"
//#define SIP_OUTBOUND_SERVER_PORT        "sip_outbound_server_port"

#define VOIP_VAD_ENABLE                 "voip_vad_enable"
#define VOIP_SI_TIME                    "voip_si_time"
#define VOIP_ATPM_KEY                   "voip_atpm_key"
#define VOIP_FD_TIMEOUT                 "voip_fd_timeout"
#define VOIP_LD_TIMEOUT                 "voip_ld_timeout"

#define PSTN_RELAY_ENABLE                "pstn_relay_enable"
#define PSTN_PREFIX                     "pstn_prefix"
#define PSTN_NUM1                       "pstn_num1"
#define PSTN_NUM2                       "pstn_num2"
#define PSTN_NUM3                       "pstn_num3"
#define PSTN_NUM4                       "pstn_num4"

#define VOIP_LINE1_ENABLE               "voip_line1_enable"
#define VOIP_LINE1_USERNAME             "voip_line1_username"
#define VOIP_LINE1_PASSWORD             "voip_line1_password"
#define VOIP_LINE1_TEL_NUM              "voip_line1_tel_num"
#define VOIP_LINE1_TEL_NAME             "voip_line1_tel_name"
#define VOIP_LINE1_PREF_CODEC           "voip_line1_pref_codec"
#define VOIP_LINE1_PREF_FAX_CODEC       "voip_line1_pref_fax_codec"
#define VOIP_LINE1_ANONYMOUS_IN         "voip_line1_anonymous_in"
#define VOIP_LINE1_RING_VALID           "voip_line1_ring_valid"
#define VOIP_LINE1_RING_ID              "voip_line1_ring_id"
#define SIP_LINE1_REG_EXPIRE            "sip_line1_reg_expire"
#define SIP_LINE1_REG_HEADSTART         "sip_line1_reg_headstart"

#define VOIP_INTERFACE "sip_voip_interface"
#define VOIP_GROUPID "sip_voip_group"
#define VOIP_UATRANSPORT "ua_transport"
#define VOIP_USRAGENTPORT "ua_port"
#define VOIP_RTP_LPMIN  "rtp_lpmin"
#define VOIP_RTP_LPMAX  "rtp_lpmax"

/**
 * IGMP Proxy
 */
#define IGMP_PROXY_GROUP_NUM				"igmp_proxy_group_id"


/* tr069 */
#define CAT_TR069							"tr069"
#define TR069_WANID							"tr069_wanid_"

#define CAT_ROUTE                            "route"
#define STATIC_ROUTE                         "static_route_"

/* check_conn */
#define CAT_CHECK_CONN                       "check_conn"
#define CHKCONN_WANIF                        "wan_if"
#define CHKCONN_UPTIME                       "uptime"
#define CHKCONN_GW                           "gateway"
#define CHKCONN_ACTYPE                       "ac_type"
#define CHKCONN_TYPE                         "type"
#define CHKCONN_LINK_UP                      "link_up"

/* http */
#define CAT_HTTP                              "http"
#define HTTP_LAN_NUM                          "lan_num"
#define HTTP_LAN_IP                           "lan_ip"
#define HTTP_LAN_MASK                         "lan_mask"
#define HTTP_USB_REMOTE_PORT                  "usb_remote_port"
#define HTTP_REMOTEMG_PORT                    "remotemg_port"

/* usb */
#define CAT_USB                               "usb"
#define USB_SD_LIST                           "sd_list"

/* traffic meter */
#define CAT_TM                                "traffic_meter"
#define TM_WAN_IF                             "wan_if"
#define TM_IDS                                "tm_ids"
#define TM_DEV_LIST                           "tm_dev_list"
#define TM_MTD_PATH                           "tm_mtd_path"
#define TM_WAN_MODE                           "tm_wan_mode"
#define TM_OP_INDEX                           "tm_op_index"

/* ipv6 */
#define CAT_IPV6                               "ipv6"
#define IPV6_WAN_MODE                          "ipv6_wan_mode"
#define IPV6_WAN_IF_NAME                       "ipv6_wan_ifname"
#define IPV6_LAN6_PAR                          "ipv6_lan6_par"

#endif 


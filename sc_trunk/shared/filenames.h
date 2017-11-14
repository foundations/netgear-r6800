/*
 * All file names including application path, config file path and temp file path
 */
#ifndef _FILE_NAMES_H_
#define _FILE_NAMES_H_


/*********************************************************
 *                  Application Path                     *
 *********************************************************/
#define VCONFIG         "/sbin/vconfig"
#ifndef ECHO
#define ECHO            "/bin/echo"
#endif
#define CAT             "/bin/cat"
#define IFCONFIG        "/sbin/ifconfig"
#define KILLALL         "/usr/bin/killall"
#define INSMOD          "/sbin/insmod"
#define RMMOD           "/sbin/rmmod"
#define BRCTL           "/usr/sbin/brctl"
#define ROBOCFG         "/usr/sbin/robocfg"
#define WLCTL           "/sbin/wl"
#define SENDARP         "/sbin/sendarp"
#define NAS             "/sbin/nas"
#define PPPD            "/usr/sbin/pppd"
#define PPTP            "/usr/sbin/pptp"
#define ATMARP          "/usr/sbin/atmarp"
#define ATMARPD          "/usr/sbin/atmarpd"
#define BR2684CTL       "/usr/sbin/br2684ctl"
#define ATMCTL          "/usr/sbin/xtmctl"
#define RT2684D         "/usr/sbin/rt2684d"
#define DHCPD_APP       "/usr/sbin/udhcpd"
#define NVRAM           "/usr/sbin/nvram"
#define TC              "/usr/sbin/tc"
#define LED_APP         "/usr/sbin/led_app"
#define LED_SWITCH		"/usr/sbin/led_switch"
#define PING            "/bin/ping"
#define BIN_LN          "/usr/sbin/ln -sf"
#define SYS_UPNP	    "/usr/sbin/miniupnpd"
#define CP              "/bin/cp"
#define EBTABLES        "/usr/sbin/ebtables"
#define RC              "/usr/sbin/rc"
#define IPTABLES        "/usr/sbin/iptables"
#define NETGEAR_NTP     "/usr/sbin/netgear_ntp"
#define DHCPC_APP       "/usr/sbin/udhcpc"
#define PPPOE_APP       "/usr/sbin/pppoe"
#define ET              "/usr/sbin/et"
#define BCMNVRAM        "/usr/sbin/bcmnvram"
#define TO_DEV_NULL     " > /dev/null"

/* gordon changed */
#define ADSLCTL 		"/usr/sbin/adslctl"
#define UDHCPC 			"/usr/sbin/udhcpc"
#define LN 				"/bin/ln"
#define IP 				"/usr/sbin/ip"

/*********************************************************
 *                  Config File Path                     *
 *********************************************************/
//#define FTPD_CONF       "/etc/bftpd.conf"
//#define FTPD_PASSWD     "/var/ftpdpassword"

/* dhcp */
#define DHCPD_CONF      "/etc/udhcpd.conf"
#define DHCPD_LEASES    "/var/udhcpd.leases"
#define DHCPD_RESERVED_INFO     "/etc/udhcpd.reserved_info"

/* httpd */
#define HTTPD_CONF      "/etc/htpasswd"
#define HTTPD_GUEST_CONF      "/etc/htpasswd_guest"
#define GUI_VERSION     "/etc/gui_version"
#define NEW_GUI_VERSION "/etc/new_gui_version"

/*FILE PATH TO SAVE SMS_MESSAGES*/
#define _TMP_MESSAGE	"/tmp/sms_msginfo"
#define _SMS_READY_FOR_SEND_	"/tmp/sms_send_ready_flag"
#define _SIM_MSG_MAX_NUM	"/tmp/sms_sim_msg_max_num"
#define _SMS_SEND_PG		"3g_SMS_send.htm"
#define _SMS_INBOX_PG		"3g_SMS_read.htm"
#define _SMS_OUTBOX_PG		"3g_SMS_outbox.htm"

//gordon
#define WAN_INFO_BASE "/tmp/w/info_"
#define DEMAND_FILE "/tmp/w/demand_"

#define PPP_DEBUG    "logfile /tmp/ppp_log debug"
#define DEBUG_TMP  "/tmp/debug"
#if 1

/* mbr1000v2 has usb1.1 and usb2.0 root controller,
 * not like dng1000, he has a mixed usb1.1&usb2.0 controller.
 * And mbr1000v2 has a hub connected to usb2.0 root controller,
 * the hub port 1 is a PCIe socket(1-1.1), hub port 2 is a usb connector(1-1.2).
 */

#define USBDEV1 "/sys/bus/usb/devices/1-1"
#define USBDEV2 "/sys/bus/usb/devices/2-1"
#define USBDEV3 "/sys/bus/usb/devices/1-1.3/"
#define USBDEV4 "/sys/bus/usb/devices/1-1.4/"
#define USBDEV5 "/sys/bus/usb/devices/1-2"

/* check hub's port 1~3 to see if usb device plugged. */
#define USB_PLUGGED()  (  ((access(USBDEV1, F_OK)==0)?USBDEV1:NULL)          \
                          ?:(   ((access(USBDEV2, F_OK)==0)?USBDEV2:NULL)    \
                                ?:( ((access(USBDEV3, F_OK)==0)?USBDEV3:NULL)?:((access(USBDEV5, F_OK) ==0)?USBDEV5:NULL))  \
                            )                                                \
                       )

/* used by apps/led_pb_api/multi_pb_app.c */
#ifdef MBR1000v2_USE_CUT2HW
#define USB_CONNECTOR_PLUGGED() (access(USBDEV1, F_OK)==0 || access(USBDEV2, F_OK)==0)
#define USB_BOTH_CONNECTOR_PLUGGED() (access(USBDEV1, F_OK)==0 && access(USBDEV2, F_OK)==0)
#define USBMODULE_PATH       USBDEV3
#define USBMODULE_PORT 3   /* start from 1 */
#else
#define USB_CONNECTOR_PLUGGED() (access(USBDEV2, F_OK)==0)
#define USBMODULE_PATH USBDEV1
#define USBMODULE_PORT 1  /* start from 1 */
#endif

#define USBMODULE_PLUGGED() ((access(USBMODULE_PATH, F_OK)==0)?USBMODULE_PATH:NULL)


#else
/* 3G */
#ifdef MBR1000v2
	#define USBDEV "/sys/bus/usb/devices/1-1/"
#elif DGN1000M
	/* for DGN1000.
	 * 1-1.2 means, bus 1 have a hub, its port 2 has device plugged. */
	#define USBDEV "/sys/bus/usb/devices/1-1.2"
#else
	/* for RusDSL */
	#define USBDEV "/sys/bus/usb/devices/1-1"
#endif
#endif


#define T3G_CONFIG              "/etc/gsmtd.conf"
#define DONGLE_PPPD_WANUP       "/tmp/wan_uptime"
//#define DONGLE_PPPD_SCRIPT      "/etc/ppp/peers/3g"
//#define DONGLE_CHAT_SCRIPT      "/etc/ppp/scripts/3g.chat"
//#define DONGLE_IP_UP_SCRIPT     "/etc/ppp/scripts/up"
//#define DONGLE_IP_DOWN_SCRIPT   "/etc/ppp/scripts/down"


#define USB_OK    "/tmp/usb_ok"

#define FAILOVER_DHCPC_SCRIPT      "/etc/udhcpc.failover.script"
#define FAILOVER_DHCPCPPTPC_SCRIPT "/etc/udhcpc.failoverpptpc.script"
#define FAILOVER_DHCPCL2TP_SCRIPT "/etc/udhcpc.failoverl2tp.script"

#define FAILOVER_DAEMON_NAME      "failover"
#define FAILOVER_INTERFACE_ID     9
#define FAILOVER_INTERFACE_ID_STR "9"
#define FAILOVER_DETECT_TABLE 200
#define FAILOVER_PKT_TOS      8  // must not use 0x08, getoption32 in busybox can't recognize

/* XXX below files are also used by $(ROOT)/Makefile, $(ROOT)/apps/etc/xdsl.sh, udhcpc.failoverpptpc.script
 * so be careful if filenames are changed. */

/* 3g/choose_wan rc needs abouts 5-30s to finish,
 * use this two files to make rc run exclusively:
 * 1) rc must create RUNNING_RC_3G/RUNNING_RC_CHOOSEWAN when rc choose_wan/3g run,
 * and delete them when rc exit.
 * 2) also need apply RUNNING_RC_CHOOSEWAN to root Makefile, check DGN1000_DEV's root Makefile as example. */
#define RUNNING_RC_3G              "/var/run/rc.3g.run"
#define RUNNING_RC_CHOOSEWAN       "/var/run/rc.choose_wan.run"

#define PPTPCFAILOVER              "/tmp/pptpfailover"
#define L2TPFAILOVER              "/tmp/l2tpfailover"

/* created/deleted by rc choose_wan, used by xdsl.sh and cgi. */
#define FAILOVER_3GONLY            "/tmp/failover_3gonly"
/* created/deleted by failover daemon, used by xdsl.sh, rc adsldetectiface, cgi. */
#define FAILOVERING                "/tmp/failovering"
/* created/updated by rc adsldetectiface, udhcpc.failover.script, ip-up9.
 * deleted by failover daemon, used by rc adsldetectrule.. */
#define FAILOVER_GW_DNS            "/tmp/failovering_gw_dns"

/* created/deleted by failover daemon, used by cgi. */
#define FAILOVER2ADSLING          "/tmp/failover2adsling"
/* created/deleted by failover daemon, used by cgi. */
#define ADSL2FAILOVERING           "/tmp/adsl2failovering"

#define FAILOVER_NULL_FILE         "/dev/null"

/* created in rc_init if device is failover mode and deleted in failover.c */
#define FAILOVER_DEV_FINISH_BOOT   "/tmp/dev_boot_fin"


#define DEBUG_MBM_DIAGNOSE_PREPARE        "/tmp/diag_mbm_prepare"
#define DEBUG_MBM_DIAGNOSING              "/tmp/diag_mbm_diagnosing"
#define DEBUG_MBM_DIAGNOSE_DONE           "/tmp/diag_mbm_result.txt"


#define DEBUG_MBM_COPS_SCAN_START         "/tmp/diag_mbm_scan_start"
#define DEBUG_MBM_COPS_SCANNING           "/tmp/diag_mbm_scanning"
#define DEBUG_MBM_COPS_SCAN_DONE          "/tmp/diag_mbm_scan_done"

#define MOBWAN_WTDG_RUNNING               "/tmp/mobwan_wtdg_running"

#define MOBILE_WAN_PHNET_LOCK  "/tmp/phnet_lock"
#define DONGLESTAT_APNWRONG    "/tmp/dongle_statwrongapn"

#ifdef _ZIP_3G_
#define EX_MONITOR              "/tmp/dongle-bin/dongle_monitor"
#define EX_MGNT                 "/tmp/dongle-bin/dongle_mgnt"
#define EX_MODULE               "/tmp/dongle-bin/usbserial.ko"

#define LO_MONITOR              "/usr/sbin/dongle_monitor"
#define LO_MGNT                 "/usr/sbin/dongle_mgnt"
#define LO_MODULE               "/lib/modules/usbserial.ko"

#define DONGLE_MONITOR          "/var/usbmodem/dongle_monitor"
#define DONGLE_MGNT             "/var/usbmodem/dongle_mgnt"
#define USBSERIAL_KO            "/var/usbmodem/usbserial.ko"
#else
#define DONGLE_MONITOR          "/usr/sbin/dongle_monitor"
#define DONGLE_MGNT             "/usr/sbin/dongle_mgnt"
#define USBSERIAL_KO            "/lib/modules/2.6.36/kernel/drivers/usb/serial/usbserial.ko"
#define CDCNCM_KO               "/lib/modules/2.6.36/kernel/drivers/net/usb/cdc_ncm.ko"
#define ASIX_KO   "/lib/modules/2.6.36/kernel/drivers/net/usb/asix.ko"
#define DM9601_KO  "/lib/modules/2.6.36/kernel/drivers/net/usb/dm9601.ko"
#define CDCETHER_KO  "/lib/modules/2.6.36/kernel/drivers/net/usb/cdc_ether.ko"




#endif

/*********************************************************
 *                   Other Files Path                    *
 *********************************************************/
#define WAN_CMD_BASE    "/var/run/wan_cmd"
#define WAN_IF_BASE     "/var/run/wan_if"
#define WAN_IPADDR      "/tmp/wan_ipaddr"
#define WAN_UPTIME_BASE "/tmp/wan_uptime"
#define WAN_INTO_BASE   "/tmp/wan_info"
#define PPPOAE_NOT_START_BASE  "/tmp/PPPOAE_not_start_WAN"
//lan start time
#define LAN_PROC_UPTIME      "/proc/uptime"
#define LAN_UPTIME_BASE "/tmp/lan_uptime"
/* CSRF Protection session file*/
#define CSRF_SESSION_FILE "/tmp/SessionFile"
/* WPS running flag file */
#define WPS_RUNNING_FLAG "/tmp/wps_is_running"
// dhcpc
#define DHCPC_SCRIPT            "/etc/udhcpc.script"
#define DHCPC_FIX_SCRIPT        "/etc/udhcpc.fix.script"
#define DHCPC_WIZARD_SCRIPT     "/etc/udhcpc.wizard.script"
#define DHCPC_PPTPC_SCRIPT      "/etc/udhcpc.pptpc.script"
#define DHCPC_L2TP_SCRIPT      "/etc/udhcpc.l2tp.script"
#define DHCPC_PPPOE_SCRIPT      "/etc/udhcpc.pppoe.script"
#define DHCPC_ROUTE_RELATED_OPTIONS_DEL	"/tmp/dhcpc_route_related_options_del"
/*
 * Show dhcpc lease info
 * format of this file:
 *  <WAN DHCP Server IP Address>
 *  <Lease Time>
 *  <Uptime when get IP Address>
 */
#define DHCPC_LEASE             "/tmp/dhcpc.lease"
#define DHCPC_SERVER             "/tmp/dhcpc.server"

//pppd
#define LCP_FILE        "/tmp/lcp"
#define PPPD_FAIL       "/tmp/pppd.fail"
#define IP_DOWN_ACT     "/tmp/ip-down-act"
#define PPP_IPUP_DEMAND_BAK     "/usr/etc/ppp/ip-up-demand"
#define PPP_IPUP_BAK    "/usr/etc/ppp/ip-up"
#define PPP_IPUP        "/etc/ppp/ip-up"
#define PPTPC_GATEWAY   "/tmp/gateway"
#define PPP_MANUAL_CONNECT "/tmp/ppp_manual_connect"

// ipoa
#define IPOA_SERV       "/tmp/ipoa_server"
#define ATMARPD_CHECK   "/tmp/atmarpd.dep"
#define ATM_DEV_CHECK   "/proc/sys/net/ipv4/conf/atm"

#define PAP_SECRET_FILE    "/etc/ppp/pap-secrets"
#define CHAP_SECRET_FILE    "/etc/ppp/chap-secrets"
#define PPP_OPTION_FILE    "/etc/ppp/options"
#define PPTP_CONFIG_PATH 	"/etc/option.pptp"
#define L2TP_CONFIG_PATH 	"/etc/openl2tpd.conf"
#define OPENL2TPD_CONFIG_PATH 	L2TP_CONFIG_PATH
#define L2TP_CONFIG_FILE    		L2TP_CONFIG_PATH
#define L2TP_GATEWAY   "/tmp/gateway"

#define DEBUG_TMP       "/tmp/debug"

//Firewall
#define FW_RULE_BASE    "/tmp/rules"

//QOS
#define FW_QOS_GX		"/tmp/qos"

// TI ENV
#define TI_ENV          "/proc/ticfg/env"
#define ANNEX_VERSION   "/etc/version"

// VLAN
#define PROC_VLAN       "/proc/vlan"

// DNRD
#define BLANK_STATE    "/tmp/blank_state.out"

// NTP
#define NTP_START       "/tmp/ntp_start"

// SYSLOG
#define ALERT_FILE      "/var/log/alert"
#define MESSAGE_FILE    "/var/log/messages"
#define FIRST_LOG_FLAG  "/var/log/first"
#define LOG_SENT_BY_CGI "/var/log/log_sent_by_cgi"     /* use this flag to show log is sent by CGI */

// SYSTEM
#define PROC_IP_FWD     "/proc/sys/net/ipv4/ip_forward"
#define RESOLV_CONF     "/etc/resolv.conf"
#define PROC_SIP_ENABLE "/proc/sip_enable"
#define PROC_ARPTABLE   "/proc/net/arp"
#define PROC_WLAN_STATISTICS_ENTRY "/proc/wifi_statistics"

//WIZARD
#define WIZARD_PID              "/var/run/wizard.pid"
#define WIZARD_OUT              "/tmp/wizard.out"
#define DETECTOUT_PC            "/tmp/detect_dhcp_pc"
#define DETECTOUT_LOCAL         "/tmp/detect_dhcp_local"
#define DETECTOUT_PPTP          "/tmp/detect_pptp"
#define DETECTOUT_PPPOE         "/tmp/detect_pppoe"

#define DETECT_AP_PID           "/var/run/detect_ap.pid"
#define DETECT_AP_OUT           "/tmp/detect_ap.out"
#define DETECT_AP_FLAG		"/tmp/ap_flag"
//PPTP
#define PPTP_PHYGATEWAY         "/tmp/gateway"          /* be use by "udhcpc.pptpc.script" "pptp" */
#define PPTP_PHYIP              "/tmp/pptpc_phyip"

/*UPnP port rules file */
#define UPNP_TAB                "/tmp/upnptab"

//WIRELESS FILE  (RTL)
#define WPS_IS_UNCONFIGED       "/var/is_unconfiged"
#define WLAN_RESTART            "/var/wlan_restart"

//define file for USB related
#define USB_ADVANCED_CONF		"/etc/usb_httpd.conf"

/* Traffic Meter */
#define TM_ETHER_BLOCK_INTERNET  "/tmp/tm_block_internet"
#define TM_3G_BLOCK_INTERNET     "/tmp/tm_3g_block_internet"
#define TM_BLOCK_INTERNET        (strcmp(nvram_safe_get("wan_mode"), "hsdpa") != 0)?TM_ETHER_BLOCK_INTERNET:TM_3G_BLOCK_INTERNET
#define TM_ETHER_FLASH_LED       "/tmp/tm_flash_led"
#define TM_3G_FLASH_LED          "/tmp/tm_3g_flash_led"
#define TM_FLASH_LED             (strcmp(nvram_safe_get("wan_mode"), "hsdpa") != 0)?TM_ETHER_FLASH_LED:TM_3G_FLASH_LED
#define TM_ETHER_REC_LED         "/tmp/rec_led"
#define TM_3G_REC_LED            "/tmp/rec_3g_led"
#define TM_ALREADY_WARNING       "/tmp/tm_already_warning"

/*********************************************************
 *                   GUI Files names                     *
 *********************************************************/
#define LAN_LAN_PG              "LAN_lan.htm"
#define LAN_RSV_ADD_PG          "LAN_reserv_add.htm"
#define LAN_RSV_EDIT_PG         "LAN_reserv_edit.htm"

#define UPNP_UPNP_PG            "UPNP_upnp.htm"

#define BKS_KEYWORD_PG          "BKS_keyword.htm"
#define BKS_SERVICE_PG          "BKS_service.htm"
#define BKS_SERVICE_ADD_PG      "BKS_service_add.htm"

#define FW_CLOUD_PG				"FW_cloud.htm"
#define FW_REMOTE_PG            "FW_remote.htm"
#define FW_SCHEDULE_PG          "FW_schedule.htm"
#define FW_EMAIL_PG             "FW_email.htm"
#define FW_IPMAC_PG		"IP_Mac.htm"
#define FW_LOG_PG               "FW_log.htm"
#define FW_FORWARD_PG           "FW_forward.htm"
#define FW_FORWARD_SRV_PG       "FW_forward_service.htm"
#define FW_PT_PG                "FW_pt.htm"
#define FW_PT_SRV_PG            "FW_pt_service.htm"

#define STR_ROUTES_PG           "STR_routes.htm"
#define STR_ADD_PG              "STR_add.htm"

#define BAS_BASIC_PG            "BAS_basic.htm"
#define BAS_ETHER_PG            "BAS_ether.htm"
#define BAS_ATM_ETHER_PG	"BAS_ATM_ether.htm"
#define BAS_PTM_ETHER_PG	"BAS_PTM_ether.htm"
#define BAS_PPPOE_PG            "BAS_pppoe.htm"
#define BAS_ATM_PPPOE_PG	"BAS_ATM_pppoe.htm"
#define BAS_PPPOA_PG		"BAS_ATM_pppoa.htm"
#define BAS_PTM_PPPOE_PG	"BAS_PTM_pppoe.htm"
#define BAS_PPTP_PG             "BAS_pptp.htm"
#define BAS_L2TP_PG 			   "BAS_l2tp.htm"

#ifdef  NZ_FEATURE
#define BRS_WIZ_PPPOE_NZ_PG         "BRS_03A_B_NZ_pppoe.html"
#define BRS_WIZ_PPPOE_NZ_REENT_PG   "BRS_03A_B_NZ_pppoe_reenter.html"
#define BRS_WIZ_DHCPC_NZ_PG         "BRS_03A_B_NZ_dhcpc.html"
#define BRS_STATICIP_NZ_PG          "BRS_03A_E_NZ_IP_problem_staticIP_A_inputIP.html"
#endif

#define WIZ_SEL_PG              "WIZ_sel.htm"
#define WIZ_CFM_PG              "WIZ_cfm.htm"
#define WIZ_PH_FAIL_PG          "WIZ_ph_fail.htm"
#define WIZ_DETWAN_PG           "WIZ_detwan.htm"
#define WIZ_DETWAN_FIX_PG       "WIZ_detwan_fix.htm"
#define WIZ_DETWAN_DYN_PG       "WIZ_detwan_dyn.htm"
#define WIZ_DETWAN_PPPOE_PG     "WIZ_detwan_pppoe.htm"
#define WIZ_DETWAN_PPTP_PG      "WIZ_detwan_pptp.htm"
#define WIZ_FIX_PG              "WIZ_fix.htm"
#define WIZ_DYN_PG              "WIZ_dyn.htm"
#define WIZ_PPPOE_PG            "WIZ_pppoe.htm"
#define WIZ_PPTP_PG             "WIZ_pptp.htm"
#define WIZ_CIP_PG              "wiz_cip.htm"

#define WAN_WAN_PG              "WAN_wan.htm"

#ifdef DUAL_BAND
	#define WLG_DUALBAND_IDX_PG		"WLG_dualband_idx.htm"
	#define WLG_DUALBAND_GUEST_IDX_PG	"WLG_dualband_guest_idx.htm"
	#define WLG_DUALBAND_ADV_PG		"WLG_dualband_adv.htm"
	#define WLG_DUALBAND_WDS_PG		"WLG_dualband_wds.htm"
#endif

#define WLG_WIRELESS_PG         "WLG_wireless.htm"
#define WLG_WIRELESS1_PG        "WLG_wireless1.htm"
#define WLG_WIRELESS2_PG        "WLG_wireless2.htm"
#define WLG_WIRELESS3_PG        "WLG_wireless3.htm"
#define WLG_WIRELESS4_PG        "WLG_wireless4.htm"
#define WLG_WIRELESS_GUEST_PG   "WLG_wireless_guest.htm"
#define WLG_WIRELESS_GUEST1_PG  "WLG_wireless_guest1.htm"
#define WLG_WIRELESS_GUEST2_PG  "WLG_wireless_guest2.htm"
#define WLG_WIRELESS_GUEST3_PG  "WLG_wireless_guest3.htm"
#define WLG_ADV_PG              "WLG_adv.htm"
#define WLG_AP_PG               "WLG_ap.htm"
#define WLG_ACL_PG              "WLG_acl.htm"
#define WLG_ACL_EDIT_PG         "WLG_acl_edit.htm"
#define WLG_WDS_PG              "WLG_wds.htm"


#define WPS_PG                  "WPS.htm"
#define WPS_PIN_PG              "WPS_PIN.htm"

#define DNS_DDNS_PG             "DNS_ddns.htm"

#define QOS_MAIN_PG             "QOS_main.htm"
#define QOS_LIST_PG             "QOS_list.htm"
#define QOS_TURBO_VIDEO_PG      "QOS_turbovideo.htm"

#define TRAFFIC_METER_PG        "traffic_meter.htm"

#define RST_STATUS_PG           "RST_status.htm"
#define RST_ST_DHCP_PG          "RST_st_dhcp.htm"
#define RST_ST_FIXIP_PG         "RST_st_fix.htm"
#define RST_ST_POE_PG           "RST_st_poe.htm"
#define RST_ST_PPTP_PG          "RST_st_pptp.htm"
#define RST_ST_HSDPA_PG         "RST_st_hsdpa.htm"

//upgrade
#define UPG_CONNECT_PG			"VER_check.htm"
#define UPG_FAIL_PG				"VER_check_error.htm"
#define UPG_NEW_PG				"VER_fw_found.htm"
#define UPG_NONEW_PG			"VER_fw_not_found.htm"
#define UPG_FW_ERROR_PG			"UPG_fw_error.htm"
#define AUTO_UPG_PG				"CheckNewFW.html"
#define AUTO_UPG_NONEW_PG		"CheckNewFW_Success.html"
#define ATUO_UPG_FAIL_PG		"NoInternetConn.htm"
#define ATUO_UPG_DOWNLOADING_PG "CheckNewFW_upgrade_downloading.htm"
#define ATUO_UPG_WRITING_PG		"CheckNewFW_upgrade_writing.htm"
#define UPG_PG					"UPG_upgrade.htm"
#define UPG_WRONG_FILE          "UPG_wrong_file.htm"
#define UPDATE_SETTING_PG       "update_setting.htm"
#define REBOOT_PG               "reboot_pg.htm"
#define UPG_NEW_GUI_PG          "VER_gui_found.htm"
#define CHECK_NEW_LANG_PG       "CheckNewLang.html"
#define CHECK_LANG_NO_CONN_PG   "CheckLangNoInternetConn.htm"
#define UPDATING_GUI_PG         "updating_gui.htm"
#define UPDATING_GUI_ERR_PG     "updating_gui_err.htm"
#define MNU_TOP_PG              "MNU_top.htm"

//Genie Wizard
#define WIZARD_UPG_NONEW_PG		 "BRS_ver_fw_not_found.html" 
#define WIZARD_UPG_NEW_PG		 "BRS_ver_fw_found.html"
#define WIZARD_UPG_CONNECT_PG	 "BRS_ver_check.html"
#define WIZARD_ATUO_UPG_DOWNLOADING_PG "BRS_CheckNewFW_upgrade_downloading.html"
#define WIZARD_ATUO_UPG_WRITING_PG		"BRS_CheckNewFW_upgrade_writing.html"
#define BRS_RESTORE_FAIL_PG         "BRS_03B_haveBackupFile_fileRestore_fail.html"
#define BRS_RESTORE_PG              "BRS_03B_haveBackupFile.html"
#define BRS_RESTORE_REBOOT_PG       "BRS_03B_haveBackupFile_waitReboot.html"
#define BRS_CHECKNET_PING_PG        "BRS_01_checkNet_ping.html"
#define BRS_GENIEHELP_PG            "BRS_02_genieHelp.html"
#define BRS_NOWAN_PG                "BRS_03A_A_noWan.html"
#define BRS_DETWAN_PG               "BRS_03A_detcInetType.html"
#define BRS_WIZMSG_PG               "BRS_03A_detcInetTypeWait.html"

#define BRS_WIZPPPOE_PG             "BRS_03A_B_pppoe.html"
#define BRS_WIZPPPOE_REENT_PG       "BRS_03A_B_pppoe_reenter.html"
#define BRS_WIZPPTP_PG              "BRS_03A_C_pptp.html"
#define BRS_WIZPPTP_REENT_PG        "BRS_03A_C_pptp_reenter.html"
#define BRS_WIZDHCPC_PG             "BRS_03A_B_dhcpc.html"

#define BRS_WIZNONE_PG              "BRS_03A_E_IP_problem.html"
#define BRS_MACCLONE_PG             "BRS_03A_E_IP_problem_staticIP_B_macClone.html"
#define BRS_STATICIP_PG             "BRS_03A_E_IP_problem_staticIP_A_inputIP.html"

#define BRS_NETWORKISSUE_PG         "BRS_05_networkIssue.html"
#define BRS_SUCCESS_PG              "BRS_success.html"
#define BRS_HIJACK_SUCCESS_PG       "BRS_hijack_success.htm"
#define BRS_NTGR_SUCCESS_PG         "BRS_netgear_success.html"
#define BRS_INDEX_PG                "BRS_index.htm"
#define BRS_HIJACK_INDEX_PG         "BRS_hijack_index.htm"
#define BRS_WIZ_SEL_PG              "BRS_WIZ_sel.htm"
#define BRS_MANUAL_PG               "BRS_start.htm"
#define BRS_SHOW_BASIC              "/tmp/show_basic_setting"
#define BRS_AP_PG               "BRS_ap_setup.html"
#define INDEX_PG		    "index.htm"
#define OPEN_VPN_PG			 "openvpn.htm"

#define BRS_TIGGER_WAN_IDLE         "/tmp/brs_trigger_wan_idle"

/*START------------Russian Wizard----------*/
/*1,start with "BRS_RUS_" is common PG
 *2,start with "BRS_RUS_auto" is auto mode PG
 *3,start with "BRS_RUS_manual" is manual  mode PG
 */
#define BRS_RUS_SELECT_CITY_ISP			"BRS_RUS_select_city_isp.html"
#define BRS_RUS_NEED_ACCOUNT_CHECK		"BRS_RUS_manual_need_account_check.html"
#define BRS_RUS_INFO_CONFIRM_PG			"BRS_RUS_manual_input_info_confirm.html"
#define BRS_RUS_STATIC_IP_INPUT_PG		"BRS_RUS_manual_static_ip_input.html"

#define BRS_RUS_AUTO_PPPOE_STATIC_IP_PG	"BRS_RUS_auto_pppoe_ip.html"
#define BRS_RUS_AUTO_ACCOUNT_INPUT_PG		"BRS_RUS_auto_account_input.html"
#define BRS_RUS_AUTO_ATTENTION_PG			"BRS_RUS_auto_attention.html"
#define BRS_RUS_AUTO_MAC_SPOOF_PG			"BRS_RUS_auto_spoof_mac.html"
#define BRS_RUS_AUTO_STATIC_IP_INPUT_PG	"BRS_RUS_auto_static_ip_input.html"
#define BRS_RUS_AUTO_START_WAN_PG			"BRS_RUS_auto_start_wan.html"
/*END------------Russian Wizard----------*/

// First page
#define RST_RELOAD_PG           "RST_reload.htm"
#define LANG_CHG_PG             "LANG_change.htm"
#define ACCESSCONTROL_PG		"AccessControl.htm"

#define ADSL_SHOWTIME   "/tmp/adsl_showtime"
#define WIZ_DETWAN_NONE_PG       "wiz_none.htm"
#define BRS_WIZPPPOE_ATT_PG         "BRS_03A_B_pppoe_att.html"
#define BRS_WIZPPPOA_PG             "BRS_03A_B_pppoa.html"
#define BRS_WIZPPPOA_REENT_PG       "BRS_03A_B_pppoa_reenter.html"
#define BRS_WIZ_CIP_PG          "BRS_wiz_cip.html"
#define BRS_WIZ_FULLSCANCONFIRM_PG  "BRS_fullscan_confirm.html"
#define BRS_WIZ_FULLSCANWAIT_PG     "BRS_fullscan_wait.html"
#define WIZ_DETWAN_PPPOA_PG     "WIZ_detwan_pppoa.htm"
#define ADSL_SETTING_PG         "adsl_setting.htm"
#define WIZ_PPPOA_PG            "WIZ_pppoa.htm"
#define BAS_ATM_PPPOA_PG            "BAS_ATM_pppoa.htm"

#endif /* _FILE_NAMES_H_ */



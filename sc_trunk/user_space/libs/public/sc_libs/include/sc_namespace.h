/* 
 * Copyright ? 2008 SerComm
 * sercomm namespace defination
 * including path, interface name
*/

/* interface name */

#ifndef __SC_NAMESPACE_H__
#  define __SC_NAMESPACE_H__
#  include "wan_namespace.h"
#  include "sw_namespace.h"
#  include "wifi_namespace.h"

#  ifndef sc_ifname_br2684
#    define sc_ifname_br2684		"nas%d"
#  endif

#  ifndef sc_ifname_ppp
#    define sc_ifname_ppp			"ppp%d"
#  endif

#  ifndef sc_ifname_sw
#    define sc_ifname_sw			"eth%d"
#  endif

#  ifndef sc_ifname_wifi
#    define sc_ifname_wifi			"wifi%d"
#  endif

/* usb */
#  define usb_mount_fmt			"/mnt/usb%d"

/* mtd */
#  define MTD_PATH_BOOTLOADER		"/dev/mtdblock/0"
#  define MTD_PATH_KERNEL			"/dev/mtdblock/1"
#  define MTD_PATH_FS			"/dev/mtdblock/2"
#  define MTD_PATH_KERNEL_FS		"/dev/mtdblock/3"
#  define MTD_PATH_NVRAM			"/dev/mtdblock/4"
#  define MTD_PATH_DPF			"/dev/mtdblock/7"

#  define LOADER_FN		MTD_PATH_BOOTLOADER
#  define KERNEL_FN		MTD_PATH_KERNEL
#  define FILESYSTEM_FN           MTD_PATH_FS


#  define DHCPD_CFG			"/etc/udhcpd.conf"
#  define DHCPD_LEASES			"/var/udhcpd.leases"

#  define HTTPD_CONF			"/etc/htpasswd"

/* pppd */
#  define PPP_LCP_FILE			"/tmp/lcp"
#  define PPPD_FAIL			"/tmp/pppd.fail"
#  define PPP_IP_DOWN_ACT			"/tmp/ip-down-act"


/* wan temp files */
#  define WAN_IPADDR 			"/tmp/wan_ipaddr"
#  define WAN_INTO_BASE			"/tmp/wan_info"
#  define WAN_UPTIME_BASE			"/tmp/wan_uptime"

/* lan temp files */
#  define LAN_UPTIME_BASE			"/tmp/lan_uptime"

/* wlan temp files */
#  define WLAN_UPTIME_BASE		"/tmp/wlan_uptime"

/* ipoa */
#  define IPOA_SERV			"/tmp/ipoa_server"
#  define ATMARPD_CHECK			"/tmp/atmarpd.dep"

/* PPPoE relay status file */
#  define RELAY_STS			"/var/run/lib_poerelay_sts"


#  define ANNEX_VERSION			"/etc/version"

#  define UPGRADE_FIFO_FILE		"/tmp/upgrade_fifo"

#  define RESTORE_FIFO_FILE		"/tmp/restore_fifo"

#  define CONFIG_PATH				"/tmp/netgear.cfg"

#  define MAGIC_STRING 			"sErCoMm"

/* syslog */
#  define MAIL_SUBJECT "NETGEAR Security Log"
#  define SYSLOG_CONF "/etc/syslog.conf"

/* dhcp */
#  define DHCPD_LEASES 	"/var/udhcpd.leases"

/* linux related */
#  define _PATH_PROCNET_DEV "/proc/net/dev"
#  define ATM_DEV_CHECK			"/proc/sys/net/ipv4/conf/atm"
#endif /* __SC_NAMESPACE_H__ */

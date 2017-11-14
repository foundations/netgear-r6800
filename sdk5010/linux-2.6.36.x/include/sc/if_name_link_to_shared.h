#ifndef _IF_NAME_H_
#define _IF_NAME_H_

/* LAN phycial interface name */
//#define LAN_PHY_IFNAME          "eth2.1"
#define LAN_PHY_IFNAME          "eth2"

/* WLAN phycial interface name */
#define WLAN_PHY_IFNAME         "ra0"
#define WLAN_PHY_IFNAME1        "ra1"
#define WLAN_PHY_IFNAME2        "ra2"
#define WLAN_PHY_IFNAME3        "ra3"
#define WLAN_PHY_RT5592_IFNAME         "rai0"

#define WLAN_5G_PHY_IFNAME      "rai0"
#define WLAN_5G_PHY_IFNAME1     "rai1"
#define WLAN_5G_PHY_IFNAME2     "rai2"
#define WLAN_5G_PHY_IFNAME3     "rai3"
/* Guest iface */
#define WLAN_MBSSID_IFNAME_PREFIX  "ra"
#define WLAN_GUEST_IFNAME  "ra1"
#define WLAN_GUEST1_PHY_IFNAME  "ra1"
#define WLAN_MBSSID_5G_IFNAME_PREFIX  "rai"
#define WLAN_GUEST1_5G_PHY_IFNAME  "rai1"

#ifdef _ADSL_
#define WAN_PHY_IFNAME          "nas0"
#else
//#define WAN_PHY_IFNAME        "eth2.2"
#define WAN_PHY_IFNAME        	"eth3"
#define WAN_L2_IFNAME  			(nvram_safe_get("wan_l2_ifname"))
#define WAN_INTRANET_IFNAME		(vlan_hidden_Intranet() ? WAN_L2_IFNAME: (nvram_safe_get("intranet_ifname")))
#endif

#define WAN_ETH_PHY_IFNAME WAN_PHY_IFNAME
/* Logic LAN interface name */
#define LAN_LOG_IFNAME          "group1"

/* SYSTEM ethernet interface name */
#define SYS_ETH_IFNAME          "eth2"

/* plug a usb2ethernet adaptor, the interface name for that adaptor */
#define ADDON_ETH_NAME "eth0"

/* VLAN start num when QOS enable */
#define VLAN_START_NUM_QOS_ENABLE          11
#define VLAN_START_NUM_QOS_DISABLE          1


#endif/*_IF_NAME_H_*/


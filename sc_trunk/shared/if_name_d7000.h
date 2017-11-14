#ifndef _IF_NAME_H_
#define _IF_NAME_H_

/* LAN phycial interface name */
#define LAN_PHY_IFNAME			"eth0"
#define LAN_PHY_IFNAME1			"eth1"
#define LAN_PHY_IFNAME2			"eth2"
#define LAN_PHY_IFNAME3			"eth3"
#define LAN_PHY_IFNAME4			"eth4"

/* WLAN phycial interface name */
#define WLAN_PHY_IFNAME         "wl0"
#define WLAN_PHY_IFNAME1        "wl0.1"
#define WLAN_PHY_IFNAME2        "wl0.2"
#define WLAN_PHY_IFNAME3        "wl0.3"
#define WLAN_5G_PHY_IFNAME      "wl1"
#define WLAN_5G_PHY_IFNAME1     "wl1.1"
#define WLAN_5G_PHY_IFNAME2     "wl1.2"
#define WLAN_5G_PHY_IFNAME3     "wl1.3"
#define WLAN_GUEST_IFNAME		"wl0.1"
#define WLAN_5G_GUEST_IFNAME	"wl1.1"
/* Guest iface */
#define WLAN_MBSSID_IFNAME_PREFIX  "ra"
#define WLAN_GUEST1_PHY_IFNAME  "wlan0-va0" //"ra1"
#define WLAN_MBSSID_5G_IFNAME_PREFIX  "rai"
#define WLAN_GUEST1_5G_PHY_IFNAME  "rai1"

#ifdef _ADSL_
#define WAN_PHY_IFNAME          "nas1"
#define WAN_L2_IFNAME  			(nvram_safe_get("wan_l2_ifname"))
#else
#define WAN_PHY_IFNAME          "eth1"
#define WAN_L2_IFNAME  			(nvram_safe_get("wan_l2_ifname"))
#endif
#define WAN_INTRANET_IFNAME             (vlan_hidden_Intranet() ? WAN_L2_IFNAME: (nvram_safe_get("intranet_ifname")))
#define WAN_ETH_PHY_IFNAME          "eth0.1"
#define WAN_DSL_PHY_IFNAME          "atm0.1"
#define WAN_VDSL_PHY_IFNAME         "ptm0.1"

/* Logic LAN interface name */
#define LAN_LOG_IFNAME          "group1"

/* SYSTEM ethernet interface name */
#define SYS_ETH_IFNAME          "eth0"


/* VLAN start num when QOS enable */
#define VLAN_START_NUM_QOS_ENABLE          11
#define VLAN_START_NUM_QOS_DISABLE          1


#endif/*_IF_NAME_H_*/


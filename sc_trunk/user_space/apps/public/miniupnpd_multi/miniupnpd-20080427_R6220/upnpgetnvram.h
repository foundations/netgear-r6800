#ifndef _UPNPGETARGUMENT_H
#define _UPNPGETARGUMENT_H

#include "nvram.h"
#include "nvram_var.h"
#ifdef AMPED
#include "config.h"
#endif

static inline char *GetRSIPAvailabled(void)
{
    return "0";
}

static inline char *GetNATEnabled(void)
{
    return nvram_safe_get("fw_nat");
}

static inline char *GetWanConnMode(void)
{
    return nvram_safe_get("wan_mode");
}

static inline char *GetWanIfName(void)
{
    return nvram_safe_get("wan_ifname");
}

static inline char *GetPortForwardRule(void)
{
    return nvram_safe_get("port_forward_rules");
}

static inline char *GetDeviceName(void)
{
	static char device_name[32];
#ifdef AMPED
	snprintf(device_name, sizeof(device_name), "%s %s", OS_NAME, nvram_safe_get("device_name"));
#else
	/* rev10: friendly name should be `<Devive Name> (Gateway)' for IGD device */
	snprintf(device_name, sizeof(device_name), "%s (Gateway)", nvram_safe_get("device_name"));
#endif
	return (char *)device_name;
}

static inline char *GetModelName(void)
{
	return nvram_safe_get("product_name");
}

static inline char *GetUpnpNotifyInterval(void)
{
	return nvram_safe_get("upnp_adv_time");
}


static inline char *GetLANNetMask(void)
{
	return nvram_safe_get("lan_netmask");
}


static inline char *GetLANIPRouterList(void)
{
	return nvram_safe_get("lan_ipaddr");
}
static inline char *GetLANIPMin(void)
{
	return nvram_safe_get("dhcp_start_ip");
}
static inline char *GetLANIPMax(void)
{
	return nvram_safe_get("dhcp_end_ip");
}
static inline char *GetLANIPReserved(void)
{
	return nvram_safe_get("dhcp_reserved");
}
static inline char *GetLANDNSServers(void)
{
	return nvram_safe_get("lan_ipaddr");
}

#ifdef UPNP_DEBUG
void debug_msg(char *fmt, ...);
#else
#define debug_msg(fmt, ...)
#endif

#endif


#ifndef _G_MISC_H_
#define _G_MISC_H_
/*
 * Please define global defines no including to filenames/if_name/nv_var
 */
#define NETGEAR_SYSLOG(fmt, args...)    syslog(LOG_INFO, fmt, ##args)
#define PNPX_HW_ID "Only used in miniupnp for init,will be modified according to model info later"

#endif /* _G_MISC_H_ */



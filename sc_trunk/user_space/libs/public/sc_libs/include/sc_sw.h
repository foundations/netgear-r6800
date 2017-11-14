#ifndef __SC_SW_H__
#  define __SC_SW_H__

/* sw infomation */
int sc_get_serialnumber(char *buf);
int sc_get_firmwareversion(char *buf);
int sc_get_manfacuture_oui(char *buf);
int sc_get_lan_maxbitrate(char *buf);
int sc_get_lan_duplexmode(char *buf);
int sc_get_lan_dnsservers(char *buf);

#endif

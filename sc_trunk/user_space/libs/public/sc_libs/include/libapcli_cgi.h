#ifndef __LIBAPCLI_CGI__
#define __LIBAPCLI_CGI__

int apcli_get_remoteap_signal(int which_band);

int apcli_cgi_link_ok(int which_band, int wait);
int apcli_cgi_wan_ok(int which_band);
int apcli_cgi_signal(int which_band);

#endif

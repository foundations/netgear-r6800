#ifndef _IPT_FBWIFI_REDIRECT_H
#define _IPT_FBWIFI_REDIRECT_H

enum ipt_fbwifi_redirect_with {
	IPT_FBWIFI_REDIRECT = 0
};

enum ipt_block_type{
	IPT_FBWIFI_SITE_REDIRECT = 0
};

struct ipt_fbwifi_redirect_info {
	enum ipt_fbwifi_redirect_with with;      /* reject type */
};

#endif 

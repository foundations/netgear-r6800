#ifndef _ADSL_H_
#define _ADSL_H_
typedef struct adsl_info_s{
	char	firmware[128];
	char 	status[128];
	int	down_stream;
	int	up_stream;
	char	line_u_att[96];
	char	line_d_att[96];
	char	u_margin[96];
	char	d_margin[96];
}cgi_adsl_info_t;

int modem_status_cgi(cgi_adsl_info_t *adsl);
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "upnpaction.h"
#include "upnpgetnvram.h"

int COMMAND(const char *format, ...)
{
	char buf[1024]="";
	va_list arg;
	int fd;
	va_start(arg, format);
	vsnprintf(buf,1024, format, arg);
	va_end(arg);
	fd=open("/etc/cmd_agent",O_WRONLY);
	if(fd<0)
		return 1;
	write(fd, buf, 1024);
	close(fd);

	usleep(1);
	return 0;	
	
}
int IsWanUp(void)
{
    if(access("/tmp/wan_uptime", F_OK) == 0)
        return 1;
    else
        return 0;
}
int DoWanConnection(void)
{
    if(IsWanUp())
        return 0;
    else
        return COMMAND("/usr/sbin/rc wan start &");
}

int DoWanDisconnection(void)
{
    return COMMAND("/usr/sbin/rc wan stop &");
}

int IsRedirectConflictWithPF(unsigned short eport,
        char *protocol,
        char *in_ip,
        unsigned short iport)
{
    char *portForwardRule = GetPortForwardRule();

    int pf_enable;
    int i = 0;
    unsigned short pf_iport_s, pf_iport_e;
    char pf_ip[16], pf_pro[8];
    char pf_rule[256];
    char *pt = NULL;

    pt = portForwardRule;
    while(*pt != '\0')
    {
        i = 0;
        while(*pt != '\0' && *pt != 0x01)
            pf_rule[i++] = *pt++;
        if(*pt != '\0')
            pt++;

        pf_rule[i] = '\0';
        debug_msg("%s:%d pf_rule=%s\n", __func__, __LINE__, pf_rule);
        sscanf(pf_rule, "%d\x02%*[^\x02]\x02%8[^\x02]\x02%hu-%hu\x02%s",
                &pf_enable, pf_pro, &pf_iport_s, &pf_iport_e, pf_ip);

        if(!pf_enable)
            continue;

        debug_msg("%s:%d pf_pro=%s, iport=%d-%d, ip=%s\n",
                __func__, __LINE__, pf_pro, pf_iport_s, pf_iport_e, pf_ip);
        debug_msg("protocol=%s, extport=%hu, ip=%s, in_port=%hu\n",
                protocol, eport, in_ip, iport);
        /*
         * All Port Forward rules, external port is the same to
         * the internal port.
         */
        if((strcasestr(pf_pro, protocol) == NULL)
                || (eport < pf_iport_s)
                || (eport > pf_iport_e))
            continue;

        /*
         * If match this, means the same redirection,
         * so take it as no conflict according to spec Chapter 1.9
         */
        if((strcmp(in_ip, pf_ip) == 0)
                && (iport >= pf_iport_s && iport <= pf_iport_e))
            return 0;

        /*
         * Other situation, take it as conflict
         */
        return 1;
    }
    return 0;
}

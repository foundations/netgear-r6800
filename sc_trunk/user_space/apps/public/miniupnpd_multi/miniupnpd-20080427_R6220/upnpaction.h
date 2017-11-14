#ifndef _UPNP_ACTION_H_
#define _UPNP_ACTION_H_

int IsWanUp(void);
int DoWanConnection(void);
int DoWanDisconnection(void);
int IsRedirectConflictWithPF(unsigned short eport,
        char *ptotocol,
        char *in_ip,
        unsigned short iport);

#endif

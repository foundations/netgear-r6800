/* leases.h */
#ifndef _LEASES_H
#define _LEASES_H
#include "packet.h"
#include <asm/param.h>

/* enlarge cid size, because some devices may have too long cid, and our udhcpd will crash down and can not offer IP address anymore. */
#define MAX_CLIENT_LEN		48

struct dhcpOfferedAddr {
	u_int8_t chaddr[16];
	u_int32_t yiaddr;	/* network order */
	u_int32_t expires;	/* host order */

#ifdef RONSCODE
	u_int8_t hostname[MAXHOSTNAMELEN];
	u_int8_t cid[MAX_CLIENT_LEN];
#endif

};

extern unsigned char blank_chaddr[];

void clear_lease(u_int8_t *chaddr, u_int32_t yiaddr, int ifid);
#ifdef RONSCODE
struct dhcpOfferedAddr *add_lease(u_int8_t *chaddr, u_int32_t yiaddr, unsigned long lease, int ifid,char *hostname,char *cid);
#else
struct dhcpOfferedAddr *add_lease(u_int8_t *chaddr, u_int32_t yiaddr, unsigned long lease, int ifid);
#endif
int lease_expired(struct dhcpOfferedAddr *lease);
struct dhcpOfferedAddr *oldest_expired_lease(int ifid);
struct dhcpOfferedAddr *find_lease_by_chaddr(u_int8_t *chaddr, int ifid);
struct dhcpOfferedAddr *find_lease_by_cid(u_int8_t *cid, int ifid);//add by jim
struct dhcpOfferedAddr *find_lease_by_yiaddr(u_int32_t yiaddr, int ifid);
u_int32_t find_address(int check_expired, int ifid);
int check_ip(u_int32_t addr, int ifid, u_int8_t *chaddr);

struct dhcpOfferedAddr *find_reserved_by_chaddr(u_int8_t *chaddr, int ifid);
struct dhcpOfferedAddr *find_reserved_by_yiaddr(u_int32_t yiaddr, int ifid);

#endif

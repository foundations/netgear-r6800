/*
 * leases.c -- tools to manage DHCP leases
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 */

#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "debug.h"
#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "leases.h"
#include "arpping.h"
#include "filenames.h"
#include "sc_debug.h"

unsigned char blank_chaddr[] = {[0 ... 15] = 0};


/* clear every lease out that chaddr OR yiaddr matches and is nonzero */
void clear_lease(u_int8_t *chaddr, u_int32_t yiaddr, int ifid)
{
	unsigned int i, j;
	for (j = 0; j < 16 && !chaddr[j]; j++);

	for (i = 0; i < server_config[ifid].max_leases; i++)
		if ((j != 16 && !memcmp(leases[i].chaddr, chaddr, 16)) ||
		    (yiaddr && leases[i].yiaddr == yiaddr)) {
            memset(&(leases[i]), 0, sizeof(struct dhcpOfferedAddr));
		}
}

#ifdef RONSCODE
/* add a lease into the table, clearing out any old ones */
struct dhcpOfferedAddr *add_lease(u_int8_t *chaddr, u_int32_t yiaddr, unsigned long lease, int ifid,char *hostname,char *cid)
#else
struct dhcpOfferedAddr *add_lease(u_int8_t *chaddr, u_int32_t yiaddr, unsigned long lease, int ifid)
#endif
{
	struct dhcpOfferedAddr *oldest;

    SC_CFPRINTF("add lease remove %02X:%02X:%02X:%02X:%02X:%02X\n", chaddr[0], chaddr[1], chaddr[2], chaddr[3], chaddr[4], chaddr[5]);
	SC_CFPRINTF("*************IP is %x, lease %ld***********\n",yiaddr, lease);
	/* clean out any old ones */
	clear_lease(chaddr, yiaddr, ifid);

	oldest = oldest_expired_lease(ifid);

	if (oldest) {
	    SC_CFPRINTF("add lease readd %02X:%02X:%02X:%02X:%02X:%02X\n", chaddr[0], chaddr[1], chaddr[2], chaddr[3], chaddr[4], chaddr[5]);
		memcpy(oldest->chaddr, chaddr, 16);
		oldest->yiaddr = yiaddr;
        oldest->expires = time(0) + lease;

#ifdef RONSCODE
		memcpy(oldest->hostname, hostname, MAXHOSTNAMELEN);
		memcpy(oldest->cid, cid, MAX_CLIENT_LEN);
#endif
	}
	else
	    SC_CFPRINTF("add lease fail %02X:%02X:%02X:%02X:%02X:%02X\n", chaddr[0], chaddr[1], chaddr[2], chaddr[3], chaddr[4], chaddr[5]);

	return oldest;
}


/* true if a lease has expired */
int lease_expired(struct dhcpOfferedAddr *lease)
{
	return (lease->expires < (unsigned long) time(0));
}


/* Find the oldest expired lease, NULL if there are no expired leases */
struct dhcpOfferedAddr *oldest_expired_lease(int ifid)
{
	struct dhcpOfferedAddr *oldest = NULL;
	unsigned long oldest_lease = time(0);
	unsigned int i;


	for (i = 0; i < server_config[ifid].max_leases; i++)
		if (oldest_lease > leases[i].expires) {
			oldest_lease = leases[i].expires;
			oldest = &(leases[i]);			
			SC_CFPRINTF("oldest_expired_lease find item mac[%d] %02X:%02X:%02X:%02X:%02X:%02X\n", i,leases[i].chaddr[0], leases[i].chaddr[1], leases[i].chaddr[2], leases[i].chaddr[3], leases[i].chaddr[4], leases[i].chaddr[5]);
		}
		
	return oldest;

}


/* Find the first lease that matches chaddr, NULL if no match */
struct dhcpOfferedAddr *find_lease_by_chaddr(u_int8_t *chaddr, int ifid)
{
	unsigned int i;
	struct in_addr addr;
	
    SC_CFPRINTF("packet's mac %02X:%02X:%02X:%02X:%02X:%02X\n", chaddr[0], chaddr[1], chaddr[2], chaddr[3], chaddr[4], chaddr[5]);
	for (i = 0; i < server_config[ifid].max_leases; i++) {
		if (!memcmp(leases[i].chaddr, chaddr, 16)) 
		{
            addr.s_addr = leases[i].yiaddr;
            DEBUG(LOG_INFO, "%02X:%02X:%02X:%02X:%02X:%02X already in lease table has IP %s",
                leases[i].chaddr[0], leases[i].chaddr[1], leases[i].chaddr[2], leases[i].chaddr[3], leases[i].chaddr[4], leases[i].chaddr[5], inet_ntoa(addr));
            SC_CFPRINTF("%02X:%02X:%02X:%02X:%02X:%02X already in lease table has IP %s\n",
                leases[i].chaddr[0], leases[i].chaddr[1], leases[i].chaddr[2], leases[i].chaddr[3], leases[i].chaddr[4], leases[i].chaddr[5], inet_ntoa(addr));
		    return &(leases[i]);
		}	
	}
	return NULL;
}

//add by jim
//////////////////////////////////////////////////////////////////////////////////////////////////////
struct dhcpOfferedAddr *find_lease_by_cid(u_int8_t *cid, int ifid)
{
	unsigned int j;
	for (j = 0; j < server_config[ifid].max_leases; j++)
	{
		if (!memcmp(leases[j].cid, cid, 7))
		{
		return &(leases[j]);
		}

	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Find the first lease that matches yiaddr, NULL is no match */
struct dhcpOfferedAddr *find_lease_by_yiaddr(u_int32_t yiaddr, int ifid)
{
	unsigned int i;

	for (i = 0; i < server_config[ifid].max_leases; i++)
		if (leases[i].yiaddr == yiaddr) return &(leases[i]);

	return NULL;
}


/* find an assignable address, it check_expired is true, we check all the expired leases as well.
 * Maybe this should try expired leases by age... */
u_int32_t find_address(int check_expired, int ifid)
{
	u_int32_t addr, ret;
	struct dhcpOfferedAddr *lease = NULL;
	addr = ntohl(server_config[ifid].start); /* addr is in host order here */
	for (;addr <= ntohl(server_config[ifid].end); addr++) {
		/* ie, 192.168.55.0 */
		if (!(addr & 0xFF)) continue;

		/* ie, 192.168.55.255 */
		if ((addr & 0xFF) == 0xFF) continue;

		/* ie, this ip is server ip */
		if (addr == ntohl(server_config[ifid].server)) continue;

		ret = htonl(addr);
		/* this ip is reserved ip */
		if(find_reserved_by_yiaddr(ret, ifid)) continue;

		/* lease is not taken */
		if ((!(lease = find_lease_by_yiaddr(ret, ifid)) ||

		     /* or it expired and we are checking for expired leases */
		     (check_expired  && lease_expired(lease))) &&

		     /* and it isn't on the network */
	    	     !check_ip(ret, ifid, NULL)) {
			return ret;
			break;
		}
	}
	return 0;
}

/*
 * check is an IP is taken
 * 1 ==> Yes. This IP is taken
 * 0 ==> No. This IP is free to use.
 */
int check_ip(u_int32_t addr, int ifid, u_int8_t *chaddr)
{
	struct in_addr temp;
	unsigned char mac[6]={0};
#if 0
#ifdef RONSCODE
	char hostname[16]="";
	char cid[16]="";
#endif
#endif

	if (arpping(addr, mac, server_config[ifid].server, server_config[ifid].arp, server_config[ifid].interface) == 0) {
		temp.s_addr = addr;
	 	/* Fix CEC Sumsung smart tv long run test : ip will change 
	 	 * reason: the tv is not normal release/renew. when it own IP
	 	 * it still send discover to DHCP server, and check ip function
	 	 * find one is using the IP(in fact, it is itself),so it will 
	 	 * get a new free IP, so the IP will change in long rn test
	 	 * now we will check ip and also compare the mac address, 
	 	 * if they are the same , it still use the IP to the device 
	 	 *                                              mike-zhou 4/16
	 	 */
        if((chaddr)
            && (memcmp(mac, chaddr,6) == 0))
        {      
            SC_CFPRINTF("%s not belongs to someone, can assign it\n",inet_ntoa(temp));
	 		SC_CFPRINTF("send mac is %02x:%02x:%02x:%02x:%02x:%02x\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
            return 0;
        }
        LOG(LOG_INFO, "%s belongs to someone, reserving it for %ld seconds",
	 		inet_ntoa(temp), server_config[ifid].conflict_time);
        SC_CFPRINTF("%s belongs to someone, reserving it for %ld seconds\n",
	 		inet_ntoa(temp), server_config[ifid].conflict_time);
#if 0 /* don't add the checking result to lease file, open it until make it smart enough */
#ifdef RONSCODE
		add_lease(blank_chaddr, addr, server_config[ifid].conflict_time, ifid,hostname,cid);
#else
		add_lease(blank_chaddr, addr, server_config[ifid].conflict_time, ifid);
#endif
#endif
		return 1;
	} else
	{
	    temp.s_addr = addr;
	    SC_CFPRINTF("%s not belongs to someone, can assign it\n",
	 		inet_ntoa(temp));
		return 0;
	}
}

/*
 * Check if we have reserved IP for @chaddr. Read from DHCPD_RESERVED_INFO.
 * Return NULL if can not find
 */
struct dhcpOfferedAddr *find_reserved_by_chaddr(u_int8_t *chaddr, int ifid) {
    static struct dhcpOfferedAddr lease;
    char filename[] =  DHCPD_RESERVED_INFO"XX";
    FILE *fp = NULL;
    struct in_addr addr;

    /* not used now */
    ifid = 0;
	if (ifid)
	{
		/* let gcc463 not complain. */
	}

    sprintf(filename,"%s%lu",DHCPD_RESERVED_INFO,server_config[ifid].group_id);
    fp = fopen(filename, "r");
    if(!fp) {
        return NULL;
    }
    while(fread(&lease, sizeof lease, 1, fp) == 1) {
        if(!memcmp(chaddr, lease.chaddr, sizeof(lease.chaddr))) {
            /* Find it! */
            fclose(fp);

            addr.s_addr = lease.yiaddr;
		if (addr.s_addr)		
		{
			/* let gcc463 not complain. */
		}

            DEBUG(LOG_INFO, "%s is reserved for %02X:%02X:%02X:%02X:%02X:%02X",
                inet_ntoa(addr), lease.chaddr[0], lease.chaddr[1], lease.chaddr[2], lease.chaddr[3], lease.chaddr[4], lease.chaddr[5]);
            return &lease;
        }
    }
    fclose(fp);
    return NULL;
}

/*
 * Check if we have reserved IP for @chaddr. Read from DHCPD_RESERVED_INFO.
 * Return NULL if can not find
 */
struct dhcpOfferedAddr *find_reserved_by_yiaddr(u_int32_t yiaddr, int ifid) {
    static struct dhcpOfferedAddr lease;
    char filename[] =  DHCPD_RESERVED_INFO"XX";
    FILE *fp = NULL;
    struct in_addr addr;

    /* not used now */
    ifid = 0;
	
	if (ifid)
	{
		/* let gcc463 not complain. */
	}


    sprintf(filename,"%s%lu",DHCPD_RESERVED_INFO,server_config[ifid].group_id);
    fp = fopen(filename, "r");
    if(!fp) {
        return NULL;
    }
    while(fread(&lease, sizeof lease, 1, fp) == 1) {
        if(yiaddr == lease.yiaddr) {
            /* Find it! */
            fclose(fp);
            addr.s_addr = lease.yiaddr;
		if (addr.s_addr)
		{
			/* let gcc463 not complain. */
		}
            DEBUG(LOG_INFO, "%s is reserved for %02X:%02X:%02X:%02X:%02X:%02X",
                inet_ntoa(addr), lease.chaddr[0], lease.chaddr[1], lease.chaddr[2], lease.chaddr[3], lease.chaddr[4], lease.chaddr[5]);
            return &lease;
        }
    }
    fclose(fp);
    return NULL;
}


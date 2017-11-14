
#ifndef __SC_IF_STATS_H_
#  define __SC_IF_STATS_H_

#  ifndef IF_NAMESIZE
#    define IF_NAMESIZE	16
#  endif

typedef enum
{
    IF_TYPE_WAN = 1,
    IF_TYPE_VLAN,		// LAN, VLAN
    IF_TYPE_WLAN,
    IF_TYPE_GROUP,
    IF_TYPE_PHONE,
} IF_TYPE_enum;



struct interface
{
    char name[6];
    unsigned long rx_packets;	/* total packets received */
    unsigned long tx_packets;	/* total packets transmitted */
    unsigned long rx_bytes;	/* total bytes received */
    unsigned long tx_bytes;	/* total bytes transmitted */
    unsigned long rx_errors;	/* bad packets received */
    unsigned long tx_errors;	/* packet transmit problems */
    unsigned long rx_dropped;	/* no space in linux buffers */
    unsigned long tx_dropped;	/* no space available in linux */
    unsigned long rx_multicast;	/* multicast packets received */
    unsigned long rx_compressed;
    unsigned long tx_compressed;
    unsigned long collisions;
    unsigned long rx_frame_errors;	/* recv'd frame alignment error */
    unsigned long rx_fifo_errors;	/* recv'r fifo overrun */
    unsigned long tx_carrier_errors;
    unsigned long tx_fifo_errors;
};


typedef struct
{
    char name[6];
    unsigned long RX_packets;	/* total packets received */
    unsigned long TX_packets;	/* total packets transmitted */
    unsigned long RX_bps;	/* received bytes per second */
    unsigned long TX_bps;	/* transmitted bytes per second */
    unsigned long collisions;	/* total collision occurred */
    unsigned long TX_errors;	/* total collision occurred */
    unsigned long RX_errors;	/* total collision occurred */
    unsigned long RX_bytes;	/* total packets received */
    unsigned long TX_bytes;	/* total packets transmitted */
    int uptime_day;
    int uptime_hr;
    int uptime_min;
    int uptime_sec;
} stats_info;


typedef struct if_info_s
{
    char ifname[16];
    char ipaddr[16];
    char mac[18];
    char mask[16];
    char gateway[16];
    char rmtaddr[16];
    int mtu;

} if_info_t;

/* Obtain a ip address string from ifname , maybe we can get it from nvram 
   without using open socket method 
*/
int getIFInfo(if_info_t * if_info);

int get_ip_addr(char *ifname, char *ipaddr);
int sc_get_mask_if(char *ifname, char *mask);
int check_interface_up(char *interface);

void if_readlist_proc(struct interface *ife);
char *get_name(char *name, char *p);
void get_dev_fields(char *bp, struct interface *ife);
void get_static_info(stats_info * ifinfo, int uptime);




/*
  int sc_get_IF_stats(IF_TYPE_enum type, int if_id, stats_info* pstat)
  Get statistics of an interface

  Parameters:
  type: IF_TYPE_WAN, IF_TYPE_VLAN, or 	IF_TYPE_WLAN
  if_id: id of the interface, start from 1
  pstat: pointer of a stats_info structure, 

  Return:
  return 1 if success, 
  0 if fail (No this interface or pstat== NULL) 
*/
int sc_get_IF_stats(IF_TYPE_enum type, int if_id, stats_info * pstat);


/*
  int sc_get_IF_stats(IF_TYPE_enum type, int if_id, stats_info* pstat)
  Get numbers of the specific type interface

  Parameters:
  type: IF_TYPE_WAN, IF_TYPE_VLAN, IF_TYPE_WLAN, IF_TYPE_GROUP , or IF_TYPE_PHONE.

  Return:
  return numbers of the specific type interface
*/
int sc_get_IF_num(IF_TYPE_enum type);

/* common interface information */
int sc_get_mac_if(char *ifname, char mac[6]);
int sc_get_if_stats(IF_TYPE_enum type, int if_id, stats_info * p);

#endif

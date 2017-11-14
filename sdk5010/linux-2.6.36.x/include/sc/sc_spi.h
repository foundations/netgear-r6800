#ifndef __SC_SPI__H_
#define __SC_SPI__H_

#include <linux/types.h>
#include <linux/param.h>
#include <linux/skbuff.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_core.h>

//#define CONFIG_SPI_FIRWALL

#define SEQ_OPERATION

#define MAX_CONNTRACK_COUNT	4096

extern int g_spi_fun_switch;
extern int g_spi_enable;
extern int g_spi_wanindex;
extern int g_spi_br_index;


#define BLACK_ENTRY 		0
#define WHITE_ENTRY 		1
#define LOCAL_SERVICE_ENTRY 2

#define DETECT_NOT_MATCH		0
#define DETECT_BLOCK			1
#define DETECT_PASS				2
#define DETECT_LOCAL_SERVICE	4

#define IPPROTO_BOTH 255

#define BLOCK_INFINITE_TIME	0xFFFFFFFF

#define RESET_TCP_SYN_BYSRCIP	0x01
#define RESET_CLAMP_BYREMOTEIP	0x02

/* block function */
extern int (*sc_check_and_block_hook)(const struct nf_conntrack_tuple *tuple);
extern int (*sc_add_block_pattern_hook)(struct nf_conntrack_tuple *tuple,
								struct nf_conntrack_tuple *mask,
								int dport_min,
								int dport_max,
								int type,
								int	timeout);
extern int (*sc_delete_block_pattern_hook)(struct nf_conntrack_tuple *tuple,
									int dport_min,
									int dport_max,
									int type);
/* special block function for port trigger */
extern int (*sc_add_special_block_pattern_hook)(int dport_min, int dport_max, 
										int type, u_int8_t protonum);
extern int (*sc_delete_special_block_pattern_hook)(int dport_min, int dport_max,
										int type, u_int8_t protonum);


/* Smurf Attack, Fraggle Attack, ICMP Flood/ICMP Storm Attack, 
	Ping Flood, UDP Flood/UDP Storm Attack function */
extern unsigned int (*sc_fake_source_detect_hook)(struct sk_buff *skb, struct nf_conn *ct, u_int8_t protonum);

/* psd and TCP/UDP Echo/Chargen Attack  detect */
extern unsigned int (*sc_psd_and_special_udp_detect_hook)(struct sk_buff *skb, struct nf_conn *ct, u_int8_t protonum);

/* tcp syn flood detect */
extern unsigned int (*sc_tcp_check_hook)(struct nf_conn *ct);
extern void (*sc_tcp_destroy_hook)(struct nf_conn *ct);
extern void (*sc_tcp_deal_establish_hook)(struct nf_conn *ct);

/*For a host on the Internet (i.e. a remote IP address), it can generate at most 100 inbound NAT sessions
 in total at a time to all Port Forwarding services and to DMZ.*/
extern unsigned int (*sc_detect_total_session_for_one_host_hook)(struct sk_buff *skb, struct nf_conn *ct);
extern void (*sc_remote_destroy_hook)(struct nf_conn *ct);
extern unsigned int (*sc_clamp_refresh_hook)(struct nf_conn *ct);
extern unsigned int (*sc_clamp_is_clamped_session_hook)(struct nf_conn *ct);


/* ip check */
extern unsigned int (*sc_packet_ip_check_hook)(struct sk_buff *skb);


#endif

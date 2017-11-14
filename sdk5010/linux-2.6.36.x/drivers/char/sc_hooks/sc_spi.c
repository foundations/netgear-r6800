/***************************************************************
*             Copyright (C) 2007 by SerComm Corp.
*                    All Rights Reserved.
*
*      Use of this software is restricted to the terms and
*      conditions of SerComm's software license agreement.
*
*                        www.sercomm.com
****************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <sc/sc_spi.h>

/* global variable*/
int g_spi_fun_switch = 0;
int g_spi_enable = 0;
int g_spi_wanindex = -1;
int g_spi_br_index = -1;

/* block function */
int (*sc_check_and_block_hook)(const struct nf_conntrack_tuple *tuple) = NULL;
int (*sc_add_block_pattern_hook)(struct nf_conntrack_tuple *tuple,
								struct nf_conntrack_tuple *mask,
								int dport_min,
								int dport_max,
								int type,
								int	timeout) = NULL;
int (*sc_delete_block_pattern_hook)(struct nf_conntrack_tuple *tuple,
									int dport_min,
									int dport_max,
									int type) = NULL;
									
/* special block function for port trigger */
int (*sc_add_special_block_pattern_hook)(int dport_min, int dport_max, 
										int type, u_int8_t protonum) = NULL;
int (*sc_delete_special_block_pattern_hook)(int dport_min, int dport_max,
										int type, u_int8_t protonum) = NULL;


/* Smurf Attack, Fraggle Attack, ICMP Flood/ICMP Storm Attack, 
	Ping Flood, UDP Flood/UDP Storm Attack function */
unsigned int (*sc_fake_source_detect_hook)(struct sk_buff *skb, struct nf_conn *ct, u_int8_t protonum) = NULL;

/* psd and TCP/UDP Echo/Chargen Attack  detect */
unsigned int (*sc_psd_and_special_udp_detect_hook)(struct sk_buff *skb, struct nf_conn *ct, u_int8_t protonum) = NULL;


/* tcp syn flood detect */
unsigned int (*sc_tcp_check_hook)(struct nf_conn *ct) = NULL;
void (*sc_tcp_destroy_hook)(struct nf_conn *ct) = NULL;
void (*sc_tcp_deal_establish_hook)(struct nf_conn *ct) = NULL;

/*For a host on the Internet (i.e. a remote IP address), it can generate at most 100 inbound NAT sessions
 in total at a time to all Port Forwarding services and to DMZ.*/
unsigned int (*sc_detect_total_session_for_one_host_hook)(struct sk_buff *skb, struct nf_conn *ct) = NULL;
void (*sc_remote_destroy_hook)(struct nf_conn *ct) = NULL;
unsigned int (*sc_clamp_refresh_hook)(struct nf_conn *ct) = NULL;
unsigned int (*sc_clamp_is_clamped_session_hook)(struct nf_conn *ct) = NULL;


/* ip check */
unsigned int (*sc_packet_ip_check_hook)(struct sk_buff *skb) = NULL;


EXPORT_SYMBOL(sc_check_and_block_hook);
EXPORT_SYMBOL(sc_add_block_pattern_hook);
EXPORT_SYMBOL(sc_delete_block_pattern_hook);
EXPORT_SYMBOL(sc_add_special_block_pattern_hook);
EXPORT_SYMBOL(sc_delete_special_block_pattern_hook);

EXPORT_SYMBOL(sc_fake_source_detect_hook);

EXPORT_SYMBOL(sc_psd_and_special_udp_detect_hook);

EXPORT_SYMBOL(sc_tcp_check_hook);
EXPORT_SYMBOL(sc_tcp_destroy_hook);
EXPORT_SYMBOL(sc_tcp_deal_establish_hook);

EXPORT_SYMBOL(sc_detect_total_session_for_one_host_hook);
EXPORT_SYMBOL(sc_remote_destroy_hook);
EXPORT_SYMBOL(sc_clamp_refresh_hook);
EXPORT_SYMBOL(sc_clamp_is_clamped_session_hook);

EXPORT_SYMBOL(sc_packet_ip_check_hook);

EXPORT_SYMBOL(g_spi_enable);
EXPORT_SYMBOL(g_spi_fun_switch);
EXPORT_SYMBOL(g_spi_wanindex);
EXPORT_SYMBOL(g_spi_br_index);


/***************************************************************
*             Copyright (C) 2007 by SerComm Corp.
*                    All Rights Reserved.
*
*      Use of this software is restricted to the terms and
*      conditions of SerComm's software license agreement.
*
*                        www.sercomm.com
****************************************************************/

#ifndef _SC_LED_H_
#define _SC_LED_H_


#include <linux/configfs.h>
#include <linux/types.h>
#include <linux/icmp.h>
#include <linux/ip.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <net/checksum.h>
#include <net/ip.h>
#include <linux/stddef.h>
#include <linux/sysctl.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/jhash.h>
#include <linux/err.h>
#include <linux/percpu.h>
#include <linux/moduleparam.h>
/*
#include <linux/netfilter_ipv4/ip_conntrack.h>
#include <linux/netfilter_ipv4/ip_conntrack_protocol.h>
#include <linux/netfilter_ipv4/ip_conntrack_helper.h>
#include <linux/netfilter_ipv4/ip_conntrack_core.h>
#include <linux/netfilter_ipv4/listhelp.h>
*/
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_core.h>

extern void (*sc_hook_destroy_conntrack)(struct nf_conn *ct);
extern int (*sc_hook_init_conntrack_test)(const struct nf_conntrack_tuple *tuple, struct nf_conntrack_l3proto *l3proto);
extern unsigned int (*sc_hook_init_conntrack_mask)(const struct nf_conntrack_tuple *tuple, struct nf_conntrack_l3proto *l3proto);
extern void (*sc_hook_init_conntrack_add)(struct nf_conn *ct);


#endif // _SC_LED_H_


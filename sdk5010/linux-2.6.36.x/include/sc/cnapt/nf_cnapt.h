
#ifndef __NF_NAPT_H
#define __NF_NAPT_H

#include <linux/netdevice.h>
#include <net/route.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_nat.h>

struct calg_hooks_t {
	void* (*alloc)(__be32 privip, __be32 pubip, __u16 privport, __u16 pubport, __u8 proto);

	void (*put)(void *calg);

	int (*add_expect)(void *calg,
							struct nf_conntrack_expect *exp,
							u_int16_t *port,
							u_int16_t *next_port);

	void (*del_expect)(struct nf_conntrack_expect *exp);

	int (*in)(struct nf_conn *ct, struct nf_conntrack_expect *exp);

	int (*out)(struct nf_conn *ct, struct nf_nat_range *range);

	int (*pubip_related)(struct nf_conn *ct, __be32 ip);
};

extern int (*cnapt_confirm_hook)(struct nf_conn *ct);

extern int (*cnapt_get_mapping_info_hook)(struct nf_conn *ct, 
								unsigned int hooknum, 
								__be32  *privip, 
								__u16 *privport, 
								__be32 *pubip, 
								__u16 *pubport);

extern void (*cnapt_cleanup_ct_hook)(struct nf_conn *ct);

extern void (*natlimit_cleanup_ct_hook)(struct nf_conn *ct);

extern void (*cpt_cleanup_ct_hook)(struct nf_conn *ct);

extern struct calg_hooks_t *calg_hooks_ptr;

#endif


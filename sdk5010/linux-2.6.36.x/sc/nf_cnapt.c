#include <linux/module.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/netfilter_ipv4.h>
#include <net/netfilter/nf_conntrack.h>

#include <sc/cnapt/nf_cnapt.h>

int (*cnapt_get_mapping_info_hook)(struct nf_conn *ct, 
                                                               unsigned int hooknum, 
                                                               __be32  *privip, 
                                                               __u16 *privport, 
                                                               __be32 *pubip, 
                                                               __u16 *pubport) = NULL;
EXPORT_SYMBOL(cnapt_get_mapping_info_hook);

int (*cnapt_confirm_hook)(struct nf_conn *ct) = NULL;
EXPORT_SYMBOL(cnapt_confirm_hook);

void (*cnapt_cleanup_ct_hook)(struct nf_conn *ct);
EXPORT_SYMBOL(cnapt_cleanup_ct_hook);

void (*natlimit_cleanup_ct_hook)(struct nf_conn *ct);
EXPORT_SYMBOL(natlimit_cleanup_ct_hook);

void (*cpt_cleanup_ct_hook)(struct nf_conn *ct);
EXPORT_SYMBOL(cpt_cleanup_ct_hook);

struct calg_hooks_t *calg_hooks_ptr;
EXPORT_SYMBOL(calg_hooks_ptr);

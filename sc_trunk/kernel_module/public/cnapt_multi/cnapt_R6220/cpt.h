
#ifndef __CPT_H
#define __CPT_H

#ifdef __KERNEL__

struct cpt_t {
	struct list_head list;

	__u8    enable;

	__u8    trigproto;
	__be32  trigip;
	__u16   trigport[2];

	unsigned long timeout;

	/* inbound mapping. */
	struct cnapt_mapping_range_t inb_mr;

	atomic_t refcnt;

	/* inbound conntrack counter */
	atomic_t ct_ref;

	/* outbound (trigger) timeout */
	unsigned long expires;

#ifdef CONFIG_SPI_FIRWALL
	struct timer_list timer;
#endif
};

#define MAX_EXPIRES (-1UL)
#define EXPIRES_MASK (MAX_EXPIRES-1)

static inline struct nf_conn_cpt* nfct_cpt(struct nf_conn *ct)
{
	struct nf_conn_nat *ct_nat = nfct_nat(ct);
	return (ct_nat ? &ct_nat->ct_cpt : NULL);
}

#endif /* __KERNEL__ */

struct cpt_args_t {
	u_int8_t enable;
	u_int8_t trigproto;
	u_int32_t trigip;
	u_int16_t trigport[2];
	unsigned long timeout;
	struct cnapt_mapping_range_t inb_mr;
};

#endif

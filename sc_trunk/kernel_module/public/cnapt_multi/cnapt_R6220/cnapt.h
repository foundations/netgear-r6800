
#ifndef _CNAPT_H
#define _CNAPT_H

#include <linux/types.h> /* for __be32,__be16 */

/* both = tcp & udp */
#define NAPT_PROTO_BOTH  0XFF

/* napt service type. */
#define NAPT_NT        0x00000000	/* normal traffic */
#define NAPT_PM        0x00000001   /* port mapping. */
#define NAPT_PT        0x00000002   /* port trigger */
#define NAPT_DMZ       0x00000004   /* dmz */
#define NAPT_HAIRPIN   0x00000010   /* hairpin */
#define NAPT_ALG       0x00000020   /* ALG */
#define NAPT_UNKNOWN   0x10000000   /* UNKNOWN. */

enum  {
	NAPT_PRIO_PM = 200,
	NAPT_PRIO_PT = 400,
	NAPT_PRIO_ALG = 500,
};

enum napt_pm_type_t {
	NAPT_PM_PF =      1, /* port forwarding */
	NAPT_PM_UPNP =    2, /* upnp mapping */
	NAPT_PM_LS =      3, /* local service */
	NAPT_PM_UNKNOWN = 254,
	NAPT_PM_MAX = 255,
};

enum napt_err_t {
	ENAPT_SERV_CONFLICT = 1983,
	ENAPT_SYS_CONFLICT = 1984,
};

struct cnapt_mapping_range_t {
	__be32  privip;       /* network byte order */
	__be32  pubip;        /* network byte order */
	/* Note: when it is a range, we always assume pubport == privport. */
	__u16   privport[2];  /* host byte order */
	__u16   pubport[2];   /* host byte order */
	__u8    proto;        /* IPPROTO_TCP/IPPROTO_UDP/NAPT_PROTO_BOTH. */
};

/* a real napt mapping, all the value MUST be valid, and never be 0.*/
struct cnapt_mapping_t {
	__be32  privip;       /* network byte order */
	__be32  pubip;        /* network byte order */
	__u16   privport;     /* host byte order */
	__u16   pubport;      /* host byte order */
	__u8    proto;        /* IPPROTO_TCP/IPPROTO_UDP. */
};

struct cnapt_flush_args_t {
	u_int32_t lan_ip;
	u_int32_t lan_mask;
};

#ifdef __KERNEL__

#include <asm/atomic.h>
#include <linux/list.h>
#include <net/netfilter/nf_nat.h>

enum cnapt_status_t {
	CNAPTS_DYING_BIT = 0,
	CNAPTS_DYING = (1 << CNAPTS_DYING_BIT),
};

struct cnapt_t {
	/* link to hash table by private side info. */
	struct hlist_node bypriv;

	/* link to hash table by public side info.*/
	struct hlist_node bypub;

	struct cnapt_mapping_t mapping;
	
	atomic_t refcnt;

	unsigned long status;
};

enum port_state_t {
	PORTS_FREE = 0,
	PORTS_USED = 1,  /* public port is in used. */
	PORTS_RESEV = 2, /* public port is only reserved. */
};

struct cnapt_serv_t {
	struct list_head list;

	/* serivce name */
	const char *name;

	/* service identify */
	int service;

	/* service priority, the smaller the higher */
	int priority;

	/* get a static/dynamic mapping from private side info. 
	 * this mapping was added by this service, and a napt mapping
	 * may not be created yet. */
	int  (*mapping_bypriv)(struct cnapt_mapping_t *mapping);

	/* get a mapping from public side info. Revert to *_bypriv. */
	int  (*mapping_bypub)(struct cnapt_mapping_t *mapping);

	/* 
	 * check if the public side info was taken(used or reserved). 
	 * when the public port is apply, return PORTS_USED. 
	 * when the public port is only reserved, return PORTS_RESEV. 
	 */
	int  (*public_taken)(const struct cnapt_mapping_t *mapping, int *port_suggest);

	/* 
	 * check if the new mapping(public/private) is conflict with configure
	 * of this service. delta will be set to the service type when conflict. 
	 * return : -ENAPT_SYS_CONFLICT, when the mapping is used by system;
	 * -ENAPT_SERV_CONFLICT, when the mapping is used by service. 
	 */
	int  (*check_config)(const struct cnapt_mapping_range_t *mr, int *delta);

	/* proc function. */
	void* (*seq_next)(void *seq, void *v);
	int   (*seq_show)(void *seq, void *v);
};

/* __h: hooknum */
#define H2NAPTDIR(__h) ((__h) == NF_INET_POST_ROUTING ? IP_CT_DIR_ORIGINAL : IP_CT_DIR_REPLY)
#if 1
#define cnapt_debug2(format, args...) do {printk("[%s - %d]:", __FUNCTION__, __LINE__); \
									printk(format, ##args); } while(0)
#endif
//#define cnapt_debug2(format, args...) do {;} while(0)
#ifdef _CNAPT_DEBUG_
#define cnapt_debug(format, args...) do {printk("[%s - %d]:", __FUNCTION__, __LINE__); \
									printk(format, ##args); } while(0)
#else
#define cnapt_debug(format, args...) do {;} while(0)
#endif

#define DUMP_MAPPING(__s, __m) \
	cnapt_debug("%s mapping: "NIPQUAD_FMT":%u -> "NIPQUAD_FMT":%u, %s\n", \
				__s, \
				NIPQUAD((__m)->privip), (__m)->privport, \
				NIPQUAD((__m)->pubip), (__m)->pubport, \
				(__m)->proto == IPPROTO_TCP ? "tcp":(__m)->proto == IPPROTO_UDP ?"udp" : "unknown")


#define DUMP_MAPPING2(__s, __m) \
	cnapt_debug2("%s mapping: "NIPQUAD_FMT":%u -> "NIPQUAD_FMT":%u, %s\n", \
				__s, \
				NIPQUAD((__m)->privip), (__m)->privport, \
				NIPQUAD((__m)->pubip), (__m)->pubport, \
				(__m)->proto == IPPROTO_TCP ? "tcp":(__m)->proto == IPPROTO_UDP ?"udp" : "unknown")

#define port_intersect(ri1, ri2, rj1, rj2) ((ri1) <= (rj2) && (ri2) >= (rj1))

extern int cnapt_htable_size;

extern int cnapt_ct_htable_size;

extern spinlock_t cnapt_htable_lock;

extern spinlock_t cnapt_lock;

extern struct cnapt_t cnapt_fake;

extern struct list_head cnapt_serv_list;

extern struct hlist_head *cnapt_htable_bypriv;

extern int cnapt_register_serv(struct cnapt_serv_t *new_serv);

extern void cnapt_unregister_serv(struct cnapt_serv_t *target);

extern struct cnapt_t *__cnapt_lookup_bypriv(const struct cnapt_mapping_t *mapping);

extern struct cnapt_t *cnapt_lookup_bypriv(const struct cnapt_mapping_t *mapping);

extern struct cnapt_t *__cnapt_lookup_bypub(const struct cnapt_mapping_t *mapping);

extern struct cnapt_t *cnapt_lookup_bypub(const struct cnapt_mapping_t *mapping);

extern struct cnapt_t *cnapt_add(const struct cnapt_mapping_t *mapping);

extern void nf_ct_detach_cnapt(struct nf_conn *ct, enum ip_conntrack_dir dir);

extern int nf_ct_attach_cnapt(struct cnapt_t *napt, struct nf_conn *ct,
						int service, enum ip_conntrack_dir dir);

extern int cnapt_out(struct nf_conn *ct, struct cnapt_mapping_t *mapping, unsigned int hooknum);

extern int cnapt_in(struct nf_conn *ct, struct cnapt_mapping_t *mapping, unsigned int hooknum);

extern int cnapt_learning(struct nf_conn *ct, enum ip_conntrack_dir dir);

extern void cnapt_put(struct cnapt_t *napt);

extern void cnapt_force_kill(struct cnapt_t *napt);

extern void cnapt_iterate_cleanup(int (*iter)(struct cnapt_t *napt, void *data), void *data);

extern int cnapt_resolv_mapping(const struct cnapt_mapping_t *mapping, 
			enum ip_conntrack_dir dir, struct cnapt_t **pnapt, int *s_type);
			
extern int cnapt_check_config(const struct cnapt_mapping_range_t *mr, 
						int (*iter_filter)(struct cnapt_t *, void *), void *data,
						int *delta);

extern int cnapt_public_taken(const struct cnapt_mapping_t *mapping, int *port_suggest);

extern int cnapt_identify_source(const struct cnapt_t *napt, __be32 source);

extern struct cnapt_t *cnapt_follow_mapping(struct nf_conn *ct, const struct cnapt_mapping_t *mapping);

extern int __cnapt_get_pubport_preferred(struct cnapt_mapping_t *mapping, int *port_suggest);

static inline struct nf_conn_cnapt *
nfct_cnapt(struct nf_conn *ct)
{
	struct nf_conn_nat *ct_nat = nfct_nat(ct);
	return (ct_nat ? &ct_nat->ct_cnapt : NULL);
}

/* revert to nfct_cnapt */
static inline struct nf_conn *
cnapt_nfct(struct nf_conn_cnapt *ct_cnapt)
{
	return ct_cnapt->ct;
}

static inline void
cnapt_init_mapping(struct nf_conn *ct, struct cnapt_mapping_t *mapping, enum ip_conntrack_dir dir)
{
	mapping->proto = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum;
	if (dir == IP_CT_DIR_ORIGINAL) {
		mapping->privip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
		mapping->privport = ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.tcp.port);
		mapping->pubip = 0;
		mapping->pubport = 0;
	} else {
		mapping->pubip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip;
		mapping->pubport = ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.tcp.port);
		mapping->privip = 0;
		mapping->privport = 0;
	}
}

static inline int
mapping_priv_equal(const struct cnapt_mapping_t *m1, const struct cnapt_mapping_t *m2)
{
	return (m1->privport == m2->privport && m1->privip == m2->privip &&
			m1->proto == m2->proto && m1->pubip == m2->pubip);
}

static inline int
mapping_pub_equal(const struct cnapt_mapping_t *m1, const struct cnapt_mapping_t *m2)
{
	return (m1->pubport == m2->pubport && m1->pubip == m2->pubip && m1->proto == m2->proto);
}

static inline int
mr_priv_intersect(const struct cnapt_mapping_range_t *mr, const struct cnapt_mapping_t *mapping)
{
	return (port_intersect(mapping->privport, mapping->privport, mr->privport[0], mr->privport[1]) &&
			mapping->privip == mr->privip && 
			(mr->proto == mapping->proto || mr->proto == NAPT_PROTO_BOTH) &&
			(mr->pubip == mapping->pubip || mr->pubip == 0));
}

static inline int
mr_pub_intersect(const struct cnapt_mapping_range_t *mr, const struct cnapt_mapping_t *mapping)
{
	return (port_intersect(mapping->pubport, mapping->pubport, mr->pubport[0], mr->pubport[1]) &&
			(mr->proto == mapping->proto || mr->proto == NAPT_PROTO_BOTH) &&
			(mr->pubip == mapping->pubip || mr->pubip == 0));
}

/* check if two mr are conflict, if conflict, return 1, else return 0. */
static inline int
mr_conflict(const struct cnapt_mapping_range_t *mr1, const struct cnapt_mapping_range_t *mr2)
{
	if (mr1->proto == mr2->proto || mr1->proto == NAPT_PROTO_BOTH || mr2->proto == NAPT_PROTO_BOTH) {
		/* view from public side */
		if (port_intersect(mr1->pubport[0], mr1->pubport[1], mr2->pubport[0], mr2->pubport[1]) &&
			(mr1->pubip == mr2->pubip || mr1->pubip==0 || mr2->pubip==0)) {
			/* when we reach here, it means the private side must be exactly the same. 
			 * so when it is possiable for private side to be different, it is conflict! 
			 * Note: mr->privip can be 0 
			 */
			if (mr1->privip != mr2->privip || mr1->privip==0)
				return 1;
			/* when mr is a range, it potentially assume privport == pubport */
			if (mr1->pubport[0] == mr1->pubport[1]) {
				if (mr2->pubport[0] == mr2->pubport[1]) {
					if (mr1->privport[0] != mr2->privport[0])
						return 1;
				} else {
					if (mr1->pubport[0] != mr1->privport[0])
						return 1;
				}
			} else {
				if (mr2->pubport[0] == mr2->pubport[1] && mr2->privport[0] != mr2->pubport[0])
					return 1;
			}
			return 0;
		}
		/* view from private side */
		if (port_intersect(mr1->privport[0], mr1->privport[1], mr2->privport[0], mr2->privport[1]) &&
			(mr1->privip == mr2->privip || mr1->privip==0 || mr2->privip==0) &&
			(mr1->pubip == mr2->pubip || mr1->pubip==0 || mr2->pubip==0)) {
			/* when reach here, there is no overlap on pubport, so it is conflict ! */
			return 1;
		}
	}
	return 0;
}

/* 
 * check if a mr is conflict with a mapping.  if conflict, return 1, else return 0. 
 * we also can convert mapping to mr, and call mr_conflict directly, but to make
 * it run faster, we choose to rewrite the following code. 
 */
static inline int
mr_mapping_conflict(const struct cnapt_mapping_range_t *mr, const struct cnapt_mapping_t *mapping)
{
	if (mr->proto == mapping->proto || mr->proto == NAPT_PROTO_BOTH) {
		if (port_intersect(mapping->pubport, mapping->pubport, mr->pubport[0], mr->pubport[1]) && 
			(mapping->pubip == mr->pubip || mr->pubip==0)) {
			/* mr->privip can be 0 */
			if (mapping->privip != mr->privip)
				return 1;
			if (mr->pubport[0] == mr->pubport[1]) {
				if (mapping->privport != mr->privport[0])
					return 1;
			} else {
				/* it is a range, MUST privport == pubport */
				if (mapping->privport != mapping->pubport)
					return 1;
			}
			return 0;
		}
		if (port_intersect(mapping->privport, mapping->privport, mr->privport[0], mr->privport[1]) &&
			(mapping->privip == mr->privip || mr->privip==0) &&
			(mapping->pubip == mr->pubip || mr->pubip==0)) {
			/* when reach here, but there is no overlap on pubport, so it is conflict ! */
			return 1;
		}
	}
	return 0;
}

static inline int
mr_mapping_match(const struct cnapt_mapping_range_t *mr, const struct cnapt_mapping_t *mapping)
{
	if (!mr_pub_intersect(mr, mapping))
		return 0;
	
	if (mr->privip && mr->privip != mapping->privip)
		return 0;

	if (mr->privport[0] != mr->privport[1]) {
		if (!port_intersect(mapping->privport, mapping->privport, mr->privport[0], mr->privport[1]))
//		if (mapping->pubport != mapping->privport)
			return 0;
	} else {
		if (mr->privport[0] != mapping->privport)
			return 0;
	}

	return 1;		
}

#endif /* __KERNEL__ */

#endif

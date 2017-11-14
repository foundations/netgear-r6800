
#ifndef __CPM_H
#define __CPM_H

#ifdef __KERNEL__

#define CPM_HASH_LOG  8
#define CPM_HASH_SIZE (1 << CPM_HASH_LOG)

struct cpm_t {
	struct list_head list;
	struct hlist_node hlist[IP_CT_DIR_MAX];
	struct cnapt_mapping_range_t mr;
	u_int8_t type; /* port-forwarding/upnp-mapping... */ 
	u_int8_t enable; /* 0: disable, 1:active */
};

#endif

struct cpm_args_t {
	struct cnapt_mapping_range_t mr;
	u_int8_t type;
	u_int8_t enable;
};

#endif

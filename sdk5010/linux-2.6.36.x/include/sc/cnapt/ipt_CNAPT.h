#ifndef _IPT_CNAPT_H
#define _IPT_CNAPT_H

#define CNAPT_TAGSIZ  32

struct ipt_cnapt_info_t {
    int dir; /* direction. */
    u_int32_t eaddr; /* network order. external ip address. */
};

#endif


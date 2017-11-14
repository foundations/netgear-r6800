
#ifndef __IPT_NATLIMIT_H
#define __IPT_NATLIMIT_H

struct ipt_natlimit_info_t {
	u_int32_t lan_addr;
	u_int32_t lan_mask;
};

#endif


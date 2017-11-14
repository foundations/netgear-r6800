#ifndef __IPT_CUDP_H
#define __IPT_CUDP_H

#define UDP_OPEN  0
#define UDP_SEC   1

struct ipt_cudp_info_t {
    int mode;
    u_int32_t ctmark;
};

#endif


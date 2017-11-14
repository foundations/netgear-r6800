#ifndef _LINK_H_
#define _LINK_H_

#include "sc_project_defines.h"

/* The following part is hardware dependent! 
 * you may need to change to match your project 
 *
    Device rear panel:
    +-----+ +-----+-----+-----+-----+
    | WAN | |  4  |  3  |  2  |  1  |
    +-----+ +-----+-----+-----+-----+

    PORT number in Kernel:
    +-----+ +-----+-----+-----+-----+
    |  0  | |  1  |  2  |  3  |  4  |
    +-----+ +-----+-----+-----+-----+

    We should NOT let user application knows this. So we need do swap here.

    In User application:
    WAN port index: WAN_PORT_INDEX
    LAN port index: LAN_PORT_INDEX, LAN_PORT_INDEX+1 ... LAN_PORT_INDEX+3
 */

/* How many ports we have including LAN and WAN port. Start from 1 */
#define MAX_PHY_PORTS   5
#define LAN_PHY_PORTS   4
#define WAN_PHY_PORTS   1

#define WAN_PORT_INDEX  (4)
#define LAN_PORT_INDEX  (0)

/* LAN1 = port3, LAN2 = port2, LAN3=port1, LAN4=port0....
 * Here LAN1/LAN2/LAN3/LAN4 is defined by GUI/layout 
 * If your layout is differnet, you need to change th epmap here. --BillGuo */


/*
 * i and PORT_SWAP(i) are like these:
 * 0 <----> 4
 * 1 <----> 3
 * 2 <----> 2
 * 3 <----> 1
 * 4 <----> 0
 */
#if defined(MTK_BSP) || defined(BCM_BSP)
#ifdef INVERT_LAN_PORT
#define PORT_SWAP(i)	((i < WAN_PORT_INDEX) ? (LAN_PHY_PORTS - i - 1) : i)
#else
#define PORT_SWAP(i)    i // (MAX_PHY_PORTS - i - 1)
#endif
#else
#define PORT_SWAP(i)   (MAX_PHY_PORTS - i - 1)
#endif


#define PHY_RATE_1000M   (1000*1000*1000)
#define PHY_RATE_100M   (100*1000*1000)
#define PHY_RATE_10M    (10*1000*1000)

#define LINK_DOWN       0
#define LINK_UP         1

#ifndef DUPLEX_HALF
#define DUPLEX_HALF     0
#endif

#ifndef DUPLEX_FULL
#define DUPLEX_FULL     1
#endif

struct port_link_status_s {
    int link;           /* LINK_DOWN or LINK_UP */
    int duplex;         /* DUPLEX_HALF or DUPLEX_FULL */
    int up_rate;        /* in bps */
    int down_rate;      /* in bps */
    unsigned long txpkts;
    unsigned long txbytes;
    unsigned long rxpkts;
    unsigned long rxbytes;
    unsigned long collisions;
};

#define VECLEN      5   /* copy from et.c */


#define ROBO_PAGE_STATUS        0x01
#define ROBO_PAGE_MIB_BASE      0x20
#define ROBO_PAGE_MIB(i)        (ROBO_PAGE_MIB_BASE+(i))

#define ROBO_REG_LINK           0x00
#define ROBO_REG_LINK_CHG       0x02
#define ROBO_REG_SPEED          0x04
#define ROBO_REG_DUPLEX         0x06

/* Group 0 */
#define ROBO_REG_TXPKTS         0x00
#define ROBO_REG_RXPKTS         0x04

/* Group 1 */
#define ROBO_REG_COLLISIONS     0x00
#define ROBO_REG_TXBYTES        0x02
#define ROBO_REG_RXBYTES        0x06


/* Get these value from broadcom borg */
#define PHY_ADDR_START          0x1
#define PHY_REG_POWER           0x1E

/*******************************************************************************
 * NAME: get_ports_link_status
 *
 * DESCRIPTION:
 *       get ALL ports status
 *
 * INPUTS:
 *
 * OUTPUTS:
 *       status --- All interfaces status including link/up_rate/down_rate
 *
 * COMMENTS:
 *
 *******************************************************************************/
struct port_link_status_s *get_ports_link_status(void);

/*******************************************************************************
 * NAME: get_wan_port_link_status
 *
 * DESCRIPTION:
 *       get wan port status
 *
 * INPUTS:
 *
 * OUTPUTS:
 *       status --- the wan interface status including link/up_rate/down_rate
 *
 * COMMENTS:
 *
 *******************************************************************************/
struct port_link_status_s *get_wan_port_link_status(void);
struct port_link_status_s *get_eth_wan_port_link_status(void);
struct port_link_status_s *get_dsl_wan_port_link_status(void);

/*******************************************************************************
 * NAME: reset_lan_switch
 *
 * DESCRIPTION:
 *       Pull-low LAN switch for 3 seconds then pull-high
 * INPUTS:
 *
 * OUTPUTS:
 *
 * COMMENTS:
 *
 *******************************************************************************/
void reset_lan_switch(void);
void reset_lan_switch2(int sleeptime);
void reset_lan_switch_DOWN();
void reset_lan_switch_UP();
int is_available_dongle_pluged(void);
#endif  /* _LINK_H_ */


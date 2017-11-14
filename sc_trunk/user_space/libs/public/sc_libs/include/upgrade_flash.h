/***************************************************************
 *             Copyright (C) 2003 by SerComm Corp.
 *                    All Rights Reserved.
 *
 *      Use of this software is restricted to the terms and
 *      conditions of SerComm's software license agreement.
 *
 *                        www.sercomm.com
 ****************************************************************/
#ifndef _UPGRADE_FLASH_H_
#define _UPGRADE_FLASH_H_

#include "sc_project_defines.h"

#define MAGIC_SIGN      "sErCoMm"
//#define Erase_All_MAGIC_CONST           0x77

#define UPGRADE_TIMEOUT 15
typedef unsigned long UINT32;
#define ERROR_REPORT(fmt, args...)  SC_XCFPRINTF("\033[31m" fmt "\033[0m", ##args)
#define LOG_REPORT(fmt, args...)    SC_XCFPRINTF("\033[32m" fmt "\033[0m", ##args)
#define TMP_FW          "/tmp/fw.img"
#define UPGRADING_FILE	"/tmp/upgrading"

#ifdef BCM_BSP

#define PCBASN_MTD_SIZE     0x00100000

#define NVRAM_DATA_OFFSET               0x10580
#define offset(para) ((unsigned int)&(((NVRAM_DATA*)0x10580)->para))

#define NVRAM_BOOTLINE_LEN              256
#define NVRAM_BOARD_ID_STRING_LEN       16
#define NVRAM_MAC_ADDRESS_LEN           6

#define NVRAM_GPON_SERIAL_NUMBER_LEN    13
#define NVRAM_GPON_PASSWORD_LEN         11

#define NVRAM_WLAN_PARAMS_LEN      256
#define NVRAM_WPS_DEVICE_PIN_LEN   8

#define THREAD_NUM_ADDRESS_OFFSET       (NVRAM_DATA_OFFSET + 4 + NVRAM_BOOTLINE_LEN + NVRAM_BOARD_ID_STRING_LEN)
#define THREAD_NUM_ADDRESS              (0x80000000 + THREAD_NUM_ADDRESS_OFFSET)

#define NP_BOOT             0
#define NP_ROOTFS_1         1
#define NP_ROOTFS_2         2
#define NP_DATA             3
#define NP_BBT              4
#define NP_TOTAL            5

#define SERIAL_NUMBER_LEN		16
#define PIN_CODE_LEN        8
#define SERCOMM_MAC_NUM     16
#define SERCOMM_PSI_SIZE    DEFAULT_PSI_SIZE
#define PCBA_SERIAL_NUM_LEN     12
#define LANGUAGEID_LEN      2

typedef struct
{
    unsigned long ulVersion;
    char szBootline[NVRAM_BOOTLINE_LEN];
    char szBoardId[NVRAM_BOARD_ID_STRING_LEN];
    unsigned long ulMainTpNum;
    unsigned long ulPsiSize;
    unsigned long ulNumMacAddrs;
    unsigned char ucaBaseMacAddr[NVRAM_MAC_ADDRESS_LEN];
    char pad;
    char backupPsi;  /**< if 0x01, allocate space for a backup PSI */
    unsigned long ulCheckSumV4;
    char gponSerialNumber[NVRAM_GPON_SERIAL_NUMBER_LEN];
    char gponPassword[NVRAM_GPON_PASSWORD_LEN];
    char wpsDevicePin[NVRAM_WPS_DEVICE_PIN_LEN];
    char wlanParams[NVRAM_WLAN_PARAMS_LEN];
    unsigned long ulSyslogSize; /**< number of KB to allocate for persistent syslog */
    unsigned long ulNandPartOfsKb[NP_TOTAL];
    unsigned long ulNandPartSizeKb[NP_TOTAL];
    char szVoiceBoardId[NVRAM_BOARD_ID_STRING_LEN];
    unsigned long afeId[2];
    char chUnused[364];
    unsigned long ulCheckSum;
} NVRAM_DATA, *PNVRAM_DATA;
#endif /* BCM_BSP */

typedef struct sercomm_pid
{
    unsigned char	magic_s[7];	/* sErCoMm */
    unsigned char	ver_control[2];	/* version control */
    unsigned char	download[2];	/* download control */
    unsigned char	hw_id[32];  	/* H/W ID */
    unsigned char	hw_ver[2];  	/* H/W version */
    unsigned char	p_id[4];    	/* Product ID */
    unsigned char	protocol_id[2];	/* Protocol ID */
    unsigned char	fun_id[6];	/* Function ID */
    unsigned char	fw_ver[2];	/* F/W version */
    unsigned char	start[2];	/* Starting code segment */
    unsigned char	c_size[2];	/* Code size (kbytes) */
    unsigned char	magic_e[7];	/* sErCoMm */
}sercomm_pid_t;

int is_downgrade(char *file);
void show_and_exit(int index);
int image_is_correct(char *file);
void gw_fw_update(char *img);
void upgrade_download_init(void);
void upgrade_failed(void);

#endif /* _UPGRADE_FLASH_H_ */


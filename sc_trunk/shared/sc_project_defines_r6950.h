#ifndef PROJECT_DEFINES_R6950
#define PROJECT_DEFINES_R6950

/* this is project depend defines. 
 * please not ifdef but just define. */

//----------------------------------
// nvram for product name
//----------------------------------
#define BOARD_ID_DEFAULT   "R6800"
#define BOARD_ID           (nvram_safe_get("product_name") ? : BOARD_ID_DEFAULT)

//----------------------------------
// SDK / CPU / SWITCH / GPIO / DSL information
//----------------------------------
/* apps/led_hw, shared/led_action.h */
#define SC_GPIO_DEFINE_R62200
/* #define SC_GPIO_DEFINE_JR6150 */

#define SC_GPIO_2G5G_SEPERATE_LED

/* apps/rc, sdk4210/user/rt2880_app/ated , sc_libs/liblink/link.c*/
#define SC_SWITCH_USE_GMAC2
/* #define SC_SWITCH_USE_VLAN */
/* #define SC_SWITCH_USE_EXTERNAL_ATH */

#define P4_AS_WAN

//#define LED_CTRL_R6220_ONLY

/* need to invert lan port , sc_libs/include/upgrade_flash.h*/
#define INVERT_LAN_PORT

/* apps/rc  sc_libs */
#define SC_CPU_MT7621

#ifndef MTK_BSP
#define MTK_BSP
#endif

#define SDK_MTK5000
#define SC_WIFI_MT7615

#define CUSTOMER_NAME "NTGR"

/* if this project has dsl and type? */
/* #define ADSL */
/* #define ADSL_VENDOR_MTK */
/* #define ADSL_VENDOR_BRCM */
/* #define ADSL_VENDOR_LANTIQ */

/* #define PROJECT_D7000 */
#define PROJECT_R6950

//----------------------------------
// flash: partition, offset, flash driver information 
//----------------------------------
#define RWFS_MTD                   "/dev/mtdblock28"
#define RWFS_MTD_SIZE_FOR_FS "30"       // in MB, should be a little smaller than real size if nand flash
#define RWFS_MTD_BLOCK_SIZE (128*1024)
#define RWFS_MOUNT_DIR           "/tmp/RWFS/"

/* move from sc_libs/include/upgrade_flash.h */
#define FLASH_128M
#define FLASH_SIZE	(128*1024*1024)
/* user_space/libs/public/sc_libs/libupgrade/libupgrade.c */
#define SUPPORT_MULTIPAL_MTD_BLOCK
#define KERNEL_OFFSET   0x200000
#define KERNEL_SIZE     0x400000
#define FS_OFFSET       0x600000
#define FS_SIZE         (40*0x100000)
#define English_OFFSET  0x2e00000

#define LANG_OFFSET_CHS 0x03000000
#define LANG_OFFSET_RUS 0x03200000
#define LANG_OFFSET_DEU 0x03400000
#define LANG_OFFSET_ESP 0x03600000
#define LANG_OFFSET_NLD 0x03800000
#define LANG_OFFSET_KOR 0x03a00000
#define LANG_OFFSET_FRA 0x03c00000
#define LANG_OFFSET_SVE 0x03e00000
#define LANG_OFFSET_CSY 0x04000000
#define LANG_OFFSET_TUR 0x04200000
#define LANG_OFFSET_ITA 0x04400000

#define English_SIZE    0x200000

/* user_space/libs/public/sc_libs/libscmisc/auto_up.c, sc_language.c ;  R6220 not use, D7000 need open*/
//#define USE_JEFFS2_FS
/* sc_libs/libflash/flash.c, defined for skipping bad block by sc, MTK for R6220 not use, BCM for D7000 need open*/
//#define SC_SKIP_BAD_BLOCK

/* 
 * user_space/libs/public/sc_libs/libscmisc/sc_language.c 
 * Define Language MTD mapping partition 
 */
#define ENU_MTD_MAPPING		"/dev/mtdblock4"
#define CHS_MTD_MAPPING		"/dev/mtdblock5"
#define RUS_MTD_MAPPING		"/dev/mtdblock6"
#define DEU_MTD_MAPPING		"/dev/mtdblock7"
#define ESP_MTD_MAPPING		"/dev/mtdblock8"
#define NLD_MTD_MAPPING		"/dev/mtdblock9"
#define KOR_MTD_MAPPING		"/dev/mtdblock10"
#define FRA_MTD_MAPPING		"/dev/mtdblock11"
#define SVE_MTD_MAPPING		"/dev/mtdblock12"
#define CSY_MTD_MAPPING		"/dev/mtdblock13"
#define TUR_MTD_MAPPING		"/dev/mtdblock14"
#define ITA_MTD_MAPPING		"/dev/mtdblock15"
#define XXX_MTD_MAPPING		"/dev/mtdblock24"

/* make sure the NULL MTD is placed at the end of the language_mtd_mapping in sc_language.c */
#define PTB_MTD_MAPPING                NULL

/*
 * sc_libs/flash.c, libupgrade.c, 
 * Define FLASH MTD mapping partition
 */
#define FILESYSTEM      	"/dev/mtdblock3"
#define KERNEL_PATH     	"/dev/mtdblock2"
#define LOADER          	"/dev/mtdblock0"
#define SC_PID          	"/dev/mtdblock20"
#define PCBASN_PATH      	"/dev/mtdblock17"
#define ENGLISH_LANGUAGE    "/dev/mtdblock4"
#define WIFI_DATA       	"/dev/mtdblock16"

#define MTD_BLOCK_TM "/dev/mtdblock19"
#define MTD_CHAR_TM "/dev/mtd19"

#define MTD_CHAR_RALINK_NVRAM "/dev/mtd22"

#define UPGRADE_FLAG		"/dev/mtdblock23" //kernel_upgrade.c

/* the partition or the file that the nvram will use it, like /dev/mtdblock15 or /config/file/nvram */
/* used by apps/nvram/nvram.c */
#define NVRAM_PATH			"/dev/mtdblock21"

/* the partition or the filename that the pot will use it */
/* Used by apps/public/pot and ntp  */
#define Partition   		"/dev/mtdblock18"
#define CHARMTD     		"/dev/mtd18"
#define OPENVPN_PKG		"/dev/mtdblock25"
/* apps/sc_libs/include/upgrade_flash.h. 
 * bootloader of r6200v3 is bigger than jr6150 0x20000 so enlarge it. */
#define SC_BOOTLOADER_PID_END 0xf0000 // XXX.par end of pid + 1, effff+1 

/*
 * sc_libs/flash.c, libupgrade.c , upgrade_flash.h
 * The assigned parameters below in SC_PRIVATA_DATA block
 */
#define USE_MODULE_ID //scfgmgr/* sc_libs/*
#define USE_CHK_CAL //scfgmgr/* sc_libs/*

#define MAC_OFFSET      0x0B0
#define SN_OFFSET       0x0B7
#define WPSPIN_OFFSET   0x0E1
#define LANG_ID_OFFSET  0x0EE
#define DOMAIN_OFFSET   0x0F2
#define REGION_OFFSET   DOMAIN_OFFSET
#define PCBA_SN_OFFSET  0x0F6     
#define REMOTESCFGMGRMAGIC_OFFSET  0x102 
#define CSN_OFFSET      SN_OFFSET  /*CSN_LEN is 42, next use 0x90*/
#define SSID_OFFSET     0x106 /* SSID_LEN is 20, next use 0x120*/
#define WPAKEY_OFFSET   0x120 /* WPAKEY_LEN is 64, next use 0x160*/
#define PRODUCT_ID_OFFSET	0x160 /* PRODUCT_ID_LEN is 4,now not use */
#define MODULE_ID_OFFSET	0x200 /* MODULE_ID_LEN is 2 */
#define CAL_OFFSET      0x10000 /* CAL_LEN is 5 */


#define PID_LEN         70
#define PID_OFFSET      SC_BOOTLOADER_PID_END - PID_LEN // offset in mtd block effba 
#define MIN_UPLOAD_LEN  (1024*1024)

/* The assigned parameters below in SC_PRIVATA_DATA block */
#define MAC_LEN         6
#define WPSPIN_LEN      12
#define LANG_ID_LEN     4
#define SN_LEN          42
#define DOMAIN_LEN      2
#define REGION_LEN      2
#define CSN_LEN         42
#define PCBA_SN_LEN     12
#define REMOTESCFGMGRMAGIC_LEN     4
#define SSID_LEN        20
#define WPAKEY_LEN      64 
#define PRODUCT_ID_LEN  4
#define MODULE_ID_LEN   2
#define CAL_LEN         5

//----------------------------------
// defines that differents between projects 
//----------------------------------
/* apps/public/ntp/netgear_ntp.c, define for multi platform */
#define NTP_RETRY_OFFSET 6
/* user_space/libs/public/sc_libs/ip_assign_chk/ip_assign_chk.c */
#define IP_ASSIGN_CHK_ADDR "192.168.1.1"

/*access_devinfo*/
#ifndef NETLINK_SCQOS
#define NETLINK_SCQOS 20
#endif
#ifndef NETLINK_ARP
#define NETLINK_ARP 21
#endif

//----------------------------------
// to move out of this .h
//----------------------------------
/* apps/rc/rc.c, apps/led_hw/ */
/* #define SC_R6200V3_CUT1 */
#define WINDOWS_GENIE_DOWNLOAD_URL		"http://updates1.netgear.com/netgeargenie/update/NETGEARGenie-install.exe"
#define WINDOWS_READYSHARE_DOWNLOAD_URL	"http://updates1.netgear.com/readysharevault/ReadySHAREVault-install.exe"
#define MAC_GENIE_DOWNLOAD_URL		"http://updates1.netgear.com/netgeargenie/mac/update/NETGEARGenieInstaller.dmg"

//#define MODEL_SUCCESS_PAGE_URL  "http://www.netgear.com/success/%s.aspx", BOARD_ID //netgear success page
#define MODEL_SUCCESS_PAGE_URL  "http://www.netgear.com/success/generic.aspx" //netgear success page
/* scfgmgr */
#define DEFAULT_REMOTE_IP   "192.168.1.1"

#define SINGLESKU_SOURCE_PATH "/etc/mtk/singlesku_r6950/"


#endif

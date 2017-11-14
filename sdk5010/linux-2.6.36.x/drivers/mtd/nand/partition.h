#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>

#define RECONFIG_PARTITION_SIZE 1

#define MTD_BOOT_PART_SIZE  0x80000
#define MTD_CONFIG_PART_SIZE    0x20000
#define MTD_FACTORY_PART_SIZE   0x20000

extern unsigned int  CFG_BLOCKSIZE;
#define LARGE_MTD_BOOT_PART_SIZE       (CFG_BLOCKSIZE<<2)
#define LARGE_MTD_CONFIG_PART_SIZE     (CFG_BLOCKSIZE<<2)
#define LARGE_MTD_FACTORY_PART_SIZE    (CFG_BLOCKSIZE<<1)

#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
#define MTD_ROOTFS_RESERVED_BLOCK	0x80000  // (CFG_BLOCKSIZE<<2)
#endif

#include "../maps/ralink-flash.h"

/*=======================================================================*/
/* NAND PARTITION Mapping                                                  */
/*=======================================================================*/
#if defined(CONFIG_SUPPORT_OPENWRT)
static struct mtd_partition g_pasStatic_Partition[] = {
	{
	        name:           "ALL",
	        size:           MTDPART_SIZ_FULL,
	        offset:         0,
	},
	/* Put your own partition definitions here */
	{
	        name:           "Bootloader",
	        size:           MTD_BOOT_PART_SIZE,
	        offset:         0,
	}, {
	        name:           "Config",
	        size:           MTD_CONFIG_PART_SIZE,
	        offset:         MTDPART_OFS_APPEND
	}, {
	        name:           "Factory",
	        size:           MTD_FACTORY_PART_SIZE,
	        offset:         MTDPART_OFS_APPEND
	}, {
	        name:           "firmware",
	        size:           MTDPART_SIZ_FULL,
	        offset:         0x140000,
	},
};
#else /* CONFIG_SUPPORT_OPENWRT */
//#ifdef CONFIG_MTD_PARTITIONS

#ifdef CONFIG_SCM_SUPPORT

	#define SC_FL_PARTS_TBL_MAGIC "SCFLMAPOK"
	#define SC_DUAL_IMAGE_MAGIC "SCDUALIMAGE"
	#define SC_PARTITION_NUM 29
	#define BLOCK_size (128*1024)
	#define PAGE_size 2048
	int sc_ranand_read(char *buf, unsigned int from, int datalen);

static struct mtd_partition g_pasStatic_Partition[] = {
	{
#if 0
                name:           "ALL",
                size:           MTDPART_SIZ_FULL,
                offset:         0,
        }, {
#endif
                name:           "Bootloader",
                size:			SC_MTD_BOOT_SIZE,
                offset:         SC_MTD_BOOT_OFFSET,
        }, {
                name:           "SC_PART_MAP",
                size:			SC_MTD_PART_MAP_SIZE,
                offset:         SC_MTD_PART_MAP_OFFSET,
        }, {
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
                name:           "Kernel",
#else
                name:           "Kernel and FS",
#endif
                size:			SC_MTD_KERNEL_SIZE,
                offset:			SC_MTD_KERNEL_OFFSET
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
        }, {
                name:           "Rootfs",
                size:			SC_MTD_ROOTFS_SIZE,
                offset:			SC_MTD_ROOTFS_OFFSET
#endif
        }, {
                name:           "English UI",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_EN_OFFSET
        }, {
                name:           "ML1",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_ML1_OFFSET
        }, {
                name:           "ML2",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_ML2_OFFSET
        }, {
                name:           "ML3",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_ML3_OFFSET
        }, {
                name:           "ML4",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_ML4_OFFSET
        }, {
                name:           "ML5",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_ML5_OFFSET
        }, {
                name:           "ML6",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_ML6_OFFSET
        }, {
                name:           "ML7",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_ML7_OFFSET
        }, {        	                
                name:           "ML8",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_ML8_OFFSET
        }, {        	                
                name:           "ML9",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_ML9_OFFSET
        }, {        	                
                name:           "ML10",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_ML10_OFFSET
        }, {        	                
                name:           "ML11",
                size:			SC_MTD_LANG_SIZE,
                offset:			SC_MTD_LANG_ML11_OFFSET
        }, {        	
                name:           "Factory", //"WiFi Data",
                size:			SC_MTD_WIFI_DATA_SIZE,
                offset:			SC_MTD_WIFI_DATA_OFFSET
        }, {
                name:           "SC Private Data",
                size:			SC_MTD_PRIVATE_SIZE,
                offset:			SC_MTD_PRIVATE_OFFSET
        }, {
                name:           "POT",
                size:           SC_MTD_POT_SIZE,
                offset:         SC_MTD_POT_OFFSET
        }, {
                name:           "Traffic Meter",
                size:			SC_MTD_TM_SIZE,
                offset:			SC_MTD_TM_OFFSET
        }, {
                name:           "SC PID",
                size:			SC_MTD_PID_SIZE,
                offset:			SC_MTD_PID_OFFSET
        }, {
                name:           "SC Nvram",
                size:           SC_MTD_NVRAM_SIZE,
                offset:         SC_MTD_NVRAM_OFFSET
        }, { /* keep the name same as in ralink nvram.h  */
                name:           "Ralink Nvram",
                size:           SC_MTD_RANVRAM_SIZE,
                offset:         SC_MTD_RANVRAM_OFFSET
        }, {
                name:           "Reserved Block1",
                size:			SC_MTD_RESERVED1_SIZE,
                offset:         SC_MTD_RESERVED1_OFFSET
        }, {
                name:           "Reserved Block2",
                size:           SC_MTD_RESERVED2_SIZE,
                offset:         SC_MTD_RESERVED2_OFFSET
        }, { 
                name:           "Reserved Block3",
                size:           SC_MTD_RESERVED3_SIZE,
                offset:         SC_MTD_RESERVED3_OFFSET
        }, {
                name:           "Reserved Block4",
                size:           SC_MTD_RESERVED4_SIZE,
                offset:         SC_MTD_RESERVED4_OFFSET
        }, {
                name:           "Reserved Block5",
                size:           SC_MTD_RESERVED5_SIZE,
                offset:         SC_MTD_RESERVED5_OFFSET
        }, {
                name:           "Reserved Block6",
                size:           SC_MTD_RESERVED6_SIZE,
                offset:         SC_MTD_RESERVED6_OFFSET
        }        
#if 0 
	/* Reserved for BBT */
	, {
                name:           "BBT",
                size:           SC_MTD_BBT_SIZE,
                offset:         SC_MTD_BBT_OFFSET
        }
#endif
};

#else



static struct mtd_partition g_pasStatic_Partition[] = {
	{
                name:           "ALL",
                size:           MTDPART_SIZ_FULL,
                offset:         0,
        },
        /* Put your own partition definitions here */
        {
                name:           "Bootloader",
                size:           MTD_BOOT_PART_SIZE,
                offset:         0,
        }, {
                name:           "Config",
                size:           MTD_CONFIG_PART_SIZE,
                offset:         MTDPART_OFS_APPEND
        }, {
                name:           "Factory",
                size:           MTD_FACTORY_PART_SIZE,
                offset:         MTDPART_OFS_APPEND
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
        }, {
                name:           "Kernel",
                size:           MTD_KERN_PART_SIZE,
                offset:         MTDPART_OFS_APPEND,
        }, {
                name:           "RootFS",
                size:           MTD_ROOTFS_PART_SIZE,
                offset:         MTDPART_OFS_APPEND,
        }, {
                name:           "RootFS_reserved",
                size:           MTD_ROOTFS_RESERVED_BLOCK,
                offset:         MTDPART_OFS_APPEND,
#ifdef CONFIG_ROOTFS_IN_FLASH_NO_PADDING
        }, {
                name:           "Kernel_RootFS",
                size:           MTD_KERN_PART_SIZE + MTD_ROOTFS_PART_SIZE,
                offset:         MTD_BOOT_PART_SIZE + MTD_CONFIG_PART_SIZE + MTD_FACTORY_PART_SIZE,
#endif
#else //CONFIG_RT2880_ROOTFS_IN_RAM
        }, {
                name:           "Kernel",
                size:           MTD_KERN_PART_SIZE,
                offset:         MTDPART_OFS_APPEND,
#endif
#ifdef CONFIG_DUAL_IMAGE
        }, {
                name:           "Kernel2",
                size:           MTD_KERN2_PART_SIZE,
                offset:         MTD_KERN2_PART_OFFSET,
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
        }, {
                name:           "RootFS2",
                size:           MTD_ROOTFS2_PART_SIZE,
                offset:         MTD_ROOTFS2_PART_OFFSET,
#endif
#endif
        }

};

#endif /* CONFIG_SCM_SUPPORT */

#endif /* CONFIG_SUPPORT_OPENWRT */
#define NUM_PARTITIONS ARRAY_SIZE(g_pasStatic_Partition)
extern int part_num;	// = NUM_PARTITIONS;
//#endif
#undef RECONFIG_PARTITION_SIZE


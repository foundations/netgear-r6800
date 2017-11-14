#include <linux/module.h>
#include <linux/ctype.h>
#include <linux/fs.h>
#include <linux/fd.h>
#include <linux/nfs_fs.h>
#include <asm/uaccess.h>
#include <linux/file.h>
#include <linux/proc_fs.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/version.h>
#include <sc_project_defines.h>

//#define DEBUG
#ifdef DEBUG
#define debug printk
#else
#define debug(...)
#endif

#define UPGRADE_FILE  "upgrade_file"
#ifdef USE_MTK_NAND_MTD_WRITE
#undef __KERNEL__
#include "../bsp/user/mtd_write/mtd.h"
#define __KERNEL__

#endif


static char line[1024] = {0,};
static char file_name[128];
#ifdef SUPPORT_MULTIPAL_MTD_BLOCK
#define MAX_UPG_MTD_NUM		30 
struct t_mtdinfo
{
	int fd;
	char mtd_name[256];
	unsigned long offset;
	unsigned long size;
#ifdef USE_MTK_NAND_MTD_WRITE
    unsigned long w;
    unsigned long e;
    struct mtd_info_user mtdInfo;
#endif

};
static struct t_mtdinfo upg_mtdinfo[MAX_UPG_MTD_NUM];
static int upg_mtd_cnt = 0;
#else
static char mtd_name[128];
static int offset, kfs_size, lang_size;
#endif
#include "unzip.c"

#define Erase_All_MAGIC_CONST           0x77
#define ZIP3G_MAGIC_CONST               0x11

#ifdef DEBUG
void kernel_reboot(void)
{
    printk("<0>""Call reboot function\n");
}
#else

void machine_restart(char *command);
void kernel_reboot(void)
{
    printk("<0>""Call reboot function\n");
    machine_restart(NULL);
}

#endif

#ifdef USE_MTK_NAND_MTD_WRITE
static int mtk_mtd_open(const char *mtd, int flags)
{
	int fd;
	char dev[PATH_MAX];
	int i;
	int ret;
	char mtd_name[32];
    char buf[1024], *pos;

	memset(mtd_name, 0, 32);
	sprintf(mtd_name, "\"%s\"",mtd);

    if (strncmp(mtd, "/dev", 4) == 0)
        return sys_open(mtd, flags, 0);
    else if ((fd = sys_open("/proc/mtd", O_RDONLY, 0)))
    {

        sys_read(fd, buf, sizeof(buf));
        
		if ((pos = strstr(buf, mtd_name)) != NULL) {
            while (pos > buf && *(pos-1) != '\n')
                pos --;
			if (sscanf(pos, "mtd%d:", &i)) {
				snprintf(dev, sizeof(dev), "/dev/mtd/mtd%d", i);
				if ((ret = sys_open(dev, flags, 0))<0) {
					snprintf(dev, sizeof(dev), "/dev/mtd%d", i);
					ret = sys_open(dev, flags, 0);
				}
				sys_close(fd);
				return ret;
			}
		}
		sys_close(fd);
	}

	return sys_open(mtd, flags, 0);
}
#endif

int upgrade_open(char *filename)
{
    int                     fd;

#ifdef USE_MTK_NAND_MTD_WRITE
    fd = mtk_mtd_open(filename, O_RDWR | O_SYNC);
    printk("Open by mtd name <%s> %s!\n", filename, fd > 0 ? "OK" : "Failed");
#else
    fd = sys_open(filename, O_RDWR | O_SYNC, 0);
#endif

    if(fd < 0)
    {
        printk("Failed to open file %s\n",filename);
        return -1;
    }
    return fd;
}
#ifdef SUPPORT_MULTIPAL_MTD_BLOCK
int write_fw_to_mtd(int fw_fd)
{
    /* jump over image header: 512 bytes */
    sys_lseek(fw_fd, 512, 0);
    printk("<0>""write_fw_to_mtd...\n");
    unzip(fw_fd);
    return 0;
}
#else
int write_fw_to_mtd(int fw_fd, int mtd_fd, unsigned long offset)
{

    /* jump over image header: 512 bytes */
    sys_lseek(fw_fd, 512, 0);
    printk("<0>""write_fw_to_mtd, offset=%lx\n", offset);
    unzip(fw_fd, mtd_fd, offset);
    return 0;
}
#endif

#ifdef MARK_UPGRADE
//#define UPGRADE_FLAG	"/dev/mtdblock19" move this marco to sc_project_defines.h
#ifdef SUPPORT_MULTIPAL_MTD_BLOCK
static int mark_upgrade_start(void)
{
	unsigned char buf[] = { 0x0 };
	int fd = upgrade_open(UPGRADE_FLAG);
	sys_write(fd, (void *)buf, sizeof(buf));
	sys_close(fd);
	return 0;
}

static int mark_upgrade_finish(void)
{
	unsigned char buf[] = { 0xff };
	int fd = upgrade_open(UPGRADE_FLAG);
	sys_write(fd, (void *)buf, sizeof(buf));
	sys_close(fd);
	return 0;
}
#else
#define BLOCK_SIZE_KM (128*1024)
static int is_a_bad_block(int fd, loff_t offset)
{
    int ret = 0;
    
    if((ret = sys_ioctl(fd, MEMGETBADBLOCK, &offset)))
    { 
        return 1; 
    }

    return ret;  
}

int need_offset_num(loff_t offset)
{
    int fd=0;
    int bad_block_num = 0;
    
    fd = sys_open(UPGRADE_FLAG_MTD, O_RDONLY, 0);
    if(fd < 0)
    {
        return 0;
    }
    
    while(is_a_bad_block(fd, offset) == 1)
    {
        bad_block_num++;
        offset += BLOCK_SIZE_KM;   
    }
    sys_close(fd);
    printk("find %d BAD block.\n", bad_block_num);
    return bad_block_num;   
    
}
static void print_SCFLAG_str(int bad_block)
{
	unsigned char buf[4] = {0};
	int fd = 0;
	
	fd = upgrade_open(UPGRADE_FLAG);
	if(bad_block > 0)
	    sys_lseek( fd, bad_block*BLOCK_SIZE_KM, 0);
	sys_read( fd,  buf, sizeof(buf));
	sys_close(fd);
    printk("\nbuf is %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3]);
    return ;
}
static int mark_upgrade_start(int bad_block)
{
	unsigned char buf[] = { 0x01 };
    int fd = 0;
    
	fd = upgrade_open(UPGRADE_FLAG);
	/* FW upgrade  flag is 4th byte, skip 3 */
	sys_lseek( fd, 3+(bad_block*BLOCK_SIZE_KM), 0);
	sys_write(fd, (void *)buf, sizeof(buf));
	sys_close(fd);
	return 0;
}
static int mark_upgrade_finish(int bad_block)
{
	unsigned char buf[4] = { 0x01, 0xff , 0xff, 0x00};
	int fd = 0;
		
	fd = upgrade_open(UPGRADE_FLAG);
	if(bad_block > 0)
	    sys_lseek( fd, bad_block*BLOCK_SIZE_KM, 0);
	sys_write(fd, (void *)buf, sizeof(buf));
	sys_close(fd);
	print_SCFLAG_str(bad_block);
	return 0;
}

#endif
#endif

#ifdef AMPED_DUAL_IMAGE
static inline int boot_from_imageA(unsigned short flag)
{
       return flag % 2 == 1;
}

static void read_image_param_from_flash(unsigned short *flag, unsigned short *failA, unsigned short *failB)
{
    char *buf = (char *) MALLOC(FLAG_BUF_SIZE);
    int fd;

    if (!buf)
    {
        printk("malloc failed!\n");
        return;
    }
	fd = upgrade_open(BOOT_FLAG);
    if (fd < 0)
    {
        printk("open failed!\n");
        return;
    }
    
    sys_read(fd, buf, FLAG_BUF_SIZE);
    sys_close(fd);
    
#ifdef MT7621_DUAL_IMG
	if( memcmp(buf, "SCDUALIMAGE", strlen("SCDUALIMAGE") ) == 0 )
	{
		printk("<0>""Get the magic work!\n");
		*flag = *((unsigned short *)(buf + FLAG_SEEK_LENGTH*2));
		*failA = *((unsigned short *)(buf + FLAG_SEEK_LENGTH*3));
		*failB = *((unsigned short *)(buf + FLAG_SEEK_LENGTH*4));
	}
#else
	*flag = *((unsigned short *)(buf));
    *failA = *((unsigned short *)(buf + FLAG_SEEK_LENGTH));
    *failB = *((unsigned short *)(buf + FLAG_SEEK_LENGTH*2));
#endif

    kfree(buf);


}

#ifndef MT7621_DUAL_IMG
#define BUFSIZE (1 * 1024)
static int mtk_mtd_write(int fd, char *data, int len)
{
	int result;
	size_t r, w, e;
	struct mtd_info_user mtdInfo;
	struct erase_info_user mtdEraseInfo;
	int	boot_from_spi = 0;
    int total = len;
    char *buf, *buf_orig;

	if(sys_ioctl(fd, MEMGETINFO, &mtdInfo)) {
		printk("Could not get MTD device info from %d\n", fd);
		return -1;
	}
		
	r = w = e = 0;

	if (mtdInfo.oobsize == 0)
		boot_from_spi = 1;

	if (boot_from_spi == 0) {	
		buf_orig = (char *)MALLOC(mtdInfo.erasesize+mtdInfo.oobblock);
		if (NULL == buf_orig) {
			printk("MALLOC failed\n");
			return -1;
		}
		memset(buf_orig, 0xff, mtdInfo.erasesize+mtdInfo.oobblock); 
		buf = ((unsigned int)(buf_orig + mtdInfo.oobblock-1)/mtdInfo.oobblock)*mtdInfo.oobblock; 	
	}
	else {
		buf = (char *)MALLOC(BUFSIZE);
		if (NULL == buf) {
			printk("MALLOC failed\n");
			return -1;
		}	
	}
	
	for (; len;) {
		if (boot_from_spi == 0) {	
			struct mtd_oob_buf oob;
			unsigned char oobd[mtdInfo.oobsize];
			oob.start = (u_int32_t)e;
			oob.length = mtdInfo.oobsize;
			oob.ptr = oobd;

			if (sys_ioctl(fd, MEMREADOOB, &oob) != 0) {
				printk("Reading %d OOB failed: %x\n   ", fd, e);
			}
			else if (oobd[0] != (unsigned char)0xff) { //CONFIG_BAD_BLOCK_POS
				printk("skip bad block %x\n   ", e);
				if (sys_lseek(fd, mtdInfo.erasesize, SEEK_CUR) == -1) {
 					printk("lseek failed.\n");
					kfree(buf);
 					return -1;
 				}
				e += mtdInfo.erasesize;
				w += mtdInfo.erasesize;
				continue;
			}
			
		}
		/* buffer may contain data already (from trx check) */
		r = 0;
		
        r = mtdInfo.erasesize > len ? len : mtdInfo.erasesize;
        memcpy(buf, data + total - len, r);

		w += r;

		/* EOF */
		if (r <= 0) break;

		len = len - r;

		/* need to erase the next block before writing data to it */
		while (w > e) {
			mtdEraseInfo.start = e;
			mtdEraseInfo.length = mtdInfo.erasesize;

			/* erase the chunk */
			if (sys_ioctl (fd,MEMERASE,&mtdEraseInfo) < 0) {
				printk("Erasing mtd failed: %d\n", fd);
				kfree(buf);
				return -1;
			}
			e += mtdInfo.erasesize;
		}

		if (boot_from_spi == 0)
			r = ((r+(mtdInfo.oobblock-1))/mtdInfo.oobblock)*mtdInfo.oobblock;	

		if ((result = sys_write(fd, buf, r)) < r) {
			if (result < 0) {
				printk("Error writing image.\n");
				kfree(buf);
				return -1;
			} else {
				printk("Insufficient space.\n");
				kfree(buf);
				return -1;
			}
		}
	}

	if (boot_from_spi == 0)
		kfree(buf_orig);
	else
		kfree(buf);

	return 0;
}
#endif
static void write_image_param_to_flash(unsigned short flag, unsigned short failA, unsigned short failB)
{
    char *buf = (char *) MALLOC(FLAG_BUF_SIZE);
    int fd;
    if (!buf)
    {
        printk("malloc failed!\n");
        return;
    }
    memset(buf, 0xff, FLAG_BUF_SIZE);

	fd = upgrade_open(BOOT_FLAG);

    if (fd < 0)
    {
        printk("open failed!\n");
        return;
    }

#ifdef MT7621_DUAL_IMG
	sys_read(fd, buf, FLAG_BUF_SIZE);
	if( memcmp(buf, "SCDUALIMAGE", strlen("SCDUALIMAGE") ) == 0 )
	{
		printk("<0>""Get the magic work!\n");
		*((unsigned short *)(buf + FLAG_SEEK_LENGTH*2)) = flag;
		*((unsigned short *)(buf + FLAG_SEEK_LENGTH*3)) = failA;
		*((unsigned short *)(buf + FLAG_SEEK_LENGTH*4)) = failB;
		sys_lseek(fd, -FLAG_BUF_SIZE, SEEK_CUR);
		sys_write(fd, buf, FLAG_BUF_SIZE);
	}
#else
    *((unsigned short *)(buf)) = flag;
    *((unsigned short *)(buf + FLAG_SEEK_LENGTH)) = failA;
    *((unsigned short *)(buf + FLAG_SEEK_LENGTH*2)) = failB;

	mtk_mtd_write(fd, buf, FLAG_BUF_SIZE);
#endif
	sys_close(fd);

    kfree(buf);
}

#endif



#ifdef SUPPORT_MULTIPAL_MTD_BLOCK
void upgrade_fw(char *fw_name)
{
    unsigned char magic;
    int len = 0;
    int fw_fd;
	int i = 0;
    mm_segment_t            fs = get_fs();
#ifdef AMPED_DUAL_IMAGE
    unsigned short boot_flag = 0xffff;  //boot_flag, odd: image A, even: image B
    unsigned short boot_failA = 0;    //fail counters for booting image A
    unsigned short boot_failB = 0;    //fail counters for booting image B
#endif

	if (len)
	{
		/* let gcc463 not complain. */
	}

    set_fs(get_ds());
    printk("<0>""Prepare to open fd\n");

    if(((fw_fd = upgrade_open(fw_name)) < 0))
    {
        set_fs(fs);
        kernel_reboot();
        return ;
    }

    /* Read flag of PID */
    sys_lseek(fw_fd, 72, 0);
    len = sys_read(fw_fd,  &magic, sizeof(magic));

#ifdef AMPED_DUAL_IMAGE
    /* read boot flag */
    read_image_param_from_flash(&boot_flag, &boot_failA, &boot_failB);

    printk("<0>""read boot flag: 0x%02x (boot image %s), boot_failA: 0x%02x, boot_failB: 0x%02x!\n", 
        boot_flag, boot_from_imageA(boot_flag) ? "A": "B", boot_failA, boot_failB);
#endif


	for (i=0; i<upg_mtd_cnt; i++){
#ifdef AMPED_DUAL_IMAGE
        if (i == 0)
        {
            strcpy(upg_mtdinfo[i].mtd_name, boot_from_imageA(boot_flag) ? IMAGE_B_PATH : IMAGE_A_PATH); 

            printk("<0>""change mtd_name to <%s> according to boot flag <0x%02x>!\n", 
                upg_mtdinfo[i].mtd_name, boot_flag);
        }
#ifdef MT7621_DUAL_IMG
        if (i == 1)
        {
            strcpy(upg_mtdinfo[i].mtd_name, boot_from_imageA(boot_flag) ? ROOT_FS_B_PATH : ROOT_FS_A_PATH); 

            printk("<0>""change mtd_name to <%s> according to boot flag <0x%02x>!\n", 
                upg_mtdinfo[i].mtd_name, boot_flag);
        }
#endif
#endif
		if ((upg_mtdinfo[i].fd = upgrade_open(upg_mtdinfo[i].mtd_name)) < 0) {
			printk("<0>open mtd %s %d/%d fail\n", upg_mtdinfo[i].mtd_name, i, upg_mtd_cnt);
            if (i == 0) {
                set_fs(fs);
                kernel_reboot();
                return ;
            }
		} else
		{
			printk("<0>open mtd %s %d/%d success\n", upg_mtdinfo[i].mtd_name, i, upg_mtd_cnt);
		}
#ifdef USE_MTK_NAND_MTD_WRITE
        mtd_unlock_init(&upg_mtdinfo[i]);
#endif
	}

    printk("<0>""Open fd sucess\n");
#ifdef MARK_UPGRADE
    printk("<0>""Mark upgrade starting!\n");
	mark_upgrade_start();
#endif
    write_fw_to_mtd(fw_fd);
    printk("<0>""Upgrade fw sucess, prepare close fd\n");

	for (i=0; i<upg_mtd_cnt; i++) {
		sys_close(upg_mtdinfo[i].fd);
	}
    sys_close(fw_fd);
#ifdef AMPED_DUAL_IMAGE
    boot_flag ++;

    if (boot_from_imageA(boot_flag)) //if boot from image A
        boot_failA = 0;
    else
        boot_failB = 0;

    write_image_param_to_flash(boot_flag, boot_failA, boot_failB);

    printk("<0>""write boot flag: 0x%02x (boot image %s), boot_failA: 0x%02x, boot_failB: 0x%02x!\n", 
        boot_flag, boot_from_imageA(boot_flag)? "A": "B", boot_failA, boot_failB);
#endif
#ifdef MARK_UPGRADE
    printk("<0>""Mark upgrade finished!\n");
	mark_upgrade_finish();
#endif

    set_fs(fs);
    if (magic != ZIP3G_MAGIC_CONST)
        kernel_reboot();
    return;
}

/* sizeof task_struct.comm is 16 */
#define NOTIFY_PROCESS  "upgrade_driver"
static int proc_write_upgrade_file(struct file *file, const char __user * buffer,
        unsigned long count, void *data)
{
#if 0
    struct task_struct *p = NULL;
#endif
	char *saved_str, *str, *p = NULL;
	int i = 0;
    
    if (p)
    {
    		/* let gcc463 not complain. */
    	}

    copy_from_user(line, buffer, count);

	saved_str = str = kstrdup(line, GFP_USER);

	if (!saved_str)
		goto out;

	if (!(p = strsep(&str, " ")) || !*p)
		goto out;

	strcpy(file_name, p); /* FW image */

	/* Multiple MTDs to be upgrade */
	while ((p = strsep(&str, " ")) && *p) {

		strcpy(upg_mtdinfo[i].mtd_name, p);

		p = strsep(&str, " ");
		if(!p || !*p)
			goto out;
		upg_mtdinfo[i].offset = simple_strtoul(p, NULL, 10);

		p = strsep(&str, " ");
		if(!p || !*p)
			goto out;
		upg_mtdinfo[i].size = simple_strtoul(p, NULL, 10);

		upg_mtdinfo[i].fd = -1;

		i++;
	}

	upg_mtd_cnt = i;

    printk("<0>Upgrading %s, MTD count is %d\n", file_name, upg_mtd_cnt);
	for (i=0; i < upg_mtd_cnt; i++) {
		printk("<0>MTD %s offset 0x%08lX size 0x%08lX\n", upg_mtdinfo[i].mtd_name, 
				upg_mtdinfo[i].offset, upg_mtdinfo[i].size);
	}

    upgrade_fw(file_name);
	
#if 0
    /* notify user process */
    read_lock(&tasklist_lock);
    for_each_process(p){
        if (strstr(p->comm, NOTIFY_PROCESS)) {
            send_sig(SIGUSR1, p, 0);
        }
    }
    read_unlock(&tasklist_lock);
#endif

out:
	if(saved_str)
		kfree(saved_str);

    return count;
}
#else
void upgrade_fw(char *fw_name, char *mtd_name, int offset)
{
    unsigned char magic;
    int len;
    int fw_fd;
    int flash_fd;
#ifdef MARK_UPGRADE
    int bad_block_num = 0;
#endif
    mm_segment_t            fs = get_fs();

    set_fs(get_ds());
    printk("<0>""Prepare to open fd\n");

    if(((fw_fd = upgrade_open(fw_name)) < 0))
    {
        set_fs(fs);
        kernel_reboot();
        return ;
    }

    /* Read flag of PID */
    sys_lseek(fw_fd, 72, 0);
    len = sys_read(fw_fd,  &magic, sizeof(magic));

    flash_fd = upgrade_open(mtd_name);
    if (flash_fd < 0) {
        set_fs(fs);
        kernel_reboot();
        return ;
    }

    printk("<0>""Open fd sucess\n");
#ifdef MARK_UPGRADE
    printk("<0>""Mark upgrade starting!\n");
	bad_block_num = need_offset_num(0x03);
	mark_upgrade_start(bad_block_num);
	print_SCFLAG_str(bad_block_num);
#endif
    write_fw_to_mtd(fw_fd, flash_fd, offset);
    printk("<0>""Upgrade fw sucess, prepare close fd\n");
    sys_close(flash_fd);
    sys_close(fw_fd);
#ifdef MARK_UPGRADE
    printk("<0>""Mark upgrade finished!\n");
	mark_upgrade_finish(bad_block_num);
#endif
    set_fs(fs);
    if (magic != ZIP3G_MAGIC_CONST)
        kernel_reboot();
    return;
}

/* sizeof task_struct.comm is 16 */
#define NOTIFY_PROCESS  "upgrade_driver"
extern int get_kernel_rootfs_offset(void);
extern int get_kernel_rootfs_size(void);
extern int get_language_size(void);
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
static ssize_t proc_write_upgrade_file (struct file *file, const char __user *buffer, size_t count, loff_t *offset)
#else
static int proc_write_upgrade_file(struct file *file, const char __user * buffer,
        unsigned long count, void *data)
#endif
{
    struct task_struct *p;

    copy_from_user(line, buffer, count);
    if(sscanf(line, "%s %s %d %d %d\n", file_name, mtd_name, &offset , &kfs_size, &lang_size)!=5) {
        return count;
    }
    printk("\nget_kernel_rootfs_offset is 0x%08d\n", get_kernel_rootfs_offset());
    printk("\nget_kernel_rootfs_size is 0x%08d\n", get_kernel_rootfs_size());
    printk("\nget_language_size is 0x%08d\n", get_language_size());
    if(get_kernel_rootfs_offset() > offset)
        offset = get_kernel_rootfs_offset();
    if(get_kernel_rootfs_size() > kfs_size)
        kfs_size = get_kernel_rootfs_size();
    if(get_language_size() > lang_size)
        lang_size = get_language_size();
          
    printk("<0>Upgrading %s to %s offset 0x%08d(0x%08d  0x%08d)\n", file_name, mtd_name, offset, kfs_size, lang_size);

    upgrade_fw(file_name, mtd_name, offset);

    /* notify user process */
    read_lock(&tasklist_lock);
    for_each_process(p){
        if (strstr(p->comm, NOTIFY_PROCESS)) {
            send_sig(SIGUSR1, p, 0);
        }
    }
    read_unlock(&tasklist_lock);

    return count;
}
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
static const struct file_operations write_upgrade_file_fops = {
	.read		= NULL,
	.write		= proc_write_upgrade_file,
};
#endif
static int __init upgrade_init(void)
{
    struct proc_dir_entry *block_file;

#if LINUX_VERSION_CODE > KERNEL_VERSION(3,10,13)
    block_file = proc_create(UPGRADE_FILE, 0666, NULL, &write_upgrade_file_fops);
#else
    block_file = create_proc_entry(UPGRADE_FILE, 0666, NULL);
//    block_file->owner = THIS_MODULE;
    block_file->read_proc = NULL;
    block_file->write_proc = proc_write_upgrade_file;
#endif
    printk("<0>Upgrading module insert successfully.\n");

    return 0;
}

static void __exit upgrade_exit(void)
{
    remove_proc_entry(UPGRADE_FILE,  NULL);
}

module_init(upgrade_init);
module_exit(upgrade_exit);

MODULE_LICENSE("GPL");



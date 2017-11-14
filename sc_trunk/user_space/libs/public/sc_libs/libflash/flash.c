/*
 * Copyright (C) 2012  Sercomm Corporation
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <net/if.h>
#ifndef MTK_NAND_FLASH_API
#include <mtd/mtd-user.h>
#endif
#include <linux/jffs2.h>
#include "upgrade_flash.h"
#include "filenames.h"
#include "flash.h"
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <libscm_wl.h>
#ifdef MTK_NAND_FLASH_API
#include <sys/ioctl.h>
#endif

#include <stdarg.h>
#include <nvram.h>
#define _SC_DEBUG_ENABLE_
#include "sc_debug.h"
#include "nvram_var.h"
#include "sc_project_defines.h"

int ft_debug(char *format,...)
{
#if 0
    va_list args;
    FILE *fp;
    fp = fopen("/tmp/cgi_debug","a+");
    if(!fp)
        return -1;
    va_start(args,format);
    vfprintf(fp,format,args);
    va_end(args);
    fprintf(fp,"\n");
    fflush(fp);
    fclose(fp);
#endif
    return 0;
}

#ifdef BOOT_NAND_FLASH
#define TMP_LOADER      "/tmp/loader"
#define TMP_FT          "/tmp/ft"
#else
#define TMP_LOADER      LOADER
#endif

#ifdef SC_SKIP_BAD_BLOCK /*D7000 BCM needs */
#define BLOCK_SIZE (128*1024)

static int BAD_BLOCK;

static int is_a_bad_block(int fd, loff_t offset)
{
    int ret = 0;
    
    if((ret = ioctl(fd, MEMGETBADBLOCK, &offset)))
    { 
        return 1; 
    }

    return ret;  
}

int need_offset_num(loff_t offset)
{
    int fd=0;
    int bad_block_num = 0;
    
    fd = open(PCBASN_PATH, O_RDONLY);
    if(fd < 0)
    {
        return 0;
    }
    
    while(is_a_bad_block(fd, offset) == 1)
    {
        bad_block_num++;
        offset += BLOCK_SIZE;   
    }
    close(fd);
    SC_XCFPRINTF("find %d BAD block.\n", bad_block_num);
    return bad_block_num;   
    
}
#endif /* SC_SKIP_BAD_BLOCK */

/*
 * flash_get_str() -- Get item from FLASH protect area to @buf.
 *  length of buf is @buf_len
 *  Offset of item is @offset, length of this item is @len
 *  A '\0' is stored after the last character in the buffer.
 *
 * Return buf if success. NULL if error
 */
static char* flash_get_str( char* buf, int buf_len, int offset, int len)
{
#ifdef SC_SKIP_BAD_BLOCK /*D7000 BCM needs */
    int fd=0;    
    
    if(buf_len < len+1) {
        return NULL;
    }
    if(BAD_BLOCK == 0)
        BAD_BLOCK = need_offset_num(offset);
    
    fd = open("/dev/mtdblock7", O_RDONLY);
    lseek(fd, offset+(BAD_BLOCK*BLOCK_SIZE), SEEK_SET);
    read(fd, buf, len);
    close(fd);
    buf[len] = '\0';
    return buf;
#else 
    FILE *fp;

    if(buf_len < len+1) {
    	SC_CFPRINTF("NOT GET this item at 0x%x since buf not enough for later read (pid: %d)\n", offset, getpid());
        return NULL;
    }

    fp = fopen(PCBASN_PATH, "r");
    fseek(fp, offset, 0);
    fread(buf, len, 1, fp);
    fclose(fp);
    buf[len] = '\0';
    return buf;
#endif /* SC_SKIP_BAD_BLOCK */
}


#ifdef BCM_BSP
#define CRC32_INIT_VALUE 0xffffffff /* Initial CRC32 checksum value */
#define CRC_LEN 4

static unsigned long Crc32_table[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

static unsigned long getCrc32(char *pdata, unsigned long size, unsigned long crc)
{
    while (size-- > 0)
        crc = (crc >> 8) ^ Crc32_table[(crc ^ *pdata++) & 0xff];

    return crc;
}
int sc_adjust_nvram_checksum(FILE* loader_fp)
{
    unsigned long crc = CRC32_INIT_VALUE,pre_crc;
    PNVRAM_DATA pNvramData = malloc(sizeof(NVRAM_DATA));
    
    if(pNvramData == NULL)
        return -1;
        
    memset(pNvramData, 0,sizeof(NVRAM_DATA));
    fseek(loader_fp, NVRAM_DATA_OFFSET, 0);
    fread(pNvramData,sizeof(NVRAM_DATA),1,loader_fp);
    pre_crc = pNvramData->ulCheckSum ;
    
    pNvramData->ulCheckSum = 0;    
    crc = getCrc32((char *)pNvramData, sizeof(NVRAM_DATA), crc);
    SC_XCFPRINTF("%s: ulCheckSum: %lu -> %lu\n",__FUNCTION__,pre_crc,crc);
    pNvramData->ulCheckSum = crc;
    
    fseek(loader_fp, NVRAM_DATA_OFFSET, 0);
    fwrite(pNvramData,sizeof(NVRAM_DATA),1,loader_fp);
    
    free(pNvramData);
    return 0;
}
#endif

#if defined(SC_SKIP_BAD_BLOCK) || defined(MTK_NAND_FLASH_API) /*D7000 BCM & MTK MT7623 needs */
static int  loader_fd;
#else
static FILE* loader_fp;
#endif

/* close mtd block */
int sc_set_close()
{
    int ret = 0;
#if defined(SC_SKIP_BAD_BLOCK) || defined(MTK_NAND_FLASH_API) /*D7000 BCM & MTK MT7623 needs */
    ret = close(loader_fd);
#else 
    ret = fclose(loader_fp);
#endif
    return ret;
}

/* open mtd block */
int sc_set_open()
{
#ifdef SC_SKIP_BAD_BLOCK /*D7000 BCM needs */
    if(BAD_BLOCK == 0)
        BAD_BLOCK = need_offset_num(CONF_OFFSET);
    if ((loader_fd=open("/dev/mtdblock7", O_RDWR,0)) < 0) {
        return 0;
    } else {
        return 1;
    }
#elif defined(MTK_NAND_FLASH_API)
    if ((loader_fd=mtk_mtd_open_rw(PCBASN_PATH)) < 0) {
        return 0;
    } else {
        return 1;
    }
#else 
    if ((loader_fp=fopen(PCBASN_PATH, "w")) != NULL) {
        return 1;
    } else {
        return 0;
    }
#endif /* SC_SKIP_BAD_BLOCK */
}

/* AMT requires to filter all 00 and all ff for the assigned items. */
int is_str_wrong(char *strname, char *str_buf, int str_len)
{
	int is_wrong = 1;
	int i;
	
	for (i=0;i<str_len;i++)
	{
		if (((unsigned char)str_buf[i]!=0xff) && ((unsigned char)str_buf[i]!=0x00))
		{
			is_wrong = 0;
			break;
		}
	}
	
	if (is_wrong)
	{
		SC_CFPRINTF("ignore wrong item %s, all %s\n", strname?:"unknown", (str_buf[i]==0x00)?"0x00":"0xff");
	}
	
	return is_wrong;
}

/*
 * flash_set_str() -- Set value of @buf to FLASH protect area.
 *  length of buf is @buf_len
 *  Offset of item is @offset, length of this item is @len
 *
 * Return 0 if success. -1 if error
 */
static int flash_set_str(char* buf, int offset, int len, char *str_name)
{
	SC_CFPRINTF("set %s offset 0x%x len 0x%x\n", str_name, offset, len);
	if (is_str_wrong(str_name, buf, len))
	{
		return -1;
	}
	
#ifdef SC_SKIP_BAD_BLOCK /*D7000 BCM needs */
	lseek(loader_fd, offset+ (BAD_BLOCK*BLOCK_SIZE), SEEK_SET);
    	if (write(loader_fd,buf, len)) {
#elif defined(MTK_NAND_FLASH_API)
        if (mtk_nand_flash_write(loader_fd, buf, offset, len) > 0) {
#else
    	fseek(loader_fp, offset, 0);
    	if (fwrite(buf, len, 1, loader_fp)) {
#endif
        return 0;
    	}
    	return -1;
}

static int flash_set_str_force(char* buf, int offset, int len, char *str_name)
{
#ifdef SC_SKIP_BAD_BLOCK /*D7000 BCM needs */
    	lseek(loader_fd, offset+ (BAD_BLOCK*BLOCK_SIZE), SEEK_SET);
    	if (write(loader_fd,buf, len)) {
#elif defined(MTK_NAND_FLASH_API)
        if (mtk_nand_flash_write(loader_fd, buf, offset, len) > 0) {
#else
    	fseek(loader_fp, offset, 0);
    	if (fwrite(buf, len, 1, loader_fp)) {
#endif
        return 0;
   	}
    	return -1;
}

/*
 * sc_get_pin() -- get WPS PIN from FLASH to @buf.
 * A '\0' is stored after the last character in the buffer.
 * Return buf if success. NULL if error
 */
char* sc_get_pin( char* buf, int len)
{
	flash_get_str(buf, len, WPSPIN_OFFSET, WPSPIN_LEN);
    {
        char *ff = strchr(buf,0xff);
        if(ff)
            *ff = '\0';
    }
    if(strspn(buf,"0123456789")!=8)
    {
        strcpy(buf,"94229882"); //Default
    }

    return buf;
}

/*
 * sc_get_sn() -- get SN from FLASH to @buf.
 * A '\0' is stored after the last character in the buffer.
 * Return buf if success. NULL if error
 */
char* sc_get_sn( char* buf, int len)
{
    return sc_get_csn(buf,len);
}

/*
 * sc_get_csn() -- get CSN from FLASH to @buf.
 * A '\0' is stored after the last character in the buffer.
 * Return buf if success. NULL if error
 */
char* sc_get_csn( char* buf, int len)
{
    flash_get_str(buf, len, CSN_OFFSET, CSN_LEN);
    {
        char *ff = strchr(buf,0xff);
        if(ff)
            *ff = '\0';
    }
	/* temp solution to fix mantis bug 0013619 DUT serial number is wrong in UPnP
	 * the serial number has not been assigned, use "1234567890" to replace it
	 */
    if(!isascii(buf[0]) || strlen(buf) == 0 || strcmp(buf, "v1") == 0) {
        /*
         * PCBA without assign SN will return 0xFFFFFFFFF,
         * but this is invalid. Copy default value for safe
         */
        strcpy(buf, "1234567890");
    }

    return buf;
}

/*
 * sc_get_mac() -- get mac from FLASH to @buf.
 * A '\0' is stored after the last character in the buffer.
 * Return buf if success. NULL if error
 */
#ifdef PROJECT_D7000
static char* flash_get_mac_str( char* buf, int buf_len, int offset, int len)
{
    FILE *fp;

    if(buf_len < len+1) {
        return NULL;
    }

    fp = fopen(LOADER, "r");
    fseek(fp, offset, 0);
    fread(buf, len, 1, fp);
    fclose(fp);
    buf[len] = '\0';
    return buf;
}
#endif

char* sc_get_mac( char* buf, int len)
{
    unsigned char mac[7];
#ifdef PROJECT_D7000
    flash_get_mac_str((char *)mac, sizeof(mac), MAC_OFFSET, MAC_LEN);
#else
    flash_get_str((char *)mac, sizeof(mac), MAC_OFFSET, MAC_LEN);
#endif
    sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    /* if not assign, the value should be FF:FF:FF:FF:FF:FF, so use the default MAC */
    if (strcmp(buf, "FF:FF:FF:FF:FF:FF") == 0 || strcmp(buf, "00:00:00:00:00:00") == 0) {
        strcpy(buf, "00:c0:02:12:35:88");
    }
    return buf;
}

/*
 * sc_get_mac_if() -- get MAC address of @ifname
 */
int sc_get_mac_if( char *ifname, char mac[6])
{
    int sockd;
    struct ifreq ifr;

    sockd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockd == -1)
        return -1;
    strcpy(ifr.ifr_name, ifname);
    if(ioctl(sockd, SIOCGIFHWADDR, &ifr) < 0)
        return -1;
    memcpy(mac,ifr.ifr_hwaddr.sa_data,6);
    return 0;
}

int sc_get_updown_if( char *ifname)
{
    int sockd;
    struct ifreq ifr;
    sockd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockd == -1)
        return -1;
    strcpy(ifr.ifr_name, ifname);
    if(ioctl(sockd, SIOCGIFFLAGS, &ifr) < 0)
        return -1;
    if(ifr.ifr_flags & IFF_UP)
	   return 0;
    else  return -1;
}

/*
 * sc_get_pid() -- get PID information from FLASH to @buf.
 *                 length of @buf is @len
 */
int sc_get_pid(char* buf, int len)
{
    FILE *fp;
    char ver[32];
    int major, minor, release;
    sercomm_pid_t *pid;

    if(len < sizeof(sercomm_pid_t)) {
        return -1;
    }

    memset(buf , 0 , len);
    /* 1. Get PID from bootloader block */
    if((fp=fopen(SC_PID, "r"))==NULL) {
        return -1;
    }

    fseek(fp, PID_OFFSET, 0);
    fread(buf, PID_LEN, 1, fp);
    fclose(fp);

    /* 2. Update FW Version */
    if((fp=fopen(ANNEX_VERSION, "r")) == NULL) {
        /* Can not open version, just use PID from bootloader */
        return 0;
    }
    fgets(ver, sizeof(ver), fp);
    fclose(fp);
    sscanf(ver, "A%*[^.].%x.%x.%x%*s", &major, &minor, &release);

    pid = (sercomm_pid_t *)buf;
    pid->fw_ver[0]=((major & 0x0F) << 4) + (minor &0x0F);
    pid->fw_ver[1]=release;
    return 0;
}

/*
 * The value in this table should be the nvram value
 */
struct region_related_s region_relateds[] = {
#ifdef ADSL
  /* region     wiz_language    wifi_region       fw_time_zone */
    {0x0001,    "English",      "United States",    "-8",    "NA",           "0","8","35","pppoe","1492"},      /* NA */
    {0x0002,    "English",      "Europe",           "GMTb",  "WW",        "1","0","38","pppoa","1458"},    /* WW */
    {0x0003,    "German",       "Europe",           "+1a",   "GR",          "0","1","32","pppoe","1492"},     /* GR */
    {0x0004,    "Chinese",      "Asia",             "+8a",   "PR",             "1","0","38","pppoa","1458"},     /* PR */
    {0x0005,    "Russian",      "Europe",           "+3a",   "RU",           "1","0","38","pppoa","1458"},     /* RU */
    {0x0006,    "English",      "South America",    "-3b",   "BZ",          "1","0","38","pppoa","1458"},     /* BZ */
    {0x0007,    "English",      "Europe",           "+5:30b", "IN",            "1","0","38","pppoa","1458"   },  /* IN */
    {0x0008,    "Korean",       "Korea",            "+9a",   "KO",            "1","0","38","pppoa","1458"},     /* KO */
    {0x0009,    "Japanese",     "Japan",            "+9a",   "JP",             "1","0","38","pppoa","1458"},     /* JP */
    {0x000a,    "English",     "Australia",         "+10a",   "AU",            "1","0","38","pppoa","1458"},     /* AU */
    {0,         NULL,           NULL,               NULL,   NULL,   NULL,   NULL,   NULL,   NULL}
#else
  /* region     wiz_language    wifi_region       fw_time_zone       name */
    {0x00000001,    "English",      "United States",    "-8",         "NA",           "0","0","0","dhcpc","1500"},
    {0x00000002,    "English",      "Europe",           "GMTb",       "WW",           "0","0","0","dhcpc","1500"},
    {0x00000003,    "German",       "Europe",           "+1a",        "GR",           "0","0","0","dhcpc","1500"}, 
    {0x00000004,    "Chinese",      "China",             "+8a",        "PR",           "0","0","0","dhcpc","1500"},
    {0x00000005,    "Russian",      "Rusia",           "+4a",        "RU",           "0","0","0","dhcpc","1500"},
    {0x00000006,    "English",      "South America",    "-3b",        "BZ",           "0","0","0","dhcpc","1500"},
    {0x00000007,    "English",      "India",           "+5:30b",     "IN",           "0","0","0","dhcpc","1500"},
    {0x00000008,    "Korean",       "Korea",            "+9a",        "KO",           "0","0","0","dhcpc","1500"},
    {0x00000009,    "Japanese",     "Japan",            "+9a",        "JP",           "0","0","0","dhcpc","1500"},
    {0x0000000a,    "English",      "Australia",        "+10b",       "AU",           "0","0","0","dhcpc","1500"},
    {0x0000000b,    "English",      "Canada",        "-5b",       "CA",           "0","0","0","dhcpc","1500"},
    {0x0000000c,    "English",      "United States",    "-8",     "US",           "0","0","0","dhcpc","1500"},
    {0,         NULL,           NULL,               NULL,            NULL}
#endif
};

#if 0
/* Here it will use  upper character, when auto_up.c use it will change to lower char */
static struct pid_related_s pid_relateds[] = {
	/* pid      		name */
	{0x00000004, 	BOARD_ID_DEFAULT},
	{0x00000003, 	"JNDR3710"},
	{0x00000001, 	"JNR3210"},
	{0x00000002,    "JNR3000"},
	{0,				 	  NULL},
};
#endif

int region_is_NA()
{
        struct region_related_s *region_p = sc_get_region();
        if (region_p && region_p->region == 1)
                return 1;
        else
                return 0;
}

/*
 * Get DUT Region related struct point
 * Return NULL if error
 */
struct region_related_s *sc_get_region(void) {
    char buf[3];    /* Region is 2 bytes, but flash_get_str() needs another byte at the tail of buf */
    unsigned int region = 0;;
    int i;
    
    memset(buf, 0, sizeof(buf));
#if 0
    flash_get_str(buf, sizeof(buf), DOMAIN_OFFSET, DOMAIN_LEN);

	SC_CFPRINTF("Original Region Data: %02x, %02x, %02x, %02x\n",
			buf[0], buf[1], buf[2], buf[3]);
	/*
	 * For DOMAIN, both FT_TOOL and NMRP can Assign it.
	 * eg:
	 *  Flash default status:  0xff 0xff 0xff 0xff
	 *  After ft assign    :  0x00 0x00 0x00 0x04
	 *  After NMRP assign       : 0xff 0xff 0x00 0x04
	 * And in fact, 2 Bytes are enough for DOMAIN.
	 * So to make the code simple, and both workable for FT& NMRP,
	 * hardcode the first and second Byte to 0x00*/

	buf[0] = 0x00;
	buf[1] = 0x00;
#else
    flash_get_str(buf, sizeof(buf), REGION_OFFSET, REGION_LEN);
    
	SC_CFPRINTF("Original Region Data: %02x, %02x\n",
			buf[0], buf[1]);    
#endif

    memcpy(&region, buf, sizeof(region));
    region = ntohs(region);
    /* Check if region is valid or not */
    if(region == 0) {
        /* Region invalid */
        return NULL;
    }

    for(i=0; region_relateds[i].region; i++) {
        if(region == region_relateds[i].region) {
            /* find it */
            return &region_relateds[i];
        }
    }
    return NULL;
}

/* get domain from flash to buf
 * return 0 if success; return -1 if fail
 */
char* sc_get_domain(char* buf, int len)
{
    return flash_get_str(buf, len, DOMAIN_OFFSET, DOMAIN_LEN);
   // flash_get_str(buf, len, DOMAIN_OFFSET, DOMAIN_LEN);
   // ft_debug("buf is %x,%x,%x,%x", *buf, *(buf+1),*(buf+2),*(buf+3));
   // return buf;
}

/* get Language ID from flash to buf
 * return 0 if success; return -1 if fail
 */
char* sc_get_country(char* buf, int len)
{
    return flash_get_str(buf, len, LANG_ID_OFFSET, LANG_ID_LEN);
}

/* get PCBA SN from flash to buf
 * return 0 if success; return -1 if fail
 */
char* sc_get_pcba_sn(char* buf, int len)
{
//#ifdef BOOT_NAND_FLASH
#if 1
    return flash_get_str(buf, len, PCBA_SN_OFFSET, PCBA_SN_LEN);
#else
    /*PCBA SN has seperate MTD as MTCode asked.*/
#define BLOCKSIZE (16*1024)	// block size
    int fd;
    loff_t offset = 0;

    if(len < PCBA_SN_LEN + 1) {
        return NULL;
    }
    
    fd = open(PCBASN_PATH, O_RDONLY);
    while(ioctl(fd, MEMGETBADBLOCK, &offset) > 0)
    {
        offset += BLOCKSIZE;
        if(offset >= PCBASN_MTD_SIZE)
        {
            SC_XCFPRINTF("%s: get pcba sn failed: all blocks bad\n",__FUNCTION__);
            close(fd);
            return "";
        }
        lseek(fd, offset, 0);
    }
    
    read(fd, buf, PCBA_SN_LEN);
    close(fd);
    buf[PCBA_SN_LEN] = '\0';
    return buf;
#endif
}

#ifdef MTK_BSP
int hextoi( char ch )
{
        if( ch >= '0' && ch <= '9')
                return (ch - '0');

        if( ch >= 'A' && ch <= 'F')
                return (ch - 'A' +10);

        if( ch >= 'a' && ch <= 'f')
                return (ch - 'a' +10);

        return 0;
}
char itohex( unsigned char value)
{
        if (value < 10)
                return  (value += '0' );
      else if( value < 16)
                return (value -10 + 'a' );

        return '0';
}
/*
// mac format:   "01:02:03:04:05:06"
*/
int mac_addr_inc( char * mac , int inc )
{
	unsigned long mac_nic =0;
	mac_nic += (hextoi( mac[9] ) <<  20);
	mac_nic += (hextoi( mac[10] ) <<  16);
	mac_nic += (hextoi( mac[12] ) << 12);
	mac_nic += (hextoi( mac[13] ) <<  8);
	mac_nic += (hextoi( mac[15] ) << 4);
	mac_nic += (hextoi( mac[16] ) );

	mac_nic += inc;

	mac[9] = itohex( (mac_nic & 0xf00000) >> 20 ) ;
	mac[10] =itohex( (mac_nic & 0x0f0000) >> 16 ) ;
	mac[12] =itohex( (mac_nic & 0x00f000) >> 12 ) ;
	mac[13] =itohex( (mac_nic & 0x000f00) >> 8 ) ;
	mac[15] =itohex( (mac_nic & 0x0000f0) >> 4 ) ;
	mac[16] =itohex( (mac_nic & 0x00000f)   ) ;
	return 1;
}

int set_ralink_mac(char *buf)
{
	if (is_str_wrong("mac", buf, MAC_LEN))
	{
		return -1;
	}
	char new_mac[32];
	snprintf((char*)new_mac, sizeof(new_mac), "%02X:%02X:%02X:%02X:%02X:%02X", \
			(unsigned char)buf[0], (unsigned char)buf[1], (unsigned char)buf[2], (unsigned char)buf[3], (unsigned char)buf[4], (unsigned char)buf[5]);
	SC_CFPRINTF("CHIP: %02X%02X - current MAC: %s\n", (unsigned char)buf[1], (unsigned char)buf[0], new_mac);

	set_ralink_wifi_mac("ra0", new_mac);
#ifdef SC_WIFI_MT7615
    mac_addr_inc(new_mac, 1);
#else 
    mac_addr_inc(new_mac, 4); //Ralink WiFi Mac must be multiple of the number of MBSSID.
#endif
	set_ralink_wifi_mac("rai0", new_mac);
	return 1;
}
#endif	
/* assign MAC to device
 * return 1 if success; return -1 if fail
 */
int sc_set_mac(char *buf)
{
#ifdef BOOT_NAND_FLASH
    FILE* mac_fp;
    int ret = 0;
    /* open MTD*/
    system("/bin/cp "LOADER" "TMP_LOADER);

    if ((mac_fp=fopen(TMP_LOADER, "r+")) == NULL) {
        return -1;
    }

    /* write it*/
    fseek(mac_fp, MAC_OFFSET, 0);
    fwrite(buf, MAC_LEN, 1, mac_fp);

#ifdef BCM_BSP
    sc_adjust_nvram_checksum(mac_fp);
#endif
    ret = fclose(mac_fp);

    if(ret == 0)
        system("/bin/flash_eraseall -j -q "LOADER"; /bin/cp "TMP_LOADER" "LOADER);
    return 0;
#else   
#ifdef MTK_BSP
	set_ralink_mac(buf);
#endif
    return flash_set_str(buf, MAC_OFFSET, MAC_LEN, "mac");
#endif
}

/* assign CSN to device
 * return 1 if success; return -1 if fail
 */
int sc_set_csn(char *buf)
{
    return flash_set_str(buf, CSN_OFFSET, CSN_LEN, "csn");
}

/* assign PIN to device
 * return 1 if success; return -1 if fail
 */
int sc_set_pin(char *buf)
{
    return flash_set_str(buf, WPSPIN_OFFSET, WPSPIN_LEN, "wpspin");
}

/* assign domain(region) to device
 * return 1 if success; return -1 if fail
 */
int sc_set_domain(char *buf)
{
    //SC_CFPRINTF("set domain buf is %x,%x,%x,%x\n", *buf, *(buf+1),*(buf+2),*(buf+3));
    //ft_debug("set domain buf is %x,%x,%x,%x", *buf, *(buf+1),*(buf+2),*(buf+3));
    
    /*Why move 2 bytes: move the svn modifaction -r 205 to here, the -r205 will cause the value
     * after DOMAIN been messed up*/
    return flash_set_str(buf+2, DOMAIN_OFFSET, DOMAIN_LEN, "domain");
}

/* assign language ID to device
 * return 1 if success; return -1 if fail
 */
int sc_set_country(char *buf)
{
    return flash_set_str(buf, LANG_ID_OFFSET, LANG_ID_LEN, "langid");
}

/* assign PCBA SN to device
 * return 1 if success; return -1 if fail
 */
int sc_set_pcba_sn(char *buf)
{
//#ifdef BOOT_NAND_FLASH
    return flash_set_str(buf, PCBA_SN_OFFSET, PCBA_SN_LEN, "pcba");
/*
#else
    SC_XCFPRINTF("%s: Not support pcba sn assign\n",__FUNCTION__);
    return 0;
#endif
*/
}

char* sc_get_def_ssid( char* buf, int len)
{
    flash_get_str(buf, len, SSID_OFFSET, SSID_LEN);
    {
        char *ff = strchr(buf,0xff);
        if(ff)
            *ff = '\0';
    }
    if(!isascii(buf[0]) || strlen(buf) == 0) {
        /*
         * without assign will return 0xFFFFFFFFF,
         * but this is invalid. Copy default value for safe
         */
#if defined(PROJECT_RE2600M) || defined(PROJECT_RTA2600R2)
        strcpy(buf, "AMPED WIRELESS");
#else
        strcpy(buf, "NETGEAR");
#endif
    }
    return buf;

}

char *sc_get_ssid(char *buf, int len)
{
    flash_get_str(buf, len, SSID_OFFSET, SSID_LEN);
    {
        char *ff = strchr(buf,0xff);
        if(ff)
            *ff = '\0';
    }
    if(!isascii(buf[0]) || strlen(buf) == 0) {
        /*
         * without assign will return 0xFFFFFFFFF,
         * but this is invalid. Copy default value for safe
         */
#if defined(PROJECT_RE2600M) || defined(PROJECT_RTA2600R2)
        strcpy(buf, "AMPED WIRELESS");
#else
        strcpy(buf, "NETGEAR");
#endif
    }
    return buf;

}

/* assign SSID to device
 * return 1 if success; return -1 if fail
 */
int sc_set_ssid(char *buf)
{
    return flash_set_str(buf, SSID_OFFSET, SSID_LEN, "ssid");
}

char* sc_get_def_wpakey( char* buf, int len)
{
    flash_get_str(buf, len, WPAKEY_OFFSET, WPAKEY_LEN);
    {
        char *ff = strchr(buf,0xff);
        if(ff)
            *ff = '\0';
    }
    if(!isascii(buf[0]) || strlen(buf) == 0) {
        /*
         * without assign will return 0xFFFFFFFFF,
         * but this is invalid. Copy default value for safe
         */
        strcpy(buf, "1234567890");
    }
    return buf;
}

char *sc_get_phrase(char *buf, int len)
{
    flash_get_str(buf, len, WPAKEY_OFFSET, WPAKEY_LEN);
    {
        char *ff = strchr(buf,0xff);
        if(ff)
            *ff = '\0';
    }
    if(!isascii(buf[0]) || strlen(buf) == 0) {
        /*
         * without assign will return 0xFFFFFFFFF,
         * but this is invalid. Copy default value for safe
         */
        strcpy(buf, "1234567890");
    }
    return buf;
}

/* assign WPAKEY to device
 * return 1 if success; return -1 if fail
 */
int sc_set_wpakey(char *buf)
{
    return flash_set_str(buf, WPAKEY_OFFSET, WPAKEY_LEN, "wpakey");
}

int sc_set_phrase(char *buf)
{
    return flash_set_str(buf, WPAKEY_OFFSET, WPAKEY_LEN, "phrase");
}

#if 0
char *sc_get_product_id(char *buf, int len)
{
#if 1	
	
	/* not used: scfgmgr will use model id, rc.c will use model id too. */
	return buf;
			
#else
#ifdef DYNAMIC_MODEL_NAME
/* this is just for JR6150 way, will remove later. */
//	struct region_related_s *region = sc_get_region();

//	if (region && !strcmp(region->name, "NA"))
//	{
//		strcpy(buf, BOARD_ID_NA);
//	} 
//	else {
//		strcpy(buf, BOARD_ID_DEFAULT);
//	}
#else
	unsigned int pid;
	int i;

    flash_get_str(buf, len, PRODUCT_ID_OFFSET, PRODUCT_ID_LEN);

    memcpy(&pid, buf, sizeof(pid));
    pid = ntohs(pid);

    for(i=0; pid_relateds[i].pid; i++) {
        if(pid == pid_relateds[i].pid) {
            /* find it */
            strcpy(buf, pid_relateds[i].name);
            return buf;
        }
    }
    /* default we use R6050 */
    strcpy(buf, pid_relateds[0].name);
#endif
#endif
    return buf;
}

int sc_set_product_id(char *buf)
{
    return flash_set_str(buf, PRODUCT_ID_OFFSET, PRODUCT_ID_LEN, "product_id");
}
#endif 

int sc_set_model_id(char *buf)
{
	SC_CFPRINTF("product id: 0x%x 0x%x \n", buf[0], buf[1]);
    //return flash_set_str(buf, PRODUCT_ID_OFFSET, PRODUCT_ID_LEN, "product_id");
    return flash_set_str(buf, MODULE_ID_OFFSET, MODULE_ID_LEN, "model_id");
}

char *sc_get_model_id(char *buf, int len)
{	
	flash_get_str(buf, len, MODULE_ID_OFFSET, MODULE_ID_LEN);
    ft_debug("buf is %x,%x\n", *buf, *(buf+1));
    return buf;
}	
	
#ifdef PROJECT_D7000
/* assign REGION to device
 * return 1 if success; return -1 if fail
 */
int sc_set_region(char *buf)
{
    return flash_set_str(buf, REGION_OFFSET, REGION_LEN, "region");
}	
#endif

char *sc_get_flash_region(char *buf)
{
        struct region_related_s *region_p = sc_get_region();
        if(region_p)
        {
                switch(region_p->region)
                {
                    case 1:strcpy(buf, "NA"); break;
                    case 2:strcpy(buf, "WW"); break;
                    case 3:strcpy(buf, "GR"); break;
                    case 4:strcpy(buf, "PR"); break;
                    case 5:strcpy(buf, "RU"); break;
                    case 6:strcpy(buf, "BZ"); break;
                    case 7:strcpy(buf, "IN"); break;
                    case 8:strcpy(buf, "KO"); break;
                    case 9:strcpy(buf, "JP"); break;
                    case 10:strcpy(buf, "AU"); break;
                    default:strcpy(buf, "UNKNOWN"); break;
                }
        }
	return buf;
}	
	
#if 0
static int mt_flash_set_str(char* buf, int offset, int len)
{
    FILE *fp;
/* the mtd should due to your flash mapping */
    fp = fopen("/dev/mtdblock5", "w");
    if(fp != NULL)
    {
        fseek(fp, offset, 0);
        if (fwrite(buf, len, 1, fp)) {
            return 0;
        }
    }
    return -1;
}

int sc_set_mt_fw_mode(char *buf)
{
	return mt_flash_set_str(buf, SWITCH_MODE_OFFSET, SWITCH_MODE_LEN);
}
#endif

#ifdef USE_CHK_CAL
int sc_get_cal(char *buf, int len)
{
#if defined(PROJECT_R6220)
//#warning "NEW CALIBRATION CHECK!!!"
	/*
	 * According to HW Ryan's comments on 2014/7/2
	 5G					

	 CH36~48	CH52~64	CH100~114	CH116~144	CH149~CH165
	 Tx0	0x69	0x6E	0x73	0x78	0x7D
	 Tx1	0x87	0x8C	0x91	0x96	0x9B

	 Frequency offset 0x9E					

	 2.4G			

	 Tx0	0x58
	 Tx1	0x5E
	 */
	unsigned char eeprom_2g[512];
    unsigned char eeprom_5g[512];
	int cal_offset_2g[] = {0x58, 0x5E, 0x0};
	int cal_offset_5g[] = {0x69, 0x6E, 0x73, 0x78, 0x7D, \
		0x87, 0x8C, 0x91, 0x96, 0x9B, 0x0};
	int i = 0;
	int idx;

    FILE *fp = fopen(WIFI_DATA, "r");
	if(!fp)
	{
		SC_CFPRINTF("File open failed!\n");
		return 0; //NG
	}
	/* get 2.4g power */
    fread(eeprom_2g, 512, 1, fp);

	/* get 5g power */
    fseek(fp, 0x8000, 0);
    fread(eeprom_5g, 512, 1, fp);

    fclose(fp);

	for(i=0; cal_offset_2g[i]!=0; i++) {
		idx = cal_offset_2g[i];
		if(eeprom_2g[idx] <= 0x0A || /* default EEPROM have set 0x0A */
				eeprom_2g[idx] == 0xff) {
			SC_CFPRINTF("2.4G EEPROM offset 0x%02X=0x%02X, invalid!\n", idx, eeprom_2g[idx]);

			return 0;
		}
	}

	for(i=0; cal_offset_5g[i]!=0; i++) {
		idx = cal_offset_5g[i];
		if(eeprom_5g[idx] <= 0x0A || /* default EEPROM have set 0x0A */
				eeprom_5g[idx] == 0xff) {
			SC_CFPRINTF("5G EEPROM offset 0x%02X=0x%02X, invalid!\n", idx, eeprom_5g[idx]);

			return 0;
		}
	}

	SC_CFPRINTF("Calibration OK!\n");

	return 1;

#elif defined(PROJECT_DEFINES_RTA2600R2)
    /* 
       HW Davy 2016/1/6:
        check data[0xf4] == 0xa8 or not?
            true: not calibrated.
            false: has calibrated.
     */
	unsigned char eeprom_2g[512];
    unsigned char eeprom_5g[512];

    FILE *fp = fopen(WIFI_DATA, "r");
	if(!fp)
	{
		SC_CFPRINTF("File open failed!\n");
		return 0; //NG
	}
	/* get 2.4g power */
    fread(eeprom_2g, 512, 1, fp);

	/* get 5g power */
    fseek(fp, 0x8000, 0);
    fread(eeprom_5g, 512, 1, fp);

    fclose(fp);

    if (eeprom_2g[0] == 0xff || eeprom_5g[0] == 0xff)
    {
        SC_CFPRINTF("Factory mtdblock does not have correct data!\n");
        return 0;
    }

    SC_CFPRINTF("2.4G EEPROM offset 0x%02X=0x%02X!\n", 0xf4, eeprom_2g[0xf4]);
    SC_CFPRINTF("5.0G EEPROM offset 0x%02X=0x%02X!\n", 0xf4, eeprom_2g[0xf4]);

    if (eeprom_2g[0xf4] == 0xa8 || eeprom_5g[0xf4] == 0xa8)
    {
        SC_CFPRINTF("Calibration NG, maybe has not calibrated yet!\n");
        return 0;
    }

	SC_CFPRINTF("Calibration OK!\n");

    return 1;

#elif defined(PROJECT_DEFINES_JR6150)
#define POWER_OFFSET_MT7620A	0x52
#define POWER_SIZE_MT7620A		(0x6E - 0x52)
#define POWER_OFFSET_MT7610E	0x78
#define POWER_SIZE_MT7610E		(0x9A - 0x78)

	unsigned char wifi_pwr[64], wifi_pwr_5g[64], buff[256];
	int i, ln=0;
    FILE *fp = fopen(WIFI_DATA, "r");

	if(!fp)
		return 0; //NG

	/* get 2.4g power */
    fseek(fp, 0x0+POWER_OFFSET_MT7620A, 0);
    fread(wifi_pwr, POWER_SIZE_MT7620A, 1, fp);

	/* get 5g power */
    fseek(fp, 0x8000+POWER_OFFSET_MT7610E, 0);
    fread(wifi_pwr_5g, POWER_SIZE_MT7610E, 1, fp);

    fclose(fp);

	for(i=0, ln=0; i<POWER_SIZE_MT7620A; i++){
		if(i % 16 == 0)
			ln += sprintf(((char*)buff)+ln, "\n");
		ln += sprintf(((char*)buff)+ln, "%02X ", wifi_pwr[i]);
	}
	buff[ln+1] = '\0';
	SC_CFPRINTF("2.4G power: %s\n", buff);

	for(i=0, ln=0; i<POWER_SIZE_MT7610E; i++){
		if(i % 16 == 0)
			ln += sprintf(((char*)buff)+ln, "\n");
		ln += sprintf(((char*)buff)+ln, "%02X ", wifi_pwr_5g[i]);
	}
	buff[ln+1] = '\0';
	SC_CFPRINTF("5G power: %s\n", buff);

	/* if all the power is different from default, we think its already calibrated */
	for(i=0; i<POWER_SIZE_MT7620A; i++){
		if(wifi_pwr[i] == 0x0 || wifi_pwr[i] == 0xff)
			return 0;
	}
	for(i=0; i<POWER_SIZE_MT7610E; i++){
		if(wifi_pwr_5g[i] == 0x0 || wifi_pwr_5g[i] == 0xff)
			return 0;
	}

	return 1;
#else
    flash_get_str(buf, len, CAL_OFFSET, CAL_LEN);

	char * ff = strchr(buf,0xff);
	char * zero = strchr(buf,0x00);
	if(ff) {
		*ff = '\0';
    	if(!isascii(buf[0]) || strlen(buf) == 0){
        	return 0;
        }
        else{
        	return 1;
        }
    }
    else if(zero) {
		*zero = '\0';
    	if(!isascii(buf[0]) || strlen(buf) == 0){
        	return 0;
        }
        else{
        	return 1;
        }
    }
    else{
    	return 1;
    }
#endif
}
#endif

int set_wireless_time_zone(char * language,char *wireless_apply,char *schedule_apply)
{
/*Follow spec Rev11,When region is WW,when user haven't clicked Apply button on the Basic
Wireless Settings page,if they change the browser language(either they selecte a different
language from the GUI or they use a different computer/browser with a different language 
active),the wireless region will also be changed accordingly(the same as Time Zone setting)

if user have clicked Apply button ,the wireless region will not be changed(the same as Time 
Zone setting)
*/
	int i;
	for(i=0; region_relateds[i].region; i++) {
        if(strcmp(language,region_relateds[i].wiz_language)==0) {


		if(*wireless_apply == '0')
		{
			 nvram_set(WIFI_REGION,  region_relateds[i].wifi_region);
        		 nvram_set("wifi_region1",  region_relateds[i].wifi_region);
			 nvram_commit();
			 system("/usr/sbin/rc wlan restart >/dev/null 2>&1");
			 system("/usr/sbin/rm /tmp/wifi_changed");
		}
		if(*schedule_apply == '0')
		{
			 nvram_set(FW_TIME_ZONE, region_relateds[i].fw_time_zone);

        		char tmp[256]="";
	        	char value[64]="";
			char *pt = value;

			strcpy(value, region_relateds[i].fw_time_zone);
			if(*pt=='+'){
				*pt='-';
				while(*(++pt)<=':' && *pt!='\0');
				*pt='\0';

			}else if(*pt=='-'){
				*pt='+';
				while(*(++pt)<=':' && *pt!='\0');
				*pt='\0';
			}
			if(strcmp(value,"GMTa")==0 || strcmp(value,"GMTb")==0)
					strcpy(tmp,"GMT+0");
			else
					sprintf(tmp,"GMT%s",value);
		    nvram_set("time_zone", tmp);
		    nvram_commit();
			system("/usr/sbin/rc timezone start");
			system("/usr/sbin/rc ntp restart >/dev/null 2>&1");
			
			system("/bin/killall -SIGPROF syslogd");
			
		}
            		
			break;
          }
		
        } 
	return 0;
}
int sc_set_remotescfgmgrmagic(char *buf)
{
	SC_CFPRINTF("buf:%02x, %02x, %02x, %02x\n", buf[0], buf[1], buf[2], buf[3]);
	return flash_set_str((char *)buf, REMOTESCFGMGRMAGIC_OFFSET, REMOTESCFGMGRMAGIC_LEN, "set REMOTESCFGMGR_FLAG");
}

int sc_clear_remotescfgmgmagic()
{
	unsigned char remotescfgmgrmagic_off[REMOTESCFGMGRMAGIC_LEN] = {0xff, 0xff, 0xff, 0xff};
	return flash_set_str_force((char *)remotescfgmgrmagic_off, REMOTESCFGMGRMAGIC_OFFSET, REMOTESCFGMGRMAGIC_LEN, "clear REMOTESCFGMGR_FLAG");
}

int sc_check_remotescfgmgrmagic()
{
	int flag = 0;
	unsigned char buf[5];
	unsigned char remotescfgmgrmagic_on[REMOTESCFGMGRMAGIC_LEN] = {0x31, 0x3e, 0xc5, 0x8d};
	
	if (flash_get_str((char *)buf, sizeof(buf), REMOTESCFGMGRMAGIC_OFFSET, REMOTESCFGMGRMAGIC_LEN))
	{
		SC_CFPRINTF("buf:%02x, %02x, %02x, %02x\n", buf[0], buf[1], buf[2], buf[3]);
		SC_CFPRINTF("magic:%02x, %02x, %02x, %02x\n", remotescfgmgrmagic_on[0], remotescfgmgrmagic_on[1], 
														remotescfgmgrmagic_on[2], remotescfgmgrmagic_on[3]);
		if (memcmp(buf, remotescfgmgrmagic_on, REMOTESCFGMGRMAGIC_LEN)==0)	
		{
			flag = 1;
		}
	}
	return flag;
}

#ifdef PROJECT_D7000
/*MT code */
char* sc_get_burning_time(char* buf, int len)
{
    return flash_get_str(buf, len, BURNNING_OFFSET, BURNNING_LEN);
}
int sc_set_burning_time(char *buf)
{
    return flash_set_str(buf, BURNNING_OFFSET, BURNNING_LEN, "burning_time");
}

char* sc_get_downloadmode(char* buf, int len)
{
    return flash_get_str(buf, len, FT_DOWNLOAD_OFFSET, FT_DOWNLOAD_LEN);
}
int sc_set_downloadmode(char *buf)
{
    return flash_set_str(buf, FT_DOWNLOAD_OFFSET, FT_DOWNLOAD_LEN, "downloadmode");
}
#endif

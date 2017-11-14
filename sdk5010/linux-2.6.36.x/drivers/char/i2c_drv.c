/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright, Ralink Technology, Inc.
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED   ``AS  IS'' AND   ANY  EXPRESS OR IMPLIED
 *  WARRANTIES,   INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO  EVENT  SHALL   THE AUTHOR  BE    LIABLE FOR ANY   DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED   TO, PROCUREMENT OF  SUBSTITUTE GOODS  OR SERVICES; LOSS OF
 *  USE, DATA,  OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 ***************************************************************************
 *
 */

#include <linux/init.h>
#include <linux/version.h>
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>   
#include <linux/fs.h>       
#include <linux/errno.h>
#include <linux/slab.h> 
#include <linux/types.h>    
#include <linux/proc_fs.h>
#include <linux/fcntl.h>    
#include <linux/delay.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)
#include <asm/system.h>
#endif
#include <linux/wireless.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>

#include "i2c_drv.h"
#include "ralink_gpio.h"

#ifdef  CONFIG_DEVFS_FS
#include <linux/devfs_fs_kernel.h>
#endif

//#define _I2C_MCU_DEBUG_

#ifdef  CONFIG_DEVFS_FS
static devfs_handle_t devfs_handle;
#endif


#if defined(CONFIG_MTK_NFC_SUPPORT)
  #if defined(CONFIG_RALINK_RT3883)
    #define MT6605_GPIO_IND         9  // MT6605_IRQ
    #define MT6605_GPIO_VEN         13 // MT6605_VEN
    #define MT6605_GPIO_RESET       11 // MT6605_RESET
  #elif defined(CONFIG_RALINK_MT7621)
    #define MT6605_GPIO_IND         10 // MT6605_IRQ
    #define MT6605_GPIO_VEN         9  // MT6605_VEN
    #define MT6605_GPIO_RESET       25  // MT6605_RESET
  #else
    #error "chip is not support"
  #endif
#endif

#if defined(CONFIG_MTK_NFC_MT6605_SIM)
extern void mt6605_sim(void);
#endif

int i2cdrv_major =  218;
unsigned long i2cdrv_addr = ATMEL_ADDR;
//unsigned long i2cdrv_addr = 0x70;
unsigned long address_bytes= 1;

//D_Jeremy add it for NA502S expander
unsigned char expander1_dir = 0x00;                     //all ouput 
unsigned char expander1_data = 0xFF;                    //0000 1101B

unsigned char expander2_dir = 0x00;                     //0000 0000B all ouput
unsigned char expander2_data = 0xFF;                    //1111 1111  all led on

int i2c_sc_debug = 0;

//spinlock_t spinlock_i2c = SPIN_LOCK_UNLOCKED;
static DEFINE_SPINLOCK(spinlock_i2c);
unsigned long flags_i2c = 0;

unsigned long switch_address_bytes(unsigned long addr_bytes)
{
	address_bytes = addr_bytes;
	//printk("I2C controller address bytes is %x\n", addr_bytes);
	return address_bytes;
}

/*----------------------------------------------------------------------*/
/*   Function                                                           */
/*           	i2c_master_init                                         */
/*    INPUTS: None                                                      */
/*   RETURNS: None                                                      */
/*   OUTPUTS: None                                                      */
/*   NOTE(S): Initialize I2C block to desired state                     */
/*                                                                      */
/*----------------------------------------------------------------------*/
void i2c_master_init(void)
{
	//spin_lock_irq(&spinlock_i2c);
	//spin_lock_irqsave(&spinlock_i2c, flags_i2c);
	u32 i;
#if defined (CONFIG_RALINK_MT7621)
	RT2880_REG(RALINK_SYSCTL_BASE + 0x60) &= ~0x4;  //MT7621 bit2
	udelay(500);
	//msleep(1);
#elif defined (CONFIG_RALINK_MT7628)
	RT2880_REG(RALINK_SYSCTL_BASE + 0x60) &= ~0x3000;  //MT7628 bit2
	udelay(500);
#else
	RT2880_REG(RALINK_SYSCTL_BASE+0x60) &= ~0x1;	
#endif
	/* reset i2c block */
	i = RT2880_REG(RT2880_RSTCTRL_REG) | RALINK_I2C_RST;
	RT2880_REG(RT2880_RSTCTRL_REG) = i;
	RT2880_REG(RT2880_RSTCTRL_REG) = i & ~(RALINK_I2C_RST);

	udelay(500);
	//msleep(1);
	
	RT2880_REG(RT2880_I2C_CONFIG_REG) = I2C_CFG_DEFAULT;

#if defined (CONFIG_RALINK_MT7621) || defined (CONFIG_RALINK_MT7628)
        i = 1 << 31; // the output is pulled hight by SIF master 0
        i |= 1 << 28; // allow triggered in VSYNC pulse
        i |= CLKDIV_VALUE << 16; //clk div
        i |= 1 << 6; // output H when SIF master 0 is in WAIT state
        i |= 1 << 1; // Enable SIF master 0
        RT2880_REG(RT2880_I2C_SM0CTL0) = i;

        RT2880_REG(RT2880_I2C_SM0_IS_AUTOMODE) = 1; //auto mode
#else
	RT2880_REG(RT2880_I2C_CLKDIV_REG) = CLKDIV_VALUE;
#endif

	/*
	 * Device Address : 
	 *
	 * ATMEL 24C152 serial EEPROM
	 *       1|0|1|0|0|A1|A2|R/W
	 *      MSB              LSB
	 * 
	 * ATMEL 24C01A/02/04/08A/16A
	 *    	MSB               LSB	  
	 * 1K/2K 1|0|1|0|A2|A1|A0|R/W
	 * 4K            A2 A1 P0
	 * 8K            A2 P1 P0
	 * 16K           P2 P1 P0 
	 *
	 * so device address needs 7 bits 
	 * if device address is 0, 
	 * write 0xA0 >> 1 into DEVADDR(max 7-bits) REG  
	 */
	RT2880_REG(RT2880_I2C_DEVADDR_REG) = i2cdrv_addr;

	/*
	 * Use Address Disabled Transfer Options
	 * because it just support 8-bits, 
	 * ATMEL eeprom needs two 8-bits address
	 */
	RT2880_REG(RT2880_I2C_ADDR_REG) = 0;
	//spin_unlock_irq(&spinlock_i2c);
	//spin_unlock_irqrestore(&spinlock_i2c, flags_i2c);
}

void i2c_exp_init(unsigned char sc_exp_i2c_addr)
{
	//spin_lock_irq(&spinlock_i2c);
	//spin_lock_irqsave(&spinlock_i2c, flags_i2c);
	u32 i;
#if defined (CONFIG_RALINK_MT7621)
	RT2880_REG(RALINK_SYSCTL_BASE + 0x60) &= ~0x4;  //MT7621 bit2
	udelay(500);
#elif defined (CONFIG_RALINK_MT7628)
	RT2880_REG(RALINK_SYSCTL_BASE + 0x60) &= ~0x3000;  //MT7628 bit2
	udelay(500);
#else
	RT2880_REG(RALINK_SYSCTL_BASE+0x60) &= ~0x1;	
#endif
	/* reset i2c block */
	i = RT2880_REG(RT2880_RSTCTRL_REG) | RALINK_I2C_RST;
	RT2880_REG(RT2880_RSTCTRL_REG) = i;
	RT2880_REG(RT2880_RSTCTRL_REG) = i & ~(RALINK_I2C_RST);

	udelay(500);
	
	RT2880_REG(RT2880_I2C_CONFIG_REG) = I2C_CFG_DEFAULT;

#if defined (CONFIG_RALINK_MT7621) || defined (CONFIG_RALINK_MT7628)
        i = 1 << 31; // the output is pulled hight by SIF master 0
        i |= 1 << 28; // allow triggered in VSYNC pulse
        i |= CLKDIV_VALUE << 16; //clk div
        i |= 1 << 6; // output H when SIF master 0 is in WAIT state
        i |= 1 << 1; // Enable SIF master 0
        RT2880_REG(RT2880_I2C_SM0CTL0) = i;

        RT2880_REG(RT2880_I2C_SM0_IS_AUTOMODE) = 1; //auto mode
#else
	RT2880_REG(RT2880_I2C_CLKDIV_REG) = CLKDIV_VALUE;
#endif

	/*
	 * Device Address : 
	 *
	 * ATMEL 24C152 serial EEPROM
	 *       1|0|1|0|0|A1|A2|R/W
	 *      MSB              LSB
	 * 
	 * ATMEL 24C01A/02/04/08A/16A
	 *    	MSB               LSB	  
	 * 1K/2K 1|0|1|0|A2|A1|A0|R/W
	 * 4K            A2 A1 P0
	 * 8K            A2 P1 P0
	 * 16K           P2 P1 P0 
	 *
	 * so device address needs 7 bits 
	 * if device address is 0, 
	 * write 0xA0 >> 1 into DEVADDR(max 7-bits) REG  
	 */
	RT2880_REG(RT2880_I2C_DEVADDR_REG) = sc_exp_i2c_addr;

	/*
	 * Use Address Disabled Transfer Options
	 * because it just support 8-bits, 
	 * ATMEL eeprom needs two 8-bits address
	 */
	RT2880_REG(RT2880_I2C_ADDR_REG) = 0;
	//spin_unlock_irq(&spinlock_i2c);
	//spin_unlock_irqrestore(&spinlock_i2c, flags_i2c);
}
/*----------------------------------------------------------------------*/
/*   Function                                                           */
/*           	i2c_WM8751_write                                               */
/*    INPUTS: 8-bit data                                                */
/*   RETURNS: None                                                      */
/*   OUTPUTS: None                                                      */
/*   NOTE(S): transfer 8-bit data to I2C                                */
/*                                                                      */
/*----------------------------------------------------------------------*/
#if defined (CONFIG_RALINK_I2S) || defined (CONFIG_RALINK_I2S_MODULE)
void i2c_WM8751_write(u32 address, u32 data)
{
	//spin_lock_irq(&spinlock_i2c);
	spin_lock_irqsave(&spinlock_i2c, flags_i2c);
	int i, j;
	unsigned long old_addr = i2cdrv_addr;
	
#if defined(CONFIG_SND_SOC_WM8960) || ((!defined(CONFIG_SND_RALINK_SOC)) && defined(CONFIG_I2S_WM8960))
	i2cdrv_addr = WM8960_ADDR;
#elif defined(CONFIG_SND_SOC_WM8750) || ((!defined(CONFIG_SND_RALINK_SOC)) && defined(CONFIG_I2S_WM8750)) || ((!defined(CONFIG_SND_RALINK_SOC)) && defined(CONFIG_I2S_WM8751))
	i2cdrv_addr = WM8751_ADDR;
#else
	i2cdrv_addr = WM8960_ADDR;
#endif
	
	i2c_master_init();	
	
	//spin_lock_irqsave(&spinlock_i2c, flags_i2c);
	/* two bytes data at least so NODATA = 0 */

	RT2880_REG(RT2880_I2C_BYTECNT_REG) = 1;
	
	RT2880_REG(RT2880_I2C_DATAOUT_REG) = (address<<1)|(0x01&(data>>8));

	RT2880_REG(RT2880_I2C_STARTXFR_REG) = WRITE_CMD;

	j = 0;
	do {
		if (IS_SDOEMPTY) {	
			RT2880_REG(RT2880_I2C_DATAOUT_REG) = (data&0x0FF);	
			break;
		}
	} while (++j<max_ee_busy_loop);
	
	i = 0;
	while(IS_BUSY && i<i2c_busy_loop){
		i++;
	};
	i2cdrv_addr = old_addr;
	//spin_unlock_irq(&spinlock_i2c);
	spin_unlock_irqrestore(&spinlock_i2c, flags_i2c);
}
#endif

/*----------------------------------------------------------------------*/
/*   Function                                                           */
/*           	i2c_write                                               */
/*    INPUTS: 8-bit data                                                */
/*   RETURNS: None                                                      */
/*   OUTPUTS: None                                                      */
/*   NOTE(S): transfer 8-bit data to I2C                                */
/*                                                                      */
/*----------------------------------------------------------------------*/
static int i2c_write(u32 address, u8 *data, u32 nbytes)
{
	//spin_lock_irq(&spinlock_i2c);
	//spin_lock_irqsave(&spinlock_i2c, flags_i2c);
	int i, j;
	u32 n;
	int abnormal_write = 0;

	/* two bytes data at least so NODATA = 0 */
	n = nbytes;// + address_bytes;
	RT2880_REG(RT2880_I2C_BYTECNT_REG) = n-1;
#if 0
	if (address_bytes == 2)
		RT2880_REG(RT2880_I2C_DATAOUT_REG) = (address >> 8) & 0xFF;
	else
		RT2880_REG(RT2880_I2C_DATAOUT_REG) = address & 0xFF;
#endif
	RT2880_REG(RT2880_I2C_STARTXFR_REG) = WRITE_CMD;
	for (i=0; i<n; i++) {
		j = 0;
		do {
			if (IS_SDOEMPTY) {
				if (address_bytes == 2) {
					if (i==0) {
						RT2880_REG(RT2880_I2C_DATAOUT_REG) = address & 0xFF;
					} else {
						RT2880_REG(RT2880_I2C_DATAOUT_REG) = data[i-1];
					}								
				} else {
					RT2880_REG(RT2880_I2C_DATAOUT_REG) = data[i];
//					printk("write [%x] to I2C_DATAOUT_REG\n", data[i]);
				}
 			break;
			}
		} while (++j<max_ee_busy_loop);
		udelay(500);
		//msleep(1);
		if(j > 2)
			abnormal_write = 1;
//		printk("write data[%d] use %d times\n", i, j+1);
	}

	i = 0;
	while(IS_BUSY && i<i2c_busy_loop){
		i++;
	};

	//spin_unlock_irq(&spinlock_i2c);
	//spin_unlock_irqrestore(&spinlock_i2c, flags_i2c);

	if(abnormal_write == 1)
		return -1;

	return 0;
}

/*----------------------------------------------------------------------*/
/*   Function                                                           */
/*           	i2c_read                                                */
/*    INPUTS: None                                                      */
/*   RETURNS: 8-bit data                                                */
/*   OUTPUTS: None                                                      */
/*   NOTE(S): get 8-bit data from I2C                                   */
/*                                                                      */
/*----------------------------------------------------------------------*/
static void i2c_read(u8 *data, u32 nbytes) 
{
	//spin_lock_irq(&spinlock_i2c);
	//spin_lock_irqsave(&spinlock_i2c, flags_i2c);
	int i, j;

	RT2880_REG(RT2880_I2C_BYTECNT_REG) = nbytes-1;
	RT2880_REG(RT2880_I2C_STARTXFR_REG) = READ_CMD;
	
	for (i=0; i<nbytes; i++) {
		j = 0;
		do {
			if (IS_DATARDY) {
				//data[i] = RT2880_REG(RT2880_I2C_DATAIN_REG);
				//printk("read [%x] from I2C_DATAIN_REG\n", data[i]);
				break;
			}
		} while(++j<max_ee_busy_loop*2);
		data[i] = RT2880_REG(RT2880_I2C_DATAIN_REG);
#ifdef _I2C_MCU_DEBUG_
		printk("read [0x%x] from I2C_DATAIN_REG\n", data[i]);
//		printk("read data[%d] use %d times\n", i, j+1);
#endif
	}

	i = 0;
	while(IS_BUSY && i<i2c_busy_loop){
		i++;
	};
	
	//spin_unlock_irq(&spinlock_i2c);
	//spin_unlock_irqrestore(&spinlock_i2c, flags_i2c);
}

static inline void random_read_any(u32 address, u8 *data, int nbytes)
{
	/* change page */
	if (address_bytes == 1) {
		int page;
		
		page = ((address >> 8) & 0x7) << 1;
		/* device id always 0 */
		RT2880_REG(RT2880_I2C_DEVADDR_REG) = (address | (page>>1));
	}

   	/* dummy write */
	
	i2c_read(data, nbytes);	
}

int random_write_any(u32 address, u8 *data, int nbytes)
{
	/* change page */
		
	int ret=0;

	if (address_bytes == 1) {
		int page;
		
		page = ((address >> 8) & 0x7) << 1;
		/* device id always 0 */
		RT2880_REG(RT2880_I2C_DEVADDR_REG) = (address | (page>>1));
	}


	ret = i2c_write(address, data, nbytes);
	udelay(500);
	//msleep(1);
	return ret;
}

enum
{
	SC_SET_OUTPUT,
	SC_SET,
	SC_CLEAR,
	SC_RUN,
};
void _sc_i2c_cmd(int cmd, int use_copy_from_user,unsigned long arg, int offset)
{
	int i = 0;
	unsigned int size;
	unsigned char *buf = NULL;
	I2C_TYPE i2c_wr = {0};
	
	if ((cmd != SC_RUN) && (cmd!=SC_SET_OUTPUT))
	{
		printk("<0>!!!!!!!!!!!! not run command:%d\n", cmd);
		return;
	}
	
	if (use_copy_from_user)
	{
		if(copy_from_user(&i2c_wr, (void *)arg, sizeof(I2C_TYPE)))
		{
			printk("copy from user error when write\n");
			return;
		}
	} else
	{
		i2c_wr.size = 2;
		i2c_wr.i2c_addr = 0x20;	
	}
			
			size = i2c_wr.size;
			i2cdrv_addr = i2c_wr.i2c_addr;

#ifdef _I2C_MCU_DEBUG_
			printk("write size %d, and write addr is 0x%x\n", size, i2c_wr.i2c_addr);
#endif
			if(!(buf = kmalloc(size, GFP_KERNEL)))
			{
				printk("kmalloc error when write\n");
				return;
			}
			
			if (use_copy_from_user)
			{
				if(copy_from_user(buf, i2c_wr.buf, size))
				{
					printk("copy from user error when write\n");
					kfree(buf);
					return;
				}
			} else
			{
				if (cmd == SC_SET_OUTPUT)
				{
					if (offset<=7)
					{
						buf[0] = 0x00; // 01 
						buf[1] = offset;
					} else
					{
						buf[0] = 0x01; // 01 
						buf[1] = offset-8;
					}
				} else
				{
					if (offset<=7)
					{
						buf[0] = 0x02; // 01 
						buf[1] = offset;
					} else
					{
						buf[0] = 0x03; // 01 
						buf[1] = offset-8;
					}
				}
				
				i2c_wr.buf = buf;
			}

#ifdef _I2C_MCU_DEBUG_
			for(i = 0; i < size; i++)
				printk("Before: write data[%d]: 0x%x\n", i, buf[i]);
#endif
			
			spin_lock_irqsave(&spinlock_i2c, flags_i2c);
			//spin_lock_bh(&spinlock_i2c);
			
			if (cmd == SC_CLEAR)
			{
				if(offset<=7)
				{
					expander1_data &= ~(1 << buf[1]);
					buf[1] = expander1_data;
				} else
				{
					expander2_data &= ~(1 << buf[1]);
					buf[1] = expander2_data;					
				}
			} else if (cmd == SC_SET)
			{
				if(offset<=7)
				{
					expander1_data |= (1 << buf[1]);
					buf[1] = expander1_data;
				} else
				{
					expander2_data |= (1 << buf[1]);
					buf[1] = expander2_data;
				}
			} else if (cmd == SC_SET_OUTPUT)
			{
				if(offset<=7)
				{
					expander1_dir &= ~(1 << buf[1]);
					buf[1] = expander1_dir;
				}
				else
				{
					expander2_dir &= ~(1 << buf[1]);
					buf[1] = expander2_dir;
				}
			} else if (cmd == SC_RUN)
			{
				if(offset<=7)
				{
					buf[1] = expander1_data;
				} else
				{
					buf[1] = expander2_data;
				}			
			}

#ifdef _I2C_MCU_DEBUG_
			for(i = 0; i < size; i++)
				printk("write data[%d]: 0x%x\n", i, buf[i]);
#endif
			
			switch_address_bytes(1);
			i2c_master_init();

			int ret_rd = 0;
			ret_rd = random_write_any(i2cdrv_addr, buf, size);
			
			//printk("%s: write offset %d: 0x%x, 0x%x\n", __FUNCTION__, offset, buf[0], buf[1]);
			if(buf)
			{
				kfree(buf);
				buf = NULL;
			}
			
			spin_unlock_irqrestore(&spinlock_i2c, flags_i2c);
			//spin_unlock_bh(&spinlock_i2c);
			if(ret_rd == -1)
			{
				printk("<0>write abnormal, should wirte again to make sure\n");
				return;
			}

#ifdef _I2C_MCU_DEBUG_
			printk("write success\n");
#endif
}

// extern 
void sc_i2c_drv_set_onoff(int offset, int on_off)
{
#ifdef _I2C_MCU_DEBUG_	
	printk("%s: set %d %d\n", __FUNCTION__, offset, on_off);
#endif	
	if (offset>=0 && offset<=15)
	{
		_sc_i2c_cmd(on_off?SC_SET:SC_CLEAR, 0,0, offset);
	} else
	{
		printk("<0>%s: error  offset %d\n", __FUNCTION__, offset);
	}
}
// extern 
void sc_i2c_drv_set_output(int offset)
{
#ifdef _I2C_MCU_DEBUG_		
	printk("%s: set %d output\n", __FUNCTION__, offset);
#endif	
	if (offset>=0 && offset<=15)
	{
		//printk("<0>%s: set  offset %d output\n", __FUNCTION__, offset);
		_sc_i2c_cmd(SC_SET_OUTPUT, 0,0, offset);
	} else
	{
		printk("<0>%s: error  offset %d\n", __FUNCTION__, offset);
	}
}



//EXPORT_SYMBOL(sc_i2c_drv_set_onoff);
//EXPORT_SYMBOL(sc_i2c_drv_set_output);
EXPORT_SYMBOL(expander1_data);
EXPORT_SYMBOL(expander2_data);
EXPORT_SYMBOL(i2c_sc_debug);



int i2c_drv_thread(void *unused)
{
	unsigned char expander1_data_last = 0;
	unsigned char expander2_data_last = 0;
	
	unsigned int expander1_counter = 0;
	unsigned int expander2_counter = 0;
	
	int i = 0;
	for (i=0; i<=15; i++)
	{
		sc_i2c_drv_set_output(i);
	}
	while (1)
	{
		if (kthread_should_stop())
		{
			printk("<0> exit\n");
			break;
		}
		
		expander1_counter++;
		expander2_counter++;
		
		if (i2c_sc_debug)
		{
			printk("%s: value: 0x%x, 0x%x\n",  __FUNCTION__, expander1_data, expander2_data);
		}
		
		if ((expander1_data_last != expander1_data) || ((expander1_counter%16)==0))
		{
		_sc_i2c_cmd(SC_RUN, 0, 0, 0);
			expander1_data_last = expander1_data;
		}
		
		if ((expander2_data_last != expander2_data) || ((expander2_counter%16)==0))
		{
		_sc_i2c_cmd(SC_RUN, 0, 0, 15);
			expander2_data_last = expander2_data;
		}
		
		msleep(50);
	}
}


#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
long i2cdrv_ioctl(struct file *filp, unsigned int cmd, 
		unsigned long arg)
#else
int i2cdrv_ioctl(struct inode *inode, struct file *filp, \
                     unsigned int cmd, unsigned long arg)
#endif
{
	unsigned int size;
	unsigned char *buf = NULL;
	I2C_TYPE i2c_wr = {0};
#ifdef _I2C_MCU_DEBUG_
	int i = 0;
#endif

	switch (cmd) {
	case RT2880_I2C_WRITE:
		if(arg)
		{
			if(copy_from_user(&i2c_wr, (void *)arg, sizeof(I2C_TYPE)))
			{
				printk("copy from user error when write\n");
				return -1;
			}
			size = i2c_wr.size;
			i2cdrv_addr = i2c_wr.i2c_addr;

#ifdef _I2C_MCU_DEBUG_
			printk("write size %d, i2c addr is 0x%x\n", size, i2c_wr.i2c_addr);
#endif

			if(!(buf = kmalloc(size, GFP_KERNEL)))
			{
				printk("kmalloc error when write\n");
				return -1;
			}
			if(copy_from_user(buf, i2c_wr.buf, size))
			{
				printk("copy from user error when write\n");
				kfree(buf);
				return -1;
			}

#ifdef _I2C_MCU_DEBUG_
			for(i = 0; i < size; i++)
				printk("write data[%d]: 0x%x\n", i, buf[i]);
#endif
			
			spin_lock_irqsave(&spinlock_i2c, flags_i2c);

			i2c_master_init();
			random_write_any(i2cdrv_addr, buf, size);

			i2c_master_init();
			unsigned char dummy[2] = {0x00};
			random_read_any(i2cdrv_addr, dummy, sizeof(dummy));

			spin_unlock_irqrestore(&spinlock_i2c, flags_i2c);
#ifdef _I2C_MCU_DEBUG_
			for(i = 0; i < 2; i++)
				printk("read dummy data[%d]: 0x%x\n", i, dummy[i]);
#endif
			
			if(buf)
			{
				kfree(buf);
				buf = NULL;
			}
#ifdef _I2C_MCU_DEBUG_
			printk("write success\n");
#endif
		}
#ifdef _I2C_MCU_DEBUG_
		printk("i2cdrv_addr is 0x%x \n", i2cdrv_addr);
#endif
		break;
	case RT2880_I2C_WRITE1:
		if(arg)
		{
			if(copy_from_user(&i2c_wr, (void *)arg, sizeof(I2C_TYPE)))
			{
				printk("copy from user error when write\n");
				return -1;
			}
			size = i2c_wr.size;
			i2cdrv_addr = (i2c_wr.i2c_addr >> 1);
#ifdef _I2C_MCU_DEBUG_
			printk("write size %d, and write addr is 0x%x\n", size, i2c_wr.i2c_addr);
#endif
			if(!(buf = kmalloc(size, GFP_KERNEL)))
			{
				printk("kmalloc error when write\n");
				return -1;
			}
			if(copy_from_user(buf, i2c_wr.buf, size))
			{
				printk("copy from user error when write\n");
				kfree(buf);
				return -1;
			}

#ifdef _I2C_MCU_DEBUG_
			for(i = 0; i < size; i++)
				printk("write data[%d]: 0x%x\n", i, buf[i]);
#endif
			
			spin_lock_irqsave(&spinlock_i2c, flags_i2c);

			switch_address_bytes(1);
			i2c_master_init();

			int ret_rd = 0;
			ret_rd = random_write_any(i2cdrv_addr, buf, size);

			spin_unlock_irqrestore(&spinlock_i2c, flags_i2c);
			if(ret_rd == -1)
			{
				printk("write abnormal, should wirte again to make sure\n");
				return -1;
			}
			
			if(buf)
			{
				kfree(buf);
				buf = NULL;
			}
#ifdef _I2C_MCU_DEBUG_
			printk("write success\n");
#endif
		}

#ifdef _I2C_MCU_DEBUG_
		printk("i2cdrv_addr is 0x%x \n", i2cdrv_addr);
#endif
		break;
	case RT2880_I2C_EXP_OUTPUT:
		if(arg)
		{
			_sc_i2c_cmd(SC_SET, 1,arg, -1);
		}
#ifdef _I2C_MCU_DEBUG_
		printk("i2cdrv_addr is 0x%x \n", i2cdrv_addr);
#endif
		break;
	case RT2880_I2C_EXP_INPUT:
		if(arg)
		{
			if(copy_from_user(&i2c_wr, (void *)arg, sizeof(I2C_TYPE)))
			{
				printk("copy from user error when write\n");
				return -1;
			}
			
			size = i2c_wr.size;
			i2cdrv_addr = i2c_wr.i2c_addr;

#ifdef _I2C_MCU_DEBUG_
			printk("write size %d, and write addr is 0x%x\n", size, i2c_wr.i2c_addr);
#endif
			if(!(buf = kmalloc(size, GFP_KERNEL)))
			{
				printk("kmalloc error when write\n");
				return -1;
			}
			if(copy_from_user(buf, i2c_wr.buf, size))
			{
				printk("copy from user error when write\n");
				kfree(buf);
				return -1;
			}

#ifdef _I2C_MCU_DEBUG_
			for(i = 0; i < size; i++)
				printk("Before: write data[%d]: 0x%x\n", i, buf[i]);
#endif

			if(i2c_wr.i2c_addr == 0x20)
			{
				expander1_dir |= (1 << buf[1]);
				buf[1] = expander1_dir;
			}
			else if(i2c_wr.i2c_addr == 0x21)
			{
				expander2_dir |= (1 << buf[1]);
				buf[1] = expander2_dir;
			}
			else
			{
				printk("Wrong expander address!\n");
				return -1;
			}

#ifdef _I2C_MCU_DEBUG_
			for(i = 0; i < size; i++)
				printk("write data[%d]: 0x%x\n", i, buf[i]);
#endif
			
			spin_lock_irqsave(&spinlock_i2c, flags_i2c);

			switch_address_bytes(1);
			i2c_master_init();

			int ret_rd = 0;
			ret_rd = random_write_any(i2cdrv_addr, buf, size);

			spin_unlock_irqrestore(&spinlock_i2c, flags_i2c);
			if(ret_rd == -1)
			{
				printk("write abnormal, should wirte again to make sure\n");
				return -1;
			}
			
			if(buf)
			{
				kfree(buf);
				buf = NULL;
			}
#ifdef _I2C_MCU_DEBUG_
			printk("write success\n");
#endif
		}

#ifdef _I2C_MCU_DEBUG_
		printk("i2cdrv_addr is 0x%x \n", i2cdrv_addr);
#endif
		break;
	case RT2880_I2C_EXP_SET:
		if(arg)
		{
			_sc_i2c_cmd(SC_SET, 1,arg, -1);
		}

#ifdef _I2C_MCU_DEBUG_
		printk("i2cdrv_addr is 0x%x \n", i2cdrv_addr);
#endif
		break;
	case RT2880_I2C_EXP_CLEAR:
		if(arg)
		{
			_sc_i2c_cmd(SC_CLEAR, 1, arg, -1);
		}

#ifdef _I2C_MCU_DEBUG_
		printk("i2cdrv_addr is 0x%x \n", i2cdrv_addr);
#endif
		break;
	case RT2880_I2C_EXP_READ:
		if(arg)
		{
			if(copy_from_user(&i2c_wr, (void *)arg, sizeof(I2C_TYPE)))
			{
				printk("copy from user error when write\n");
				return -1;
			}
			size = i2c_wr.size;
			i2cdrv_addr = i2c_wr.i2c_addr;

#ifdef _I2C_MCU_DEBUG_
			printk("write size %d, i2c addr is 0x%x\n", size, i2c_wr.i2c_addr);
#endif

			if(!(buf = kmalloc(size, GFP_KERNEL)))
			{
				printk("kmalloc error when write\n");
				return -1;
			}

			if(copy_from_user(buf, i2c_wr.buf, size))
			{
				printk("copy from user error when write\n");
				kfree(buf);
				return -1;
			}

#ifdef _I2C_MCU_DEBUG_
			for(i = 0; i < size; i++)
				printk("write data[%d]: 0x%x\n", i, buf[i]);
#endif
			
			spin_lock_irqsave(&spinlock_i2c, flags_i2c);

			i2c_master_init();
			random_write_any(i2cdrv_addr, buf, size);

			i2c_master_init();
			unsigned char dummy = 0x00;
			random_read_any(i2cdrv_addr, &dummy, sizeof(dummy));

			spin_unlock_irqrestore(&spinlock_i2c, flags_i2c);
			
			i2c_wr.read_val = dummy;
			copy_to_user((void *)arg, &i2c_wr, sizeof(I2C_TYPE));

#ifdef _I2C_MCU_DEBUG_
			printk("read dummy data: 0x%x\n", dummy);
#endif
			
			if(buf)
			{
				kfree(buf);
				buf = NULL;
			}
#ifdef _I2C_MCU_DEBUG_
			printk("write success\n");
#endif
		}
#ifdef _I2C_MCU_DEBUG_
		printk("i2cdrv_addr is 0x%x \n", i2cdrv_addr);
#endif
		break;
	case RT2880_I2C_EXP_WRITE:
		if(arg)
		{
			if(copy_from_user(&i2c_wr, (void *)arg, sizeof(I2C_TYPE)))
			{
				printk("copy from user error when write\n");
				return -1;
			}
			
			size = i2c_wr.size;
			i2cdrv_addr = i2c_wr.i2c_addr;

#ifdef _I2C_MCU_DEBUG_
			printk("write size %d, and write addr is 0x%x\n", size, i2c_wr.i2c_addr);
#endif
			if(!(buf = kmalloc(size, GFP_KERNEL)))
			{
				printk("kmalloc error when write\n");
				return -1;
			}
			if(copy_from_user(buf, i2c_wr.buf, size))
			{
				printk("copy from user error when write\n");
				kfree(buf);
				return -1;
			}

#ifdef _I2C_MCU_DEBUG_
			for(i = 0; i < size; i++)
				printk("write data[%d]: 0x%x\n", i, buf[i]);
#endif
			
			spin_lock_irqsave(&spinlock_i2c, flags_i2c);

			switch_address_bytes(1);
			i2c_master_init();

			int ret_rd = 0;
			ret_rd = random_write_any(i2cdrv_addr, buf, size);

			spin_unlock_irqrestore(&spinlock_i2c, flags_i2c);
			if(ret_rd == -1)
			{
				printk("write abnormal, should wirte again to make sure\n");
				return -1;
			}
			
			if(buf)
			{
				kfree(buf);
				buf = NULL;
			}
#ifdef _I2C_MCU_DEBUG_
			printk("write success\n");
#endif
		}

#ifdef _I2C_MCU_DEBUG_
		printk("i2cdrv_addr is 0x%x \n", i2cdrv_addr);
#endif
		break;
	default :
		printk("i2c_drv: command format error\n");
	}

	return 0;
}

struct file_operations i2cdrv_fops = {
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,35)
	unlocked_ioctl: i2cdrv_ioctl,
#else
	ioctl:  i2cdrv_ioctl,
#endif
};

struct task_struct *i2c_task_id = NULL;
int i2cdrv_init(void)
{
#if !defined (CONFIG_DEVFS_FS)
	int result=0;
#endif

	/* configure i2c to normal mode */
	RT2880_REG(RALINK_SYSCTL_BASE + 0x60) &= ~RALINK_GPIOMODE_I2C;

#ifdef  CONFIG_DEVFS_FS
	if(devfs_register_chrdev(i2cdrv_major, I2C_DEV_NAME , &i2cdrv_fops)) {
		printk(KERN_WARNING " i2cdrv: can't create device node\n");
		return -EIO;
	}

	devfs_handle = devfs_register(NULL, I2C_DEV_NAME, DEVFS_FL_DEFAULT, i2cdrv_major, 0, \
			S_IFCHR | S_IRUGO | S_IWUGO, &i2cdrv_fops, NULL);
#else
	result = register_chrdev(i2cdrv_major, I2C_DEV_NAME, &i2cdrv_fops);
	if (result < 0) {
		printk(KERN_WARNING "i2c_drv: can't get major %d\n",i2cdrv_major);
		return result;
	}

	if (i2cdrv_major == 0) {
		i2cdrv_major = result; /* dynamic */
	}
#endif

#if defined(CONFIG_MTK_NFC_SUPPORT)

        MT6605_gpio_init(MT6605_GPIO_IND, GPIO_INPUT); // IND
        MT6605_gpio_init(MT6605_GPIO_VEN, GPIO_OUTPUT); // VEN
        MT6605_gpio_init(MT6605_GPIO_RESET, GPIO_OUTPUT); // Reset

        MT6605_gpio_write(MT6605_GPIO_RESET, 0);
        MT6605_gpio_write(MT6605_GPIO_VEN, 1);
#endif

	i2c_task_id = kthread_run(i2c_drv_thread, NULL, "i2c_drv_thread");

	printk("i2cdrv_major = %d\n", i2cdrv_major);
	return 0;
}


static void i2cdrv_exit(void)
{
	printk("i2c_drv exit\n");

#ifdef  CONFIG_DEVFS_FS
	devfs_unregister_chrdev(i2cdrv_major, I2C_DEV_NAME);
	devfs_unregister(devfs_handle);
#else
	unregister_chrdev(i2cdrv_major, I2C_DEV_NAME);
#endif
	
	if (i2c_task_id && (!IS_ERR(i2c_task_id)))
	{
		printk("<0> kthread_stop\n");
		kthread_stop(i2c_task_id);	
	}
	
}

#if defined (CONFIG_RALINK_I2S) || defined (CONFIG_RALINK_I2S_MODULE)
EXPORT_SYMBOL(i2c_WM8751_write);
#endif

#if defined (CONFIG_MT7621_FPGA) || defined (CONFIG_MT7628_FPGA)
EXPORT_SYMBOL(pcie_phy_init);
EXPORT_SYMBOL(chk_phy_pll);
#endif

module_init(i2cdrv_init);
module_exit(i2cdrv_exit);

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,12)
MODULE_PARM (i2cdrv_major, "i");
#else
module_param (i2cdrv_major, int, 0);
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Ralink I2C Driver");

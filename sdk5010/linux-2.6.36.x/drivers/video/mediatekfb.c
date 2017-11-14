
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/cpufreq.h>

#include <asm/io.h>
#include <asm/div64.h>
#include "mediatekfb.h"
#ifdef CONFIG_PM
#include <linux/pm.h>
#endif

#if defined (CONFIG_RALINK_MT7620) 
int LCD_GPIO_NUM = 15;   //GPIO 55
int LCD_GPIO_RESET = 14; //GPIO 54
#endif 
#if defined (CONFIG_RALINK_MT7620)
int spich = 0;
static u32 spi0_reg[] = { RT2880_SPI0_STAT_REG, RT2880_SPI0_CFG_REG, RT2880_SPI0_CTL_REG, RT2880_SPI0_DATA_REG } ;
static u32 spi1_reg[] = { RT2880_SPI1_STAT_REG, RT2880_SPI1_CFG_REG, RT2880_SPI1_CTL_REG, RT2880_SPI1_DATA_REG } ;
static u32* spi_register[2] = { spi0_reg, spi1_reg };
int times = 0;
int doublecount =0;
#endif



#if defined (CONFIG_RALINK_MT7621)
#include "../mtd/ralink/bbu_spiflash.h"
extern spinlock_t flash_lock;
#if defined (CONFIG_FB_MEDIATEK_TRULY)
int LCD_GPIO_RST = 9;
#endif
#if defined (CONFIG_FB_MEDIATEK_ILITEK)
#define LCD_GPIO_CMD 9
#define LCD_GPIO_RESET 10
#endif
#define _ra_inl(addr)  (*(volatile unsigned int *)(addr))
#define _ra_outl(addr, value)  (*(volatile unsigned int *)(addr) = (value))
#define SPIC_READ_BYTES (1<<0)
#define SPIC_WRITE_BYTES (1<<1)

#define ra_aor(addr, a_mask, o_value)  ra_outl(addr, (ra_inl(addr) & (a_mask)) | (o_value))
#define ra_and(addr, a_mask)  ra_aor(addr, a_mask, 0)
#define ra_or(addr, o_value)  ra_aor(addr, -1, o_value)
#endif

#if defined (CONFIG_RALINK_MT7621) 
static int lcd_spic_busy_wait(void)
{
	int n = 100000;
	//printk("SPI_REG_MASTER = %x\n", sysRegRead(SPI_REG_MASTER));
	do {
		if ((ra_inl(SPI_REG_CTL) & SPI_CTL_BUSY) == 0)
			return 0;
		udelay(1);
	} while (--n > 0);

	printk("%s: fail \n", __func__);
	return -1;
}

static int spic_trans(u8 data, u8 OP)
{
	u32 temp;
	lcd_spic_busy_wait();
  if (OP == 0){ //CMD
  	temp = (0x70) | (data << 24);
  	RT2880_REG(SPI_REG_OPCODE) = temp;
  } else if (OP == 1){ //parameter
  	temp = (0x72) | (data << 24);
  	RT2880_REG(SPI_REG_OPCODE) = temp;
  } 
	/* step 3. set mosi_byte_cnt */
	ra_and(SPI_REG_CTL, ~SPI_CTL_TX_RX_CNT_MASK);
	ra_or(SPI_REG_CTL, 2);
  
	/* step 4. kick */
	ra_or(SPI_REG_CTL, SPI_CTL_START);
	/* step 5. wait spi_master_busy */
	lcd_spic_busy_wait();

	return 0;
}
#endif




static struct fb_fix_screeninfo mdiatekfb_fix __devinitdata = {
	.id =		"mediatekfb",
	.smem_len = 240*320*3,
};

static struct fb_var_screeninfo mdiatekfb_var __devinitdata = {
	.xres		= 320,
	.yres		= 240,
	.xres_virtual	= 320,
	.yres_virtual	= 240,
};
#if defined (CONFIG_RALINK_MT7620)
/*----------------------------------------------------------------------*/
/*   Function                                                           */
/*           	spi_chip_select                                         */
/*    INPUTS: ENABLE or DISABLE                                         */
/*   RETURNS: None                                                      */
/*   OUTPUTS: None                                                      */
/*   NOTE(S): Pull on or Pull down #CS                                  */
/*                                                                      */
/*----------------------------------------------------------------------*/
static inline void spi_chip_select(u8 enable)
{
	int i;
	u32* spireg = spi_register[spich];
	
	for (i=0; i<spi_busy_loop; i++) {
		if (!IS_BUSY) {
			/* low active */
			if (enable) {
				RT2880_REG(spireg[SPICTL]) |= SPICTL_SPIENA_ASSERT;
			} else  {
				RT2880_REG(spireg[SPICTL]) &= SPICTL_SPIENA_NEGATE;			
			}		
			break;
		}
	}

#ifdef DBG
	if (i == spi_busy_loop) {
		printk("warning : spi_transfer (spi_chip_select) busy !\n");
	}
#endif
}
/*----------------------------------------------------------------------*/
/*   Function                                                           */
/*           	spi_write                                               */
/*    INPUTS: 8-bit data                                                */
/*   RETURNS: None                                                      */
/*   OUTPUTS: None                                                      */
/*   NOTE(S): transfer 8-bit data to SPI                                */
/*                                                                      */
/*----------------------------------------------------------------------*/
static void spi_write(u8 data)
{
	int i;
	u32* spireg = spi_register[spich];
	u8 value;
	for (i=0; i<spi_busy_loop; i++) {

    value =(sysRegRead(RT2880_SPI_FIFO_STATUS)&0x20000) >> 17;
    if(value==1) printk("TX FULL\n");
 
		if ((!IS_BUSY) && ((sysRegRead(RT2880_SPI_FIFO_STATUS)&0x20000) >> 17)!=1) {
			RT2880_REG(spireg[SPIDATA]) = data;
			/* start write transfer */
			//RT2880_REG(spireg[SPICTL]) = SPICTL_HIZSDO | SPICTL_STARTWR ;
			RT2880_REG(spireg[SPICTL]) = SPICTL_STARTWR ;
			break;
		}
	}
	
#ifdef DBG
	if (i == spi_busy_loop) {
		printk("warning : spi_transfer (write %02x) busy !\n", data);
	}
#endif
}
#endif
#if defined (CONFIG_RALINK_MT7620)
static void spi_write_fifo(unsigned char * data)
{
	int i;
	u32 reg;
	int n_buf = 240*320*3 ;
	
	u32 user;
	unsigned long time1,time2;
  RT2880_REG(RT2880_SPI_ARB_REG) = (1<<16);
  RT2880_REG(RT2880_SPICFG_REG) = (SPICFG_SPIENMODE ) | (SPICFG_SPICLK_DIV16)  | SPICFG_TXCLKEDGE_FALLING  | SPICFG_RXCLKEDGE_FALLING | SPICFG_MSBFIRST;
 
   
  user = SPIUSR_SINGLE | (SPIUSR_SINGLE << 3) | (SPIUSR_SINGLE << 6) | (SPIUSR_SINGLE << 9) | (SPIUSR_WRITE_DATA << 12) | (SPIUSR_NO_DUMMY << 14) | (SPIUSR_NO_MODE << 16) | (SPIUSR_NO_ADDR << 17) | (SPIUSR_NO_DATA << 20) | (1 << 21);
 
  RT2880_REG(RT2880_SPIUSER_REG) =user;
  	
	RT2880_REG(RT2880_SPIBS_REG) = n_buf;
  RT2880_REG(RT2880_SPICTL_REG) = SPICTL_START ;
  time1=jiffies;
	for(i =0; i<240*320; i++){
		
		do {
				reg = (u32) ((RT2880_REG(RT2880_SPIFIFOSTAT_REG ) & 0xff00) >> 8);
		
		} while (reg >= 14);
		
		RT2880_REG(RT2880_SPITXFIFO_REG) = (data[56+3*i]&0xff);
		RT2880_REG(RT2880_SPITXFIFO_REG) = (data[55+3*i]&0xff);
		RT2880_REG(RT2880_SPITXFIFO_REG) = (data[54+3*i]&0xff);
					
	}
	time2=jiffies;
	printk("cost = %d\n", time2-time1);

  for (i=0; i<spi_busy_loop; i++) {
		if ((!IS_BUSY0)) {
			break;
		}
	}
  RT2880_REG(RT2880_SPI_ARB_REG) = (1<<31);
	RT2880_REG(RT2880_SPICFG_REG) =  RT2880_REG(RT2880_SPICFG_REG) & (~(SPICFG_SPIENMODE | SPICFG_RXENVDIS));
}

void spi_lcd_write(int D_CX, u32 value)
{
	if (D_CX == 0){
#if defined (CONFIG_RALINK_MT7620) && defined (CONFIG_FB_MEDIATEK_ILITEK)
		sysRegWrite(RALINK_REG_PIO7140RESET, 1 << LCD_GPIO_NUM);
#endif
#if defined (CONFIG_RALINK_MT7620)
		spi_chip_select(HIGH);
	  spi_chip_select(LOW);
#if defined (CONFIG_FB_MEDIATEK_TRULY)
	  spi_write(0x70); 
#endif
		spi_write(value); 
#endif
	}else{
#if defined (CONFIG_RALINK_MT7620) && defined (CONFIG_FB_MEDIATEK_ILITEK)
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
#endif
#if defined (CONFIG_RALINK_MT7620)
		spi_chip_select(HIGH);
	  spi_chip_select(LOW);
#if defined (CONFIG_FB_MEDIATEK_TRULY)
	  spi_write(0x72);
#endif
		spi_write(value); 
#endif
	}
	RT2880_REG(RT2880_SPI0_CTL_REG) = (~SPIARB_SPI0_ACTIVE_MODE)&0x1;
  spi_chip_select(HIGH);
}
#endif
void spi_lcd_master_init(void)
{
#if defined(CONFIG_RALINK_MT7620)
	u32* spireg;
	spich=1;
	spireg = spi_register[spich];

	RT2880_REG(RT2880_SPI_ARB_REG) = SPIARB_ARB_EN | (SPIARB_SPI1_ACTIVE_MODE<<1) | SPIARB_SPI0_ACTIVE_MODE;


	if (spich > 0){
	    //RT2880_REG(RT2880_SPI_ARB_REG) |= SPIARB_CS1CTL;
	}

	printk("SPIARB_SPI1_ACTIVE_MODE =%d\n",SPIARB_SPI1_ACTIVE_MODE);
	RT2880_REG(RT2880_SPI1_CTL_REG) = (~SPIARB_SPI1_ACTIVE_MODE)&0x1;     //disable first
//#endif
	RT2880_REG(RT2880_SPI0_CTL_REG) = (~SPIARB_SPI0_ACTIVE_MODE)&0x1;     //disable first

	RT2880_REG(spireg[SPICFG]) = SPICFG_MSBFIRST
								| SPICFG_RXCLKEDGE_FALLING
								| SPICFG_TXCLKEDGE_FALLING
								| SPICFG_SPICLK_DIV16;

	//RT2880_REG(spireg[SPICFG]) = SPICFG_MSBFIRST | SPICFG_SPICLK_DIV128;
					

#ifdef DBG
	printk("SPICFG = %08x\n", RT2880_REG(RT2880_SPICFG_REG));
	printk("is busy %d\n", IS_BUSY);
#endif	
#endif

#if defined(CONFIG_RALINK_MT7621)
				ra_or(SPI_REG_MASTER, (1 << 29));//chip select 1
        ra_and(SPI_REG_MASTER, ~(0xfff << 16));
        ra_or(SPI_REG_MASTER, (0x3 << 16));//44M
				RT2880_REG(SPI_REG_CTL) &= 0;
        ra_or(SPI_REG_CTL, (0x02 << 19));//addr size = 24bit
#endif        
        
 }
#if defined(CONFIG_RALINK_MT7621) 
static int spi_send(u8 *buf)
{
	u32 tmp1, tmp2, tmp3, DATA0;
	spin_lock(&flash_lock);
	spi_lcd_master_init();
	/* step 0. enable more byte mode */
	ra_or(SPI_REG_MASTER, (1 << 2));
	lcd_spic_busy_wait();
	tmp1 = (*(buf + 2) & 0xFC) | ((*(buf + 1) & 0xC0) >> 6);
	tmp2 = ((*(buf + 1) & 0x3C) << 2) | ( (*(buf + 0) & 0xF0) >> 4);
	tmp3 =  (*(buf + 0) & 0x0F) << 4;
	
	DATA0 =  ((0x72)<<24) | (tmp1 << 16) | (tmp2 << 8) | tmp3;
	RT2880_REG(SPI_REG_OPCODE) = DATA0;
	ra_or(SPI_REG_MOREBUF, (32 << 24));
  ra_or(SPI_REG_MOREBUF, 0);
	/* step 4. kick */
	ra_or(SPI_REG_CTL, SPI_CTL_START);	
	lcd_spic_busy_wait();
		/* step #. disable more byte mode */
	ra_and(SPI_REG_MASTER, ~(1 << 2));	
  spin_unlock(&flash_lock);
	return 0;
}
#endif  

void lcd_init(void)
{

#if defined (CONFIG_RALINK_MT7620) && defined (CONFIG_FB_MEDIATEK_ILITEK)
	  int i;
 		RT2880_REG(RALINK_REG_GPIOMODE) &= 0xFF9FE7E1; //for MT7620 ==> MAND pin is GPIO mode 
 		//RT2880_REG(RALINK_REG_GPIOMODE) &= 0xFF9FE7FD;
 		
		RT2880_REG(RT2880_RSTCTRL_REG) |= RSTCTRL_SPI_RESET;	//* reset spi block 
		RT2880_REG(RT2880_RSTCTRL_REG) &= ~(RSTCTRL_SPI_RESET);
		for ( i = 0; i < 1000; i++);
		spi_chip_select(HIGH);
		
		RT2880_REG(RALINK_REG_PIO7140DIR) |= (3 << 14);
		
		RT2880_REG(RALINK_REG_PIO7140SET) |= (1 << LCD_GPIO_RESET);
		mdelay(10);
		RT2880_REG(RALINK_REG_PIO7140RESET) |= (1 << LCD_GPIO_RESET);
		mdelay(10);
		RT2880_REG(RALINK_REG_PIO7140SET) |= (1 << LCD_GPIO_RESET);
		mdelay(120);
		spi_lcd_master_init();
	  spi_lcd_write(SPI_LCD_CMD, 0xCF);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x00); //parameter1
	  spi_write(0x8B);//parameter2
	  spi_write(0x30);//parameter3 
	  spi_chip_select(HIGH);
	  
	  spi_lcd_write(SPI_LCD_CMD, 0xED);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x64); //parameter1
	  spi_write(0x03);//parameter2
	  spi_write(0x12);//parameter3 
	  spi_write(0x81);//parameter4 
	  spi_chip_select(HIGH);
	
	  spi_lcd_write(SPI_LCD_CMD, 0xE8);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x85); //parameter1
	  spi_write(0x01);//parameter2
	  spi_write(0x79);//parameter3 
	  spi_chip_select(HIGH);
	  
	  spi_lcd_write(SPI_LCD_CMD, 0xCB);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x39); //parameter1
	  spi_write(0x2C);//parameter2
	  spi_write(0x00);//parameter3 
	  spi_write(0x34);//parameter4  
	  spi_write(0x02);//parameter5   
	  spi_chip_select(HIGH);
	  
	  spi_lcd_write(SPI_LCD_CMD, 0xF7);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x20); //parameter1
	  spi_chip_select(HIGH);
	  
	  spi_lcd_write(SPI_LCD_CMD, 0xEA);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x00); //parameter1
	  spi_write(0x00);//parameter2
	  spi_chip_select(HIGH);
	  
	  spi_lcd_write(SPI_LCD_CMD, 0xC0);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x1D); //parameter1
	  spi_chip_select(HIGH);
	  
	  spi_lcd_write(SPI_LCD_CMD, 0xC1);
	  sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x10); //parameter1  
	  spi_chip_select(HIGH);
	 
	  spi_lcd_write(SPI_LCD_CMD, 0xC5);
	 	sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x33); //parameter1
	  spi_write(0x34);//parameter2 
	  spi_chip_select(HIGH);
	 
	  spi_lcd_write(SPI_LCD_CMD, 0xC7);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0xBE); //parameter1   
	  spi_chip_select(HIGH);
	 
	  spi_lcd_write(SPI_LCD_CMD, 0x36);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
		spi_write(0x08);
		spi_chip_select(HIGH);
		
	  spi_lcd_write(SPI_LCD_CMD, 0xB1);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x00); //parameter1
	  spi_write(0x1F);//parameter2	
		spi_chip_select(HIGH);
		
	  spi_lcd_write(SPI_LCD_CMD, 0xB6);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x0A); //parameter1
	  spi_write(0xA2);//parameter2		
		spi_chip_select(HIGH);
		
	  spi_lcd_write(SPI_LCD_CMD, 0xF2);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
		spi_write(0x00);	
		spi_chip_select(HIGH);
		
	  spi_lcd_write(SPI_LCD_CMD, 0x26);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x01);
		spi_chip_select(HIGH);
		
		spi_lcd_write(SPI_LCD_CMD, 0x3A);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x66);
		spi_chip_select(HIGH);
		
		
	  spi_lcd_write(SPI_LCD_CMD, 0xE0);
	 	sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x0F); //parameter1
	  spi_write(0x21);//parameter2
	  spi_write(0x1E);//parameter3 
	  spi_write(0x0C);//parameter4  
	  spi_write(0x0F);//parameter5   
	  spi_write(0x07);//parameter2
	  spi_write(0x4B);//parameter3 
	  spi_write(0xD5);//parameter4  
	  spi_write(0x3B);//parameter5
	  spi_write(0x0A);//parameter2
	  spi_write(0x14);//parameter3 
	  spi_write(0x06);//parameter4  
	  spi_write(0x0E);//parameter5   
	  spi_write(0x07);//parameter2
	  spi_write(0x00);//parameter3 
		spi_chip_select(HIGH);  
	  
	  spi_lcd_write(SPI_LCD_CMD, 0xE1);
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
	  spi_write(0x00); //parameter1
	  spi_write(0x1E);//parameter2
	  spi_write(0x21);//parameter3 
	  spi_write(0x03);//parameter4  
	  spi_write(0x10);//parameter5   
	  spi_write(0x08);//parameter2
	  spi_write(0x34);//parameter3 
	  spi_write(0xA2);//parameter4  
	  spi_write(0x44);//parameter5
	  spi_write(0x05);//parameter2
	  spi_write(0x0B);//parameter3 
	  spi_write(0x09);//parameter4  
	  spi_write(0x31);//parameter5   
	  spi_write(0x38);//parameter2
	  spi_write(0x0F);//parameter3 
		spi_chip_select(HIGH);
		
		
		spi_lcd_write(SPI_LCD_CMD, 0x11);//sleep out
		mdelay(120);
	  spi_lcd_write(SPI_LCD_CMD, 0x29);
	  mdelay(100);
		spi_lcd_write(SPI_LCD_CMD, 0x2C);  	
		sysRegWrite(RALINK_REG_PIO7140SET, 1 << LCD_GPIO_NUM);
		spi_chip_select(HIGH);
		spi_chip_select(LOW);
#endif
#if defined (CONFIG_RALINK_MT7620) && defined (CONFIG_FB_MEDIATEK_TRULY)
		int i;
	  printk("lcd_init\n");
 		RT2880_REG(RALINK_REG_GPIOMODE) &= 0xFF9FE7E1; //for MT7620 ==> MAND pin is GPIO mode 
		RT2880_REG(RT2880_RSTCTRL_REG) |= RSTCTRL_SPI_RESET;	//* reset spi block 
		RT2880_REG(RT2880_RSTCTRL_REG) &= ~(RSTCTRL_SPI_RESET);
		for ( i = 0; i < 1000; i++);
		spi_chip_select(HIGH);
		RT2880_REG(RALINK_REG_PIO7140DIR) |= (1 << LCD_GPIO_RESET);
		RT2880_REG(RALINK_REG_PIO7140SET) |= (1 << LCD_GPIO_RESET);
		mdelay(10);
		RT2880_REG(RALINK_REG_PIO7140RESET) |= (1 << LCD_GPIO_RESET);
		mdelay(10);
		RT2880_REG(RALINK_REG_PIO7140SET) |= (1 << LCD_GPIO_RESET);
		mdelay(120);
	  spi_lcd_master_init();
	  spi_lcd_write(SPI_LCD_CMD, 0xFF);
	  spi_lcd_write(SPI_LCD_DATA, 0x01);
	  spi_lcd_write(SPI_LCD_CMD, 0x76);
	  spi_lcd_write(SPI_LCD_DATA, 0x01);
	  spi_lcd_write(SPI_LCD_CMD, 0xFF);
	  spi_lcd_write(SPI_LCD_DATA, 0x00);
	  spi_lcd_write(SPI_LCD_CMD, 0x17);
	  spi_lcd_write(SPI_LCD_DATA, 0x06);
	  spi_lcd_write(SPI_LCD_CMD, 0x18);
	  spi_lcd_write(SPI_LCD_DATA, 0x99);	
	  spi_lcd_write(SPI_LCD_CMD, 0x19);
	  spi_lcd_write(SPI_LCD_DATA, 0x01);
	  spi_lcd_write(SPI_LCD_CMD, 0x1B);
	  spi_lcd_write(SPI_LCD_DATA, 0x1A); 
	  spi_lcd_write(SPI_LCD_CMD, 0x1C);
	  spi_lcd_write(SPI_LCD_DATA, 0x04);   
	  spi_lcd_write(SPI_LCD_CMD, 0x24);
	  spi_lcd_write(SPI_LCD_DATA, 0x20);   
	  spi_lcd_write(SPI_LCD_CMD, 0x25);
	  spi_lcd_write(SPI_LCD_DATA, 0x5C);   
	  spi_lcd_write(SPI_LCD_CMD, 0x23);
	  spi_lcd_write(SPI_LCD_DATA, 0x85);   
	  spi_lcd_write(SPI_LCD_CMD, 0x1F);
	  spi_lcd_write(SPI_LCD_DATA, 0x88);     
	  mdelay(50);
	  spi_lcd_write(SPI_LCD_CMD, 0x1F);
	  spi_lcd_write(SPI_LCD_DATA, 0x80); 
	  mdelay(5);
	  spi_lcd_write(SPI_LCD_CMD, 0x1F);
	  spi_lcd_write(SPI_LCD_DATA, 0x90); 
	  mdelay(5);  
	  spi_lcd_write(SPI_LCD_CMD, 0x1F);
	  spi_lcd_write(SPI_LCD_DATA, 0xD0); 
	  mdelay(5); 
	  
	  spi_lcd_write(SPI_LCD_CMD, 0x28);
	  spi_lcd_write(SPI_LCD_DATA, 0x38);  
	  mdelay(40);
	    
	  
	  spi_lcd_write(SPI_LCD_CMD, 0x28);
	  spi_lcd_write(SPI_LCD_DATA, 0x3C); 
	  
	  spi_lcd_write(SPI_LCD_CMD, 0x35);
	  spi_lcd_write(SPI_LCD_DATA, 0x00);
	  spi_lcd_write(SPI_LCD_CMD, 0x36);
	  spi_lcd_write(SPI_LCD_DATA, 0x09); 
	  spi_lcd_write(SPI_LCD_CMD, 0x31);
	  spi_lcd_write(SPI_LCD_DATA, 0x00);   	
	  spi_lcd_write(SPI_LCD_CMD, 0x32);
	  spi_lcd_write(SPI_LCD_DATA, 0x0C); 	
	  spi_lcd_write(SPI_LCD_CMD, 0x40);
	  spi_lcd_write(SPI_LCD_DATA,0x01); 
	  spi_lcd_write(SPI_LCD_CMD, 0x41);
	  spi_lcd_write(SPI_LCD_DATA,0x02);
	  spi_lcd_write(SPI_LCD_CMD, 0x42);
	  spi_lcd_write(SPI_LCD_DATA,0x00);
	  spi_lcd_write(SPI_LCD_CMD, 0x43);
	  spi_lcd_write(SPI_LCD_DATA,0x13);
	  spi_lcd_write(SPI_LCD_CMD, 0x44);
	  spi_lcd_write(SPI_LCD_DATA,0x12);   
	  spi_lcd_write(SPI_LCD_CMD, 0x45);
	  spi_lcd_write(SPI_LCD_DATA,0x25); 
	  spi_lcd_write(SPI_LCD_CMD, 0x46);
	  spi_lcd_write(SPI_LCD_DATA,0x08);
	  spi_lcd_write(SPI_LCD_CMD, 0x47);
	  spi_lcd_write(SPI_LCD_DATA,0x55);
	  spi_lcd_write(SPI_LCD_CMD, 0x48);
	  spi_lcd_write(SPI_LCD_DATA,0x02);
	  spi_lcd_write(SPI_LCD_CMD, 0x49);
	  spi_lcd_write(SPI_LCD_DATA,0x14);
	  spi_lcd_write(SPI_LCD_CMD, 0x4A);
	  spi_lcd_write(SPI_LCD_DATA,0x1A);
	  spi_lcd_write(SPI_LCD_CMD, 0x4B);
	  spi_lcd_write(SPI_LCD_DATA,0x1B); 
	  spi_lcd_write(SPI_LCD_CMD, 0x4C);
	  spi_lcd_write(SPI_LCD_DATA,0x16); 
	  spi_lcd_write(SPI_LCD_CMD, 0x50);
	  spi_lcd_write(SPI_LCD_DATA,0x1A);
	  spi_lcd_write(SPI_LCD_CMD, 0x51);
	  spi_lcd_write(SPI_LCD_DATA,0x2D);
	  spi_lcd_write(SPI_LCD_CMD, 0x52);
	  spi_lcd_write(SPI_LCD_DATA,0x2C);
	  spi_lcd_write(SPI_LCD_CMD, 0x53);
	  spi_lcd_write(SPI_LCD_DATA,0x3F);
	  spi_lcd_write(SPI_LCD_CMD, 0x54);
	  spi_lcd_write(SPI_LCD_DATA,0x3D);
	  spi_lcd_write(SPI_LCD_CMD, 0x55);
	  spi_lcd_write(SPI_LCD_DATA,0x3E);
	  spi_lcd_write(SPI_LCD_CMD, 0x56);
	  spi_lcd_write(SPI_LCD_DATA,0x2A);
	  spi_lcd_write(SPI_LCD_CMD, 0x57);
	  spi_lcd_write(SPI_LCD_DATA,0x77);
	  spi_lcd_write(SPI_LCD_CMD, 0x58);
	  spi_lcd_write(SPI_LCD_DATA,0x09);
	  spi_lcd_write(SPI_LCD_CMD, 0x59);
	  spi_lcd_write(SPI_LCD_DATA,0x04);
	  spi_lcd_write(SPI_LCD_CMD, 0x5A);
	  spi_lcd_write(SPI_LCD_DATA,0x05);
	  spi_lcd_write(SPI_LCD_CMD, 0x5B);
	  spi_lcd_write(SPI_LCD_DATA,0x0B);
	  spi_lcd_write(SPI_LCD_CMD, 0x5C);
	  spi_lcd_write(SPI_LCD_DATA,0x1D);
	  spi_lcd_write(SPI_LCD_CMD, 0x5D);
	  spi_lcd_write(SPI_LCD_DATA,0xCC); 
	  spi_lcd_write(SPI_LCD_CMD, 0x22);
	  spi_chip_select(HIGH);
#endif

#if defined (CONFIG_RALINK_MT7621) && defined (CONFIG_FB_MEDIATEK_TRULY)
	int i;
		  //RT2880_REG(RALINK_REG_GPIOMODE) &= 0xFF9FE7FD; //for MT7620  modify 
	  RT2880_REG(RALINK_REG_GPIOMODE) &= 0x1CFFFF; //for MT7621 SPI normal mode
	  RT2880_REG(RALINK_REG_GPIOMODE) |= 0x60;// set GPIO9, GPIO10 =>GPIO mode
		RT2880_REG(RT2880_RSTCTRL_REG) |= RSTCTRL_SPI_RESET;	//* reset spi block 
		RT2880_REG(RT2880_RSTCTRL_REG) &= ~(RSTCTRL_SPI_RESET);

	for ( i = 0; i < 1000; i++);
	
	sysRegWrite(RALINK_REG_PIODIR, 0x1200 ); // set LCD_DCX GPIO = 9, 12 , direction output
	                                         //GPIO 9 LCM RST
	                                         //GPIO 12 NRST
	printk("GPIO Direction = %x\n", sysRegRead(RALINK_REG_PIODIR));

// reset LCD	
 	//sysRegWrite(RALINK_REG_PIODIR, 1 << 10); 
	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_RST);  //GPIO output = 1
	mdelay(1);
  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_RST); //GPIO output = 0
	mdelay(10);
  sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_RST); //GPIO output = 1
  mdelay(120);
	spi_lcd_master_init();
	spic_trans(0xFF, CMD);
  spic_trans(0x01, DATA);
  
	spic_trans(0x76, CMD);
  spic_trans(0x01, DATA);

	spic_trans(0xFF, CMD);
  spic_trans(0x00, DATA);

	spic_trans(0x17, CMD);
  spic_trans(0x06, DATA);
  
	spic_trans(0x18, CMD);
  spic_trans(0x99, DATA);
  
 	spic_trans(0x19, CMD);
  spic_trans(0x01, DATA);
  
 	spic_trans(0x1B, CMD);
  spic_trans(0x1A, DATA);
  spic_trans(0x1C, CMD);
  spic_trans(0x04, DATA);
  spic_trans(0x24, CMD);
  spic_trans(0x20, DATA);
  spic_trans(0x25, CMD);
  spic_trans(0x5C, DATA);
  spic_trans(0x23, CMD);
  spic_trans(0x85, DATA);
  spic_trans(0x1F, CMD);
  spic_trans(0x88, DATA);
  mdelay(50);
  spic_trans(0x1F, CMD);
  spic_trans(0x80, DATA);
  mdelay(5);

   spic_trans(0x1F, CMD);
  spic_trans(0x90, DATA);
  mdelay(5);

 
  mdelay(5);
  spic_trans(0x1F, CMD);
  spic_trans(0xD0, DATA); 
  mdelay(5);

  
  spic_trans(0x28, CMD);
  spic_trans(0x38, DATA); 
  mdelay(40); 
  spic_trans(0x28, CMD);
  spic_trans(0x3C, DATA);
  
  spic_trans(0x35, CMD);
  spic_trans(0x00, DATA);
  spic_trans(0x36, CMD);
  spic_trans(0x09, DATA);
    
  spic_trans(0x31, CMD);
  spic_trans(0x00, DATA); 
  spic_trans(0x32, CMD);
  spic_trans(0x0C, DATA);
  
  spic_trans(0x40, CMD);
  spic_trans(0x01, DATA); 
  spic_trans(0x41, CMD);
  spic_trans(0x02, DATA);
  spic_trans(0x42, CMD);
  spic_trans(0x00, DATA);
  spic_trans(0x43, CMD);
  spic_trans(0x13, DATA);
  spic_trans(0x44, CMD);
  spic_trans(0x12, DATA);   
  spic_trans(0x45, CMD);
  spic_trans(0x25, DATA); 
  spic_trans(0x46, CMD);
  spic_trans(0x08, DATA);
  spic_trans(0x47, CMD);
  spic_trans(0x55, DATA);
  spic_trans(0x48, CMD);
  spic_trans(0x02, DATA);
  spic_trans(0x49, CMD);
  spic_trans(0x14, DATA);
  spic_trans(0x4A, CMD);
  spic_trans(0x1A, DATA);
  spic_trans(0x4B, CMD);
  spic_trans(0x1B, DATA); 
  spic_trans(0x4C, CMD);
  spic_trans(0x16, DATA); 
  spic_trans(0x50, CMD);
  spic_trans(0x1A, DATA);
  spic_trans(0x51, CMD);
  spic_trans(0x2D, DATA);
  spic_trans(0x52, CMD);
  spic_trans(0x2C, DATA);
  spic_trans(0x53, CMD);
  spic_trans(0x3F, DATA);
  spic_trans(0x54, CMD);
  spic_trans(0x3D, DATA);
  spic_trans(0x55, CMD);
  spic_trans(0x3E, DATA);
  spic_trans(0x56, CMD);
  spic_trans(0x2A, DATA);
  spic_trans(0x57, CMD);
  spic_trans(0x77, DATA);
  spic_trans(0x58, CMD);
  spic_trans(0x09, DATA);
  spic_trans(0x59, CMD);
  spic_trans(0x04, DATA);
  spic_trans(0x5A, CMD);
  spic_trans(0x05, DATA);
  spic_trans(0x5B, CMD);
  spic_trans(0x0B, DATA);
  spic_trans(0x5C, CMD);
  spic_trans(0x1D, DATA);
  spic_trans(0x5D, CMD);
  spic_trans(0xCC, DATA);  
  spic_trans(0x22, CMD);
#endif

#if defined (CONFIG_RALINK_MT7621) && defined (CONFIG_FB_MEDIATEK_ILITEK)
  int i;
	//RT2880_REG(RALINK_REG_GPIOMODE) &= 0xFF9FE7FD; //for MT7620  modify 
	RT2880_REG(RALINK_REG_GPIOMODE) &= 0x1CFFFF; //for MT7621 SPI
	RT2880_REG(RALINK_REG_GPIOMODE) |= 0x60;// set GPIO9, GPIO10 =>GPIO mode
	RT2880_REG(RT2880_RSTCTRL_REG) |= RSTCTRL_SPI_RESET;	//* reset spi block 
	RT2880_REG(RT2880_RSTCTRL_REG) &= ~(RSTCTRL_SPI_RESET);
	for ( i = 0; i < 1000; i++);
			printk("RT2880_REG(RALINK_REG_PIODIR)=%x\n", RT2880_REG(RALINK_REG_PIODIR));

	RT2880_REG(RALINK_REG_PIODIR) |= 0x600;
	printk("RT2880_REG(RALINK_REG_PIODIR)=%x\n", RT2880_REG(RALINK_REG_PIODIR));
	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_RESET);  //GPIO output = 1
	mdelay(1);
	sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_RESET); //GPIO output = 0
	mdelay(10);
	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_RESET); //GPIO output = 1
	mdelay(120);
	printk("RT2880_REG(RALINK_REG_PIODIR)=%x\n", RT2880_REG(RALINK_REG_PIODIR));
  spi_lcd_master_init();
  
  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0xCF, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd
	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x00, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1
	bbu_spic_trans(NO_USE, 0x8B, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter2
	bbu_spic_trans(NO_USE, 0x30, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter3


  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0xED, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x64, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1
	bbu_spic_trans(NO_USE, 0x03, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter2
	bbu_spic_trans(NO_USE, 0x12, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter3	
	bbu_spic_trans(NO_USE, 0x81, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter4	

  
  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0xE8, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x85, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1
	bbu_spic_trans(NO_USE, 0x01, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter2
	bbu_spic_trans(NO_USE, 0x79, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter3


  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0xCB, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x39, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1
	bbu_spic_trans(NO_USE, 0x2C, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter2
	bbu_spic_trans(NO_USE, 0x00, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter3	
	bbu_spic_trans(NO_USE, 0x34, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter4		
	bbu_spic_trans(NO_USE, 0x02, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter4		
	
  

  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0xF7, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x20, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1


  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD); 
  bbu_spic_trans(NO_USE, 0xEA, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd 

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x00, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1
	bbu_spic_trans(NO_USE, 0x00, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter2


  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0xC0, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd   

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x1D, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1


  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0xC1, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd   

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x10, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1
 

  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0xC5, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd   

 	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
 	bbu_spic_trans(NO_USE, 0x33, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1
	bbu_spic_trans(NO_USE, 0x34, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter2


  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0xC7, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd  

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0xBE, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1


  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD); 
  bbu_spic_trans(NO_USE, 0x36, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd  

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x08, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1

	
  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0xB1, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd 

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
 	bbu_spic_trans(NO_USE, 0x00, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1
	bbu_spic_trans(NO_USE, 0x1F, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter2


  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);	
  bbu_spic_trans(NO_USE, 0xB6, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd 

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
 	bbu_spic_trans(NO_USE, 0x0A, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1
	bbu_spic_trans(NO_USE, 0xA2, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter2
		

  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0xF2, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd 		

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x00, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1

	
  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);		
  bbu_spic_trans(NO_USE, 0x26, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd 

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x01, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1


  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);	
  bbu_spic_trans(NO_USE, 0x3A, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd 	

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x66, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1


	
  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);	
  bbu_spic_trans(NO_USE, 0xE0, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd 	

 	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
 	bbu_spic_trans(NO_USE, 0x0F, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1
	bbu_spic_trans(NO_USE, 0x21, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter2
 	bbu_spic_trans(NO_USE, 0x1E, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter3
	bbu_spic_trans(NO_USE, 0x0C, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter4
 	bbu_spic_trans(NO_USE, 0x0F, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter5
	bbu_spic_trans(NO_USE, 0x07, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter6
 	bbu_spic_trans(NO_USE, 0x4B, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter7
	bbu_spic_trans(NO_USE, 0xD5, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter8
 	bbu_spic_trans(NO_USE, 0x3B, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter9
	bbu_spic_trans(NO_USE, 0x0A, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter10
 	bbu_spic_trans(NO_USE, 0x14, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter11
	bbu_spic_trans(NO_USE, 0x06, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter12
 	bbu_spic_trans(NO_USE, 0x0E, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter13
	bbu_spic_trans(NO_USE, 0x07, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter14
 	bbu_spic_trans(NO_USE, 0x00, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter15

  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);	 
  bbu_spic_trans(NO_USE, 0xE1, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd 	 

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	bbu_spic_trans(NO_USE, 0x00, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter1
	bbu_spic_trans(NO_USE, 0x1E, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter2
 	bbu_spic_trans(NO_USE, 0x21, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter3
	bbu_spic_trans(NO_USE, 0x03, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter4
 	bbu_spic_trans(NO_USE, 0x10, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter5
	bbu_spic_trans(NO_USE, 0x08, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter6
 	bbu_spic_trans(NO_USE, 0x34, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter7
	bbu_spic_trans(NO_USE, 0xA2, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter8
 	bbu_spic_trans(NO_USE, 0x44, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter9
	bbu_spic_trans(NO_USE, 0x05, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter10
 	bbu_spic_trans(NO_USE, 0x0B, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter11
	bbu_spic_trans(NO_USE, 0x09, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter12
 	bbu_spic_trans(NO_USE, 0x31, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter13
	bbu_spic_trans(NO_USE, 0x38, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter14
 	bbu_spic_trans(NO_USE, 0x0F, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//parameter15

	
  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0x11, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//sleep out		

	mdelay(120);
  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0x29, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd 		
 
  
  sysRegWrite(RALINK_REG_PIORESET, 1 << LCD_GPIO_CMD);
  bbu_spic_trans(NO_USE, 0x2C, NO_USE, NO_USE, NO_USE, NO_USE, LCD_USE);//cmd 		

	sysRegWrite(RALINK_REG_PIOSET, 1 << LCD_GPIO_CMD);
	mdelay(10);

#endif

}

void lcd_img_send_logo(void)
{

	char *filenameh="/etc_ro/mediatek_logo.bmp";
	struct file *srcf;
	char *src;
	mm_segment_t orgfs;
	unsigned char *buffer;
	const int bufSize = 1024*1024;
	int i;
	unsigned long filesize;
	//printk("initial send\n");
 	 	buffer = kmalloc(bufSize, GFP_ATOMIC);
	  src = filenameh;
		orgfs = get_fs();
		set_fs(KERNEL_DS);
		srcf = filp_open(src, O_RDONLY, 0);
		if (IS_ERR(srcf))
		{
			kfree(buffer);
		}
		else
		{
			memset(buffer, 0x00, bufSize);
			filesize = srcf->f_op->read(srcf, buffer, bufSize, &srcf->f_pos);
			filp_close(srcf,NULL);
		}
  //time1=jiffies;
#if defined (CONFIG_RALINK_MT7620) && defined (CONFIG_FB_MEDIATEK_TRULY) 
	u8 tmp1, tmp2, tmp3; 
	spi_chip_select(HIGH);
	for (i=0; i < 240*320 ; i++){
		tmp1 = (buffer[56+3*i] & 0xFC) | ((buffer[55+3*i] & 0xC0) >> 6);
	  tmp2 = ((buffer[55+3*i] & 0x3C) << 2) | ( (buffer[54+3*i] & 0xF0) >> 4);
	  tmp3 = (buffer[54+3*i] & 0x0C) << 6;
		spi_chip_select(LOW);
		
	  spi_write(0x72);
		spi_write(tmp1);
	  spi_write(tmp2);
		spi_write(tmp3);
		spi_chip_select(HIGH);
   }
#endif
    
#if defined (CONFIG_RALINK_MT7620) && defined (CONFIG_FB_MEDIATEK_ILITEK)    
  
	for (i=0; i < 240*320 ; i++){
		spi_write(buffer[56+3*i]);
	  spi_write(buffer[55+3*i]);
		spi_write(buffer[54+3*i]);
	}
#endif


#if defined (CONFIG_RALINK_MT7621) && defined (CONFIG_FB_MEDIATEK_TRULY)    
  
  	 for (i=0; i < 240*320; i++){
  	  spi_send(&buffer[54 + 3*i]);
	 	 }
#endif

#if defined (CONFIG_RALINK_MT7621) && defined (CONFIG_FB_MEDIATEK_ILITEK) 

	spin_lock(&flash_lock);	 	
	 	  for (i=0; i < 6400; i++){
  	bbu_mb_spic_trans(NO_USE, NO_USE, &buffer[54 + 36*i], NO_USE, NO_USE, NO_USE, LCD_USE);
	}
	spin_unlock(&flash_lock);
#endif


	set_fs(orgfs);
	kfree(buffer);
}	

/* useful functions */
unsigned char *buffer_mmap;
static int mediatekfb_mmap(struct fb_info *info, struct vm_area_struct *vma)
{
  int nRet;
	unsigned long size = vma->vm_end-vma->vm_start;

	buffer_mmap = kmalloc(size, GFP_ATOMIC);
	memset(buffer_mmap, 0, size);
	nRet = remap_pfn_range(vma, vma->vm_start, virt_to_phys((void *)buffer_mmap) >> PAGE_SHIFT,  size, vma->vm_page_prot);
	
	if( nRet != 0 )
	{
		printk("memory map failed\n");
		return -EIO;
	}
	return 0;
}
int spilcdinit = 0;
//static void start_send_image(struct fb_info *info, const struct fb_image *image)
static void mediatekfb_imgeblit(void)
{
	int i;
	if(spilcdinit == 0){			
		spi_lcd_master_init();
		spilcdinit = 1;
	}
	
#if defined (CONFIG_RALINK_MT7620) && defined (CONFIG_FB_MEDIATEK_TRULY)
	u8 tmp1, tmp2, tmp3; 
	 spi_chip_select(HIGH);
	 //time1=jiffies;
	for (i=0; i < 240*320 ; i++){
		tmp1 = (buffer_mmap[56+3*i] & 0xFC) | ((buffer_mmap[55+3*i] & 0xC0) >> 6);
	  tmp2 = ((buffer_mmap[55+3*i] & 0x3C) << 2) | ( (buffer_mmap[54+3*i] & 0xF0) >> 4);
	  tmp3 = (buffer_mmap[54+3*i] & 0x0C) << 6;
	  //tmp3 = ((buffer_mmap[54+3*i] & 0x0F) << 4) | 0x0F;
	 
		spi_chip_select(LOW);
		
	  spi_write(0x72);
		spi_write(tmp1);
	  spi_write(tmp2);
		spi_write(tmp3);
		spi_chip_select(HIGH);
	}

  //time2=jiffies;
  //printk("cost=%d\n", time2-time1); 
	kfree(buffer_mmap);
#endif

#if defined (CONFIG_RALINK_MT7620) && defined (CONFIG_FB_MEDIATEK_ILITEK)
		spi_write_fifo(buffer_mmap);
		kfree(buffer_mmap);
#endif

#if defined (CONFIG_RALINK_MT7621) && defined (CONFIG_FB_MEDIATEK_TRULY)
		 spin_lock(&flash_lock);
  	 for (i=0; i < 240*320; i++){
  	  bbu_spic_trans(NO_USE, NO_USE, &buffer_mmap[54 + 3*i], NO_USE, NO_USE, NO_USE, TRULY_USE);
	 	 }  	 
	 	 spin_unlock(&flash_lock);	
		kfree(buffer_mmap);
#endif

#if defined (CONFIG_RALINK_MT7621) && defined (CONFIG_FB_MEDIATEK_ILITEK)
/*
	 for (i=0; i < 6400; i++){
  	  lcd_mb_spic_trans(&buffer_mmap[54 + 36*i]);
	 }*/
		spin_lock(&flash_lock);
  for (i=0; i < 6400; i++){
  	bbu_mb_spic_trans(NO_USE, NO_USE, &buffer_mmap[54 + 36*i], NO_USE, NO_USE, NO_USE, LCD_USE);
	}
	spin_unlock(&flash_lock);
	 kfree(buffer_mmap);
#endif

	

}

static int mediatekfb_ioctl(struct fb_info *fbi,
		unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case FBIOIMAGEBLIT: //kick off display
    mediatekfb_imgeblit();
		break;
	default:
		printk("spi_drv: command format error\n");
	}

	return 0;
}
static struct fb_ops mediatekfb_ops = {
	.owner		= THIS_MODULE,
	.fb_read        = NULL,
	.fb_write       = NULL,
	.fb_set_par	= NULL,
	.fb_blank	= NULL,
	.fb_setcolreg	= NULL,
	.fb_fillrect	= cfb_fillrect,
	.fb_copyarea	= cfb_copyarea,
	.fb_imageblit	= cfb_imageblit,
	.fb_mmap	= mediatekfb_mmap,
	.fb_ioctl	= mediatekfb_ioctl,
};


static char driver_name[] = "mediatekfb";

static int __devinit mediatekfb_probe(struct platform_device *dev)
{
  struct fb_info *fbinfo;
	int ret;
	printk("************mediatekfb_probe*******************\n");
	fbinfo = framebuffer_alloc(sizeof(u32) * 256, &dev->dev);
  fbinfo->fbops		    = &mediatekfb_ops;
  strcpy(fbinfo->fix.id, driver_name);
	ret = register_framebuffer(fbinfo);
	if (ret < 0) {
		printk(KERN_ERR "Failed to register framebuffer device: %d\n",
			ret);
	}
	lcd_init();
  /*
  	 	for (i=0; i<240*320; i++){
  spi_chip_select(LOW);	
	spi_write(0x72);
	spi_write(0x00);
	spi_write(0x0F);
	spi_write(0XF0);
	spi_chip_select(HIGH);
	}
	for (i=0; i<240*320; i++){
  spi_chip_select(LOW);	
	spi_write(0x72);
	spi_write(0xFF);
	spi_write(0x00);
	spi_write(0X00);
	spi_chip_select(HIGH);
	}*/
	//ilitek_lcd_img_send();

	lcd_img_send_logo();

	return ret;
}


/*
 *  Cleanup
 */
static int __devexit mediatekfb_remove(struct platform_device *pdev)
{
	#if(0)
	struct fb_info *fbinfo = platform_get_drvdata(pdev);
	struct mediatekfb_info *info = fbinfo->par;
	int irq;

	unregister_framebuffer(fbinfo);
	s3c2410fb_cpufreq_deregister(info);

	s3c2410fb_lcd_enable(info, 0);
	msleep(1);

	s3c2410fb_unmap_video_memory(fbinfo);

	if (info->clk) {
		clk_disable(info->clk);
		clk_put(info->clk);
		info->clk = NULL;
	}

	irq = platform_get_irq(pdev, 0);
	free_irq(irq, info);

	iounmap(info->io);

	release_resource(info->mem);
	kfree(info->mem);

	platform_set_drvdata(pdev, NULL);
	framebuffer_release(fbinfo);
  #endif
	return 0;
}

#ifdef CONFIG_PM

/* suspend and resume support for the lcd controller */
static int mediatekfb_suspend(struct platform_device *dev, pm_message_t state)
{
	#if(0)
	struct fb_info	   *fbinfo = platform_get_drvdata(dev);
	struct s3c2410fb_info *info = fbinfo->par;

	s3c2410fb_lcd_enable(info, 0);

	/* sleep before disabling the clock, we need to ensure
	 * the LCD DMA engine is not going to get back on the bus
	 * before the clock goes off again (bjd) */

	msleep(1);
	clk_disable(info->clk);
  #endif
	return 0;
}

static int mediatekfb_resume(struct platform_device *dev)
{
	#if(0)
	struct fb_info	   *fbinfo = platform_get_drvdata(dev);
	struct s3c2410fb_info *info = fbinfo->par;

	clk_enable(info->clk);
	msleep(1);

	s3c2410fb_init_registers(fbinfo);

	/* re-activate our display after resume */
	s3c2410fb_activate_var(fbinfo);
	s3c2410fb_blank(FB_BLANK_UNBLANK, fbinfo);
  #endif
	return 0;
}

#else
#define mediatekfb_suspend NULL
#define mediatekfb_resume  NULL
#endif

static struct platform_driver mediatekfb_driver = {
	.probe		= mediatekfb_probe,
	.suspend	= mediatekfb_suspend,
	.resume		= mediatekfb_resume,
	.driver		= {
		.name	= "mediatekfb",
	},
};

static struct platform_device *mediatekfb_device;
int __init mediatekfb_init(void)
{

	int ret;
	printk("*************mediatek_init**************\n");


	ret = platform_driver_register(&mediatekfb_driver);

	if (!ret) {
		mediatekfb_device = platform_device_register_simple("mediatekfb", 0,
								NULL, 0);
    
		if (IS_ERR(mediatekfb_device)) {
			printk("**********register wrong******************\n");
			platform_driver_unregister(&mediatekfb_driver);
			ret = PTR_ERR(mediatekfb_device);
		}
	}
		return ret;
}

static void __exit mediatekfb_cleanup(void)
{
	platform_driver_unregister(&mediatekfb_driver);
}

module_init(mediatekfb_init);
module_exit(mediatekfb_cleanup);

MODULE_AUTHOR("Harry Huang");
MODULE_DESCRIPTION("Framebuffer driver for the APSOC ILITEK LCD");
MODULE_LICENSE("GPL");
MODULE_ALIAS("MT7620");

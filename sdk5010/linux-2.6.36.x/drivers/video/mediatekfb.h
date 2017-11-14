/*
 * Copyright (C) 2013 Noralf Tronnes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __LINUX_FBTFT_H
#define __LINUX_FBTFT_H

#include <linux/fb.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/timer.h>
#include <asm/rt2880/rt_mmap.h>


/**
 * struct fbtft_gpio - Structure that holds one pinname to gpio mapping
 * @name: pinname (reset, dc, etc.)
 * @gpio: GPIO number
 *
 */
#define phys_to_bus(a) (a & 0x1FFFFFFF)

#define PHYS_TO_K1(physaddr) KSEG1ADDR(physaddr)
#define sysRegRead(phys)        \
        (*(volatile unsigned int *)PHYS_TO_K1(phys))

#define sysRegWrite(phys, val)  \
        ((*(volatile unsigned int *)PHYS_TO_K1(phys)) = (val))

#define RT2880_REG(x)		(*((volatile u32 *)(x)))
//mediatek lcd used register
#define RALINK_SYSCTL_ADDR		RALINK_SYSCTL_BASE	// system control
#define RALINK_REG_GPIOMODE		(RALINK_SYSCTL_ADDR + 0x60)
#define RT2880_RSTCTRL_REG		(RALINK_SYSCTL_BASE+0x34)
#define RSTCTRL_SPI_RESET		RALINK_SPI_RST
#define RALINK_PRGIO_ADDR		RALINK_PIO_BASE // Programmable I/O
#define RALINK_REG_PIODIR		(RALINK_PRGIO_ADDR + 0x00)
#define RALINK_REG_PIOSET		(RALINK_PRGIO_ADDR + 0x30)
#define RALINK_REG_PIORESET		(RALINK_PRGIO_ADDR + 0x40)

#if defined (CONFIG_RALINK_MT7621)
#define CMD 0
#define DATA 1
#endif

#if defined (CONFIG_RALINK_MT7620)
#define RT2880_SPI_REG_BASE		(RALINK_SPI_BASE)
#define RT2880_SPI0_STAT_REG	(RT2880_SPI_REG_BASE+0x00)
#define RT2880_SPI0_CFG_REG		(RT2880_SPI_REG_BASE+0x10)

#define RT2880_SPI0_CTL_REG		(RT2880_SPI_REG_BASE+0x14)
#define RT2880_SPI0_DATA_REG	(RT2880_SPI_REG_BASE+0x20)

#define RT2880_SPI_FIFO_STATUS (RT2880_SPI_REG_BASE+0x38)

#define RT2880_SPI1_STAT_REG	(RT2880_SPI_REG_BASE+0x40)
#define RT2880_SPI1_CFG_REG		(RT2880_SPI_REG_BASE+0x50)
#define RT2880_SPI1_CTL_REG		(RT2880_SPI_REG_BASE+0x54)
#define RT2880_SPI1_DATA_REG	(RT2880_SPI_REG_BASE+0x60)
#define RT2880_SPI_DATA_DMA	(RT2880_SPI_REG_BASE+0x80)
#define RT2880_SPI_ARB_REG		(RT2880_SPI_REG_BASE+0xf0)


#define RT2880_SPIUSER_REG		(RT2880_SPI_REG_BASE+0x2C)
#define RT2880_SPIADDR_REG		(RT2880_SPI_REG_BASE+0x24)
#define RT2880_SPIMODE_REG		(RT2880_SPI_REG_BASE+0x3c)
#define RT2880_SPIBS_REG		(RT2880_SPI_REG_BASE+0x28)
#define RT2880_SPITXFIFO_REG	(RT2880_SPI_REG_BASE+0x30)
#define RT2880_SPIRXFIFO_REG	(RT2880_SPI_REG_BASE+0x34)
#define RT2880_SPIFIFOSTAT_REG	(RT2880_SPI_REG_BASE+0x38)
#define RT2880_SPICTL_REG		(RT2880_SPI_REG_BASE+0x14)
#define RT2880_SPICFG_REG		(RT2880_SPI_REG_BASE+0x10)
#define RT2880_SPIDATA_REG		(RT2880_SPI_REG_BASE+0x20)



#define RT2880_SPI1_CTL_REG		(RT2880_SPI_REG_BASE+0x54)
#define RT2880_SPI_DMA			(RT2880_SPI_REG_BASE+ 0x80)
#define RT2880_SPI_ARB_REG		(RT2880_SPI_REG_BASE+0xf0)
#define SPICFG_SPICLKPOL		(1<<6)		/* spi clk*/

/* SPICTL register bit field */
#define SPICTL_HIZSDO				(1<<3)
#define SPICTL_STARTWR				(1<<2)
#define SPICTL_STARTRD				(1<<1)
#define SPICTL_SPIENA_NEGATE		(~(1<<0))
#define SPICTL_SPIENA_ASSERT		(1<<0)
/* SPICFG register bit field */
#define SPICFG_LSBFIRST				(0<<8)
#define SPICFG_MSBFIRST				(1<<8)

#define SPICFG_HIZSPI             (1<<3)
#define SPICFG_RXCLKEDGE_FALLING	(1<<5)		/* rx on the falling edge of the SPICLK signal */
#define SPICFG_TXCLKEDGE_FALLING	(1<<4)		/* tx on the falling edge of the SPICLK signal */

#define SPICFG_SPICLK_DIV2			(0<<0)		/* system clock rat / 2  */
#define SPICFG_SPICLK_DIV4			(1<<0)		/* system clock rat / 4  */
#define SPICFG_SPICLK_DIV8			(2<<0)		/* system clock rat / 8  */
#define SPICFG_SPICLK_DIV16			(3<<0)		/* system clock rat / 16  */
#define SPICFG_SPICLK_DIV32			(4<<0)		/* system clock rat / 32  */
#define SPICFG_SPICLK_DIV64			(5<<0)		/* system clock rat / 64  */
#define SPICFG_SPICLK_DIV128		(6<<0)		/* system clock rat / 128 */

#define SPICFG_SPICLKPOL		(1<<6)		/* spi clk*/

#define SPICFG_ADDRMODE			(1 << 12)
#define SPICFG_RXENVDIS			(1<<11)
#define SPICFG_RXCAP			(1<<10)
#define SPICFG_SPIENMODE		(1<<9)

/* SPICTL register bit field */
#define SPICTL_HIZSDO				(1<<3)
#define SPICTL_STARTWR				(1<<2)
#define SPICTL_STARTRD				(1<<1)
#define SPICTL_SPIENA_LOW			(0<<0)		/* #cs low active */
#define SPICTL_SPIENA_HIGH			(1<<0)

/* SPI COMMAND MODE */
#define SPICTL_START				(1<<4)
#define SPIFIFO_TX_FULL				(1 << 17)
#define SPIFIFO_RX_EMPTY			(1 << 18)
#define SPIINT_SPIDONE				(1<<0)
#define SPIINT_ILLSPI				(1<<1)
#define SPIINT_RX_EMPTY_RD			(1<<2)
#define SPIINT_TX_FULL_WR			(1<<3)
#define SPIINT_DMA_EMPTY_RD			(1<<4)
#define SPIINT_DMA_FULL_WR			(1<<5)
/* SPIARB register bit field */
#define SPIARB_ARB_EN			(1<<31)
/* SPI USER MODE */
#define SPIUSR_SINGLE				0x1
#define SPIUSR_DUAL					0x2
#define SPIUSR_QUAD					0x4
#define SPIUSR_NO_DATA				0x0
#define SPIUSR_READ_DATA			0x1
#define SPIUSR_WRITE_DATA			0x2
#define SPIUSR_NO_DUMMY				0x0
#define SPIUSR_ONE_DUMMY			0x1
#define SPIUSR_TWO_DUMMY			0x2
#define SPIUSR_THREE_DUMMY			0x3
#define SPIUSR_NO_MODE				0x0
#define SPIUSR_ONE_MODE				0x1
#define SPIUSR_NO_ADDR				0x0
#define SPIUSR_ONE_BYTE_ADDR		0x1
#define SPIUSR_TWO_BYTE_ADDR		0x2
#define SPIUSR_THREE_BYTE_ADDR		0x3
#define SPIUSR_FOUR_BYTE_ADDR		0x4
#define SPIUSR_NO_INSTRU			0x0
#define SPIUSR_ONE_INSTRU			0x1
#define IS_BUSY0				(RT2880_REG(RT2880_SPI0_STAT_REG) & 0x01)
#define SPI_LCD_CMD    0
#define SPI_LCD_DATA   1
#define RALINK_REG_PIO7140INT		(RALINK_PRGIO_ADDR + 0x60)
#define RALINK_REG_PIO7140EDGE		(RALINK_PRGIO_ADDR + 0x64)
#define RALINK_REG_PIO7140RENA		(RALINK_PRGIO_ADDR + 0x68)
#define RALINK_REG_PIO7140FENA		(RALINK_PRGIO_ADDR + 0x6C)
#define RALINK_REG_PIO7140DATA		(RALINK_PRGIO_ADDR + 0x70)
#define RALINK_REG_PIO7140DIR		(RALINK_PRGIO_ADDR + 0x74)
#define RALINK_REG_PIO7140POL		(RALINK_PRGIO_ADDR + 0x78)
#define RALINK_REG_PIO7140SET		(RALINK_PRGIO_ADDR + 0x7C)
#define RALINK_REG_PIO7140RESET		(RALINK_PRGIO_ADDR + 0x80)
#define RALINK_REG_PIO7140TOGGLE	(RALINK_PRGIO_ADDR + 0x84)
#define HIGH	1
#define LOW	0
#if defined(CONFIG_RALINK_SPI_CS0_HIGH_ACTIVE)
#define SPIARB_SPI0_ACTIVE_MODE		1
#else
#define SPIARB_SPI0_ACTIVE_MODE		0
#endif
#if defined(CONFIG_RALINK_SPI_CS1_HIGH_ACTIVE)
#define SPIARB_SPI1_ACTIVE_MODE		1
#else
#define SPIARB_SPI1_ACTIVE_MODE		0
#endif

//#define IS_BUSY				(RT2880_REG(spireg[SPISTAT]) & 0x01)
#define IS_BUSY				(RT2880_REG(RT2880_SPI1_STAT_REG) & 0x01)


#define spi_busy_loop 3000
#define max_ee_busy_loop 500

#define	SPISTAT		0
#define SPICFG		1
#define SPICTL		2
#define SPIDATA		3	

/* SPIARB register bit field */
#define SPIARB_ARB_EN			(1<<31)
#define SPIARB_CS1CTL			(1<<16)
#endif









#endif /* __LINUX_FBTFT_H */

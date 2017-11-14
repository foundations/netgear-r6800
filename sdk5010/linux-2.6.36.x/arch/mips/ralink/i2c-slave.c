#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/i2c/at24.h>
#include <linux/i2c.h>
#include <asm/mach-ralink/rt_mmap.h>

#define I2S_WM8960 (0X34>>0)
#define ALSA_WM8960 (0X34>>1)
#define I2S_WM8751 (0x36>>0)
#define I2S_WM8750 (0x36>>1)

static struct at24_platform_data eeprom_data = {
	.byte_len	= 1024 / 8,
	.page_size	= 8,
};

static struct i2c_board_info mtk_i2c_info[] __initconst =  {

	{
		I2C_BOARD_INFO("at24c01", 0x50),
		.platform_data = &eeprom_data
	},
	{
		I2C_BOARD_INFO("codec_wm8960", I2S_WM8960),
	},
	{
		I2C_BOARD_INFO("wm8960", ALSA_WM8960),
	},
	{
		I2C_BOARD_INFO("codec_wm8751", I2S_WM8751),
	},
	{
		I2C_BOARD_INFO("codec_wm8750", I2S_WM8750),
	},

};

static struct resource i2c_resources[] = {
	{
		.start		= RALINK_I2C_BASE,
		.end		= RALINK_I2C_BASE + 255,
		.flags		= IORESOURCE_MEM,
	},
};

static struct platform_device mtk_i2c_device = {
	.name		= "MTK-I2C",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(i2c_resources),
	.resource	= i2c_resources,
};

int __init mtk_i2c_register(void)
{

	platform_device_register(&mtk_i2c_device);
	i2c_register_board_info(0, mtk_i2c_info, ARRAY_SIZE(mtk_i2c_info));

	return 0;
}
arch_initcall(mtk_i2c_register);

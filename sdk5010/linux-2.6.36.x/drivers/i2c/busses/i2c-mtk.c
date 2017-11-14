#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/io.h>

#include <asm/rt2880/rt_mmap.h>
#include "i2c-mtk.h"

static int mtk_id;

static void mtk_i2c_reset(void)
{
	u32 val;

    // reset i2c block 
    val = RT2880_REG(RT2880_RSTCTRL_REG);
	val = val | RALINK_I2C_RST;
    RT2880_REG(RT2880_RSTCTRL_REG) = val;

	val = val & ~RALINK_I2C_RST;
    RT2880_REG(RT2880_RSTCTRL_REG) = val;
} 

static void mtk_i2c_master_init(struct i2c_msg *msg)
{
	u32 i;
#if defined (CONFIG_RALINK_MT7621)
	RT2880_REG(RALINK_SYSCTL_BASE + 0x60) &= ~0x4;  //MT7621 bit2
	udelay(500);
#elif defined (CONFIG_RALINK_MT7628)
	RT2880_REG(RALINK_SYSCTL_BASE + 0x60) &= ~(0x3<<20);//~0x3000;  //MT7628 bit20
	udelay(500);
#else
	RT2880_REG(RALINK_SYSCTL_BASE+0x60) &= ~0x1;	
#endif
	mtk_i2c_reset();
	udelay(500);
	
	RT2880_REG(RT2880_I2C_CONFIG_REG) = I2C_CFG_DEFAULT;

#if defined (CONFIG_RALINK_MT7621) || defined (CONFIG_RALINK_MT7628)
    i = 1 << 31; // the output is pulled hight by SIF master 0
    i |= 1 << 28; // allow triggered in VSYNC pulse
    i |= CLKDIV_VALUE<<16;//CLKDIV_VALUE << 16; //clk div
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
	RT2880_REG(RT2880_I2C_DEVADDR_REG) = msg->addr;

	/*
	 * Use Address Disabled Transfer Options
	 * because it just support 8-bits, 
	 * ATMEL eeprom needs two 8-bits address
	 */
	RT2880_REG(RT2880_I2C_ADDR_REG) = 0;
}

static int mtk_i2c_wait_tx_done(void)
{
	while(!(RT2880_REG(RT2880_I2C_STATUS_REG) & RALINK_I2C_SDOEMPTY));
	return 0;
}

static int mtk_i2c_wait_rx_done(void)
{
	while(!(RT2880_REG(RT2880_I2C_STATUS_REG) & RALINK_I2C_DATARDY));
	return 0;
}

static int mtk_i2c_wait(void)
{

	while(RT2880_REG(RT2880_I2C_STATUS_REG) & RALINK_I2C_BUSY);

	return 0;
}

static int mtk_i2c_handle_msg(struct i2c_adapter *i2c_adap, struct i2c_msg* msg) 
{
	int i=0, j=0, pos=0;
	int nblock = msg->len / READ_BLOCK;
        int rem = msg->len % READ_BLOCK;

	if (msg->flags & I2C_M_TEN) {
		printk("10 bits addr not supported\n");
		return -EINVAL;
	}

	if ((msg->flags&I2C_M_RD)) {
		for(i=0; i< nblock; i++) {
			mtk_i2c_wait();
			RT2880_REG(RT2880_I2C_BYTECNT_REG) = READ_BLOCK-1;
			RT2880_REG(RT2880_I2C_STARTXFR_REG) = READ_CMD;
			for(j=0; j< READ_BLOCK; j++) {
				mtk_i2c_wait_rx_done();
				msg->buf[pos++] = RT2880_REG(RT2880_I2C_DATAIN_REG);
			}
		}

		mtk_i2c_wait();
		RT2880_REG(RT2880_I2C_BYTECNT_REG) = rem-1;
		RT2880_REG(RT2880_I2C_STARTXFR_REG) = READ_CMD;
		for(i=0; i< rem; i++) {
			mtk_i2c_wait_rx_done();
			msg->buf[pos++] = RT2880_REG(RT2880_I2C_DATAIN_REG);
		}
	} else {
		rem = msg->len;
		mtk_i2c_wait();
		RT2880_REG(RT2880_I2C_BYTECNT_REG) = rem-1;
		for(i=0; i< rem; i++) {
			RT2880_REG(RT2880_I2C_DATAOUT_REG) = msg->buf[i];
			if(i==0)
				RT2880_REG(RT2880_I2C_STARTXFR_REG) = WRITE_CMD;		
			mtk_i2c_wait_tx_done();
		}
	}	
	return 0;
}

/*
 * master_xfer() - main read/write entry
 */
static int 
mtk_i2c_master_xfer(struct i2c_adapter *i2c_adap, struct i2c_msg *msgs, 
				int num)
{
	int im = 0;
	int ret = 0;
 
	mtk_i2c_wait();
	mtk_i2c_master_init(msgs);
	for (im = 0; ret == 0 && im != num; im++) {
		ret = mtk_i2c_handle_msg(i2c_adap, &msgs[im]);
	}

	if(ret)
		return ret;
	return im;   
}

static u32 mtk_i2c_func(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL;
}

static const struct i2c_algorithm mtk_i2c_algo = {
	.master_xfer	= mtk_i2c_master_xfer,
	.smbus_xfer    = NULL,
	.functionality	= mtk_i2c_func,
};

static int mtk_i2c_adapter_remove(struct platform_device *pdev)
{
	struct i2c_adapter *padapter = platform_get_drvdata(pdev);
	struct i2c_algo_mtk_data *adapter_data = 
		(struct i2c_algo_mtk_data *)padapter->algo_data;

	release_mem_region(RALINK_I2C_BASE, 256);
	kfree(adapter_data);
	kfree(padapter);

	platform_set_drvdata(pdev, NULL);

	return 0;
}

static int mtk_i2c_adapter_probe(struct platform_device *pdev)
{
	struct resource *res;
	int ret;
	struct i2c_adapter *mtk_adapter;
	struct i2c_algo_mtk_data *adapter_data;


	mtk_adapter = kzalloc(sizeof(struct i2c_adapter), GFP_KERNEL);
	if (!mtk_adapter) {
		ret = -ENOMEM;
		goto out;
	}

	adapter_data = kzalloc(sizeof(struct i2c_algo_mtk_data), GFP_KERNEL);
	if (!adapter_data) {
		ret = -ENOMEM;
		goto free_adapter;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		ret = -ENODEV;
		goto free_both;
	}

	if (!request_mem_region(RALINK_I2C_BASE, resource_size(res), pdev->name)) {
		ret = -EBUSY;
		goto free_both;
	}

	adapter_data->id = mtk_id++;
	memcpy(mtk_adapter->name, pdev->name, strlen(pdev->name));
	mtk_adapter->owner = THIS_MODULE;
	mtk_adapter->class = I2C_CLASS_HWMON | I2C_CLASS_SPD;
	mtk_adapter->dev.parent = &pdev->dev;
	mtk_adapter->nr = pdev->id;
	mtk_adapter->timeout = HZ;
	mtk_adapter->algo = &mtk_i2c_algo;

	init_waitqueue_head(&adapter_data->waitq);
	spin_lock_init(&adapter_data->lock);

	platform_set_drvdata(pdev, mtk_adapter);
	mtk_adapter->algo_data = adapter_data;

	ret=i2c_add_numbered_adapter(mtk_adapter);

	return 0;

free_both:
	kfree(adapter_data);
free_adapter:
	kfree(mtk_adapter);
out:
	return ret;
}


static struct platform_driver mtk_i2c_driver = {
	.probe		= mtk_i2c_adapter_probe,
	.remove		= mtk_i2c_adapter_remove,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "MTK-I2C",
	},
};

static int __init i2c_mtk_init (void)
{
	return platform_driver_register(&mtk_i2c_driver);
}

static void __exit i2c_mtk_exit (void)
{
	printk("MTK I2C Exit\n");
	platform_driver_unregister(&mtk_i2c_driver);
}

module_init (i2c_mtk_init);
module_exit (i2c_mtk_exit);


MODULE_DESCRIPTION("MTK I2C host driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:MTK-I2C");

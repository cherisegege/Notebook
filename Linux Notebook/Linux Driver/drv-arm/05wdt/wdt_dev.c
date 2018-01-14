/****************************
 * 为三总线硬件准备并注册对应的platform_device
 * 本例针对看门狗
 * 在pdev中，要封装设备的硬件信息
 * Author: zht
 * Date: 2016-11-13
 ****************************/
#include <linux/module.h>
#include <mach/irqs.h> //定义片内设备的中断号
#include <linux/platform_device.h> //platform_device

//定义设备的硬件信息
#define WDT_BASE	0x10060000	//物理基地址
#define WDT_SIZE	0x10		//4个寄存器一共占16字节

//准备resource数组
static struct resource wdt_res[] = {
	[0] = {
		.start	= WDT_BASE,
		.end	= WDT_BASE + WDT_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_WDT,
		.end	= IRQ_WDT,
		.flags	= IORESOURCE_IRQ,
	},
};

//如果设备支持注销，则应该提供release方法
//device->release()
static void wdt_release(struct device *tmp)
{
	struct platform_device *pdev = container_of(tmp, struct platform_device, dev);
	printk("%s(%d) : release()\n", pdev->name, pdev->id);
}


//准备platform_device
static struct platform_device wdt_pdev = {
	.name	= "e4412-wdt",
	.id		= -1,
	.num_resources = ARRAY_SIZE(wdt_res),
	.resource = wdt_res,
	.dev	= {
		.release = wdt_release,
	},
};

static int __init my_init(void)
{
	//注册platform_device
	return platform_device_register(&wdt_pdev);
}

static void __exit my_exit(void)
{
	//注销platform_device
	platform_device_unregister(&wdt_pdev);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


/****************************
 * 为三总线硬件准备并注册对应的platform_device
 * 本例针对RTC
 * 在pdev中，要封装设备的硬件信息
 * Author: zht
 * Date: 2016-11-12
 ****************************/
#include <linux/module.h>
#include <mach/irqs.h> //定义片内设备的中断号
#include <linux/platform_device.h> //platform_device

//定义设备的硬件信息
#define RTC_BASE	0x10070000	//物理基地址
#define RTC_SIZE	0x1000		//4K，一页

//准备resource数组
static struct resource rtc_res[] = {
	[0] = {
		.start	= RTC_BASE,
		.end	= RTC_BASE + RTC_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_RTC_ALARM,
		.end	= IRQ_RTC_ALARM,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.start	= IRQ_RTC_TIC,
		.end	= IRQ_RTC_TIC,
		.flags	= IORESOURCE_IRQ,
	},
};

//如果设备支持注销，则应该提供release方法
//device->release()
static void rtc_release(struct device *tmp)
{
	struct platform_device *pdev = container_of(tmp, struct platform_device, dev);
	printk("%s(%d) : release()\n", pdev->name, pdev->id);
}


//准备platform_device
static struct platform_device rtc_pdev = {
	.name	= "e4412-rtc",
	.id		= -1,
	.num_resources = ARRAY_SIZE(rtc_res),
	.resource = rtc_res,
	.dev	= {
		.release = rtc_release,
	},
};

static int __init my_init(void)
{
	//注册platform_device
	return platform_device_register(&rtc_pdev);
}

static void __exit my_exit(void)
{
	//注销platform_device
	platform_device_unregister(&rtc_pdev);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


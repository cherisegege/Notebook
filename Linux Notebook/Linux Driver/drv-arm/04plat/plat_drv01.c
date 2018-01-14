/******************************
 * 准备并注册platform_driver
 * platform_driver是在原有驱动的外面套了一层外壳，将以前模块入口/出口中的工作放到pdev->probe()和pdrv->remove()中完成
 * Author: zht
 * Date: 2016-11-12
 ******************************/
#include <linux/module.h>
//根据驱动要求，可以包括<linux/fs.h>/<linux/interrupt.h>等对应的头文件
#include <linux/platform_device.h> //platform_driver

//驱动可以实现char驱动
//准备xx_open / xx_release / xx_read / xx_write / xx_ioctl
//准备file_operations

//驱动可以中断处理以及下半部
//xx_service()

//驱动还可以支持proc文件，应提供对应的操作函数

//驱动如果支持设备模型，增应该根据设备的总线类型，提供对应的操作函数集
static int my_probe(struct platform_device *pdev)
{
	struct resource *res1, *res2, *res3;
	//在probe函数中，完成以前模块入口中的驱动注册等工作；
	printk("%s(%d) : probe()\n", pdev->name, pdev->id);

	//从pdev中获得硬件信息，并打印
	//第三个参数为同类型资源的编号，不是数组下标
	res1 = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	res2 = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	res3 = platform_get_resource(pdev, IORESOURCE_IRQ, 1);

	if (res1==NULL && res2==NULL && res3==NULL) {
		printk("Cannot get resource\n");
		return -1;
	}
	
	if (res1)
		printk("%s(%d) : phyaddr from %#x to %#x\n", pdev->name, pdev->id, res1->start, res1->end);
	if (res2)
		printk("%s(%d) : irq num %d\n", pdev->name, pdev->id, res2->start);
	if (res3)
		printk("%s(%d) : irq num %d\n", pdev->name, pdev->id, res3->start);
		
	return 0;
}

static int my_remove(struct platform_device *pdev)
{
	//在remove函数中，完成以前模块入口中的注销工作；
	printk("%s(%d) : remove()\n", pdev->name, pdev->id);
	return 0;
}

//支持设备的列表，列出驱动可以支持哪些设备
//数组以NULL作为结尾
static struct platform_device_id my_ids[] = {
	{ "e4412-wdt", 0 },
	{ "e4412-rtc", 0 },
	{ "e4412-adc", 0 },
	{},
};

static struct platform_driver my_pdrv = {
	.probe = my_probe,
	.remove = my_remove,
	.id_table = my_ids,
	.driver = {
		.name = "plat-drvs",
	},
};


static int __init my_init(void)
{
	//注册pdrv
	return platform_driver_register(&my_pdrv);
}

static void __exit my_exit(void)
{
	//注销pdrv
	platform_driver_unregister(&my_pdrv);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


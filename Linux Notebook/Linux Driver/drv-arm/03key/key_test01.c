/******************************
 * 按键的中断测试例子1
 * 测试中断的触发模式(边沿触发 or 电平触发)
 * 按键1采用边沿模式触发；按键2采用电平模式触发；
 * 按键1～4对应的GPIO为GPX3_2~GPX3_5;
 * 中断的处理是完全在内核进行的，和用户态无关；
 * Author: zht
 * Date: 2016-10-23
 ******************************/
#include <linux/module.h>
#include <linux/interrupt.h> //request_irq & free_irq
#include <linux/gpio.h> //gpio_to_irq 
#include <mach/gpio.h> //gpio号

//用全局变量来记录中断号，中断的发生次数等
static int key1_irq, key2_irq;
static int key1_cnt = 0, key2_cnt = 0;

//定义中断的处理函数
static irqreturn_t
key1_service(int irq, void *dev_id)
{
	key1_cnt++;
	printk("key1(%d): %d times\n", irq, key1_cnt);
	return IRQ_HANDLED; //IRQ_NONE
}

static irqreturn_t
key2_service(int irq, void *dev_id)
{
	key2_cnt++;
	printk("key2(%d): %d times\n", irq, key2_cnt);
	return IRQ_HANDLED; //IRQ_NONE
}

static int __init my_init(void)
{
	int flags, ret;

	//1.初始化按键1的信息，并注册中断处理
	flags = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING;
	key1_irq = gpio_to_irq(EXYNOS4_GPX3(2));

	ret = request_irq(key1_irq, //中断号
			key1_service, //处理函数
			flags, //中断标志，片内中断写0即可
			"mykey1", //处理函数的名字
			NULL); //传递给中断处理函数的参数
	if (ret) {
		printk("Cannot request irq %d\n", key1_irq);
		return -1;
	}

	//2.初始化按键2的信息，并注册中断处理
	flags = IRQF_TRIGGER_LOW;
	key2_irq = gpio_to_irq(EXYNOS4_GPX3(3));

	ret = request_irq(key2_irq, //中断号
			key2_service, //处理函数
			flags, //中断标志，片内中断写0即可
			"mykey2", //处理函数的名字
			NULL); //传递给中断处理函数的参数
	if (ret) {
		printk("Cannot request irq %d\n", key2_irq);
		free_irq(key1_irq, NULL);
		return -1;
	}
	return 0;
}

static void __exit my_exit(void)
{
	free_irq(key1_irq, NULL);
	free_irq(key2_irq, NULL);
}
module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZHT");


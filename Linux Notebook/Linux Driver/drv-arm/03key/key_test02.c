/******************************
 * 按键的中断测试例子2
 * 测试中断的屏蔽(disable_irq()和enable_irq())
 * 按键1用于产生中断(电平触发)；
 * 按键2按下调用disable_irq，屏蔽按键1的中断；
 * 按键3按下调用enable_irq，解除对按键1中断的屏蔽；
 * 需要先disable再enable;
 * 按键1～4对应的GPIO为GPX3_2~GPX3_5;
 * Author: zht
 * Date: 2016-10-23
 ******************************/
#include <linux/module.h>
#include <linux/interrupt.h> //request_irq & free_irq
#include <linux/gpio.h> //gpio_to_irq 
#include <mach/gpio.h> //gpio号

//用全局变量来记录中断号，中断的发生次数等
static int key1_irq, key2_irq, key3_irq;
static int key1_cnt = 0;
static int irq_disabled = 0; //中断屏蔽的计数

//定义中断的处理函数
static irqreturn_t
key1_service(int irq, void *dev_id)
{
	key1_cnt++;
	printk("key1: %d times\n", key1_cnt);
	return IRQ_HANDLED; //IRQ_NONE
}

//按下按键2，屏蔽按键1的中断
static irqreturn_t
key2_service(int irq, void *dev_id)
{
	irq_disabled++;
	disable_irq(key1_irq);
	printk("key1: irq mask %d times\n", irq_disabled);
	return IRQ_HANDLED; //IRQ_NONE
}

//按下按键3，解除按键1的中断屏蔽
static irqreturn_t
key3_service(int irq, void *dev_id)
{
	if (irq_disabled == 0) {
		printk("key1: irq unmask\n");
		return IRQ_HANDLED;
	}
	irq_disabled--;
	enable_irq(key1_irq);
	if (irq_disabled == 0)
		printk("key1: irq unmask\n");
	else
		printk("key1: irq mask %d times\n", irq_disabled);
	return IRQ_HANDLED; //IRQ_NONE
}


static int __init my_init(void)
{
	int flags, ret;

	//1.初始化按键1的信息，并注册中断处理
	flags = IRQF_TRIGGER_LOW;
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
	flags = IRQF_TRIGGER_FALLING;
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

	//3.初始化按键3的信息，并注册中断处理
	flags = IRQF_TRIGGER_FALLING;
	key3_irq = gpio_to_irq(EXYNOS4_GPX3(4));

	ret = request_irq(key3_irq, //中断号
			key3_service, //处理函数
			flags, //中断标志，片内中断写0即可
			"mykey3", //处理函数的名字
			NULL); //传递给中断处理函数的参数
	if (ret) {
		printk("Cannot request irq %d\n", key3_irq);
		free_irq(key1_irq, NULL);
		free_irq(key2_irq, NULL);
		return -1;
	}

	return 0;
}

static void __exit my_exit(void)
{
	free_irq(key1_irq, NULL);
	free_irq(key2_irq, NULL);
	free_irq(key3_irq, NULL);
}
module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZHT");


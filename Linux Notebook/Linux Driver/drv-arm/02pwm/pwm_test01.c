/*****************************
 * pwm的char驱动测试例子1
 * 根据电路图，蜂鸣器连接的GPIO是GPD0_0；
 * 本例将该GPIO设置为输出，通过输出0/1控制蜂鸣器响或不响
 * 支持write，echo on|off >/dev/mybuzz
 * Author: zht
 * Date: 2016-10-15
 ******************************/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>

//首先确定对应的GPIO号(GPD0_0)
#define BUZZ_IO		EXYNOS4_GPD0(0)

//对应用户态echo on|off >/dev/mybuzz
static ssize_t
buzz_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	char tmp[10] = {0};

	if (copy_from_user(tmp, buf, 3))
		return -EFAULT;

	if (strncmp(tmp, "on", 2)==0) {
		gpio_set_value(BUZZ_IO, 1);
	} else if (strncmp(tmp, "off", 3)==0) {
		gpio_set_value(BUZZ_IO, 0);
	} else {
		printk("only support on|off\n");
		return -1;
	}
	return count;
}

static struct file_operations buzz_fops = {
	.owner = THIS_MODULE,
	.write = buzz_write,
};

static struct miscdevice buzz_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "mybuzz",
	.fops  = &buzz_fops,
};

static int __init my_init(void)
{
	int ret;
	//首先申请gpio
	ret = gpio_request(BUZZ_IO, "mybuzz");
	if (ret) {
		printk("Cannot request gpio %d\n", BUZZ_IO);
		return ret;
	}
	//配置为输出，且默认蜂鸣器不响
	//如果要把GPIO配置为通过定时器来控制输出PWM方波，则GPIO应配置为：
	//s3c_gpio_cfgpin(BUZZ_IO, S3C_GPIO_SFN(2)
	s3c_gpio_cfgpin(BUZZ_IO, S3C_GPIO_OUTPUT);
	gpio_set_value(BUZZ_IO, 0);

	//注册misc设备
	ret = misc_register(&buzz_misc);
	if (ret) {
		gpio_free(BUZZ_IO);
		return ret;
	}
	return 0;
}

static void __exit my_exit(void)
{
	misc_deregister(&buzz_misc);
	gpio_free(BUZZ_IO);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


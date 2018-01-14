/******************************
 * led灯的char驱动测试例子3
 * 用GPIO库来控制GPIO；
 * 用ioctl来控制灯的亮灭等；
 * 用miscdevice替代cdev来注册设备
 * (miscdevice适合于一个驱动一个设备；而cdev适合于一个驱动对应多个设备)
 * 用proc文件获取设备当前信息
 * Author: zht
 * Date: 2016-10-15
 *******************************/
#include <linux/module.h>
//char驱动需要的头文件
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h> 
//proc文件需要的头文件
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
//gpio库需要的头文件
#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>

//定义硬件信息
//用miscdevice注册，主设备号固定为10
#define LED_NUM		4

//定义ioctl命令号
#define LED_TYPE	'L'
#define LED_ON		_IOW(LED_TYPE, 1, int)
#define LED_OFF		_IOW(LED_TYPE, 2, int)
#define LED_ALLON	_IO(LED_TYPE, 3)
#define LED_ALLOFF	_IO(LED_TYPE, 4)

//定义变量
static int led_gpios[LED_NUM]; //记录每个IO的GPIO号

//proc文件的读函数
static int
led_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int i, ret = 0;
	ret += sprintf(page, "====LED当前信息====\n");
	for (i = 0;  i < LED_NUM; i++) {
		ret += sprintf(page+ret, "LED%d: %s\n", \
			i, gpio_get_value(led_gpios[i]) ? "灭" : "亮");
	}
	return ret;
}

//file_operations 

static long
led_ioctl(struct file *filp, unsigned int req, unsigned long arg)
{
	int i;
	switch (req) {
		case LED_ON:
			if (arg > LED_NUM) {
				printk("Only support led 0~%d\n", LED_NUM-1);
				return -1;
			}
			gpio_set_value(led_gpios[arg], 0);
			break;
		case LED_OFF:
			if (arg > LED_NUM) {
				printk("Only support led 0~%d\n", LED_NUM-1);
				return -1;
			}
			gpio_set_value(led_gpios[arg], 1);
			break;
		case LED_ALLON:
			for (i = 0; i < LED_NUM; i++)
				gpio_set_value(led_gpios[i], 0);
			break;
		case LED_ALLOFF:
			for (i = 0; i < LED_NUM; i++)
				gpio_set_value(led_gpios[i], 1);
			break;
		default:
			printk("Cannot support ioctl %#x\n", req);
			return -1;
	}
	return 0;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = led_ioctl,
};

static struct miscdevice led_misc = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "myled",
	.fops	= &led_fops,
};

static int __init my_init(void)
{
	int i, j, ret;

	//1.申请并初始化gpio
	for (i = 0; i < LED_NUM; i++) {
		led_gpios[i] = EXYNOS4212_GPM4(i);
		ret = gpio_request(led_gpios[i], "myio");
		if (ret) {
			printk("Cannot request gpio %d\n", led_gpios[i]);
			goto err_req;
		}
		s3c_gpio_cfgpin(led_gpios[i], S3C_GPIO_OUTPUT);
	}

	//2.创建proc文件
	create_proc_read_entry("led_info", 0444, NULL, \
			led_proc_read, NULL);

	//3.注册miscdevice
	ret = misc_register(&led_misc);
	if (ret) {
		printk("Cannot register misc device\n");
		goto err_misc;
	}

	printk("Register miscdevice for LED ok\n");
	return 0;
err_misc:
	remove_proc_entry("led_info", NULL);
err_req:
	for (j = 0; j < i; j++)
		gpio_free(led_gpios[j]);
	return ret;
}

static void __exit my_exit(void)
{
	int i;
	misc_deregister(&led_misc);
	remove_proc_entry("led_info", NULL);
	for (i = 0; i < LED_NUM; i++)
		gpio_free(led_gpios[i]);
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");

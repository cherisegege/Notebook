/*****************************
 * pwm的char驱动测试例子2
 * 根据电路图，蜂鸣器连接的GPIO是GPD0_0；
 * 将GPIO设置为BUZZ输出，用BUZZ信号来控制蜂鸣器
 * 可以调整BUZZ信号的频率来控制音调的高低；
 * 占空比的作用相对不大；
 * 用pwm库来控制BUZZ输出；
 * char驱动支持ioctl命令；
 * Author: zht
 * Date: 2016-10-15
 ******************************/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/pwm.h>
#include <linux/proc_fs.h>
#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>

//首先确定对应的GPIO号(GPD0_0)，BUZZ通道号等
#define BUZZ_IO		EXYNOS4_GPD0(0)
#define BUZZ_ID		0 //BUZZ通道0

//定义默认值
#define FREQ_MIN	1		//1HZ
#define	FREQ_MAX	10000	//100000HZ
#define FREQ_DEF	1000
#define DUTY_MIN	0		//0%
#define DUTY_MAX	100		//100%
#define DUTY_DEF	50
#define NS_IN_1S	1000000000L	//1秒钟含的ns数

//定义ioctl命令号
#define BUZZ_TYPE		'P'
#define BUZZ_START		_IO(BUZZ_TYPE, 1)
#define BUZZ_STOP		_IO(BUZZ_TYPE, 2)
#define BUZZ_SET_FREQ	_IOW(BUZZ_TYPE, 3, int)
#define BUZZ_SET_DUTY	_IOW(BUZZ_TYPE, 4, int)

//定义全局变量
static struct pwm_device *dev;
static int cur_freq; //当前的频率
static int cur_duty; //当前的占空比

//proc文件的读函数
static int
buzz_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0;
	ret += sprintf(page, "====BUZZ通道(%d)的当前信息====\n", BUZZ_ID);
	ret += sprintf(page+ret, "当前频率为%dHZ; 当前占空比%d%%\n", cur_freq, cur_duty);
	return ret;
}

int buzz_start(void)
{
	//计算整个周期的时间和高电平持续时间
	int full = NS_IN_1S / cur_freq;
	int high = full / 100 * cur_duty;

	//首先设置GPIO为PWM输出
	s3c_gpio_cfgpin(BUZZ_IO, S3C_GPIO_SFN(2));
	//设置频率及占空比
	pwm_config(dev, high, full);
	pwm_enable(dev);
	return 0;
}

int buzz_stop(void)
{
	pwm_disable(dev);
	s3c_gpio_cfgpin(BUZZ_IO, S3C_GPIO_OUTPUT);
	gpio_set_value(BUZZ_IO, 0);
	return 0;
}

int buzz_set_freq(int freq)
{
	int full, high;

	cur_freq = freq;
	full = NS_IN_1S / cur_freq;
	high = full / 100 * cur_duty;
	pwm_config(dev, high, full);
	return 0;
}

int buzz_set_duty(int duty)
{
	int full, high;

	cur_duty = duty;
	full = NS_IN_1S / cur_freq;
	high = full / 100 * cur_duty;
	pwm_config(dev, high, full);
	return 0;
}


//file_operations
static long
buzz_ioctl(struct file *filp, unsigned int req, unsigned long arg)
{
	int ret = 0;
	switch (req) {
		case BUZZ_START:
			ret = buzz_start();
			break;
		case BUZZ_STOP:
			ret = buzz_stop();
			break;
		case BUZZ_SET_FREQ:
			if (arg < FREQ_MIN || arg > FREQ_MAX) {
				printk("频率应该在%d~%dHZ之间\n", FREQ_MIN, FREQ_MAX);
				return -1;
			}
			ret = buzz_set_freq(arg);
			break;
		case BUZZ_SET_DUTY:
			if (arg < DUTY_MIN || arg > DUTY_MAX) {
				printk("占空比应该在%d~%d%%之间\n", DUTY_MIN, DUTY_MAX);
				return -1;
			}
			ret = buzz_set_duty(arg);
			break;
		default:
			printk("Cannot support ioctl %#x\n", req);
			return -1;
	}
	return ret;
}

static struct file_operations buzz_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = buzz_ioctl,
};

static struct miscdevice buzz_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "mybuzz",
	.fops  = &buzz_fops,
};

static int __init my_init(void)
{
	int ret;

	//初始化频率和占空比
	cur_freq = FREQ_DEF;
	cur_duty = DUTY_DEF;
	
	//1.首先申请gpio
	ret = gpio_request(BUZZ_IO, "mybuzz");
	if (ret) {
		printk("Cannot request gpio %d\n", BUZZ_IO);
		return ret;
	}
	//配置为输出，且默认蜂鸣器不响
	s3c_gpio_cfgpin(BUZZ_IO, S3C_GPIO_OUTPUT);
	gpio_set_value(BUZZ_IO, 0);

	//2.申请pwm通道0（相当于申请硬件定时器0）
	dev = pwm_request(BUZZ_ID, "mypwm");
	if (IS_ERR(dev)) {
		printk("Cannot request pwm_device %d\n", BUZZ_ID);
		ret = PTR_ERR(dev);
		goto err_req;
	}

	//3.创建proc文件
	create_proc_read_entry("buzz_info", 0444, NULL, \
			buzz_proc_read, NULL);

	//4.注册misc设备
	ret = misc_register(&buzz_misc);
	if (ret) {
		printk("Cannot register misc device for pwm\n");
		goto err_misc;
	}
	return 0;
err_misc:
	remove_proc_entry("buzz_info", NULL);
	pwm_free(dev);
err_req:
	gpio_free(BUZZ_IO);
	return ret;
}

static void __exit my_exit(void)
{
	misc_deregister(&buzz_misc);
	remove_proc_entry("buzz_info", NULL);
	buzz_stop();
	pwm_free(dev);
	gpio_free(BUZZ_IO);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


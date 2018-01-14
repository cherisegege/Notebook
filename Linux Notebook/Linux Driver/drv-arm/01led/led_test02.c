/******************************
 * led的测试例子2
 * 用char驱动进行控制，支持ioctl；
 * 可以用proc文件来获取灯的状态
 * 支持用ioctl命令来进行控制；
 * 将4个灯识别为一个设备，分别进行控制
 * Author: zht
 * Date: 2016-09-25
 *****************************/
#include <linux/module.h>
//char驱动需要的头文件
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
//proc文件的头文件
#include <linux/proc_fs.h>
//访问寄存器需要的头文件
#include <linux/ioport.h>
#include <linux/io.h>

//定义常量
#define LED_MAJOR	56
#define LED_NUM		4

#define GPIO_BASE	0x11000000
#define GPIO_SIZE	0x1000
#define GPM4CON		0x2E0
#define GPM4DAT		0x2E4

struct led_info {
	int led_num; 
	int led_status;
};

struct led_all_info {
	int led_status[LED_NUM];
};

//定义ioctl号，参数为灯的编号(0~3)
//STATUS命令可以用参数或返回值代表灯的状态
#define LED_TYPE	'L'
#define LED_ON		_IOW(LED_TYPE, 1, int)
#define LED_OFF		_IOW(LED_TYPE, 2, int)
#define LED_STATUS	_IOWR(LED_TYPE, 3, struct led_info)

#define LED_ALLON	_IO(LED_TYPE, 4)
#define LED_ALLOFF	_IO(LED_TYPE, 5)
#define LED_ALLSTATUS	_IOR(LED_TYPE, 6, struct led_all_info)


//如果一个驱动支持一个设备，则可以不定义私有结构体，用全局变量来存储灯的状态等
static int led_state[LED_NUM];	//1:亮; 0:灭
static dev_t dev_id;			//设备号
static struct cdev led_cdev;	//用于注册
static void __iomem *vir_base;	//虚拟基地址

//proc文件的读函数，用于获取灯的状态，替代驱动的read函数
static int
my_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int i, ret = 0;
	ret = sprintf(page, "====LED(%d:%d)的当前状态====\n", MAJOR(dev_id), MINOR(dev_id));
	for (i=0; i<LED_NUM; i++)
		ret += sprintf(page+ret, "LED%d: %s\n", \
			i, (led_state[i] ? "亮" : "灭"));
	ret += sprintf(page+ret, "物理基地址为: %#x\n", GPIO_BASE);
	ret += sprintf(page+ret, "虚拟基地址为: %p\n", vir_base);
	return ret;
}

//实现file_operations，包括
static long
led_ioctl(struct file *filp, unsigned int req, unsigned long arg)
{
	struct led_all_info info;
	int i, value; 
	switch (req) {
		//前3个命令针对一个灯
		case LED_ON:
		case LED_OFF:
		case LED_STATUS:
		//后3个命令支持所有灯
		case LED_ALLON:
			for (i=0; i<LED_NUM; i++) {
				value = readl(vir_base+GPM4DAT);
				value &= ~(0x1<<i);
				writel(value, vir_base+GPM4DAT);
				led_state[i] = 1;
			}
			break;
		case LED_ALLOFF:
			for (i=0; i<LED_NUM; i++) {
				value = readl(vir_base+GPM4DAT);
				value |= (0x1<<i);
				writel(value, vir_base+GPM4DAT);
				led_state[i] = 0;
			}
			break;
		case LED_ALLSTATUS:
			for (i=0; i<LED_NUM; i++)
				info.led_status[i] = led_state[i];
			if (copy_to_user((void *)arg, &info, sizeof(info)))
				return -EFAULT;
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

static int __init my_init(void)
{
	int value, i;
	//1.将物理地址转换为虚拟地址
	vir_base = ioremap(GPIO_BASE, GPIO_SIZE);
	if (!vir_base) {
		printk("Cannot ioremap %#x\n", GPIO_BASE);
		return -EIO;
	}

	//2.将GPIO配置为输出，并确保灯默认灭
	for (i=0; i<LED_NUM; i++) {
		//配为输出
		value = readl(vir_base+GPM4CON);
		value &= ~(0xF<<i*4);
		value |= 0x1<<i*4;
		writel(value, vir_base+GPM4CON);
		//默认关闭
		led_state[i] = 0; //0:灭
		value = readl(vir_base+GPM4DAT);
		value |= 0x1<<i;
		writel(value, vir_base+GPM4DAT);
	}

	//3.为设备分配设备号，用cdev注册
	dev_id = MKDEV(LED_MAJOR, 0);
	cdev_init(&led_cdev, &led_fops);
	cdev_add(&led_cdev, dev_id, 1);

	//4.创建一个只读的proc文件
	create_proc_read_entry("led_info2", 0444, NULL, \
			my_proc_read, NULL);

	printk("Led register OK\n");
	return 0;
}

static void __exit my_exit(void)
{
	remove_proc_entry("led_info2", NULL);
	cdev_del(&led_cdev);
	iounmap(vir_base);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


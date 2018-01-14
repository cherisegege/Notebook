/******************************
 * led的测试例子1
 * 用char驱动进行控制，支持write和ioctl；
 * 可以支持用echo on0~3|off0~3 >/dev/myled的方式控制某个灯的亮灭
 * 可以用proc文件来获取灯的状态
 * 还可以用ioctl命令来进行控制；
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
#define LED_MAJOR	55
#define LED_NUM		4

#define GPIO_BASE	0x11000000
#define GPIO_SIZE	0x1000
#define GPM4CON		0x2E0
#define GPM4DAT		0x2E4

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
	ret += sprintf(page+ret, "虚拟基地址为: 0x%p\n", vir_base);
	return ret;
}

//实现file_operations，包括
//open & release & write & unlocked_ioctl
//如果open和release中只是返回0，则可以不写，代表open操作默认成功

//和用户态的echo on0~3|off0~3 >/dev/myled对应
static ssize_t
led_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	char str[10] = {0};
	int led_num = 0, value;
	//首先将echo后面的控制字符串读到内核
	if (copy_from_user(str, buf, 4))
		return -EFAULT;

	if (strncmp(str, "on", 2)==0) {
		//确定要控制的灯的编号
		//对寄存器的操作应按照读-修改-写的原则
		led_num = LED_NUM - ('3'-str[2]) - 1;
		if (led_num >= LED_NUM) {
			printk("Only support led 0~%d\n", LED_NUM-1);
			return -1;
		}
		value = readl(vir_base+GPM4DAT);
		value &= ~(0x1<<led_num);
		writel(value, vir_base+GPM4DAT);
		led_state[led_num] = 1;
	}
	else if (strncmp(str, "off", 3)==0) {
		//确定要控制的灯的编号
		//对寄存器的操作应按照读-修改-写的原则
		led_num = LED_NUM - ('3'-str[3]) - 1;
		if (led_num >= LED_NUM) {
			printk("Only support led 0~%d\n", LED_NUM-1);
			return -1;
		}
		value = readl(vir_base+GPM4DAT);
		value |= (0x1<<led_num);
		writel(value, vir_base+GPM4DAT);
		led_state[led_num] = 0;
	}
	else {
		printk("Only support onx|off0~3\n");
		return -1;
	}
	return count;
}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
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
	create_proc_read_entry("led_info", 0444, NULL, \
			my_proc_read, NULL);

	printk("Led register OK\n");
	return 0;
}

static void __exit my_exit(void)
{
	remove_proc_entry("led_info", NULL);
	cdev_del(&led_cdev);
	iounmap(vir_base);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


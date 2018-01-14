/***************************
 * WDT的char驱动
 * 1.支持通过write/ioctl进行控制
 * 2.支持中断和reset模式
 * 3.支持通过proc文件获得狗的当前状态
 * 4.驱动用platform_driver进行封装
 * 5.用miscdevice进行注册
 * 6.用spinlock锁进行保护，确保同一时间只有一个人控制狗
 * 由于三星也提供了自己的看门狗驱动，因此要将三星的驱动去掉
 * Author: zht
 * Date: 2016-11-13
 ****************************/
#include <linux/module.h>
//char驱动需要的头文件
#include <linux/fs.h> //file_operations
#include <linux/uaccess.h> //copy_to|from_user
#include <linux/miscdevice.h> //miscdevice
//中断处理需要的头文件
#include <linux/interrupt.h> //request_irq
//proc文件需要的头文件
#include <linux/proc_fs.h> 
//寄存器访问需要的头文件
#include <linux/ioport.h> //ioremap
#include <linux/io.h> //readl & writel
//锁需要的头文件
#include <linux/spinlock.h> //spinlock_t
//platform总线需要的头文件
#include <linux/platform_device.h> //platform_driver
//时钟需要的头文件
#include <linux/clk.h> //clk
//内核时间需要的头文件
#include <linux/time.h> //timeval
//自己定义的头文件
#include "regs-wdt.h"

//常量定义
#define MODE_RESET	1
#define MODE_IRQ	2
#define	DEF_MODE	MODE_IRQ	//默认的模式

#define DEF_TIME	10		//默认的喂狗间隔(s)
#define MIN_TIME	1		//最小的喂狗间隔(s)
#define	MAX_TIME	21		//最大的喂狗间隔(s)

#define DEF_PRESC	255		//默认采用最大的预分频值
#define DEF_CNT_1S	3052	//在分频比最大时，每秒递减的计数值

//定义ioctl命令号
#define WDT_TYPE	'W'
#define WDT_ON		_IO(WDT_TYPE, 1) //使能看门狗
#define WDT_OFF		_IO(WDT_TYPE, 2) //关闭看门狗
#define WDT_FEED	_IO(WDT_TYPE, 3) //喂狗
#define WDT_TIME	_IOW(WDT_TYPE, 4, int) //设置新的喂狗间隔
#define WDT_MODE	_IOW(WDT_TYPE, 5, int) //设置新的模式 

//定义全局变量，存储当前信息
static int wdt_base; //物理基地址
static int wdt_size; //物理地址范围
static void __iomem *vir_base; //虚拟基地址
static int wdt_irq;	 //中断号
static spinlock_t wdt_lock; //锁
static struct clk *wdt_clk; //狗的时钟结构体

static int wdt_time;	//喂狗的间隔时间
static int wdt_cnt;		//和喂狗间隔时间对应的倒计数值
static int wdt_mode;	//当前的模式


//proc文件的读函数
static int
wdt_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0, wtcon, wtcnt, wtdat;

	//获取看门狗寄存器的当前值
	wtcon = readl(vir_base+WTCON);
	wtcnt = readl(vir_base+WTCNT);
	wtdat = readl(vir_base+WTDAT);

	//打印全局变量以及寄存器的值
	ret += sprintf(page, "====e4412看门狗信息====\n");
	ret += sprintf(page+ret, "wdt_time = %d秒; wdt_cnt = %d; wdt_mode = %d\n", wdt_time, wdt_cnt, wdt_mode);
	ret += sprintf(page+ret, "wtcon = %#x; wtcnt = %d; wtdat = %d\n", wtcon, wtcnt, wtdat);

	return ret;
}

//中断处理函数
static irqreturn_t
wdt_service(int irq, void *dev_id)
{
	//一旦中断发生，说明在给定间隔之内没有喂狗，需要提醒用户喂狗
	//需要清除中断的状态寄存器(向WTCLRINT写任意值)
	struct timeval val;
	do_gettimeofday(&val);
	printk("应该喂我了(%ld秒)\n", val.tv_sec);
	writel(1, (vir_base+WTCLRINT));

	return IRQ_HANDLED; //IRQ_NONE
}

static void wdt_on(void)
{
	int value;
	value = readl(vir_base+WTCON);
	value |= WTCON_WDTEN;
	writel(value, (vir_base+WTCON));
}

static void wdt_off(void)
{
	int value;
	value = readl(vir_base+WTCON);
	value &= ~WTCON_WDTEN;
	writel(value, (vir_base+WTCON));
}

static void wdt_feed(void)
{
	writel(wdt_cnt, (vir_base+WTCNT));
}

static void wdt_set_heartbeat(int time)
{
	wdt_time = time;
	wdt_cnt  = wdt_time * DEF_CNT_1S;
	writel(wdt_cnt, (vir_base+WTCNT));
	writel(wdt_cnt, (vir_base+WTDAT));
}

static void wdt_set_mode(int mode)
{
	int value;
	value = readl(vir_base+WTCON);
	if (mode == MODE_RESET) {
		value |= WTCON_RSTEN;
		value &= ~WTCON_INTEN;
	} else {
		value |= WTCON_INTEN;
		value &= ~WTCON_RSTEN;
	}
	writel(value, (vir_base+WTCON));
}


//file_operations
//支持用户态用echo对狗进行控制
//$>echo on|off|feed >/dev/mydog
static ssize_t 
wdt_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	char tmp[10] = {0};
	if (copy_from_user(tmp, buf, 4))
		return -EFAULT;

	//在设置寄存器之前，应首先持有锁，保证同一时间只有一个人控制狗
	spin_lock(&wdt_lock);
	if (strncmp(tmp, "on", 2)==0) {
		wdt_on();
	} else if (strncmp(tmp, "off", 3)==0) {
		wdt_off();
	} else if (strncmp(tmp, "feed", 4)==0) {
		wdt_feed();
	} else {
		printk("Only support on|off|feed\n");
		spin_unlock(&wdt_lock);
		return -1;
	}
	spin_unlock(&wdt_lock);
	return count;
}

static long
wdt_ioctl(struct file *filp, unsigned int req, unsigned long arg)
{
	int ret = 0;
	//在访问寄存器之前应该先持有锁
	switch (req) {
		case WDT_ON:
			wdt_on();
			break;
		case WDT_OFF:
			wdt_off();
			break;
		case WDT_FEED:
			wdt_feed();
			break;
		case WDT_TIME:
			if (arg<MIN_TIME || arg>MAX_TIME) {
				printk("喂狗间隔应该在%d~%d秒之间\n", MIN_TIME, MAX_TIME);
				ret = -1;
				goto out;
			}
			wdt_set_heartbeat(arg);
			break;
		case WDT_MODE:
			if (arg!=MODE_RESET && arg!=MODE_IRQ) {
				printk("狗只支持reset和irq模式(错误模式%ld)\n", arg);
				ret = -1;
				goto out;
			}
			wdt_set_mode(arg);
			break;
		default:
			printk("不能支持命令: %#x\n", req);
			ret = -1;
			goto out;
	}
out:
	spin_unlock(&wdt_lock);
	return ret;
}

static struct file_operations wdt_fops = {
	.owner = THIS_MODULE,
	.write = wdt_write,
	.unlocked_ioctl = wdt_ioctl,
};

static struct miscdevice wdt_misc = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "mywdt",
	.fops	= &wdt_fops,
};


//准备platform_driver
static int
wdt_probe(struct platform_device *pdev)
{
	struct resource *res1, *res2;
	int value, ret;

	//1.从pdev中获得资源
	res1 = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	res2 = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res1 || !res2) {
		printk("%s: cannot get enough resource\n", pdev->name);
		return -1;
	}
	wdt_base = res1->start;
	wdt_size = res1->end - res1->start + 1;
	wdt_irq	 = res2->start;

	//2.ioremap
	vir_base = ioremap(wdt_base, wdt_size);
	if (!vir_base) {
		ret = -EIO;
		printk("%s: ioremap %#x(%#x) error\n", pdev->name, wdt_base, wdt_size);
		goto err_map;
	}
	printk("%s: ioremap %#x to 0x%p\n", pdev->name, wdt_base, vir_base);

	//3.初始化全局变量
	wdt_time = DEF_TIME;
	wdt_mode = DEF_MODE;
	wdt_cnt  = wdt_time * DEF_CNT_1S;
	spin_lock_init(&wdt_lock);
	
	//4.获得并使能看门狗的时钟
	//linux内核默认将很多设备的时钟关闭，需要驱动主动打开
	//三星在硬件初始化阶段，会为每个设备准备clk结构体
	//驱动通过名字匹配将该结构体找到
	//三星在arch/arm/mach-exynos/clock-exynos4.c中定义设备对应的clk结构体
	wdt_clk = clk_get(&pdev->dev, "watchdog");
	if (IS_ERR(wdt_clk)) {
		printk("%s: cannot get clock\n", pdev->name);
		ret = PTR_ERR(wdt_clk);
		goto err_clk;
	}
	clk_enable(wdt_clk);

	//5.初始化看门狗的寄存器
	//选定默认的模式，将分频设为最大
	value = 0;
	if (wdt_mode == MODE_RESET)
		value |= WTCON_RSTEN;
	else
		value |= WTCON_INTEN;
	value |= WTCON_DIV128;
	value |= WTCON_PRESC(DEF_PRESC);

	writel(value, (vir_base+WTCON));
	writel(wdt_cnt, (vir_base+WTDAT));
	writel(wdt_cnt, (vir_base+WTCNT));
	writel(0, (vir_base+WTCLRINT));
	
	//6.注册中断处理函数
	ret = request_irq(wdt_irq, //中断号
			wdt_service,//中断处理函数
			0,			//flags
			"wdt-irq",	//name
			NULL);
	if (ret) {
		printk("%s: register irq %d error\n", pdev->name, wdt_irq);
		goto err_irq;
	}
	
	//7.创建proc文件
	create_proc_read_entry("wdtinfo", 0444, NULL, \
			wdt_proc_read, NULL);
		
	//8.注册miscdevice
	ret = misc_register(&wdt_misc);
	if (ret) {
		printk("%s: miscdevice register error\n", pdev->name);
		goto err_misc;
	}

	//9.如果一切顺利，打印成功信息
	value = readl(vir_base+WTCON);
	printk("%s: register OK\n", pdev->name);
	printk("wdt %sable; reset %sable; irq %sable\n", \
			(value & WTCON_WDTEN) ? "en" : "dis", \
			(value & WTCON_RSTEN) ? "en" : "dis", \
			(value & WTCON_INTEN) ? "en" : "dis");

	return 0;
err_misc:
	remove_proc_entry("wdtinfo", NULL);
	free_irq(wdt_irq, NULL);
err_irq:
	clk_disable(wdt_clk);
	clk_put(wdt_clk);
err_clk:
	iounmap(vir_base);
err_map:
	return ret;
}

static int
wdt_remove(struct platform_device *pdev)
{
	writel(0, (vir_base+WTCON)); //关闭狗
	free_irq(wdt_irq, NULL);
	misc_deregister(&wdt_misc);
	remove_proc_entry("wdtinfo", NULL);
	clk_disable(wdt_clk);
	clk_put(wdt_clk);
	iounmap(vir_base);
	return 0;
}

static struct platform_device_id wdt_ids[] = {
	{ "e4412-wdt", 0 },
	{ "e6410-wdt", 0 },
	{ "e4418-wdt", 0 },
	{},
};

static struct platform_driver wdt_pdrv = {
	.probe		= wdt_probe,
	.remove		= wdt_remove,
	.id_table	= wdt_ids,
	.driver		= {
		.name = "wdtdrvs",
	},
};

static int __init my_init(void)
{
	return platform_driver_register(&wdt_pdrv);
}

static void __exit my_exit(void)
{
	platform_driver_unregister(&wdt_pdrv);
}
module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZHT");


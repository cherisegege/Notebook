/**************************
 * char驱动的测试例子1
 * 采用缓冲区作为硬件，用户态用echo/cat来进行测试；
 * 驱动应支持open/release/read/write/unlocked_ioctl; 
 * 本例只支持一个缓冲区设备；
 * 增加proc文件用于获取缓冲区的使用状态；
 * 驱动负责选定设备号和ioctl号，用户态负责使用这些号
 * Author: zht
 * Date: 2016-09-24
 **************************/
#include <linux/module.h>
//char驱动需要的头文件
#include <linux/fs.h> //file_operations 
#include <linux/cdev.h> //cdev，用于注册
#include <linux/uaccess.h> //copy_to|from_user
//proc文件需要的头文件
#include <linux/proc_fs.h>

//定义驱动要用到的常量
//定义驱动选择的主设备号，可以参考/proc/devices，选择一个目前没人使用的主设备号即可
#define MEM_MAJOR	50
#define MEM_SIZE	100 //缓冲区的大小

struct mem_info {
	u32 size;
	u32 wp;
};

//定义ioctl号
//#define MEM_RESET		101
//#define MEM_RESIZE	102
//#define MEM_STATUS	103
#define	MEM_TYPE	'M'
#define	MEM_RESET	_IO(MEM_TYPE, 1)
#define MEM_RESIZE	_IOW(MEM_TYPE, 2, int)
#define MEM_STATUS	_IOR(MEM_TYPE, 3, struct mem_info)


//如果一个驱动支持多个设备，则应该定义设备的私有结构体
//将设备特定的信息存入私有结构体
//如果一个驱动只支持一个设备，则可以用全局变量来存设备信息
struct mem_priv {
	char *start; //缓冲区的起始地址
	int size;	 //缓冲区的大小
	int wp;		 //缓冲区当前写入内容的偏移
	dev_t dev_id; //设备号
	struct cdev mem_cdev; //用于注册的结构体
};

//分配一个结构体，代表一个设备
static struct mem_priv dev;

//proc文件的读函数
static int
my_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0;
	ret = sprintf(page+ret, "====缓冲区信息(%d:%d)====\n", \
			MAJOR(dev.dev_id), MINOR(dev.dev_id));
	ret += sprintf(page+ret, "缓冲区大小为%d字节，当前的wp = %d\n", dev.size, dev.wp);
	ret += sprintf(page+ret, "缓冲区的基地址为%p\n", dev.start);
	return ret;
}

//实现char驱动要求的函数 open/release/read/write/ioctl

//和用户态的open操作对应
//一般硬件的初始化都放到模块的入口中完成，open经常没啥事儿可做
//如果open中只是return0，则可以不写(意味着open默认成功)
static int
mem_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int
mem_release(struct inode *inode, struct file *filp)
{
	return 0;
}

//和用户态的read(fd, buf, 10)对应
//内核read的参数和用户态read对应，但多了第4个参数f_pos，指向文件内部的偏移
//用文件的内部偏移作为读的偏移位置
static ssize_t
mem_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	//首先计算缓冲区可以读多少字节
	size_t cnt = min(count, (size_t)(dev.wp-*f_pos));
	if (0 == cnt)
		return 0;
	//copy_to_user(to,from,cnt)
	if (copy_to_user(buf, (dev.start+*f_pos), cnt))
		return -EFAULT;
	//更新读指针
	*f_pos += cnt;
	return cnt;
}

//和用户态的write(fd, "hello", 5)对应
static ssize_t
mem_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	//首先判断缓存区可以写多少字节
	size_t cnt = min(count, (size_t)(dev.size-dev.wp));
	if (0 == cnt)
		return 0;
	//copy_from_user(to,from,cnt)
	if (copy_from_user((dev.start+dev.wp), buf, cnt))
		return -ENOMEM;
	//更新wp
	dev.wp += cnt;
	return cnt;
}

//和用户态的ioctl(fd, 命令号)或ioctl(fd,命令号,参数)对应
static long
mem_ioctl(struct file *filp, unsigned int req, unsigned long arg)
{
	char *tmp;
	struct mem_info myinfo;
	switch (req) {
		case MEM_RESET:
			memset(dev.start, 0, dev.size);
			dev.wp = 0;
			break;
		case MEM_RESIZE: //arg为新的缓冲区大小
			tmp = (char *)kzalloc(arg, GFP_KERNEL);
			if (!tmp)
				return -ENOMEM;
			kfree(dev.start);
			dev.wp = 0;
			dev.size = arg;
			dev.start = tmp;
			break;
		case MEM_STATUS: //arg为用户态分配的mem_info的地址
			myinfo.size = dev.size;
			myinfo.wp = dev.wp;
			if (copy_to_user((void *)arg, &myinfo, sizeof(myinfo)))				return -EFAULT;
			break;
		default: 
			printk("Cannot support ioctl %#x\n", req);
			return -1;
	}
	return 0;
}

static struct file_operations mem_fops = {
	.owner		= THIS_MODULE,
	.open		= mem_open,
	.release	= mem_release,
	.read		= mem_read,
	.write		= mem_write,
	.unlocked_ioctl = mem_ioctl,
};

static int __init my_init(void)
{
	//1.初始化私有结构体并分配缓冲区
	dev.size = MEM_SIZE;
	dev.wp = 0;
	dev.start = (char *)kzalloc(dev.size, GFP_KERNEL);
	if (!dev.start) 
		return -ENOMEM;

	//2.为设备分配设备号并用cdev向VFS注册
	dev.dev_id = MKDEV(MEM_MAJOR, 0);
	cdev_init(&dev.mem_cdev, &mem_fops);
	cdev_add(&dev.mem_cdev, dev.dev_id, 1);

	//3.创建一个只读的proc文件
	create_proc_read_entry("buf_info", 0444, NULL, \
			my_proc_read, NULL);

	return 0;
}
static void __exit my_exit(void)
{
	remove_proc_entry("buf_info", NULL);
	cdev_del(&dev.mem_cdev);
	kfree(dev.start);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


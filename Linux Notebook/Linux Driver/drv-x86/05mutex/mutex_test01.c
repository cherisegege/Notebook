/****************************
 * 循环缓冲区(FIFO)的例子
 * 本设备可以在用户态测试，使用方法类似于管道；
 * 在私有结构体中增加读和写指针，测试时会分别修改指针；
 * 为了保护对wp/rp的操作，应增加mutex锁
 * 如果FIFO满则进程继续写时休眠；
 * 如果FIFO空则读进程休眠；需要两个等待队列
 * Author: zht
 * Date: 2014-08-15
 ****************************/
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/wait.h> //wait_queue_head_t
#include <linux/mutex.h> //mutex
#include <asm/atomic.h> //atomic_t

#define DEF_LEN 	100
#define DEF_MAJOR 	75

//描述缓冲区的私有结构体
struct mem_priv {
	char *start, *end;
	char *wp, *rp; //读写指针
	int buf_len;
	dev_t dev_id;
	struct cdev mem_cdev;
	//当缓冲区满时，继续echo的进程在writeq上睡眠
	//当缓冲区空时，继续cat的进程在readq上睡眠
	wait_queue_head_t writeq, readq;
	//增加锁，保护wp/rp指针
	struct mutex mem_lock;
	//设备打开计数
	atomic_t open_cnt;
};

static struct mem_priv *dev;


//proc文件的读函数，用于获取缓冲区的访问情况
static int 
mem_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0;
	ret += sprintf(page+ret, "====FIFO(%d:%d)====\n", MAJOR(dev->dev_id), MINOR(dev->dev_id));
	ret += sprintf(page+ret, "st: %p; ed: %p\n", \
			dev->start, dev->end);
	ret += sprintf(page+ret, "wp: %p; rp: %p\n", \
			dev->wp, dev->rp);
	ret += sprintf(page+ret, "缓冲区大小%d字节，当前%d人打开\n",\
			dev->buf_len, atomic_read(&dev->open_cnt));
	return ret;
}


static int 
mem_open(struct inode *inode, struct file *filp)
{
	atomic_inc(&dev->open_cnt);
	return 0;
}


static int 
mem_release(struct inode *inode, struct file *filp)
{
	atomic_dec(&dev->open_cnt);
	return 0;
}


static ssize_t 
mem_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	size_t cnt;
	//在访问缓冲区的wp/rp之前，首先获得锁
	//判断缓冲区是否为空，如果为空则释放锁，然后wait_event_interruptible
	//如果从wait_event_interruptible退出，说明缓冲区当前非空，重新获得锁
	//如果确保缓冲区非空(wp!=rp)且持有锁：
	//首先计算缓冲区可以读多少字节
	//if (wp>rp)
	//	cnt = min(count, (size_t)(wp-rp));
	//else
	//	cnt = min(count, (size_t)(end-rp));
	//copy_to_user(...);
	//更新rp，如果rp==end，则回卷
	// rp += cnt;
	// if (rp == end)
	//	rp = start;
	//解锁
	//唤醒之前因为缓冲区满而睡眠的进程
	//wake_up(writeq);
	return cnt;
}


static ssize_t 
mem_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	//在访问缓冲区的wp/rp之前，首先获得锁
	//判断缓冲区是否为满，如果为满则释放锁，然后wait_event_interruptible
	//如果从wait_event_interruptible正确退出，说明缓冲区当前非满，重新获得锁
	//如果确保缓冲区非满(...)且持有锁：
	//首先计算缓冲区可以写多少字节
	//更新wp，如果wp==end，则回卷
	//解锁
	//唤醒之前因为缓冲区空而睡眠的进程
	//wake_up(readq);
	return cnt;
}

static struct file_operations mem_fops = {
	.owner = THIS_MODULE, 
	.open = mem_open,
	.release = mem_release,
	.read = mem_read,
	.write = mem_write,
};

static int __init my_init(void)
{
	int ret;

	//1.分配私有结构体
	dev = (struct mem_priv *)kzalloc(sizeof(*dev), GFP_KERNEL);
	if (NULL == dev)
		return -ENOMEM;

	//2.分配并初始化FIFO缓冲区
	dev->start = (char *)kzalloc(DEF_LEN, GFP_KERNEL);
	if (NULL == dev->start) {
		kfree(dev);
		return -ENOMEM;
	}
	dev->buf_len = DEF_LEN;
	dev->wp = dev->start;
	dev->rp = dev->start;
	dev->end = dev->start + dev->buf_len;

	//3.初始化等待队列和锁
	init_waitqueue_head(&dev->readq);
	init_waitqueue_head(&dev->writeq);
	mutex_init(&dev->mem_lock);

	//4.分配设备号并注册
	dev->dev_id = MKDEV(DEF_MAJOR, 0);
	cdev_init(&dev->mem_cdev, &mem_fops);
	cdev_add(&dev->mem_cdev, dev->dev_id, 1);

	//5.创建只读的proc文件
	create_proc_read_entry("fifo_info", 
		0, NULL, mem_proc_read, NULL);
	return 0;
}

static void __exit my_exit(void)
{
	remove_proc_entry("fifo_info", NULL);
	cdev_del(&dev->mem_cdev);
	kfree(dev->start);
	kfree(dev);
}
module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZHANG");


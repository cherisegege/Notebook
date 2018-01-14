/****************************
 * 内存管道(FIFO)的例子1
 * 例子用内存缓冲区模拟管道的功能
 * 缓冲区空或满都会导致用户态进程的睡眠
 * 用mutex锁来保护缓冲区的指针wp/rp
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

#define DEF_LEN 	100
#define DEF_MAJOR 	75

//描述FIFO的私有结构体
struct mem_priv {
	char *start, *end;
	char *wp, *rp;
	int buf_len;
	dev_t dev_id;
	struct cdev mem_cdev;
	wait_queue_head_t writeq, readq;
	struct mutex mem_lock;
};

//全局的私有结构体指针
static struct mem_priv *dev;

static int 
mem_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0;

	//打印缓冲区的信息，可以加锁
	ret += sprintf(page+ret, "-----DEV %d:%d-----\n", MAJOR(dev->dev_id), MINOR(dev->dev_id));
	ret += sprintf(page+ret, "st = 0x%p\n", dev->start);
	ret += sprintf(page+ret, "ed = 0x%p\n", dev->end);
	ret += sprintf(page+ret, "wp = 0x%p\n", dev->wp);
	ret += sprintf(page+ret, "rp = 0x%p\n", dev->rp);
	return ret;
}


static ssize_t 
mem_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	size_t cnt;

	//首先获得锁
	if (mutex_lock_interruptible(&dev->mem_lock))
		return -ERESTARTSYS;

	//如果缓冲区空，则首先释放锁，然后阻塞
	while (dev->rp == dev->wp) {
		mutex_unlock(&dev->mem_lock);
		if (wait_event_interruptible(dev->readq, dev->rp!=dev->wp))
			return -ERESTARTSYS;
		if (mutex_lock_interruptible(&dev->mem_lock))
			return -ERESTARTSYS;
	}
	
	//此时可以保证获得锁且缓冲区非空
	if (dev->wp > dev->rp)
		cnt = min(count, (size_t)(dev->wp-dev->rp));
	else
		cnt = min(count, (size_t)(dev->end-dev->rp));

	if (copy_to_user(buf,dev->rp,cnt)) {
		mutex_unlock(&dev->mem_lock);
		return -EFAULT;
	}

	//更新rp指针，如果rp==end，回卷
	dev->rp += cnt;
	if (dev->rp == dev->end)
		dev->rp = dev->start;

	//解锁，唤醒等待向缓冲区写的进程
	mutex_unlock(&dev->mem_lock);
	wake_up(&dev->writeq);
	
	return cnt;
}

//返回值为缓冲区的可写字节(0~99)
//如果返回0说明缓冲区满
static inline int 
space_free(struct mem_priv *tmp)
{
	if (tmp->rp > tmp->wp)
		return (tmp->rp-tmp->wp-1);
	else
		return (tmp->buf_len+tmp->rp-tmp->wp-1);
}


static ssize_t 
mem_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	size_t cnt;

	//首先获得锁
	if (mutex_lock_interruptible(&dev->mem_lock))
		return -ERESTARTSYS;

	//判断缓冲区是否为满
	while (space_free(dev)==0) {
		mutex_unlock(&dev->mem_lock);
		if (wait_event_interruptible(dev->writeq, !!space_free(dev)))
			return -ERESTARTSYS;
		if (mutex_lock_interruptible(&dev->mem_lock))
			return -ERESTARTSYS;
	}

	//此时持有锁且缓冲区非满
	cnt = min(count, (size_t)(space_free(dev)));
	if (dev->wp>=dev->rp)
		cnt = min(cnt, (size_t)(dev->end-dev->wp));

	if (copy_from_user(dev->wp,buf,cnt)) {
		mutex_unlock(&dev->mem_lock);
		return -EFAULT;
	}

	//更新指针
	dev->wp += cnt;
	if (dev->wp == dev->end)
		dev->wp = dev->start;

	//解锁，唤醒之前因为空而睡眠的进程
	mutex_unlock(&dev->mem_lock);
	wake_up(&dev->readq);
	return cnt;
}

static struct file_operations mem_fops = {
	.owner = THIS_MODULE, 
	.read = mem_read,
	.write = mem_write,
};


static int __init my_init(void)
{
	int ret;

	//1.分配并初始化私有结构体
	dev = (struct mem_priv *)kzalloc(sizeof(*dev), GFP_KERNEL);
	if (NULL == dev) {
		ret = -ENOMEM;
		goto err_alloc;
	}

	//2.为缓冲区分配空间并初始化对应指针
	dev->start = (char *)kzalloc(DEF_LEN, GFP_KERNEL);
	if (!dev->start) {
		ret = -ENOMEM;
		goto err_buf;
	}
	dev->buf_len = DEF_LEN;
	dev->end = dev->start + dev->buf_len;
	dev->wp = dev->start;
	dev->rp = dev->start;

	//3.初始化等待队列及锁
	init_waitqueue_head(&dev->writeq);
	init_waitqueue_head(&dev->readq);
	mutex_init(&dev->mem_lock);

	//4.注册
	dev->dev_id = MKDEV(DEF_MAJOR, 0);
	cdev_init(&dev->mem_cdev, &mem_fops);
	dev->mem_cdev.owner = THIS_MODULE;
	cdev_add(&dev->mem_cdev, dev->dev_id, 1);

	//5.创建proc文件
	create_proc_read_entry("pipe_test", 
		0, NULL, mem_proc_read, NULL);

	return 0;
err_buf:
	kfree(dev);
err_alloc:
	remove_proc_entry("pipe_test", NULL);
	return ret;
}

static void __exit my_exit(void)
{
	remove_proc_entry("pipe_test", NULL);
	cdev_del(&dev->mem_cdev);
	kfree(dev->start);
	kfree(dev);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZHANG");


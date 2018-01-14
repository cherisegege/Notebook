/**************************
 * proc文件的测试例子2
 * 创建一个可读写的proc文件，使用老的proc接口，适用于3.10内核之前
 * Author: zht
 * Date: 2016-09-11
 **************************/
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h> //jiffies
#include <linux/uaccess.h> //copy_to|from_user

//全局指针，存储用户态echo的字符串
static char *str;

//proc文件的写函数，当用户态echo时调用
//$>echo asdf3edd >/proc/abc_rw
//$>echo 1242234333333 >/proc/abc_rw
//本例不解读写入的参数，只是存储在内核
static int 
my_proc_write(struct file *filp, const char __user *buf, unsigned long count, void *data)
{
	char *tmp = kzalloc(count+1, GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;
	//copy_from_user(to,from,cnt)
	if (copy_from_user(tmp,buf,count)) {
		kfree(tmp);
		return -EFAULT;
	}
	//释放str中存储的旧字符串
	kfree(str); 
	str = tmp;
	return count;
}

//proc文件的读函数，当用户态cat时调用
static int my_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0;
	ret = sprintf(page, "kernel info is %s\n", str);
	ret += sprintf(page+ret, "kernel time is %ld\n", jiffies);
	return ret;
}


static int __init my_init(void)
{
	struct proc_dir_entry *file;
	//创建proc文件
	file = create_proc_entry("abc_rw", //名字
			0644, //mode
			NULL); //parent
	if (!file) {
		printk("Cannot create /proc/abc_rw\n");
		return -1;
	}
	//关联读写函数
	file->read_proc = my_proc_read;
	file->write_proc = my_proc_write;
	return 0;
}

static void __exit my_exit(void)
{
	//删除proc文件，一定别写错了
	remove_proc_entry("abc_rw", NULL);
	kfree(str);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


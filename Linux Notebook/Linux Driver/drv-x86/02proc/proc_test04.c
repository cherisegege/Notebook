/*************************
 * proc文件的测试例子4
 * 使用seq_file接口实现可读写的proc文件，功能同proc_test02.c
 * 由于3.10内核以后，不再支持老的proc接口(读写函数)，因此一旦使用比较新的内核，必须换用新的seq_file接口
 * 参考LDD3书中的seq_file一节
 * Author: zht
 * Date: 2014-08-08
 *************************/
#include <linux/module.h>
#include <linux/slab.h> //kzalloc
#include <linux/sched.h> //jiffies
#include <linux/proc_fs.h>
#include <linux/uaccess.h> //copy_to|from_user
#include <linux/fs.h>
#include <linux/seq_file.h>

static char *str = NULL;

/*********************
 * seq_operations->show
 * 必须返回0，否则什么也显示不出来
 * 相当于老proc文件关联的read_proc接口
 * 将格式化字符串输出到seq_file文件中
 *********************/
static int 
my_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "current kernel time is %ld\n", jiffies);
	seq_printf(m, "str is %s\n", str);

	return 0;
}

/***********************
 * file_operations->open
 ***********************/
static int 
my_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, my_proc_show, NULL);
}

/************************
 * file_operations->write
 * 用于替代老proc实现中的write_proc接口
 ************************/
static ssize_t 
my_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos)
{
	char *tmp = kzalloc((count+1), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;

	if (copy_from_user(tmp,buf,count)) {
		kfree(tmp);
		return -EFAULT;
	}
	kfree(str);
	str = tmp;
	return count;
}

static struct file_operations my_fops = {
	.owner	= THIS_MODULE,
	.open	= my_proc_open,
	.release = single_release,
	.read	= seq_read,
	.llseek	= seq_lseek,
	.write 	= my_proc_write,
};

static int __init my_init(void)
{
	struct proc_dir_entry *file;
	//创建proc文件并关联file_operations
	file = proc_create("abc_rw", 0644, NULL, &my_fops);
	if (!file)
	    return -ENOMEM;

	return 0;
}

static void __exit my_exit(void)
{
	remove_proc_entry("abc_rw", NULL);
	kfree(str);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("aran");
MODULE_LICENSE("GPL");

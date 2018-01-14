/**************************
 * proc文件的测试例子1
 * 创建一个只读的proc文件，使用老的proc接口，适用于3.10内核之前
 * Author: zht
 * Date: 2016-09-11
 **************************/
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h> //jiffies

//proc文件的读函数，当用户态read时调用
static int 
my_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0;
	ret = sprintf(page, "====kernel info====\n");
	ret += sprintf(page+ret, "kernel time is %ld\n", jiffies);
	return ret;
}


static int __init my_init(void)
{
	struct proc_dir_entry *file;
	//创建proc文件
	file = create_proc_entry("abc_ro", //名字
			0444, //mode
			NULL); //parent
	if (!file) {
		printk("Cannot create /proc/abc_ro\n");
		return -1;
	}
	//关联读函数
	file->read_proc = my_proc_read;
	return 0;
}

static void __exit my_exit(void)
{
	//删除proc文件，一定别写错了
	remove_proc_entry("abc_ro", NULL);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


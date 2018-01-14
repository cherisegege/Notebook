/**************************
 * proc文件的测试例子3
 * 创建一个可读写的proc文件，使用老的proc接口，适用于3.10内核之前
 * 用链表存储多个字符串，遍历时依次打印
 * Author: zht
 * Date: 2016-09-11
 **************************/
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h> //copy_to|from_user
#include <linux/list.h>

//定义链表的头结构体，只分配一个
struct proc_head {
	struct list_head head;
	//锁，用于保护链表
	int file_size; //字符串的总大小
};

//定义链表的数据结构体，用户态每echo一个字符串，内核都应该分配一个，并add到链表中
struct proc_item {
	char *str;
	struct list_head item;
};

//分配链表头
static struct proc_head myhead;


//proc文件的写函数，当用户态echo时调用
static int my_proc_write(struct file *filp, const char __user *buf, unsigned long count, void *data)
{
	//为proc_item分配空间
	//为proc_item->str分配空间
	//copy_from_user
	//INIT_LIST_HEAD
	//list_add or list_add_tail
	struct proc_item *tmp = kzalloc(sizeof(*tmp), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;
	tmp->str = (char *)kzalloc(count+1, GFP_KERNEL);
	if (!tmp->str) {
		kfree(tmp);
		return -ENOMEM;
	}
	if (copy_from_user(tmp->str, buf, count)) {
		kfree(tmp->str);
		kfree(tmp);
		return -EFAULT;
	}
	INIT_LIST_HEAD(&tmp->item);
	list_add(&tmp->item, &myhead.head);
	myhead.file_size += count;
	return count;
}

//proc文件的读函数，当用户态cat时调用
static int 
my_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	//变量链表，打印每个proc_item中的str
	//list_for_each_entry(...) {
	//	...;
	//}
	//最后再打印文件的大小, myhead.file_size;
	struct proc_item *tmp;
	int ret = 0;
	list_for_each_entry(tmp, &myhead.head, item) {
		ret += sprintf(page+ret, "%s", tmp->str);	
	}
	ret += sprintf(page+ret, "file_size is %dbytes\n", myhead.file_size);
	return ret;
}


static int __init my_init(void)
{
	struct proc_dir_entry *file;

	//1.初始化链表头
	INIT_LIST_HEAD(&myhead.head);
	myhead.file_size = 0;

	//2.创建proc文件
	file = create_proc_entry("abc_list", //名字
			0644, //mode
			NULL); //parent
	if (!file) {
		printk("Cannot create /proc/abc_list\n");
		return -1;
	}
	//关联读写函数
	file->read_proc = my_proc_read;
	file->write_proc = my_proc_write;
	return 0;
}

static void __exit my_exit(void)
{
	struct proc_item *tmp1, *tmp2;
	//删除proc文件，一定别写错了
	remove_proc_entry("abc_list", NULL);
	//释放链表
	list_for_each_entry_safe(tmp1, tmp2, &myhead.head, item) {
		list_del(&tmp1->item);
		kfree(tmp1->str);
		kfree(tmp1);
	}
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


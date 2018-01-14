/*********************
 * 模块的测试例子3
 * 调用其他模块导出的符号
 * Author: zht
 * Date: 2016-09-10
 *********************/
#include <linux/module.h>

extern int value_a;
extern void my_printf(void);

static int __init my_init(void)
{
	printk("In yy03.c, call my_printf()\n");
	my_printf();
	printk("In yy03.c, 设置value_a\n");
	value_a = 100;
	printk("In yy03.c, 再次调用my_printf()\n");
	my_printf();
	return 0;
}

static void __exit my_exit(void)
{}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


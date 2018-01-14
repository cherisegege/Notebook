/*********************
 * 模块的测试例子2
 * 测试导出符号的宏EXPORT_SYMBOL
 * Author: zht
 * Date: 2016-09-10
 *********************/
#include <linux/module.h>

int value_a = 10;
int value_b = 20;
EXPORT_SYMBOL(value_a);

void my_printf(void)
{
	printk("In yy02.c->my_printf(), value_a = %d\n", value_a);
}
EXPORT_SYMBOL(my_printf);

static int __init my_init(void)
{
	return 0;
}

static void __exit my_exit(void)
{}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


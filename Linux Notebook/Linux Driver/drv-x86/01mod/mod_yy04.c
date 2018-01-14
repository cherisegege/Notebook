/****************************
 * 模块的测试例子4
 * 测试模块的参数，在insmod时传递参数
 * $>insmod mod_yy04.ko
 * $>insmod mod_yy04.ko value=10 name="shrek" test=y
 * Author: zht
 * Date: 2016-09-11
 ****************************/
#include <linux/module.h>

//用宏module_param()将普通的变量改造为模块参数
int value = 35;
char *name = "uplooking";
int test = 0;
module_param(value, int, 0444);
module_param(name, charp, 0444);
module_param(test, bool, 0644);

static int __init my_init(void)
{
	printk("In init(): value = %d; name is %s; test = %d\n", \
			value, name, test);
	return 0;
}

static void __exit my_exit(void)
{
	printk("In exit(): value = %d; name is %s; test = %d\n", \
			value, name, test);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


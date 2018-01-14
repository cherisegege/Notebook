/***********************
 * 第一个模块的测试例子
 * 测试模块的基本内容，包括模块入口，出口等
 * $>insmod mod_yy01.ko
 * $>rmmod mod_yy01
 * Author: zht
 * Date: 2016-09-10
 ***********************/
#include <linux/module.h>

//模块入口函数，在insmod时调用
static int __init my_init(void)
{
	printk("Hello, kernel world\n");
	return 0;
}

//模块出口函数，在rmmod时调用
static void __exit my_exit(void)
{
	printk("Farewell, kernel\n");
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT"); //taint
MODULE_LICENSE("GPL"); //General Public License


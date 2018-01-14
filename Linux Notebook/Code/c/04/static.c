/*
 static的使用
	1.修饰局部变量
		局部静态变量，放在内存的bss段，未初始化值为0
		只初始化一次

	2.修饰全局变量
		限制全部变量的作用域,只能在本文件中使用

	3.修饰函数
		限制修饰的函数只能在本文件中使用

全局变量的使用规则
	1.能不用则不用
	2.一定要用，用static修饰

 * */
#include <stdio.h>

static void test(void);
int main(void)
{
	auto int a, b, c, d, e, f;//局部自动变量
	static int aa, bb, cc, dd, ee, ff;//局部静态变量


	printf("a = %d, b = %d, c = %d, d = %d, e = %d, f = %d\n", a, b, c, d, e, f);

	printf("aa = %d, bb = %d, cc = %d, dd = %d, ee = %d, ff = %d\n", aa, bb, cc, dd, ee, ff);

	test();
	test();
	test();
	test();
	test();

	return 0;
}

static void test(void)
{
	static int i = 1;
	
	i++;
	printf("i = %d\n", i);
}





/*
 *函数指针：
 *	用于存储函数的地址
 *类型的函数：
 *	<1>参数类型相同
 *	<2>返回值类型相同
 *定义：
 *	int (*p)(void);
 * */
#include <stdio.h>

void test1(void)
{
	printf("test1: %p\n", test1);
	printf("hello\n");
}

void test2(void)
{
	printf("afternoon\n");
}

int test3(const char *p)
{
	return printf("%s\n", p);	
}

int test4(const char *p)
{
	int sum = 0;

	while (*p) {
		sum += *p++;
	}

	return sum;
}

void test5(void (*p)(void), int (*q)(const char *), const char *ptr)
{
	p();
	q(ptr);
}

int main(void)
{

	test5(test1, test3, "i am a handsome boy");


	return 0;
}



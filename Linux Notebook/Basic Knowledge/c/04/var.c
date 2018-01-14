#include <stdio.h>

int glob;//函数体外定义的变量-----全局变量

void test(void);
void test2(void);

int main(void)
{
	int num;//函数体内定义的变量-----局部变量
	int glob;

	num = 100;
	glob = 10;

	test();
	test2();

	printf("main: num = %d, glob = %d\n", num, glob);

	return 0;
}

void test(void)
{
	int num;//局部变量
	int glob;	

	num = 200;
	glob = 20;

	printf("test: num = %d, glob = %d\n", num, glob);
}

void test2(void)
{
	printf("test2: glob = %d\n", glob);
}



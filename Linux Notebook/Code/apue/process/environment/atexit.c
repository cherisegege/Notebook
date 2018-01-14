#include <stdio.h>
#include <stdlib.h>

static void register_fun1(void)
{
	printf("%s is performed\n", __FUNCTION__);
}

static void register_fun2(void)
{
	printf("%s is performed\n", __FUNCTION__);
}

static void register_fun3(void)
{
	printf("%s is performed\n", __FUNCTION__);
}

int main(void)
{
	atexit(register_fun1);
	atexit(register_fun2);
	atexit(register_fun3);
	atexit(register_fun3);


	printf("hello world\n");

//	return 0;
	//_exit(0);
	exit(0);
}


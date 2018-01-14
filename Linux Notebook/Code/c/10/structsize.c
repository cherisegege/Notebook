#include <stdio.h>

//当前编译器按照结构体成员变量所占的最大字节对齐
//跨主机传输的时候，结构体一定不对齐
struct test_st {
	char a;
	//char b;
	//short c;
	//int d;
	long e;
	char f;
	long g;
}__attribute__((packed));

struct test2_st{
	long a;
	char name[15];
};


int main(void)
{
	struct test_st var;
	printf("%p %p %p\n", &var.a, &var.f, &var.e);

	printf("%d\n", sizeof(struct test_st));

	printf("%d\n", sizeof(struct test2_st));

	return 0;
}


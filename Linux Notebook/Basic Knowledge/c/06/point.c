/*
	指针： 
 		存储地址的变量		
	指针的定义：
		int *p;		
		int *是变量p的类型
	指针的字节数: 
 		64os----->指针就是8字节
		32os----->指针就是4字节 
	指针的运算:
		int *p;
		p+1----->sizeof(int)	
		char *p;
		p+1----->sizeof(char)
			
		int arr[3] = {1,2,3};
		int *p, *q;
		p = &arr[2];
 		q = &arr[0];
		p - q = 2;

	指针的意义：
		实现形参改变实参
	void *:
		万能指针类型
		void *p;
		p+1;//错误的
	NULL:
		(void *)0	

	int m;
	int *p = &m;
	称作指针变量p指向整型变量m

*/
#include <stdio.h>

int main(void)
{
	int arr[3] = {1,2,3};
	int *p = NULL;//避免野指针的产生
	int *n = NULL;
	char *q = NULL;
	long *m = NULL;
	//int *p = arr;

	p = arr;
	//*p = arr;//！！！！！！！！错误的
	n = arr+2;

	printf("n - p = %d\n", n-p);

	p++; //== arr+1   

	printf("*p = %d\n", *p);

	printf("sizeof(p) = %ld\n", sizeof(p));
	printf("sizeof(q) = %ld\n", sizeof(q));
	printf("sizeof(m) = %ld\n", sizeof(m));

	return 0;
}


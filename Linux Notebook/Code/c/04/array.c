/*
	数组:	
		相同类型元素的集合							
	数组的定义：		
		type name[nmemb];	
		int arr[8];
		有8个整型变量组成的数组

	数组初始化：
		int arr[8] = {};//将数组当中的8个成员全部初始化为0
		int arr[8] = {1,2,3,4,5,6,7,8};
		int arr[8] = {1,2,3};//数组前三个值为1,2,3,其余都为0
		int arr[8] = {1,2,[7]=8};//数组的前两个值为1,2 最后一个值为8

	数组赋值：
		arr[0] = 10;
		arr[6] = 19;
		循环
		int arr[8];
		arr[8] = {1,2,3,4};//错误的！！！！！！！！！

	数组的成员访问:
		数组成员通过下标值来访问
		下标从0开始
		有n个成员的数组，下标0~n-1
		int arr[8] = {};
		int num;
		num = arr[4];
		
	左值和右值:
		左值赋值运算符左边
		int var = 10;
		int num;
		num = var;//var右值表示取变量所对应的值
		var = 20;//var左值表示var变量所对应的存储空间

		int arr[3] = {1,2,3};
		arr[0] = 100;//表示数组的第一个成员所对应的存储空间 
		num = arr[1];//表示将数组的第二个成员对应的值赋值给num变量

	数组名的意义：
		数组名是一个常量值，表示数组的首地址	
		arr == &arr[0];

	数组名的类型：
		int arr[3] = {1,2,3};		
		arr类型int *	
		整型数的地址类型是int *
	
	两个运算符
		&:取址运算符, 提升地址级别
		*:取值运算符, 降低地址级别
		int arr[3] = {1,2,3};
		*arr == arr[0];
		 ||
		*(&arr[0]) == arr[0];
*/

#include <stdio.h>

#define ARRNR	10

int main(void)
{
	int arr[ARRNR];	
	int brr[] = {1,2};//只有数组初始化的时候，成员个数才可以缺省
	int i;

#if 1
	//数组赋值
	for (i = 0; i < ARRNR; i++) {
		arr[i] = i+1;
	}
#endif
	//遍历数组
	for (i = 0; i < ARRNR; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	printf("arr: %p\n", arr);			
	printf("&arr[0]: %p\n", &arr[0]);	

	//arr----->int *	arr+1---->sizeof(int)
	printf("arr+1: %p\n", arr+1);
	printf("*(arr+1): %d\n", *(arr+1));
	printf("*(&arr[0]+1): %d\n", *(&arr[0]+1));
	printf("arr[1]: %d\n", arr[1]);
	//arr[1] ==== *(arr+1)

	printf("*(arr+0) = %d\n", *(arr+0));
	printf("arr[0] = %d\n", arr[0]);

	return 0;
}














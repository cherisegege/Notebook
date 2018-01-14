/*
	const关键字 
		<1>修饰变量------变量不能通过变量名赋值
		<2>修饰指针 
			指针常量:
			指针是修饰词，常量是名词
				int *const m = &n;
				表示指针变量m只读
			常量指针:
			常量是修饰词，指针名词
				const int *p = &n;
				int const *q = &n;
				p和q等效
				表示p所存储的空间只读
 
	const int const *const w;
	w指针所存储的内容不能改变，w本身也不能改变
  */
#include <stdio.h>

int main(void)
{
#if 0
	const int num;//只读变量
	int *p = (int *)&num;

//	num = 10;
	*p = 10;

	printf("num = %d\n", num);
#endif
	int arr[3] = {1,2,3};
	const int *ptr;
	int const *ppt;
	int *const ppm = arr;
	const int const *const w = arr;

	ptr = arr;
	ppt = arr;

/*
 	//都是不允许
	w++;
	*w = 100;
*/
	ptr ++;
	ppt ++;
	//ppm ++;错误的，指针变量只读

	*ppm = 30;

	//*ppt = 20;错误的,对指针常量赋值出错

	//*ptr = 10;错误的,对指针常量赋值出错

	printf("*ptr = %d\n", *ptr);

	return 0;
}



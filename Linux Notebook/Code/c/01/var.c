/*
 *变量
 * */
#include <stdio.h>

int glob;//全局变量未初始化，值一定为0

int main(void)
{

	printf("glob = %d\n", glob);	

	/*num和val局部变量：未初始化值是随机值*/
	int num;//变量的定义
	int val = 20;//变量的初始化
	/*
	 *定义会分配存储空间，声明不会
	 * */

	//num = 10;//变量的赋值

	printf("num = %d\n", num);


	return 0;
}

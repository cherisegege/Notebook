/*
	函数：						  
	三要素： 
 		函数的实现
		函数的声明
 		函数的调用
	必要性： 
		1.模块化编程			 		
 		2.避免代码的重复
 		3.条理清楚
*/
#include <stdio.h>

/*
	函数的声明
   	组成：
		函数名
		参数类型	
		返回值类型
*/
int add(int m, int n);//m n可省
int main(void)
{
	int num1, num2;
	int result;

	printf("input two numbers\n");
	scanf("%d %d", &num1, &num2);
	/*
		函数的调用 
	 	组成：	
	 		函数名
			函数的实参
		谨记：函数的传参是值传递的过程
	 */

	result = add(num1, num2);
	printf("%d+%d = %d\n", num1, num2, result);

	return 0;
}

/*
	函数的实现 
	组成：	 
 		函数名
 		函数的形式参数(本质就是函数的局部变量)
 		函数的返回值类型
 */
int add(int m, int n)
{
	int sum;

	sum = m+n;

	return sum;
}






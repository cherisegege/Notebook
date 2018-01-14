/*变参函数
 *	函数的参数个数是可变的
 *要求：
 *	1.第一个参数不能省
 *		fun(...);不允许
 *	2.函数的变参之前参数，指明变参个数
 * */
#include <stdio.h>
#include <stdarg.h>

int sum_arg(int *res, int n, ...);
int main(void)
{
	int res1, res2;

	sum_arg(&res1, 3, 11,22,33);
	
	sum_arg(&res2, 5, 5,2,1,9,8);

	printf("res1 = %d, res2 = %d\n", res1, res2);



	return 0;
}

int sum_arg(int *res, int n, ...)
{
	va_list ap;//char *
	int sum = 0;

	va_start(ap, n);//将ap指针指向变参的第一个参数

	while (n > 0) {
		sum += va_arg(ap, int);//1.取得ap存储的地址对应的值 2.将ap指针指向下一个变参
		n--;
	}

	va_end(ap);//ap = NULL;

	*res = sum;

	return sum;
}


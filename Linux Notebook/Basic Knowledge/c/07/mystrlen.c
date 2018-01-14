#include <stdio.h>

size_t mystrlen(const char *p);
int main(void)
{
	char str[] = "hello world";
	char *p = "good morning";

	printf("%ld\n", mystrlen(str));
	printf("%ld\n", mystrlen(p));

	return 0;		
}

/*
 实现统计字符串的长度
 要求知道：字符串的首地址	形参：首地址	
 反馈：字符串的字节个数(不包括'0')	返回值：长度
 * */
size_t mystrlen(const char *p)
{
#if 0
	size_t len = 0;	

	while (*p) {
		len++;
		p++;
	}

	return len;
#endif
	size_t i;

	for (i = 0; *(p+i) != '\0'; i++) 
		;

	return i; 
}	









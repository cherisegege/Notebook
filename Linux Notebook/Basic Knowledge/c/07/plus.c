#include <stdio.h>

int main(void)
{
	int arr[] = {1,8,5,10,15};			
	int *p = arr;
	int n;
	int i;

#if 0
	n = (*p)++;
	printf("n = %d\n", n);
	n = ++*p;
	printf("n = %d\n", n);
#endif

	printf("%d %d\n", *p, *p++, p++);	
	//printf参数执行顺序是从右向左

	for (i = 0; i < 5; i++)
		printf("%d ", arr[i]);
	printf("\n");

	printf("0: %p\n", p);
	printf("*p++: %d\n", *p++);//p先取值再p自增
	printf("1: %p\n", p);
	printf("*(p++): %d\n", *(p++));//同上
	printf("2: %p\n", p);
	printf("(*p)++: %d\n", (*p)++);//先取值再将p存储的值自增
	printf("3: %p\n", p);
	printf("*++p: %d\n", *++p);//p先自增在取值
	printf("4: %p\n", p);
	printf("++*p: %d\n", ++*p);//先p存储的值自增再取值
	printf("5: %p\n", p);

	for (i = 0; i < 5; i++)
		printf("%d ", arr[i]);
	printf("\n");


	return 0;
}


#include <stdio.h>
//指针提供能够在一个函数中访问另一个函数的局部变量

int max_of_array(int *p, int num, int *max);
int main(void)
{
	int arr[] = {1, 6, 2, 9, 4, 3};
	int max_arr;

	max_of_array(arr, sizeof(arr)/sizeof(*arr), &max_arr);

	printf("the max is %d\n", max_arr);

	return 0;
}

/*
	返回值：
   		0成功	非0失败	
 */
int max_of_array(int *p, int num, int *max)
{
	int i;
	int m;

	//int arr[3];	sizeof(arr)/sizeof(*arr)
	m = *p;
	for (i = 1; i < num; i++) {
		if (p[i] > m)//p[i]	== *(p+i)
			m = p[i];
	}

	*max = m;//回填
}




/*
二维数组 
	相同类型的数组的集合 
相同类型的数组
	<1>成员个数相同
	<2>成员类型相同
 
二维数组的定义
	int arr[3][2]; 
注意：
	<1>一维数组的数组名的地址表示的是整个数组的地址
	<2>从存储的角度，不存在多维数组，推荐将所有的数组都按照一维数组理解
 
 */
#include <stdio.h>

int main(void)
{
#if 0
	int arr[3] = {1,2,3};

	printf("arr: %p\n", arr);
	printf("&arr: %p\n", &arr);

	printf("arr+1: %p\n", arr+1);
	printf("&arr+1: %p\n", &arr+1);
#endif
	//二维数组初始化
	int array[2][3] = {
		{1,2,3},
		{4,5,6}
	};
	int i, j;

	printf("array: %p\n", array);		
	printf("array+1:%p\n", array+1);
	printf("array[0]: %p\n", array[0]);
	printf("array[0]+1: %p\n", array[0]+1);
	printf("&array: %p\n", &array);
	printf("&array+1: %p\n", &array+1);
	
	//二维数组的遍历
	//i遍历array数组中的每一个数组成员
	for (i = 0; i < 2; i++) {
		//j遍历每个数组中的每一个元素
		for (j = 0; j < 3; j++) {
			printf("%d ", array[i][j]);
		}	
		printf("\n");
	}


	return 0;
}






/*
 *	指针数组：
 *		数组中存放相同类型的指针
 *	主要用途：
 *		字符串的集合
 *	典型实例：
 *		main函数的参数
 * */

#include <stdio.h>

//ls -l -h
int main(int argc, char *argv[]/*char **argv*/)
{
	char *arr[10] = {};
	int i;
	/*
	 *	argc:命令行参数的个数
	 *	argv:存储命令行的每一个参数的指针数组的收地址
	 * */
		
	for (i = 0; /*i < argc*/argv[i] != NULL; i++) {
		printf("%s\n", argv[i]);
	}

	for (i = 0; i < 10 && argv[i] != NULL; i++) {
		arr[i] = argv[argc-i-1];
	}	

	printf("******************************\n");
	for ( i = 0; arr[i] != NULL; i++) {
		printf("%s\n", arr[i]);
	}


	return 0;
}



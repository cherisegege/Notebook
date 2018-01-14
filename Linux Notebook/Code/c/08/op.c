/*
 *四则运算
 * */

#include <stdio.h>

static int add(int a, int b)
{
	return a + b;
}

static int sub(int a, int b)
{
	return a - b;
}

static int mul(int a, int b)
{
	return a * b;
}

static int divi(int a, int b)
{
	return a / b;
}

int main(int argc, char **argv)
{
	int left, right;

	if (argc < 4) {
		printf("Usage ...\n");
		goto ERROR;
	}

	left = atoi(argv[1]);
	right = atoi(argv[3]);
	
	//./a.out 1 + 2
	if (!strcmp(argv[2], "+")) {
		printf("%d %s %d = %d\n", left, argv[2], right, add(left, right));			
	} else if (!strcmp(argv[2], "-")) {
		printf("%d %s %d = %d\n", left, argv[2], right, sub(left, right));			
	} else if (!strcmp(argv[2], "x")) {
		printf("%d %s %d = %d\n", left, argv[2], right, mul(left, right));			
	} else if (!strcmp(argv[2], "/")) {
		printf("%d %s %d = %d\n", left, argv[2], right, divi(left, right));			
	} else {
		printf("对不起！臣妾做不到！！！\n");
	}

	return 0;
ERROR:
	return 1;
}









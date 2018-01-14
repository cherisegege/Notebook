/*
 *通过命令行传参，判断命令行的第二个参数是否为回文整型数
 * */
#include <stdio.h>

static int myatoi(char *p, int *n);
int main(int argc, char **argv)
{
	//./a.out 1123
	int num;

	if (argc < 2) {
		printf("Usage...\n");
		goto ERROR;
	}

	printf("atoi(%s): %d\n", argv[1], atoi(argv[1]));

		
	if (myatoi(argv[1], &num) < 0) {
		printf("参数无效\n");
	} else {
		printf("num = %d\n", num);
	}
#if 0
	if (huiwen(num)) {
		printf("%d 是一个回文整型数\n", num);
	}
#endif

	return 0;
ERROR:	
	return 1;
}

static int myatoi(char *p, int *n)
{
	//p---->"123a"
	int result = 0;	
	int flag = 1;

	if (*p == '-') {
		flag = -1;
		p++;
	}

	while (*p) {
		if (!(*p >= '0' && *p <= '9'))
			break;
		result = result * 10 + (*p - '0');
		p++;
	}
	
	if (result == 0) {
		return -1;
	}
	*n = result*flag;

	return 0;
}







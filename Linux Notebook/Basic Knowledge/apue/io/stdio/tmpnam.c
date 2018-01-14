#include <stdio.h>

int main(void)
{
	char *ret, *ret2;
	char str1[1024] = {};
	char str2[1024] = {};

#if 0
	//静态区
	ret = tmpnam(NULL);

	ret2 = tmpnam(NULL);
#endif

	ret = tmpnam(str1);
	ret2 = tmpnam(str2);

	puts(ret);
	puts(ret2);

	return 0;
}

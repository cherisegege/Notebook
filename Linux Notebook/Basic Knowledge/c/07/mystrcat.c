#include <stdio.h>
#include <string.h>

char *mystrcat(char *dest, const char *src);
int main(void)
{
	char src[] = "mello";
	char dest[100] = "hungry";

#if 0
	printf("the result: %s\n", strncat(dest, src, 1));//无论如何目的地址都是有'\0'
	printf("the result: %s\n", strncpy(dest, src, 1));//当n < strlen(src),dest当中是没有复制'\0'
#endif
	printf("the result: %s\n", mystrcat(dest, src));

	return 0;
}

char *mystrcat(char *dest, const char *src)
{
	char *ret = dest;		

	while (*dest)
		dest++;

	while (*src) {
		*dest++ = *src++;
	}
//	*dest = 0;

	return ret;
}


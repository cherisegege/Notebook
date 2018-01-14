#include <stdio.h>
#include <string.h>

#define BUFSIZE 32

char *mystrcpy(char *dest, const char *src);
int main(void)
{
	//char src[] = "good boys";
	char *src = "good boys";
	char dest[BUFSIZE] = "girls morning hello";
	int i;

	printf("the result: %s\n", mystrcpy(dest, src));

	for (i = 0; i < BUFSIZE; i++) {
		printf("%c ", dest[i]);
	}
	printf("\n");

	return 0;
}

char *mystrcpy(char *dest, const char *src)
{
	char *ret = dest;			

	while (*src)
		*dest++ = *src++;
	*dest = 0;

	return ret;
#if 0
	int i;

	for (i = 0; src[i] != '\0'; i++) {
		dest[i] = src[i];
	}
	dest[i] = '\0';

	return dest;

#endif
}





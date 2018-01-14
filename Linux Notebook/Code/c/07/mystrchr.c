#include <stdio.h>
#include <string.h>

char *mystrchr(const char *p, int ch);
char *mystrrchr(const char *p, int ch);
int main(void)
{
	char *s1 = "hello world";
	int ch = 'l';
	char *ret;

	ret = mystrchr(s1, ch);
	if (ret == NULL) {
		printf("%c is not in %s\n", ch, s1);
	} else {
		printf("%s\n", ret);	
	}
	//printf("%s\n", mystrrchr(s1, ch));	

	return 0;
}

char *mystrchr(const char *p, int ch)
{
	while (*p) {	
		if (*p == ch)
			return (char *)p;
		p++;
	}
	return NULL;
}

char *mystrrchr(const char *p, int ch)
{
	int i = strlen(p);	

	i--;//'\0'前一个字符的下标
	while (i >= 0) {
		if (p[i] == ch)
			return (void *)(p+i);
		i--;
	}

	return NULL;
}


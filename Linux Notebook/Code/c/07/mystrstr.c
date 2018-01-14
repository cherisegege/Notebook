#include <stdio.h>
#include <string.h>

char *mystrstr(const char *mother, const char *child);
int main(void)
{
	char *p = "hello, iiiii aam i am a boy";	
	char *q = "i am";
	char *ret;

	ret = mystrstr(p, q);
	if (NULL == ret) {
		printf("%s is not in %s\n", q, p);	
	} else {
		printf("the result: %s\n", ret);
	}

	return 0;
}

char *mystrstr(const char *mother, const char *child)
{
#if 0
	//方法一
	const char *m_save, *c_save;		

	while (*mother) {
		if (*mother == *child) {
			m_save = mother;
			c_save = child;
			while (*++m_save == *++c_save) {
				if (*(c_save+1) == '\0')
					return (char *)mother;
			}
		}
		mother ++;
	}	

	return NULL;
#endif
	//方法二
	while (*mother) {
		if (!strncmp(mother, child, strlen(child))) {
			return (char *)mother;
		}
		mother++;
	}

	return NULL;
}






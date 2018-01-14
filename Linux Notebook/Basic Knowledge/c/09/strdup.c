#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *mystrdup(const char *p);
int main(int argc, char **argv)
{
	char *p = NULL;	

	if (argc < 2) {
		return 1;
	}

	p = mystrdup(argv[1]);

	printf("%s\n", p);

	return 0;
}

char *mystrdup(const char *p)
{
	char *str = NULL;	

	str = malloc(strlen(p)+1);
	if (NULL == str)
		return NULL;
	strcpy(str, p);

	return str;
}


#include <stdio.h>
#include <string.h>

int mystrcmp(const char *p, const char *q);
int main(void)
{
	char *p = "helloma";
	char *q = "hellom";

	printf("%d\n", mystrcmp(p, q));

	return 0;
}

int mystrcmp(const char *p, const char *q)
{
	while (*p || *q) {
		if (*p != *q)	
			return *p-*q;
		else {
			p++;
			q++;
		}
	}	

	return 0;
}


#include <stdio.h>

int main(void)
{
	char buf[] = "hello   guys";
	char buf1[32] = {};
	char buf2[32] = {};

	sscanf(buf, "%s %s", buf1, buf2);

	puts(buf1);
	puts(buf2);


	return 0;
}

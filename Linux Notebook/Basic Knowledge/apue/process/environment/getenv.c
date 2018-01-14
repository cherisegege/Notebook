#include <stdio.h>
#include <stdlib.h>

int main(void)
{


	printf("%s\n", getenv("PATH"));
	printf("%s\n", getenv("LANG"));

	setenv("NEW", "this is a test", 0);
	printf("after setenv():%s\n", getenv("NEW"));


	exit(0);
}

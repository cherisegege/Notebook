#include <stdio.h>
#include <stdlib.h>

static void test(const char *str)
{
	printf("%s is beginning...!!!\n", __FUNCTION__);
	puts(str);
	printf("%s is ending...!!!\n", __FUNCTION__);
	exit(0);
}

int main(int argc, char **argv)
{

	printf("%s is beginning...!!!\n", __FUNCTION__);


	test("good morining");



	printf("%s is ending...!!!\n", __FUNCTION__);

//	return 0;
}


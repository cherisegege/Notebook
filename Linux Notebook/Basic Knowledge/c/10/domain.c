#include <stdio.h>

//位域(unsigned char / unsigned int)
struct test_st {
	unsigned char a:1;
	unsigned char b:2;
	unsigned char c:3;
};

union var_un {
	int num;
	struct test_st st;
};

int main(void)
{
	union var_un var;

	var.num = 252;

	printf("%d\n", sizeof(var));

	printf("%d %d %d\n", var.st.a, var.st.b, var.st.c);

	return 0;
}

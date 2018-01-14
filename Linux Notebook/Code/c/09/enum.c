/*
 枚举类型
 	enum
  */
#include <stdio.h>

enum menu{ADD, DELETE, SCAN=10, UPDATE};
/*
 #define ADD 	0
 #define DELETE 1
 * */

int main(void)
{
	enum menu var;
	// enum menu枚举类型
	
	var = DELETE;	
	printf("var: %d\n", var);

	return 0;
}



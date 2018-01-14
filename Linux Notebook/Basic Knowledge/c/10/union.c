/*
 *共用体/联合体
 *	关键字:union	
 *	共用体中所有成员共用同一段存储空间
 *
 *大端存储/小端存储
 *大端存储:高字节存放在低地址, 低字节存放在高地址
 *小端存储:高字节存放在高地址, 低字节存放在低地址
 *写一段代码，验证当前的os是大端还是小端?
 * */

#include <stdio.h>

union test_un{
	char a;
	int b;
	short c;
};

union type_un{
	char a;
	int b;
};

int main(void)
{
	union type_un tt;

	tt.b = 0x12345678;

	if (tt.a == 0x78) {
		printf("小端\n");
	} else if (tt.a = 0x12) {
		printf("大端\n");
	}

#if 0
	union test_un var;

	var.b = 131;
	//10000011
	//11111101

	printf("%d\n", sizeof(var));
	
	printf("%d %d %d\n", var.a, var.b, var.c);
#endif

	return 0;
}



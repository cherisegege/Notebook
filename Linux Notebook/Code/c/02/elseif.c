#if 0
	任意输入一个有效的成绩(0~100)，判断此成绩所在的等级
	90~100	A
	80~89	B
	70~79	C
	60~69	D
	0~59	E
#endif
#include <stdio.h>

int main(void)
{
	int score;

	printf("input a score\n");
	scanf("%d", &score);

#if 0
	if (score >= 90 && score <= 100) {
		printf("***********A***********\n");
	} else if (score >= 80 && score <= 89) {
		printf("***********B***********\n");
	} else if (score >= 70 && score <= 79) {
		printf("***********C***********\n");
	} else if (score >= 60 && score <= 69) {
		printf("***********D***********\n");
	} else if (score >= 0 && score <= 59) {
		printf("***********E***********\n");
	} else {
		printf("是不是傻\n");
	}
#endif

	/**
	 *注意：switch()多用于菜单选择
	 *switch (变量/变量表达式)
	 *case 常量:
	 *不要忘记break;
	 *default不要省略
	 */
	switch (score/10) {
		case 10:
		case 9:
			printf("************A***************\n");
			break;
		case 8:
			printf("************B***************\n");
			break;
		case 7:
			printf("************C***************\n");
			break;
		case 6:
			printf("************D***************\n");
			break;
		case 5:
		case 4:
		case 3:
		case 2:
		case 1:
		case 0:
			printf("************E***************\n");
			break;
		default:	
			printf("是不是傻\n");
			break;
	}

	return 0;
}



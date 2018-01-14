#include <stdio.h>

int main(void)
{
#if 0
	//作业1
	int num1, num2;		
	int cur_num;	
	int i;

	num1 = num2 = 1;
	//第一项和第二项
	printf("%d %d ", num1, num2);	
	for (i = 3; i <= 20; i++) {
		cur_num = num1 + num2;	
		printf("%d ", cur_num);	
		num1 = num2;
		num2 = cur_num;	
	}	
	printf("\n");
#endif

#if 0
	//作业2
	int num1, num2;	
	int num1_s, num2_s;
	int tmp;

	printf("输入两个整型数\n");
	scanf("%d%d", &num1, &num2);	

	//保证num1存储两个数中较大的
	if (num1 < num2) {
#if 0
		tmp = num1;	
		num1 = num2;	
		num2 = tmp;
#endif
		num1 = num1 ^ num2;	
		num2 = num1 ^ num2;//num1 ^ num2 ^ num2
		num1 = num1 ^ num2;//num1 ^ num1 ^ num2
	}
	
	num1_s = num1;
	num2_s = num2;
	//最大公约数
#if 0
	while ((tmp = num1 % num2) != 0) {
		num1 = num2;	
		num2 = tmp;
	}
#endif

	while (1) {
		tmp = num1 % num2;
		if (tmp == 0)
			break;
		num1 = num2;
		num2 = tmp;
	}
	printf("%d和%d的最大公约数是%d, 最小共倍数是%d\n", num1_s, num2_s, num2, (num1_s*num2_s)/num2);
#endif

#if 0 
	//作业3
	int num, save;			
	int i;//遍历所有num的因子
	int flag = 0;

	printf("input a number\n");
	scanf("%d", &num);	
	
	save = num;
	for (i = 2; i <= num; i++) {	
		if (num % i == 0) {		
			printf("%d * ", i);
			num /= i;
			i--;
			flag ++;
		}
	}
	if (flag == 1) {
		printf("1   ");
	}
	printf("\b\b= %d\n", save);
#endif
#if 0 
	//作业4
	int num, save;
	int result;

	printf("input a number\n");
	scanf("%d", &num);

	save = num;	

	//123
	for (result = 0; num > 0; num /= 10) {
		result = result*10 + (num % 10);	
	}

	if (save == result) {
		printf("%d 是回文整型数\n", save);
	} else {
		printf("%d 不是回文整型数\n", save);
	}
#endif
#if 0
	//作业5
	int num;		
	int i;
	
	printf("input a number\n");
	scanf("%d", &num);

	for (i = 2; i <= num/2; i++) {
		if (num % i == 0) {
			break;	
		}
	}
	if (i == num/2+1) {
		printf("%d is a primer\n", num);
	} else {
		printf("%d is not a primer\n", num);
	}
#endif

#if 0
	//作业6
	int num1, num2, num3;
	int min, max, mid;

	printf("input three numbers\n");
	scanf("%d%d%d", &num1, &num2, &num3);

	max = (num1>num2?num1:num2)>num3?(num1>num2?num1:num2):num3;
	min = (num1>num2?num2:num1)>num3?num3:(num1>num2?num2:num1);
	mid = (num1+num2+num3)-(max+min);

	printf("%d %d %d\n", min, mid, max);
#endif
	//作业7	
	int i;

	for (i = 1; i <= 100; i++) {
		if (i % 4 == 0 && i % 6 != 0) {
			printf("%d\n", i);
		}
	}

	return 0;
}








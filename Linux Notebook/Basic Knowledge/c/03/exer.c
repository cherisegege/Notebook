#include <stdio.h>

int main(void)
{
	int row;
	int i, j;
	int cnt;

#if 0
	printf("input the row\n");	
	scanf("%d", &row);

	//练习1
	//行数
	for (i = 0; i < row; i++) {
		//每一行
		for (j = 0; j <= i; j++) {
			printf("* ");
		}
		printf("\n");
	}
	//练习2	
	for (i = 0; i < row; i++) {		
		//空格
		for (j = 0; j < row-i-1; j++) {
			printf("%2c", ' ');
		}
		//星号		
		for (j = 0; j < 2*i+1; j++) {
			printf("%2c", '*');	
		}
		printf("\n");	
	}
	//练习3
	for (i = 0; i < row; i++) {
		for (j = 0; j <= i; j++) {
			printf("%2c", 'A'+(i+j)%26);
		}
		printf("\n");
	}	
#endif
	//练习4
	for (i = 1; i <= 9; i++) {
		for (j = 1; j <= i; j++) {
			printf("%d*%d=%-3d", j, i, j*i);
		}
		printf("\n");
	}				
	//练习5
	cnt = 0;
	for (i = 0; i <= 50; i++) {
		for (j = 0; j <= 20; j++) {
			if ((100-i*2-j*5 >= 0) && (100-i*2-j*5)%10 == 0) {
				cnt ++;	
			}		
		}
	}

	printf("cnt = %d\n", cnt);

	return 0;
}







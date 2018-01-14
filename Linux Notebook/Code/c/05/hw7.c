#include <stdio.h>

#define NR	10

int main(void)
{
	int arr[NR] = {};//0表示活着，1表示退出
	int cnt = 0;//数数
	int kill = 0;//统计退出个数
	int i;

	for (i = 0; kill < NR-1; i = (i+1)%NR) {
		if (arr[i] == 0) {	
			cnt ++;
			if (cnt == 3) {
				arr[i] = 1;
				printf("%d号退出了游戏\n", i+1);
				kill ++;
				cnt = 0;
			}
		}
	}

	for (i = 0; i < NR; i++) {
		if (arr[i] == 0) {
			break;	
		}
	}
	printf("%d号胜利了！！！！！！！！\n", i+1);
 
	return 0;
}

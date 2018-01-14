#include <stdio.h>

#define NAMESIZE	20	
#define NMEMB		10

struct stu_st {
	int id;
	char name[NAMESIZE];
	int score;
};

int ispass(int score);
int main(void)
{
	//结构体类型的数组
	struct stu_st arr[NMEMB] = {};
	int i;
			
	for (i = 0; i < NMEMB; i++) {
		printf("input the %dth stu info(id, name, score)\n", i+1);
		scanf("%d%s%d", &arr[i].id, arr[i].name, &arr[i].score);	
		if (ispass(arr[i].score)) {
			printf("%s %d %d\n", arr[i].name, arr[i].id, arr[i].score);
		}
	}


	return 0;
}

int ispass(int score)
{
	return score >= 60;
}



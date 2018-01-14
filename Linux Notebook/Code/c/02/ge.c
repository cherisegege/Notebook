#include<stdio.h>

#define BASEYEAR 1990

int main(void){
	int i,year,month,monthdays;
	int sumdays = 0;
	int flag = 0;
	do{
		if(flag == 0){
			flag = 1;
			printf("please input year and month\n");
			scanf("%d%d",&year,&month);
		}else{
			printf("the input is error\n");
			printf("please reinput year and month\n");
			scanf("%d%d",&year,&month);	
		}   	
	}while(!(year >= 1990 && month > 0 && month <= 12));
	//计算1990 --- year 有多少天
	for(i = BASEYEAR;i < year;i++){
		if((i % 4 == 0 && i % 100 != 0) || (i % 400 == 0)){
			sumdays += 366;
		}else{
			sumdays += 365;
		}
	}
	//计算year.1.1 --- year.month.1有多少天
	switch(month -1){
		case 11:
			sumdays += 30;
		case 10:
			sumdays += 31;
		case 9:
			sumdays += 30;
		case 8:
			sumdays += 31;
		case 7:
			sumdays += 31;
		case 6:
			sumdays += 30;
		case 5:
			sumdays += 31;
		case 4:
			sumdays += 30;
		case 3:
			sumdays += 31;
		case 2:
			sumdays += (28 + ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)));
		case 1:
			sumdays += 31;
		break;
		default:
			break;
	}
	sumdays += 1;
	sumdays %= 7;
	printf("%d的%d月1号是星期%d\n",year,month,sumdays);

	//year.month 有多少天
	switch(month){
		case 1:case 3:case 5:case 7:case 8:case 10:case 12:
			monthdays = 31;
			break;
		case 4:case 6:case 9:case 11:
			monthdays = 30;
			break;
		case 2:
			monthdays = (28 + ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)));
			break;
		default:
			break;
	}
	//打印year.month
	printf("         %d年    %d月\n",year,month);
	printf("sun mon tue wen thu fri sat\n");
	for(i = 0;i < sumdays;i++){
	printf("    ");
	}
	for(i = 1;i <= monthdays;i++){
		printf("%3d%c",i,((sumdays + i) % 7 == 0 ? '\n':' '));
	}
	printf("\n");
}















/*
	复合类型		
	1.结构体
		关键字:struct

	2.共用体/联合体

	3.位域

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAMESIZE	1024

/*
 *	struct stu_st结构体类型
 *	结构体类型成员变量要求：
 *		<1>可以为任意类型变量
 * 		<2>成员变量不能进行初始化
 * */
struct stu_st {
	int id;
	int age;
	char sex;
	char name[NAMESIZE];
}var, *p;
//var也是struct stu_st 类型的变量 p是结构体类型的指针

int main(void)
{
	//结构体类型的变量stu 初始化
	struct stu_st stu = {1, 20, 'f', "zhangyiwen"};

	struct stu_st stu2 = {.id=2, .sex='m', .name="lifumiao"};

	//结构体变量取成员
	printf("stu:%d %d %c %s\n", stu.id, stu.age, stu.sex, stu.name);
	printf("stu2:%d %d %c %s\n", stu2.id, stu2.age, stu2.sex, stu2.name);

	strcpy(stu.name, "zhangjia");
	printf("stu:%d %d %c %s\n", stu.id, stu.age, stu.sex, stu.name);
	//结构体类型的指针
	struct stu_st *ptr = NULL;

	ptr = malloc(sizeof(*ptr));
	if (NULL == ptr)
		return 1;
	//结构体类型的指针取成员变量
	ptr->id = 3;
	ptr->age = 29;
	ptr->sex = 'f';
	strcpy(ptr->name, "liangyuan");

	printf("%d %d %c %s\n", ptr->id, ptr->age, ptr->sex, ptr->name);
	free(ptr);

	return 0;
}



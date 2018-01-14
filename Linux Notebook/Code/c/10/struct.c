#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int red;
	int blue;
	int green;
	//color_t *cc;错误的
}color_t;

struct dog_st {
	int age;
	color_t color;
	char *name;
	struct dog_st *dd;
	//struct dog_st mm;错误的
};

int main(void)
{
	struct dog_st dog1;	

	dog1.age = 1;
	dog1.color.red = 100;
	dog1.color.blue = 180;
	dog1.color.green = 50;
	dog1.name = strdup("xiaobai");

	printf("dog:%d %s\n", dog1.age, dog1.name);

	free(dog1.name);

	return 0;
}

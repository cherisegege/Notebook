#include <stdio.h>

#include "list.h"

#define NAMESIZE	32

struct stu_st {
	int id;
	char name[NAMESIZE];
};

static void print_fun(const void *data)
{
	const struct stu_st *d = data;

	printf("%d %s\n", d->id, d->name);
}

int main(void)
{
	list_t *mylist;		
	struct stu_st stu;
	int i;

	if (list_init(sizeof(struct stu_st), &mylist) < 0) {
		fprintf(stderr, "list_init(): failed\n");
		goto ERROR;
	}

	for (i = 0; i < 10; i++) {
		stu.id = i+1;
		snprintf(stu.name, NAMESIZE, "%c%c%chaha", 'a'+rand()%26, 'A'+rand()%26, 'a'+rand()%26);
		list_insert(mylist, &stu, TAIL);
	}
	
	list_traval(mylist, print_fun);

	list_destroy(mylist);

	return 0;
ERROR:
	return 1;
}


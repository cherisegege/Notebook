#include <stdio.h>

#include "seqlist.h"

static int int_cmp(const void *data, const void *key)
{
	const int *d = data;
	const int *k = key;

	return *k-*d;
}

static void traval_int(const void *data)
{
	const int *d = data;

	printf("%-4d", *d);
}	

int main(void)
{
	struct slist_st *mylist;
	int i, num;
	int delid;

	mylist = seqlist_init(sizeof(int));
	//if error
	
	srand(time(NULL));
	for (i = 0; i < 10; i++) {
		num = rand() % 100;	
		printf("%-4d", num);
		if(seqlist_insert_sort(mylist, &num, int_cmp)){
			printf("插入数据时产生错误\n");
			break;	
		}
//		seqlist_insert(mylist, &num);
	}
	printf("\n");

	seqlist_traval(mylist, traval_int);
	printf("\n");
/*
	seqlist_sort(mylist, int_cmp);
	printf("after sort\n");
	seqlist_traval(mylist, traval_int);
	printf("\n");

	delid = 1;
	seqlist_delete(mylist, &delid, int_cmp);

	printf("after delete\n");
	seqlist_traval(mylist, traval_int);
	printf("\n");
*/
	seqlist_destroy(mylist);

	return 0;
}

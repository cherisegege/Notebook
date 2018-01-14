#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"

#define NAMESIZE 20

//data
struct stu_st {
	int id;
	char name[NAMESIZE];
};

int cmp_insert(const void *data1, const void *data2)
{
	struct stu_st *d1 = (void *)data1;
	struct stu_st *d2 = (void *)data2;

	return (d1->id - d2->id);
}

int cmp_delete(const void *data1, int key)
{
	struct stu_st *d1 = (void *)data1;

	return (d1->id - key);
}

int cmp_search(const void *data1, int key)
{
	struct stu_st *d1 = (void *)data1;

	return (d1->id - key);
}

void prt(const void *data)
{
	struct stu_st *d = (void *)data;
	printf("id = %d, name = %s\n", d->id, d->name);
}

int main(void)
{
//	struct node_st *rootp = NULL;	
	struct stu_st stu, *srch;
	int id[] = {5,9,3,6,10,15,2,1,7};
//	int id[] = {1,2,3,4,5,6,7,8,9,10};
	int i, delid;	
	head_t *head;

	head = tree_init(sizeof(struct stu_st));
	for (i = 0; i < sizeof(id)/sizeof(*id); i++) {
		stu.id = id[i];	
		snprintf(stu.name, NAMESIZE, "stu%d", i+1);//printf("ccc\n");	
//		tree_insert(&head->node_h.left, &stu, head->size, cmp);
		tree_insert(head, &stu, cmp_insert);
	}
	tree_traval_mid(head->node_h.left, prt);

	tree_draw(head->node_h.left, prt);

#if 0
	//tree_traval_mid(rootp);
	stu.id = 4;
	strncpy(stu.name, "newnode", NAMESIZE);
	tree_insert_r(&rootp, stu);
	printf("after insert\n");
	tree_draw(rootp);
#endif

	printf("input the id which you want to delete\n");
	scanf("%d", &delid);
	tree_delete(&head->node_h.left, delid, cmp_delete);
	tree_draw(head->node_h.left, prt);

	printf("which to find(id)\n");
	scanf("%d", &delid);
	srch = tree_search(head->node_h.left, delid, cmp_search);
	if (srch == NULL)
		printf("not found\n");
	else
		prt(srch);




	return 0;
}


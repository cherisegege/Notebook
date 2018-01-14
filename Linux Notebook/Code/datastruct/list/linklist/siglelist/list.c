#include <stdlib.h>
#include <string.h>

#include "list.h"

int list_init(int size, list_t **head)
{
	list_t *me = NULL;

	me = malloc(sizeof(list_t));
	if (me == NULL) {
		*head = NULL;
		return -1;
	}
	memset(me, '\0', sizeof(list_t));
	me->head.data = NULL;
	me->head.next = &me->head;
	me->size = size;

	*head = me;

	return 0;
}

int list_insert(list_t *list, const void *data, int way)
{		
	struct node_st *new;
	struct node_st *cur;

	//1.为新结点结构开辟存储空间
	new = malloc(sizeof(*new));	
	if (new == NULL)
		return -1;
	//2.为新结点数据域开辟存储空即那
	new->data = malloc(list->size);
	if (NULL == new->data) {
		free(new);
		return -1;
	}
	memcpy(new->data, data, list->size);
	
	//2.安排新结点的指针域
	if (way == FRONT) {
		new->next = list->head.next;	
		list->head.next = new;	
	} else if (way == TAIL) {
		for (cur = list->head.next; cur->next != &list->head; cur = cur->next)
			;
		new->next = cur->next;
		cur->next = new;
	} else {
		free(new->data);	
		free(new);
		return -2;
	}	

	return 0;
}

void list_traval(list_t *list, pri_t pri)
{
	struct node_st *cur;	

	for (cur = list->head.next; cur != &list->head; cur = cur->next)
		pri(cur->data);
}

void list_destroy(list_t *list)
{
	struct node_st *cur, *step;	

	for (cur = list->head.next; cur != &list->head; cur = step) {
		step = cur->next;
		free(cur->data);
		free(cur);
	}

	free(list);
}


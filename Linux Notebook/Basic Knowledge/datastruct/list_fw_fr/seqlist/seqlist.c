#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "seqlist.h"

struct slist_st *seqlist_init(int size)
{
	struct slist_st *me = NULL;

	me = malloc(sizeof(*me));
	if (NULL == me)
		return NULL;
	memset(me, '\0', sizeof(*me));
	me->start = NULL;
	me->size = size;
	me->nmemb = 0;

	return me;
}

int seqlist_insert(struct slist_st *ptr, const void *data)
{
	char *new = NULL;	
	//新成员开辟存储空间
	new = malloc(ptr->size * (ptr->nmemb+1));	
	if (new == NULL)
		return -1;
	memcpy(new, ptr->start, ptr->nmemb*ptr->size);
	free(ptr->start);
	ptr->start = new;
	
	//新数据copy到新空间
	memcpy((char *)(ptr->start)+ptr->size*ptr->nmemb,\
			data, ptr->size);
	ptr->nmemb ++;

	return 0;
}
//找到指定元素的下标
static int seqlist_srh_index(struct slist_st *ptr, const void *key, cmp_t cmp)
{
	int i;

	for (i = 0; i < ptr->nmemb; i++) {
		if (!cmp((char *)(ptr->start)+i*ptr->size, key))
			return i;
	}

	return -1;
}

int seqlist_delete(struct slist_st *ptr, const void *key, cmp_t cmp)
{
	int ret;
	char *new = NULL;

	ret = seqlist_srh_index(ptr, key, cmp);
	if (ret < 0)
		return -1;
	//ret就是要删除元素的下标
	memmove((char *)ptr->start+ret*ptr->size, \
			(char *)ptr->start+(ret+1)*ptr->size,\
			(ptr->nmemb-ret-1)*ptr->size);
	ptr->nmemb --;
	if (ptr->nmemb == 0) {
		ptr->start = NULL;
	} else {
		new = realloc(ptr->start, ptr->nmemb*ptr->size);
		if (new == NULL)
			return -1;
		ptr->start = new;
	}

	return 0;
}

const void *seqlist_search(struct slist_st *ptr, const void *key, cmp_t cmp)
{
	int ret;	

	ret = seqlist_srh_index(ptr, key, cmp);
	if (ret < 0) {
		return NULL;	
	}

	return (char *)(ptr->start)+ret*ptr->size;
}

void seqlist_traval(const struct slist_st *ptr, print_t pri)
{
	int i;

	for (i = 0; i < ptr->nmemb; i++) {
		pri((char *)(ptr->start)+i*ptr->size);
	}
}

void seqlist_sort(struct slist_st *ptr, cmp_t cmp)
{
	qsort(ptr->start, ptr->nmemb, ptr->size, cmp);
}

void seqlist_destroy(struct slist_st *ptr)
{
	free(ptr->start);
	free(ptr);
}

int seqlist_insert_sort(struct slist_st *ptr, const void *data, cmp_t cmp)
{
	int i;
	char *new;
	new = realloc(ptr->start, ptr->size*(ptr->nmemb+1));
	if(new == NULL){
		return 1;
	}
	ptr->start = new;
	ptr->nmemb++;
	for(i = ptr->nmemb-2; i >= 0; i--){
		if(cmp(data, (char *)ptr->start+i*ptr->size) > 0){
			memcpy((char *)ptr->start+(i+1)*ptr->size, (char *)ptr->start+i*ptr->size, ptr->size);
		}else{
			break;
		}
	}
	memcpy((char *)ptr->start+(i+1)*ptr->size, data, ptr->size);

	return 0;
}

int seqlist_save(struct slist_st *ptr, char *path)
{
	FILE *fp;

	fp = fopen(path, "w");
	if(fp == NULL){
		printf("open file failed!\n");
		return 1;
	}
	fwrite(ptr->start, 1, ptr->nmemb*ptr->size, fp);
	fclose(fp);

	return 0;
}

int seplist_read(struct slist_st *ptr, char *path)
{
	FILE *fp;
	int num;

	fp = fopen(path, "r");
	if(fp == NULL){
		printf("open file failed!\n");
		return 1;
	}
	
	fseek(fp, 0L, SEEK_END);
	num = ftell(fp);
	rewind(fp);
	ptr->start = malloc(num);
	num = fread(ptr->start, 1, num, fp);
	ptr->nmemb = num / (ptr->size);
	fclose(fp);

	return 0;
}











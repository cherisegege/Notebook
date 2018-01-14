#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"


struct stack_st *init_stack(int size)
{
	struct stack_st *me;

	me = malloc(sizeof(*me));
	if(NULL == me){
		return NULL;
	}
	me->base = malloc(NUM * size);
	if(NULL == me->base){
		free(me);
		return NULL;
	}
	me->maxnmemb = NUM;
	me->top = me->base;
	me->size = size;

	return me;
}

int empty(struct stack_st *s)
{
	return (!(s->base == s->top));
}

int full(struct stack_st *s)
{
	return (!(((char *)s->top-(char *)s->base) == s->maxnmemb*s->size));
}

int pop(struct stack_st *s, void *data)
{
	if(!empty(s)){
		return 1;	
	}
	s->top = (char *)s->top - s->size;
	memcpy(data, s->top, s->size);
	return 0;
}

int push(struct stack_st *s, void *data)
{
	char *new;
	if(!full(s)){
		if(s->maxnmemb == MAXSIZE){
			return 1;
		}else{
			new = realloc(s->base,(s->maxnmemb + INC)*s->size);
			if(NULL == new){
				return 2;
			}
			s->base = new;
			s->top = (char *)s->base + s->maxnmemb * s->size;
			s->maxnmemb = s->maxnmemb + INC;
		}
	}
	memcpy(s->top, data, s->size);
	s->top = (char *)s->top + s->size;	

	return 0;
}

int destroy(struct stack_st *s)
{
	free(s->base);
	free(s);

	return 0;
}


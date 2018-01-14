#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

stack_t *stack_init(int size)
{
	stack_t *me;
	me = malloc(sizeof(*me));
	if(NULL == me){
		return NULL;
	}
	me->base = malloc(STACKBASE*size);
	if(NULL == me->base){
		free(me);
		return NULL;
	}
	me->top = me->base;
	me->size = size;
	me->maxnmemb = STACKBASE;

	return me;
}

int stack_full(stack_t *s)
{
	return (!(((char *)s->top - (char *)s->base) / s->size == s->maxnmemb));
}

int stack_empty(stack_t *s)
{
//	printf("haha\n");
	return (!(s->base == s->top));
}

int stack_push(stack_t *s, const void *data)
{
	char *new;
	if(!stack_full(s)){
		if(s->maxnmemb == STACKMAXS){
			return 1;
		}
		new = realloc(s->base, (s->maxnmemb+STACKINCR)*s->size);
		if(NULL == new){
			return 2;
		}
		s->base = new;
		s->top = (char *)s->base + s->maxnmemb*s->size;
		s->maxnmemb += STACKINCR;
	}	
	memcpy(s->top, data, s->size);
	s->top = (char *)s->top + s->size;

	return 0;
}

int stack_pop(stack_t *s, void *data)
{
	if(!stack_empty(s)){
		return 1;
	}
	s->top = (char *)s->top - s->size;
	memcpy(data, s->top, s->size);

	return 0;
}

void stack_destroy(stack_t *s)
{
	free(s->base);
	free(s);
}

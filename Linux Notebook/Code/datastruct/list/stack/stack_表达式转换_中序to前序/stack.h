#ifndef _STACK_H
#define _STACK_H

#define NUM     30
#define INC     5
#define MAXSIZE 50


struct stack_st{
	void *top;
	void *base;
	int size;
	int maxnmemb;
};

struct stack_st *init_stack(int size);

int pop(struct stack_st *s, void *data);

int push(struct stack_st *s, void *data);

int empty(struct stack_st *s);

int full(struct stack_st *s);

int destroy(struct stack_st *s);

#endif


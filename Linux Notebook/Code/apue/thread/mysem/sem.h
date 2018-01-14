#ifndef _MYSEM_H
#define _MYSEM_H

typedef void mysem_t;

mysem_t *sem_init(int n);

int sem_fetch(mysem_t *, int num);

int sem_return(mysem_t *, int num);

void sem_destroy(mysem_t *);

#endif


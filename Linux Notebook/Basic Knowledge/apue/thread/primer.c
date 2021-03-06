#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define START	30000000
#define END		30000200
#define THRNR	((END)-(START)+1)

static void *thr_fun(void *p)
{
	int n = *(int *)p;
	int i, mark;

	mark = 0;
	for (i = 2; i <= n/2; i++) {
		if (n % i == 0) {
			mark = 1;	
			break;
		}
	}

	if (!mark) {
		 printf("%d is a primer\n", n);	
	}

	pthread_exit(NULL);
}

int main(void)
{
	pthread_t arr[THRNR] = {};
	int i, err;
	int *tmp[THRNR] = {};

	for (i = START; i <= END; i++) {
		tmp[i-START] = malloc(sizeof(int));
		//if error
		*tmp[i-START] = i; 
		err = pthread_create(arr+i-START, NULL, thr_fun, tmp[i-START]);		

		if (err) {
			fprintf(stderr, "pthread_create(): %s\n", strerror(err));
			exit(1);
		}
	}

	for (i = 0; i < THRNR; i++) {
		pthread_join(arr[i], NULL);	
		free(tmp[i]);
	}

	exit(0);
}


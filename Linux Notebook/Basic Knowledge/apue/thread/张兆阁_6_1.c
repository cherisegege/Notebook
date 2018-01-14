#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THRNR	20

//互斥量
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
//条件变量
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//临界区
static int pool = 1;
FILE *fp;

static void *thr_fun(void *p)
{
	char  tmp[2];

	pthread_mutex_lock(&mut);

	rewind(fp);
	fread(&tmp, 1, 2, fp);
	tmp[1]++;
	if(tmp[1] > '9'){
		tmp[0]++;
		tmp[1] = '0';
	}
	rewind(fp);
	fwrite(&tmp, 1, 2, fp);

	pthread_mutex_unlock(&mut);

	pthread_exit(NULL);
}

int main(void)
{
	pthread_t tids[THRNR] = {};
	int i, err;
	char  tmp[2] = "00";

	if((fp = fopen("/tmp/out", "w+")) == NULL){
		perror("fopen()");
	}

	rewind(fp);
	fwrite(&tmp, 1, 2, fp);

	for (i = 0; i < THRNR; i++) {
		err = pthread_create(tids+i, NULL, thr_fun, NULL);
		if (err) {
			fprintf(stderr, "pthread_create(): %s\n", strerror(err));
			exit(1);
		}
	}	
	
	for (i = 0; i < THRNR; i++) {
		pthread_join(tids[i], NULL);
	}	

	fclose(fp);
	pthread_mutex_destroy(&mut);
	pthread_cond_destroy(&cond);

	exit(0);
}





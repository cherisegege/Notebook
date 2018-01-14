#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LEFT	      2
#define RIGHT         20000
#define THREAD		  3

static int is_primer(int n);


int main(void)
{
	int num_per;
	pid_t pid;
	int i = 0, j = 0, k = 0, l = 0, z = 0;
	int buf[3][(RIGHT - LEFT + 1) / THREAD] = {};

	num_per = (RIGHT - LEFT + 1) / THREAD;

	for(i = LEFT; i <= RIGHT; i++){
		if(i % 3 == 0){
			buf[0][j++] = i;
		}
		if(i % 3 == 1){
			buf[1][k++] = i;
		}
		if(i % 3 == 2){
			buf[2][l++] = i;
		}
	}

	for(i = 0; i <  THREAD; i++){
		fflush(NULL);
		pid = fork();
		if(pid < 0){
			perror("fork()");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			for(z = 0; z < num_per; z++){
				if(is_primer(buf[i][z])){
					printf("%d id primer!\n", buf[i][z]);
				}
			}
			exit(EXIT_SUCCESS);
		}
	}
	exit(EXIT_SUCCESS);
}

static int is_primer(int n)
{
	int i;		
	for (i = 2; i <= n/2; i++) {
		if (n % i == 0)
			return 0;	
	}
	return 1;
}



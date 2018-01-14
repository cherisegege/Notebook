#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
	int i, j;
	pid_t pid;

	for(i = 0; i < 1000; i++){
		fflush(NULL);
		pid = fork();
		if(pid < 0){
			perror("fork()");
			printf("%d\n", i+1);		
			for(j = 0; j < i; j++){
				wait(NULL);
			}
			exit(EXIT_FAILURE);
		}
		if(pid == 0){	
			sleep(1);	
			exit(EXIT_SUCCESS);
		}
	}	

	exit(EXIT_SUCCESS);
}


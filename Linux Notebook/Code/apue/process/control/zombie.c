#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	int i;				
	pid_t pid, rpid;
	int status;

	for (i = 0; i < 10; i++) {
		fflush(NULL);	
		pid = fork();	
		if (pid < 0)
			exit(EXIT_FAILURE);
		if (pid == 0) {
			//sleep(100);
			printf("the %dth child process:%ld\n", i+1, getpid());
			sleep(1);
			exit(100);	
		}
	}
	
	//sleep(1000);
	
	for (i = 0; i < 10; i++) {
#if 0
		rpid = wait(&status);
		printf("return %ld status:%d\n", rpid, status);
#endif
		wait(&status);
		printf("%d\n", WEXITSTATUS(status));
	}

	exit(EXIT_SUCCESS);
}


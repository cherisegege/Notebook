#include <stdio.h>
#include <stdlib.h> /*for exit*/
#include <unistd.h> /*for fork*/

int main(void)
{
	pid_t pid;

	printf("[%d]beginning!!!!!\n", getpid());

	/*!!!!!!!!!!!!!!!!!1*/
	fflush(NULL);
	pid = fork();	
	if (pid < 0) {
		perror("fork()");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {/*child*/
		sleep(1);
		printf("[child]pid:%ld, ppid:%ld\n", getpid(), getppid());
	} else {
		printf("[parent]pid:%ld, ppid:%ld, cid:%ld\n",\
				getpid(), getppid(), pid);
	}

	printf("[%d]ending!!!!\n", getpid());

	exit(EXIT_SUCCESS);
}

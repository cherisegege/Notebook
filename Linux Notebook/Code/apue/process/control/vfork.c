#include <stdio.h>	/*for stdio*/
#include <sys/types.h>
#include <unistd.h>	/*for fork()*/
#include <stdlib.h>	/*for exit()*/

static int glob = 10;

int main(void)
{
	pid_t pid;
	int value = 100;
	
	printf("[%d]parent glob = %d, value = %d\n", getpid(), glob, value);
	pid = vfork();
	if (pid < 0) {
		perror("fork()");
		exit(EXIT_FAILURE);	
	}
	if (pid == 0) {
		/*child*/
		printf("[%d]before glob = %d, value = %d\n", getpid(), glob, value);
		glob ++;
		value ++;	
		printf("[%d]after glob = %d, value = %d\n", getpid(), glob, value);
		_exit(EXIT_SUCCESS);
	}

	sleep(5);

	printf("[%d]parent glob = %d, value = %d\n", getpid(), glob, value);


	exit(EXIT_SUCCESS);
}


#include <stdio.h>
#include <unistd.h>	/*for fork*/
#include <sys/types.h>	/*for wait*/
#include <sys/wait.h>
#include <stdlib.h>	/*for exit*/

#define LEFT	20000
#define RIGHT	20200

static int is_primer(int n)
{
	int i;		
	for (i = 2; i <= n/2; i++) {
		if (n % i == 0)
			return 0;	
	}

	return 1;
}

int main(void)
{
	pid_t pid;
	int i;

	for (i = LEFT; i <= RIGHT; i++) {
		fflush(NULL);
		pid = fork();
		if (pid < 0) {
			perror("fork()");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			//child
			if (is_primer(i)) {
				printf("[%ld] %d is a primer\n", getpid(), i);
			}
			exit(EXIT_SUCCESS);
		}
	}

	for (i = LEFT; i <= RIGHT; i++)
		wait(NULL);

	exit(EXIT_SUCCESS);
}


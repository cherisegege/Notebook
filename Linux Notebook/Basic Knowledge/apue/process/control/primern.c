#include <stdio.h>
#include <stdlib.h> /*for exit*/
#include <unistd.h> /*for fork*/

#define LEFT	30000000
#define RIGHT	30000200

#define PNR		3

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
	int i, n;

	for (n = 0; n < PNR; n++) {
		pid = fork();
		if (pid < 0) {
			perror("fork()");
			exit(1);
		}
		if (pid == 0) {
			for (i = LEFT+n; i <= RIGHT; i += PNR) {
				if (is_primer(i))
					printf("[%d] %d is a primer\n", n, i);
			}
			exit(0);
		}
	}

	for (i = 0; i < PNR; i++)
		wait(NULL);

	exit(EXIT_SUCCESS);
}


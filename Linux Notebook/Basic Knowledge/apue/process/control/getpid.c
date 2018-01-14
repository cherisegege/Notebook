#include <stdio.h>
#include <sys/types.h> /*for getpid*/
#include <unistd.h>
#include <stdlib.h>  /*for exit*/

int main(void)
{
	printf("the calling process pid is %ld\n", getpid());
	printf("the calling process ppid is %ld\n", getppid());

#if 0
	while (1) 
		;	
#endif

	exit(EXIT_SUCCESS);
}



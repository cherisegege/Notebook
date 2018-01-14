#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{

	printf("%s is starting...\n", __FUNCTION__);

#if 0
	execl("/bin/ls", "ls", "-l", NULL);
	perror("execl()");
	exit(1);
#endif

#if 0
	setenv("PATH", ".", 1);
	execlp("getpid", "getpid", NULL);
#endif

#if 0
	char *const arr[] = {"ls", "-l", NULL};
	execv("/bin/ls", arr);
#endif

#if 0
	char *const arr[] = {"ls", "-l", NULL};
	execvp("ls", arr);
#endif

	char *const envp[] = {"PWD=/home/emb09", "PATH=.", NULL};

	//execle("./getpid", "getpid", NULL, envp);
	execle("/bin/ls", "ls", NULL, envp);

	printf("%s is ending...\n", __FUNCTION__);

	exit(EXIT_SUCCESS);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	char *const envp[] = {"USER=haha", "PATH=hihi", NULL};

	//execle("./execle2", "./execle2", NULL, envp);
	execl("./execle2", "./execle2", NULL);

	exit(0);
}


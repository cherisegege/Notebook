#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc < 2)
		exit(1);

	setuid(atoi(argv[1]));

	printf("real user:%d, effective user: %d\n", getuid(), geteuid());


	exit(0);
}


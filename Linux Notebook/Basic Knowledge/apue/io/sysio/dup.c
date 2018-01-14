#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int fd;	

#if 0
	fd = dup(1);
	//if error
	printf("fd = %d\n", fd);

	close(1);


	write(fd, "class is over\n", strlen("class is over\n"));
#endif

	if (argc < 2) {
		fprintf(stderr, "Usage....\n");
		return 1;
	}
	
	fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	//if error
	
#if 0
	//不原子操作
	close(1);
	dup(fd);
#endif
	//原子化操作
	//dup2(fd, 1);
	close(1);
	fcntl(fd, F_DUPFD, 1);	

	puts("class is over");

	return 0;
}


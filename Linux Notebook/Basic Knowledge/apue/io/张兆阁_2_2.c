#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int df;
	int ret;

	if(argc < 3){
		printf("参数太少了!\n");
		printf("请加上要输出的内容!\n");
		return 1;
	}
	printf("更改之前的输出!\n");
	puts(argv[2]);
	df = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if(df == EOF){
		perror("open()");
		return -1;
	}
	ret = close(STDOUT_FILENO);
	if(ret == EOF){
		perror("open()");
		return -1;
	}
	ret = dup(df);
	if(ret == EOF){
		perror("dup()");
		return -1;
	}
	printf("更改之后的输出在这啦!\n");
	puts(argv[2]);

	ret = close(df);
	if(ret == EOF){
		perror("open()");
		return -1;
	}

	return 0;
}








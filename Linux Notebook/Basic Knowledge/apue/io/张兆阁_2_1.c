#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int mywc(const char *file, int *line);

int main(int argc, char **argv)
{
	int chno, line;

	if(argc < 2){
		printf("参数太少了!\n");
		return 1;
	}

	chno = mywc(argv[1], &line);

	printf("%d  %d\n", chno, line);

	return 0;
}

static int mywc(const char *file, int *line)
{
	int fd;
	int lineno = 0, chno = 0;
	char ch;
	int ret;

	fd = open(file, O_RDONLY);
	if(fd < 0){
		perror("open()");
	}
	while(1){
		ret = read(fd, &ch, 1);
		if(ret == EOF){
			perror("read()");
			return -1;
		}
		if(ret == 0)
			break;
		if(ch == '\n')
			lineno++;
		chno++;
	}
	if(close(fd) < 0){
		perror("close()");
	}

	*line = lineno;

	return chno;
}




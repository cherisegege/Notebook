#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FNAME	"./holefile"

int main(void)
{
	int fd;

	fd = open(FNAME, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	//if error

	write(fd, "good boys", 9);

	lseek(fd, 1024L*1024L*1024L-10L, SEEK_END);

	write(fd, "W", 1);

	close(fd);

	return 0;
}


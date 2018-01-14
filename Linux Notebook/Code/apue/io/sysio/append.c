#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;	
	char buf[100] = {};

	fd = open("./appendfile", O_RDWR | O_APPEND);
	if(EOF == fd){
		perror("open()");
	}
	//if error

	if (lseek(fd, 1L, SEEK_SET) < 0) {
		perror("lseek()");
		return 1;
	}

	read(fd, buf, 100);
	puts(buf);

	close(fd);

	return 0;
}

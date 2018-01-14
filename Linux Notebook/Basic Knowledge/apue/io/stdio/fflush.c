#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(void)
{

	while (1) {
		printf("good morning");
		if (fflush(stdout) == EOF) {//fflush(NULL);刷新流
			fprintf(stderr, "fflush():%s\n", strerror(errno));
			break;
		}
		usleep(100000);
	}

	return 0;
}


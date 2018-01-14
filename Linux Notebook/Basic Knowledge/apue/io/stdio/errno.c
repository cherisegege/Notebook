#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	FILE *fp = NULL;


	fp = fopen("/etc/passwd", "m");
	if (fp == NULL) {
		fprintf(stderr, "[%s][%d]fopen():%s\n", __FUNCTION__,__LINE__,strerror(errno));
		perror("fopen()");
	}

	return 0;
}


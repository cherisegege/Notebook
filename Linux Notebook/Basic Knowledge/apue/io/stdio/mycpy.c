#include <stdio.h>

#define BUFSIZE 1024

int main(int argc, char **argv)
{
	FILE *rp = NULL;
	FILE *wp = NULL;
	int cnt;
	char buf[BUFSIZE] = {};

	if (argc < 3) {
		fprintf(stderr, "Usage...\n");
		goto ERROR;
	}

	rp = fopen(argv[1], "r");	
	if (NULL == rp) {
		perror("fopen()");
		goto ERROR;
	}

	wp = fopen(argv[2], "w");
	if (NULL == wp) {
		perror("fopen()");	
		goto ERROR;
	}

	//将文件argv[1]中的内容全部输出到argv[2]
	while (1) {
		cnt = fread(buf ,1, BUFSIZE,rp);
		if (cnt == 0)
			break;
		fwrite(buf, 1, cnt, wp);
	}

	if (fclose(rp) == EOF) {
		perror("fclose()");
		goto ERROR;
	}

	if (fclose(wp) == EOF) {
		perror("fclose()");
		goto ERROR;
	}

	return 0;
ERROR:
	return 1;
}


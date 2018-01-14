#include <stdio.h>

#define BUFSIZE	100

int main(int argc, char **argv)
{
	FILE *fp = NULL;
	char buf[BUFSIZE] = {};
	size_t cnt;

	if (argc < 2) {
		printf("Usage...\n");
		goto ERROR;
	}

	fp = fopen(argv[1], "r");
	if (NULL == fp) {
		printf("fopen() failed\n");
		goto ERROR;
	}

	//read
	while (1) {
		cnt = fread(buf, 1, BUFSIZE, fp);
		if (cnt == 0)
			break;
		fwrite(buf, 1, cnt, stdout);
	}

	fclose(fp);

	return 0;
ERROR:
	return 1;
}


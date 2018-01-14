#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *fp = NULL;
	char *ptr = NULL;
	size_t n = 0;
	int cnt = 0;

	if (argc < 2) {
		goto ERROR;
	}

	fp = fopen(argv[1], "r");	
	if (fp == NULL) {
		perror("fopen()");
		goto ERROR;
	}

	cnt = getline(&ptr, &n, fp);
	if (cnt < 0) {
		fprintf(stderr, "getline() is failed\n");
		goto ERROR;
	}

	fseek(fp, cnt, SEEK_SET);

	fseek(fp, 5, SEEK_CUR);

	cnt = getline(&ptr, &n, fp);
	puts(ptr);

	return 0;
ERROR:
	return 1;
}


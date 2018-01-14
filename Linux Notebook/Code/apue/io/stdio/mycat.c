#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *rp = NULL;
	int ch;

	if (argc < 2) {
		fprintf(stderr, "Usage...\n");
		goto ERROR;
	}

	rp = fopen(argv[1], "r");	
	if (NULL == rp) {
		perror("fopen()");
		goto ERROR;
	}

	//将文件argv[1]中的内容全部输出到标准输出
	while (1) {
		ch = fgetc(rp);	
		if (ch == EOF) {
			if (ferror(rp)) {
				fprintf(stderr, "fgetc() failed\n");
			}
			if (feof(rp)) {
				printf("the file read over\n");	
			}
			break;
		}
		putchar(ch);
	}

	if (fclose(rp) == EOF) {
		perror("fclose()");
		goto ERROR;
	}

	return 0;
ERROR:
	return 1;
}


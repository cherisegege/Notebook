#include <stdio.h>

#define BUFSIZE	100

int main(int argc, char **argv)
{
	FILE *rp = NULL;
	FILE *wp = NULL;
	char buf[BUFSIZE] = {};
	size_t cnt;
	int ch;

	if (argc < 3) {
		fprintf(stderr, "Usage...\n");
		goto ERROR;
	}

	rp = fopen(argv[1], "r");
	if (NULL == rp) {
		fprintf(stderr, "fopen() failed\n");
		goto ERROR;
	}

	wp = fopen(argv[2], "w");
	if (NULL == wp) {
		fprintf(stderr, "fopen() failed\n");
		goto ERROR;
	}

	//read
	while (1) {
#if 0
		//二进制读写
		cnt = fread(buf, 1, BUFSIZE, rp);
		if (cnt == 0)
			break;
		fwrite(buf, 1, cnt, wp);
#endif
#if 0
		//以字符形式读写
		ch = fgetc(rp);	
		if (ch == EOF)
			break;
		fputc(ch, wp);
#endif
		//以行读写
		if (fgets(buf, BUFSIZE, rp) == NULL)
			break;
		fputs(buf, wp);
		//puts(buf);
	}

	fclose(rp);
	fclose(wp);

	return 0;
ERROR:
	return 1;
}


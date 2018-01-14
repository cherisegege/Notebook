
/*<1>复制"/etc/passwd"文件到家目录，名为"test"	
 *<2>将"test"文件的第三行行首插入字符串argv[1],其他内容不变
 * */
#include <stdio.h>
#include <string.h>

#define SRC		"/etc/passwd"
#define DEST	"./test"
#define BUFSIZE	100

int main(int argc, char **argv)
{
	FILE *rp, *wp;
	FILE *tmp;
	char buf[BUFSIZE] = {};
	int line = 0;
	int flag = 0;
	int cnt = 0;

	if (argc < 2) {
		fprintf(stderr, "Usage....\n");
		goto ERROR;
	}

	rp = fopen(SRC, "r");
	if (NULL == rp) {
		perror("fopen()");
		goto ERROR;
	}

	tmp = tmpfile();
	if (NULL == tmp) {
		perror("tmpfile()");
		goto ERROR;
	}

	/*
	 *SRC--->line3+argv[1]--->tmpfile
	 *tmpfile--->DEST
	 * */

	while (1) {
		if (fgets(buf, BUFSIZE, rp) == NULL)
			break;
		fputs(buf, tmp);	
		if (strlen(buf) < BUFSIZE-1  || buf[BUFSIZE-2] == '\n') {
			line++;	
		}
		if (line == 2 && flag == 0) {
			fputs(argv[1], tmp);
			flag = 1;
		}
	}

	fclose(rp);

	wp = fopen(DEST, "w+");
	if (NULL == wp) {
		perror("fopen()");
		goto ERROR;
	}

	rewind(tmp);

	while (1) {
		cnt = fread(buf, 1, BUFSIZE, tmp);
		if (cnt == 0)
			break;
		fwrite(buf, 1, cnt, wp);
	}

	fclose(tmp);
	fclose(wp);

	return 0;
ERROR:
	return 1;
}







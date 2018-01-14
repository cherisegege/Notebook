#include <stdio.h>

int main(void)
{
	FILE *fp = NULL;
	char buf[100] = {};

	fp = tmpfile();
	if (NULL == fp) {
		perror("tmpfile()");
		return 1;
	}

	fputs("good morning", fp);

	rewind(fp);

	fgets(buf, 100, fp);

	puts(buf);

	fclose(fp);


	return 0;
}

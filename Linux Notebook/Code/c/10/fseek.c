#include <stdio.h>

#define BUFSIZE	100

int main(void)
{
	FILE *fp = NULL;
	char buf[BUFSIZE] = {};

	fp = fopen("test", "w+");
	//if error;

	fputs("hello boys and girls", fp);
	
	printf("%ld\n", ftell(fp));

	//fseek(fp, 15, SEEK_SET);
	//fseek(fp, -5, SEEK_CUR);
	rewind(fp);
	/*(void)fseek(fp, 0L, SEEK_SET)*/
	printf("%ld\n", ftell(fp));

	fgets(buf, BUFSIZE, fp);

	puts(buf);

	fclose(fp);

	return 0;
}


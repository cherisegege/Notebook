/*
 *从标准输入文件读取一行,将其解析(以空格区分)
 * */
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1024

int main(void)
{
	char buf[BUFSIZE] = {};
	char *p = buf;
	char *ret;

	//"hello\n"
	fgets(buf, BUFSIZE, stdin);
	//去除'\n'
	if (strlen(buf) < BUFSIZE-1 || buf[BUFSIZE-2] == '\n')
		buf[strlen(buf)-1] = '\0';
	
	while (1) {
		ret = strtok(p, " ");//strsep();
		if (ret == NULL)
			break;
		puts(ret);
		p = NULL;
	}

	return 0;	
}


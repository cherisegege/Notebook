/*
 *封装一个函数，实现从标准输入读取一行字符串，存入动态存储空间中,将地址回填，函数的返回值为int(成功读取的字符的个数)
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_string(char **ptr)
{
	char *p = NULL;
	char *tmp = NULL;
	int i;
	int cursize = 0;
	
	p = malloc(10);	
	//p = calloc(10, 1);
	if (p == NULL)
		return -1;
	memset(p, '\0', 10);
	//memset(p, 0x00, 10);

	cursize = 10;
	i = 0;
	while (1) {
		scanf("%c", p+i);		
		if (*(p+i) == '\n') {
			p[i] = '\0';
			break;
		}
		i++;
		if (i == cursize) {
			p = realloc(p, cursize+5);	
			if (p == NULL) {
				return -1;
			}
			cursize += 5;
		}
	}

	if (i+1 < cursize) {
		p = realloc(p, i+1);	
		if (p == NULL)
			return -1;
	}

	*ptr = p;

	return i;
}

int main(void)
{
	char **p;
	int ret;

	ret = get_string(p);
	if (ret < 0) {
		printf("FAILED\n");
		goto ERROR;
	}	

	printf("%s\n", *p);


	return 0;
ERROR:
	return 1;
}


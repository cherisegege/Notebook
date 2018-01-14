#include <stdio.h>
#include <string.h>

int main(void)
{
	char str[] = "cla aaaaaaa ss   is  \nc  program";
	char *delim = "a \n";
	char *ret;
	char *p = str;
	
	while (1){
		ret = strtok(p, delim);
		if (ret == NULL)
			break;
		printf("%s\n", ret);
		p = NULL;
	}

	return 0;
}

#include <stdio.h>
#include <string.h>

int main(void)
{
	char str[] = "class   is a  pro\ngram";
	char *dilim = " \n";
	char *ret = NULL;
	char *p = str;


	while (1) {
		ret = strsep(&p, dilim);
		if (ret == NULL)
			break;
		//分割符连续的时候
//		if (*ret == '\0')
//			continue;
		printf("%s\n", ret);
	}

	return 0;
}

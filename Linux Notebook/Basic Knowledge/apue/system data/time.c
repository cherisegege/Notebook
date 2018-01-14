#include <stdio.h>
#include <time.h>

#define BUFSIZE	1024

int main(void)
{
	time_t tm;
	struct tm *tmp;
	char buf[BUFSIZE] = {};

	time(&tm);

	tmp = localtime(&tm);
	if (NULL == tmp) {
		fprintf(stderr, "localtime() failed\n");
		goto ERROR;
	}

	//strftime(buf, BUFSIZE, "%D %H:%M:%S", tmp);
	strftime(buf, BUFSIZE, "%m月 %d  %H:%M", tmp);

	puts(buf);

	puts(ctime(&tm));

	printf("%lld\n", mktime(tmp));

	return 0;

ERROR:
	return 1;
}


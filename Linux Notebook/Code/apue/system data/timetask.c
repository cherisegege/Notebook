/*
 *打印输出argv[1]天后的日期
 * */
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
	time_t tm;
	struct tm *tmp;
	char buf[BUFSIZ] = {};

	if (argc < 2)
		return 1;	

	tm = time(NULL);

	//tmp = localtime(&tm);
	tmp = gmtime(&tm);
	//if error

	strftime(buf, BUFSIZ, "%D %H", tmp);
	puts(buf);

	tmp->tm_mday += atoi(argv[1]);
	(void)mktime(tmp);	
	
	strftime(buf, BUFSIZ, "%D %H", tmp);
	puts(buf);

	return 0;
}


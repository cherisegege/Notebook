#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
 Y:2016
 y:16
 m:month
 d:day
 -H:hour  12  24
 -M:minute
 -S:second
 * */

#define OPTSTR	"-yYmdH:MS"
#define BUFSIZE	1024

int main(int argc, char **argv)
{
	int c;
	time_t tm;
	struct tm *tmp;
	char buf[BUFSIZE] = {};
	char format[BUFSIZE] = {};
	FILE *fp = stdout;

	while (1) {
		c = getopt(argc, argv, OPTSTR);
		if (c < 0)
			break;
		switch (c) {
			case 1:
				fp = fopen(argv[optind-1], "w");	
				if (fp == NULL) {
					perror("fopen()");
					fp = stdout;
				}
				break;
			case 'Y':
				strncat(format, "%Y年", BUFSIZE);	
				break;
			case 'y':
				strncat(format, "%y年", BUFSIZE);
				break;
			case 'm':
				strncat(format, "%m月", BUFSIZE);
				break;
			case 'd':
				strncat(format, "%d日", BUFSIZE);
				break;
			case 'H':
				if (!strcmp(optarg, "12"))
					strncat(format, "%I(%p)时", BUFSIZE);
				else if (!strcmp(optarg, "24")) 
					strncat(format, "%H时", BUFSIZE);
				else
					_exit(1);
				break;
			case 'M':
			   strncat(format, "%M分", BUFSIZE);	
			   break;
			case 'S':
			   strncat(format, "%S秒", BUFSIZE);
			   break;
			default:
				break;
		}

	}

	time(&tm);

	tmp = localtime(&tm);

	strncat(format, "\n", BUFSIZE);
	strftime(buf, BUFSIZE, format, tmp);

	fputs(buf, fp);

	exit(EXIT_SUCCESS);	//EXIT_FAILURE
}


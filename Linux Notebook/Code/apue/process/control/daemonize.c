/*
 *创建一个守护进程，每秒钟向"/tmp/out"文件写入时间(年/月/日 时:分:秒)
 *
 * */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	/*sysio*/
#include <stdio.h>	/*stdio*/
#include <unistd.h>	/*for fork*/
#include <stdlib.h>	/*for exit*/
#include <time.h>	/*time*/
#include <errno.h>
#include <syslog.h>

#define BUFSIZE	1024

static void mydaemon(void)
{
	pid_t pid;
	int fd;

	pid = fork();
	if (pid < 0) {
//		perror("fork()");
		syslog(LOG_ERR, "fork():%s", strerror(errno));		
		exit(1);
	}
	if (pid > 0) /*parent*/
		exit(0);

	setsid();//new session pid == pgid == sid

	syslog(LOG_INFO, "mydaemon is ok");

	fd = open("/dev/null", O_WRONLY);
	if (fd < 0) {
		//perror("open()");
		syslog(LOG_ERR, "open(): %s", strerror(errno));
		exit(1);
	}
	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);
	if (fd > 2)
		close(fd);

	umask(0);
	chdir("/");
}

int main(void)
{
	FILE *fp;
	time_t tm;
	struct tm *tmp;
	char buf[BUFSIZE] = {};

	openlog("mydaemon", LOG_PID | LOG_PERROR, LOG_DAEMON);

//	mydaemon();
	daemon(0, 0);
	
	fp = fopen("/tmp/out", "w");
	if (NULL == fp) {
		//perror("fopen()");
		syslog(LOG_ERR, "fopen():%s", strerror(errno));
		exit(1);
	}

	while (1) {
		time(&tm);
		tmp = localtime(&tm);
		if (tmp == NULL) {
			//fprintf(stderr, "localtime() failed\n");
			syslog(LOG_ERR, "localtime() failed");
			break;
		}
		//fprintf(fp, "%d/%d/%d %02d:%02d:%02d\n", tmp->tm_year+1900...);
		strftime(buf, BUFSIZE, "%D %H:%M:%S\n", tmp);
		fputs(buf, fp);
		fflush(NULL);
		syslog(LOG_INFO, "write to \"/tmp/out\" is ok");
		sleep(1);
	}

	fclose(fp);
	closelog();

	exit(0);
}


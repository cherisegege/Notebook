/**************************
 * 蜂鸣器的用户态测试
 * $>buzz01 /dev/mybuzz start
 * $>buzz01 /dev/mybuzz stop
 * $>buzz01 /dev/mybuzz freq	1~10000
 * $>buzz01 /dev/mybuzz duty	0~100
 * Author: zht
 * Date: 2016-10-16
 ***************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

//定义ioctl命令号
#define BUZZ_TYPE		'P'
#define BUZZ_START		_IO(BUZZ_TYPE, 1)
#define BUZZ_STOP		_IO(BUZZ_TYPE, 2)
#define BUZZ_SET_FREQ	_IOW(BUZZ_TYPE, 3, int)
#define BUZZ_SET_DUTY	_IOW(BUZZ_TYPE, 4, int)

int main(int argc, char *argv[])
{
	int ret, value, fd;

	if (argc<3 || argc>4) {
		printf("Usage: %s /dev/xxx start|stop|freq|duty ...\n");
		exit(1);
	}

	fd = open(argv[1], O_RDWR);
	if (fd<0) {
		printf("Cannot open %s\n", argv[1]);
		exit(1);
	}

	if (strncmp(argv[2], "start", 5)==0) {
		ret = ioctl(fd, BUZZ_START);
	} 
	else if (strncmp(argv[2], "stop", 4)==0) {
		ret = ioctl(fd, BUZZ_STOP);
	} 
	else if (strncmp(argv[2], "freq", 4)==0) {
		value = atoi(argv[3]);
		ret = ioctl(fd, BUZZ_SET_FREQ, value);
	}
	else if (strncmp(argv[2], "duty", 4)==0) {
		value = atoi(argv[3]);
		ret = ioctl(fd, BUZZ_SET_DUTY, value);
	}
	else {
		printf("Only support start|stop|freq|duty\n");
		exit(1);
	}

	if (ret) {
		printf("%s error\n", argv[2]);
	} else {
		printf("%s OK\n", argv[2]);
	}

	close(fd);
	exit(0);
}


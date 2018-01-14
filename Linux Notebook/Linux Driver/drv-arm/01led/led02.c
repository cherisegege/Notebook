/**************************
 * 用户态的led测试例子
 * (针对led_test03.c)
 * $>led02 /dev/myled on 0~3
 * $>led02 /dev/myled off 0~3
 * $>led02 /dev/myled allon
 * $>led02 /dev/myled alloff
 * Author: zht
 * Date: 2016-10-15
 **************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

//定义ioctl命令号
#define LED_TYPE	'L'
#define LED_ON		_IOW(LED_TYPE, 1, int)
#define LED_OFF		_IOW(LED_TYPE, 2, int)
#define LED_ALLON	_IO(LED_TYPE, 3)
#define LED_ALLOFF	_IO(LED_TYPE, 4)


int main(int argc, char *argv[])
{
	int i, ret, fd, value;
	if (argc<3 || argc>4) {
		printf("Usage: %s /dev/xxx on|off|allon|alloff 0~3\n", argv[0]);
		exit(1);
	}

	//打开设备文件
	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("Cannot open %s\n", argv[1]);
		exit(1);
	}

	if (strncmp(argv[2], "on", 2)==0) {
		value = atoi(argv[3]);
		ret = ioctl(fd, LED_ON, value);
		printf("点亮led%d\n", value);
	} else if (strncmp(argv[2], "off", 3)==0) {
		value = atoi(argv[3]);
		ret = ioctl(fd, LED_OFF, value);
		printf("熄灭led%d\n", value);
	} else if (strncmp(argv[2], "allon", 5)==0) {
		ret = ioctl(fd, LED_ALLON);
		printf("点亮所有灯\n");
	} else if (strncmp(argv[2], "alloff", 6)==0) {
		ioctl(fd, LED_ALLOFF);
		printf("熄灭所有灯\n");
	} else {
		printf("Only support on, off, allon, alloff\n");
		exit(1);
	}
	
	close(fd);
	exit(0);
}


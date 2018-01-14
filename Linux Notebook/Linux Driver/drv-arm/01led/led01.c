/**************************
 * 用户态的led测试例子
 * $>led01 /dev/myled allon
 * $>led01 /dev/myled alloff
 * $>led01 /dev/myled allstatus
 * Author: zht
 * Date: 2016-09-25
 **************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define LED_NUM		4

struct led_info {
	int led_num; 
	int led_status;
};

struct led_all_info {
	int led_status[LED_NUM];
};

//定义ioctl号，参数为灯的编号(0~3)
//STATUS命令可以用参数或返回值代表灯的状态
#define LED_TYPE	'L'
#define LED_ON		_IOW(LED_TYPE, 1, int)
#define LED_OFF		_IOW(LED_TYPE, 2, int)
#define LED_STATUS	_IOWR(LED_TYPE, 3, struct led_info)

#define LED_ALLON	_IO(LED_TYPE, 4)
#define LED_ALLOFF	_IO(LED_TYPE, 5)
#define LED_ALLSTATUS	_IOR(LED_TYPE, 6, struct led_all_info)

int main(int argc, char *argv[])
{
	int i, ret, fd;
	struct led_all_info info;
	if (argc != 3) {
		printf("Usage: %s /dev/xxx allon|alloff|allstatus\n", argv[0]);
		exit(1);
	}

	//打开设备文件
	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("Cannot open %s\n", argv[1]);
		exit(1);
	}

	if (strncmp(argv[2], "allon", 5)==0) {
		ioctl(fd, LED_ALLON);
		printf("点亮所有灯\n");
	}
	else if (strncmp(argv[2], "alloff", 6)==0) {
		ioctl(fd, LED_ALLOFF);
		printf("熄灭所有灯\n");
	}
	else if (strncmp(argv[2], "allstatus", 8)==0) {
		ret = ioctl(fd,	LED_ALLSTATUS, &info);
		if (ret != 0) {
			printf("get status error\n");
			exit(1);
		}
		for (i=0; i<LED_NUM; i++) {
			printf("led%d: %s\n", i, info.led_status[i] ? "亮" : "灭");
		}
	}
	else {
		printf("Only support allon, alloff, allstatus\n");
		exit(1);
	}
	
	close(fd);
	exit(0);
}


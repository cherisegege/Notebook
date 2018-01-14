/******************************
 * 用户态ioctl的测试例子
 * 使用方法：
 * $>user01 /dev/mybuf reset
 * $>user01 /dev/mybuf getst
 * $>user01 /dev/mybuf resize 500
 * Author: zht
 * Date: 2016-09-25
 ******************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>

//从驱动中获取ioctl号的定义
struct mem_info {
	int size;
	int wp;
};

#define	MEM_TYPE	'M'
#define	MEM_RESET	_IO(MEM_TYPE, 1)
#define MEM_RESIZE	_IOW(MEM_TYPE, 2, int)
#define MEM_STATUS	_IOR(MEM_TYPE, 3, struct mem_info)

int main(int argc, char *argv[])
{
	int ret, value, fd;
	struct mem_info tmp;

	//检查参数
	if (argc < 3 || argc > 4) {
		printf("Usage: %s /dev/xxx reset|getst|resize 500\n", argv[0]);
		exit(1);
	}

	//打开文件
	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("Cannot open %s\n", argv[1]);
		exit(1);
	}

	//根据argv[2]决定调用哪个命令
	if (strncmp(argv[2], "reset", 5)==0) {
		ret = ioctl(fd, MEM_RESET);
		if (ret)
			printf("reset %s error\n", argv[1]);
	}
	else if (strncmp(argv[2], "resize", 6)==0) {
		value = atoi(argv[3]);
		ret = ioctl(fd, MEM_RESIZE, value);
		if (ret)
			printf("resize %s to %dBytes error\n",  argv[2], value);
	}
	else if (strncmp(argv[2], "getst", 5)==0) {
		ret = ioctl(fd, MEM_STATUS, &tmp);
		if (ret) 
			printf("get status error\n");
		else
			printf("%s: 大小为%dB, wp = %d\n", argv[2], tmp.size, tmp.wp);
	}
	else {
		printf("Only support reset|resize|getst\n");
		exit(1);
	}

	close(fd);
	exit(0);
}


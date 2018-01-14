/*************************
 * 用户态的测试例子
 * $>plat01 /dev/xxx get 0x10
 * $>plat01 /dev/xxx set 0x0 0x12345678
 * 参数3为寄存器偏移，参数4为要设置的值
 * Author: zht
 * Date: 2015-09-08
 *************************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

//定义ioctl命令
struct reg_info {
	int offset; //寄存器的偏移
	int value; //寄存器的值
};

#define PLAT_TYPE	'P'
#define GET_REG		_IOWR(PLAT_TYPE, 1, struct reg_info)
#define SET_REG		_IOW(PLAT_TYPE, 2, struct reg_info)

int main(int argc, char *argv[])
{
	int ret=0, fd;
	struct reg_info info = {0};

	//首先判断参数是否正确
	if (argc<4 || argc>5) {
		printf("Usage: %s /dev/xxx get|set offset value\n", argv[0]);
		exit(1);
	}

	//打开设备文件
	fd = open(argv[1], O_RDWR);
	if (fd == -1) {
		printf("Cannot open %s\n", argv[1]);
		exit(1);
	}

	//获得寄存器偏移
	info.offset = strtoul(argv[3], NULL, 16);

	//根据argv[2]来发送命令
	if (strncmp(argv[2], "get", 3)==0) {
		info.value = 0;
		ret = ioctl(fd, GET_REG, &info);
	}
	else if (strncmp(argv[2], "set", 3)==0) {
		info.value = strtoul(argv[4], NULL, 16);
		ret = ioctl(fd, SET_REG, &info);
	}
	else {
		printf("Only support get|set\n");
		exit(1);
	}

	//ret为0成功；非0失败
	if (ret) {
		printf("%s reg %#x, error(%#x)\n", argv[2], info.offset, ret);
		exit(1);
	}
	else {
		printf("%s reg %#x, value %#x\n", argv[2], info.offset, info.value);
	}

	close(fd);
	exit(0);
}


/******************************
 * tty串口的测试例子2
 * 测试串口的数据发送；
 * $>tty_write
 * $>tty_write /dev/ttySAC0
 * 参数：设备文件名，默认为ttySAC0
 * Author: zht
 * Date: 2016-11-26
 ******************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h> //posix终端控制，参考bits/termios.h

#define DEF_TTY		"/dev/ttySAC0"
#define DEF_BAUD	115200
#define DEF_DATAS	8
#define DEF_PARITY	'N'
#define DEF_STOPS	1

#define BUF_SIZE	20
#define NAME_SIZE	30
#define SPEED_CNT	5


const int speeds[SPEED_CNT] = {
	B9600, B19200, B38400, 
	B57600,	B115200
};
const int names[SPEED_CNT] = {
	9600, 19200, 38400,
	57600, 115200
};

char dev_name[NAME_SIZE] = {0};
char *str = "hello, just a test string\r\n";


//设置波特率
int set_speed(int fd, int speed)
{
	int i;
	int status;
	struct termios options;

	tcgetattr(fd, &options);
	for (i=0; i<SPEED_CNT; i++) {
		if (speed != names[i])
			continue;
		tcflush(fd, TCIOFLUSH);
		cfsetispeed(&options, speeds[i]);
		cfsetospeed(&options, speeds[i]);
		status = tcsetattr(fd, TCSANOW, &options);
		if (status != 0) {
			perror("tcsetattr");
			return -1;
		}
		tcflush(fd, TCIOFLUSH);
		return 0;
	}
	printf("Cannot find suitable speed\n");
	return -1;
}


//设置数据位、停止位和校验位
int set_parity(int fd, int databits, int parity, int stopbits)
{
	struct termios options;
	if (tcgetattr(fd, &options) != 0) {
		perror("tcgetattr");
		return -1;
	}

 	//设置数据位数
	options.c_cflag &= ~CSIZE;
	switch (databits) {
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		printf("Unsupported data size %d", \
			databits); 
		return -1;
	}

	//设置校验位
	switch (parity) {
	case 'n':
	case 'N':
		//清除校验使能位
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
		break;
	case 'o':
	case 'O':
		//使能奇校验
		options.c_cflag |= (PARODD | PARENB);
		options.c_iflag |= INPCK;
		break;
	case 'e':
	case 'E':
		//使能偶校验
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;
	case 'S':
	case 's':
		//无校验，无停止位
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		printf("Unsupported parity %c\n", parity);
		return -1;
	}

	//设置停止位
	switch (stopbits) {
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		printf("Unsupported stop bits %d\n", stopbits);
		return -1;
	}

	/* Set input parity option */
	if ((parity != 'n') || (parity != 'N'))
		options.c_iflag |= INPCK;
	tcflush(fd, TCIFLUSH);
	options.c_cc[VTIME] = 150; //设置超时15秒
	options.c_cc[VMIN] = 0;    //有数据马上读写

	//采用原始模式通讯
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_oflag &= ~OPOST;

	if (tcsetattr(fd, TCSANOW, &options) != 0) {
		perror("tcsetattr");
		return -1;
	}
	return 0;
}


int main(int argc, char *argv[])
{
	int fd, ret;
	int nwrite = 0;

	if (argc == 1) {
		snprintf(dev_name, NAME_SIZE, DEF_TTY);
	} else if (argc == 2) {
		snprintf(dev_name, NAME_SIZE, argv[1]);
	} else {
		printf("Please use $>tty01 /dev/xxx\n");
		exit(1);
	}

	//打开串口设备，后面的两个标志必须要加
	fd = open(dev_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd == -1) {
		printf("Cannot open %s\n", dev_name);
		exit(1);
	}
	printf("====open %s====\n", dev_name);

	//初始化设备
	ret = set_speed(fd, 115200);
	if (ret == -1) {
		printf("Cannot set baudrate to 115200\n");
		goto out;
	}

	if (set_parity(fd, 8,'N', 1) == -1) {
		printf("Set Parity error");
		exit(1);
	}

	//循环向设备写入数据
	printf("====Begin to write to %s====\n", dev_name);
	while (1)
	{
		nwrite = write(fd, str, strlen(str));
		printf("write %dB, %s\n", nwrite, str);
		sleep(1);
	}
	close(fd);
	return 0;
out:
	close(fd);
	exit(1);
}





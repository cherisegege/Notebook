/*************************
 * FT5206GE1芯片的头文件
 * 定义寄存器的偏移；
 * 定义驱动要使用的结构体(包括私有结构体)；
 * Author: zht
 * Date: 2016-08-12
 *************************/
#ifndef __LINUX_TS5206_H__
#define __LINUX_TS5206_H__

#define TS5206_NAME		"ft5206_ts"
#define TS5206_PT_MAX	5  //最多5点触控

/****************************
 * 定义FT5206芯片中部分寄存器的偏移
 ****************************/
#define REG_PMODE		0xA5
#define REG_FIRMID		0xA6
#define REG_CPTMID		0xA8
#define REG_ERR			0xA9
#define REG_CLB			0xAA


//一次采样获得的触控点信息
struct ft5206_event {
	int touch_point;	//当前支持几点触控
	u16 x[TS5206_PT_MAX];	//触控点的坐标(最多5点)
	u16 y[TS5206_PT_MAX];
	u16 pressure;
};


//私有结构体
struct ft5206_ts_priv {
	struct input_dev *idev;
	struct ft5206_event event;

	//gpio号(GPX1_6)
	//用gpio_to_irq()从GPIO号转为中断号，存入i2c_client->irq
	int gpio_irq;

	//屏幕分辨率和压力的最大值
	int screen_max_x;
	int screen_max_y;
	int pressure_max;

	//在中断的下半部中读取坐标数据;
	//向input子系统提交input_event;
	//tasklet, workqueue(工作队列)
	struct work_struct work;
};

#endif	// __LINUX_TS5206_H__

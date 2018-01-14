/**************************
 * FT5206电容屏的input驱动
 * 通过查5206的电路图，获知：
 * 5206和4412之间有中断，采用的IO为GPX1_6
 * 5206和4412的I2C控制器1相连；
 * 驱动用i2c_driver来封装
 * 在i2c_driver->probe中初始化并注册input_dev；
 * 在中断处理程序中生成input_event并传递给input子系统；
 * Author: zht
 * Date: 2016-08-12
 **************************/
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/time.h>  //timeval
#include <linux/slab.h> //kzalloc
#include <linux/interrupt.h> //中断
#include <linux/workqueue.h> //workqueue下半部
#include <linux/input.h> //input_dev & input_event
#include <linux/gpio.h> //gpio_to_irq
#include <linux/i2c.h> //i2c_client & i2c_driver
#include <plat/ft5x0x_touch.h>
#include "ts5206.h"

/* 开关：支持多点触控还是单点触控 */
#define FT5206_MULTITOUCH	1

//i2c_client在i2c_driver->probe函数中传入
//通过i2c接口库访问i2c设备时，需要该结构体的指针，所以要保存为驱动全局变量
static struct i2c_client *client;
static int irq_cnt;


/*****************************
 * 从Ft5206读入多个字节寄存器
 * rxdata: 返回的寄存器
 * length: 字节数
 * offset: 要访问的寄存器的偏移
 *****************************/
static int ft5206_i2c_rxdata(char *rxdata, int length, u8 offset) 
{
	int ret;

	//i2c_msg[0]: 发送寄存器偏移
	//i2c_msg[1]: flags为I2C_M_RD，从Ft5206读数据
	struct i2c_msg msgs[] = {
	{
		.addr	= client->addr,
		.flags	= 0, //写寄存器偏移
		.len	= 1,
		.buf	= &offset,
	},
	{	//读多个寄存器
		.addr	= client->addr,
		.flags	= I2C_M_RD,
		.len	= length,
		.buf	= rxdata,
	},
	};

	ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (ret < 0)
		printk("%s: i2c read error: %d\n", __func__, ret);

	return ret;
}


/***************************
 * 从ft5206读入一个寄存器
 * offset: 寄存器的偏移(见ts5206.h)
 * pdata: 从寄存器读入的数据
 ***************************/
static int ft5206_read_reg(u8 offset, u8 *pdata) 
{
	int ret;

	//每个i2c_msg用于完成一次i2c事务
	struct i2c_msg msgs[] = {
	{
		.addr	= client->addr,
		.flags	= 0,//向5206写寄存器偏移
		.len	= 1,
		.buf	= &offset,
	},
	{		
		.addr	= client->addr,
		.flags	= I2C_M_RD,//从5206读
		.len	= 1,
		.buf	= pdata,
	},
	};

	//将初始化好的两个i2c_msg传给i2c接口库
	//函数有可能阻塞，一旦返回则获得ft5206对应寄存器的值
	ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (ret < 0)
		printk("read reg (0x%02x) error, %d\n", offset, ret);

	return ret;
}


/***********************
 * 在工作队列的执行函数中调用
 * 向input子系统报告input_event
 * 支持多点或单点触控
 ***********************/
static void ft5206_ts_report(struct ft5206_ts_priv *priv) 
{
	struct ft5206_event *event = &priv->event;
	int x, y;
	int i = 0;

#ifdef FT5206_MULTITOUCH
	//多点触控时不需要报告BTN_TOUCH(最多5点)
	for (i = 0; i < event->touch_point; i++) {
		x = event->x[i];
		y = event->y[i];
		input_report_abs(priv->idev, ABS_MT_POSITION_X, x);
		input_report_abs(priv->idev, ABS_MT_POSITION_Y, y);
		input_mt_sync(priv->idev);
	}
#else 
	//单点触控
	if (event->touch_point == 1) {
		x = event->x[0];
		y = event->y[0];
		input_report_abs(priv->idev, ABS_X, x);
		input_report_abs(priv->idev, ABS_Y, y);
	}
	input_report_key(priv->idev, BTN_TOUCH, 1);
#endif  /* FT5206_MULTITOUCH */

	input_sync(priv->idev);
}


/*************************
 * 手离开电容屏后，向input子系统报告触控结束
 *************************/
static void ft5206_ts_release(struct ft5206_ts_priv *priv) 
{
#ifdef FT5206_MULTITOUCH
	input_mt_sync(priv->idev);
#else
	input_report_abs(priv->idev, ABS_PRESSURE, 0);
	input_report_key(priv->idev, BTN_TOUCH, 0);
#endif
	input_sync(priv->idev);
}


/*********************************
 * 每次发生中断时调用
 * 从电容屏读入一组触控信息(返回0为成功)
 * 填入私有结构体中的ft5206_event中
 * 单点触控读入前7个字节; 
 * 多点触控读入前31个字节(参FT5206应用手册P8的Operating Mode)
 **********************************/
static int 
ft5206_read_data(struct ft5206_ts_priv *priv) 
{
	struct ft5206_event *event = &priv->event;
	u8 buf[32] = { 0 };
	int ret;

#ifdef FT5206_MULTITOUCH
	ret = ft5206_i2c_rxdata(buf, 31, 0);
#else
	ret = ft5206_i2c_rxdata(buf, 7, 0);
#endif
	if (ret < 0) {
		printk("%s: read touch data failed, %d\n", __func__, ret);
		return ret;
	}

	memset(event, 0, sizeof(struct ft5206_event));

	//5206的02h中记录触控点的数量
	//如果触控点数量为0，则报告触控结束
	event->touch_point = buf[2] & 0x07;
	if (event->touch_point == 0) {
		ft5206_ts_release(priv);
		return 1;
	}

#ifdef FT5206_MULTITOUCH
	switch (event->touch_point) {
	case 5:
		event->x[4] = (s16)(buf[0x1b] & 0x0F)<<8 | (s16)buf[0x1c];
		event->y[4] = (s16)(buf[0x1d] & 0x0F)<<8 | (s16)buf[0x1e];
	case 4:
		event->x[3] = (s16)(buf[0x15] & 0x0F)<<8 | (s16)buf[0x16];
		event->y[3] = (s16)(buf[0x17] & 0x0F)<<8 | (s16)buf[0x18];
	case 3:
		event->x[2] = (s16)(buf[0x0f] & 0x0F)<<8 | (s16)buf[0x10];
		event->y[2] = (s16)(buf[0x11] & 0x0F)<<8 | (s16)buf[0x12];
	case 2:
		event->x[1] = (s16)(buf[0x09] & 0x0F)<<8 | (s16)buf[0x0a];
		event->y[1] = (s16)(buf[0x0b] & 0x0F)<<8 | (s16)buf[0x0c];
	case 1:
		event->x[0] = (s16)(buf[0x03] & 0x0F)<<8 | (s16)buf[0x04];
		event->y[0] = (s16)(buf[0x05] & 0x0F)<<8 | (s16)buf[0x06];
		break;
	default:
		printk("%s: invalid touch data, %d\n", __func__, event->touch_point);
			return -1;
	}
#else
	if (event->touch_point == 1) {
		event->x[0] = (s16)(buf[0x03] & 0x0F)<<8 | (s16)buf[0x04];
		event->y[0] = (s16)(buf[0x05] & 0x0F)<<8 | (s16)buf[0x06];
	}
#endif

	event->pressure = 200;

	return 0;
}


/**************************
 * 下半部的执行函数
 **************************/
static void 
ft5206_ts_work(struct work_struct *work) 
{
	struct ft5206_ts_priv *priv = container_of(work, struct ft5206_ts_priv, work);

	if (!ft5206_read_data(priv)) {
		ft5206_ts_report(priv);
	}

	//使能触摸屏中断
	enable_irq(client->irq);
}


/***********************
 * 5206的中断处理函数
 ***********************/
static irqreturn_t 
ft5206_ts_interrupt(int irq, void *dev_id) 
{
	struct ft5206_ts_priv *priv = dev_id;
	struct timeval time;

	irq_cnt++;
	do_gettimeofday(&time);
	printk("%d-->%lds:%ldus\n", irq_cnt, time.tv_sec, time.tv_usec);
	//关闭中断并启动工作队列
	disable_irq_nosync(client->irq);
	schedule_work(&priv->work);

	return IRQ_HANDLED;
}


/**********************
 * i2c_driver->probe
 **********************/
static int 
ft5206_ts_probe(struct i2c_client *i2c_dev, const struct i2c_device_id *id)
{
	struct ft5x0x_i2c_platform_data *pdata;
	struct ft5206_ts_priv *priv;
	struct input_dev *input_dev;
	u8 val;
	int err = -EINVAL;

	client = i2c_dev;
	printk("*** ft5206芯片的地址: %#x ***\n", client->addr);

	//1.检验adapter是否支持需要的功能(可选)
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
		return -ENODEV;

	//读入并检查厂商ID
	err = ft5206_read_reg(REG_CPTMID, &val);
	if (err < 0) {
		dev_err(&i2c_dev->dev, "chip not found\n");
		return -ENODEV;
	}
	printk("%s: CPTM vendor ID 0x%x\n", TS5206_NAME, val);

	//2.分配私有结构体，存入i2c_client中
	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		err = -ENOMEM;
		goto alloc_failed;
	}
	i2c_set_clientdata(client, priv);
	//platform_set_drvdata(pdev, priv);

	//3.从i2c_client中取得platform_data(随i2c_client一起提供的设备特定硬件信息)
	pdata = client->dev.platform_data;
	if (!pdata) {
		dev_err(&client->dev, "failed to get platform data\n");
		goto no_pdata;
	}
	priv->screen_max_x = pdata->screen_max_x;
	priv->screen_max_y = pdata->screen_max_y;
	priv->pressure_max = pdata->pressure_max;

	//4.初始化GPIO
	//用gpio_to_irq获得和4412的GPIO对应的中断号
	if (pdata->gpio_irq != -EINVAL) {
		priv->gpio_irq = pdata->gpio_irq;
		client->irq = gpio_to_irq(priv->gpio_irq);
	} else {
		goto no_pdata;
	}
	printk("===gpio号: %d===\n", priv->gpio_irq);
	printk("===irq号: %d===\n", client->irq);

	//5.初始化work_struct(工作队列下半部)
	INIT_WORK(&priv->work, ft5206_ts_work);

	//6.分配并初始化input_dev，指针存入私有结构体
	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		dev_err(&client->dev, "failed to allocate input device\n");
		goto input_alloc_failed;
	}
	priv->idev = input_dev;

	//7.进一步初始化input_dev
	//ft5x0x_ts是input_dev名字，由于友善在android的HAL层根据该名字做了一些工作，因此只能用这个名字
	input_dev->name = "ft5x0x_ts";
	input_dev->id.bustype = BUS_I2C;
	input_dev->id.vendor  = 0xC3B4;
	input_dev->id.product = 0xABCD;
	input_dev->id.version = 0x0F00;

	//初始化input设备的位图
	//set_bit(第几位,位图的基地址)
	//clear_bit()
	set_bit(EV_SYN, input_dev->evbit);
	set_bit(EV_ABS, input_dev->evbit);
	set_bit(EV_KEY, input_dev->evbit);

#ifdef FT5206_MULTITOUCH //支持多点触控
	set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	set_bit(ABS_MT_POSITION_Y, input_dev->absbit);

	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, priv->screen_max_x, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, priv->screen_max_y, 0, 0);
#else //单点触控
	set_bit(ABS_X, input_dev->absbit);
	set_bit(ABS_Y, input_dev->absbit);
	set_bit(BTN_TOUCH, input_dev->keybit);
	input_set_abs_params(input_dev, ABS_X, 0, priv->screen_max_x, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, priv->screen_max_y, 0, 0);
#endif //FT5206_MULTITOUCH

	//8.注册input_dev
	err = input_register_device(input_dev);
	if (err) {
		dev_err(&client->dev, "failed to register input device %s, %d\n", dev_name(&client->dev), err);
		goto input_register_failed;
	}

	//9.注册中断处理函数
	err = request_irq(client->irq, ft5206_ts_interrupt,
			IRQF_TRIGGER_FALLING, "ft5206_ts", priv);
	if (err < 0) {
		dev_err(&client->dev, "Request IRQ %d failed, %d\n", client->irq, err);
		goto irq_failed;
	}

	printk("FocalTech ft5x0x TouchScreen initialized\n");
	return 0;

irq_failed:
	input_unregister_device(input_dev);
input_register_failed:
	input_free_device(input_dev);
input_alloc_failed:
	cancel_work_sync(&priv->work);
	i2c_set_clientdata(client, NULL);
no_pdata:
	kfree(priv);
alloc_failed:
	dev_err(&client->dev, "probe ft5x0x TouchScreen failed, %d\n", err);
	return err;
}


/**********************
 * i2c_driver->remove
 **********************/
static int __devexit 
ft5206_ts_remove(struct i2c_client *i2c_dev) 
{
	struct ft5206_ts_priv *priv = i2c_get_clientdata(i2c_dev);

	if (i2c_dev->irq) {
		free_irq(i2c_dev->irq, priv);
	}
	flush_scheduled_work();
	input_unregister_device(priv->idev);
	input_free_device(priv->idev);
	kfree(priv);

	return 0;
}

//platform总线的match函数优先比较device_id
//如果i2c_driver不包含id_table，则比较传统的driver->name
//我支持谁
static struct i2c_device_id ft5206_ts_id[] = {
	{ "e4412-5206", 0 },
	{ "e4412-5406", 0 },
	{ "e4412-ft5206", 0 },
	{ "alpha-test", 0 },
	{ },
};
//下面的宏可以在运行modinfo命令时显示驱动的别名
MODULE_DEVICE_TABLE(i2c, ft5206_ts_id);


static struct i2c_driver ft5206_ts_driver = {
	.probe		= ft5206_ts_probe,
	.remove		= ft5206_ts_remove,
	.id_table	= ft5206_ts_id,
	.driver	= {
		.name	= TS5206_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init my_init(void)
{
	//注册i2c_driver
	return i2c_add_driver(&ft5206_ts_driver);
}

static void __exit my_exit(void)
{
	//注销i2c_driver
	i2c_del_driver(&ft5206_ts_driver);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("FocalTech ft5206 TouchScreen driver");


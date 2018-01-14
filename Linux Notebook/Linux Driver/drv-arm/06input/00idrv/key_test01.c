/***********************
 * Input driver for key
 * support linux 2.6.28
 * author: zhanghongtao
 * date: 2012-02-03
 ***********************/

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/timer.h>
#include <linux/input.h>

#define KEY_NUM	 4	/* maximum 4 */

/* private struct for one key */
struct key_input {
	int 	key_irq;	/* related key */
	char 	irq_name[10];	/* handler name */
	int	key_code;
	int 	num;
	struct timer_list timer;
	int 	ready;
	int 	key_press;
	int 	timers;
	unsigned long last_changed;
};

/* 每个按键用一个key_input记录，四个按键合在一起分配一个input_dev */
static struct key_input *devs[KEY_NUM];
static struct input_dev *idev;


/*********************
 * timer handler
 *********************/
static void 
key_timer(unsigned long data)
{
	struct key_input *dev = (void *)data;

	if (dev->last_changed == 0) {
		dev->last_changed = jiffies;
		goto pressed;
	}

	/* if press too fast, not record */
	if ((dev->last_changed+60) > jiffies) {
		dev->ready = 0;
		return;
	}

	/* 报告按键按下后迅速报告按键抬起，其实不太合理 */
pressed:
	input_report_key(idev, dev->key_code, 1);
	input_report_key(idev, dev->key_code, 0);
	input_sync(idev);
	dev->ready = 0;
	dev->last_changed = jiffies;
}


/************************
 * key interrupt handler 
 ************************/
static irqreturn_t 
key_handler(int irq, void *dev_id)
{
	struct key_input *dev = dev_id;
	if (!dev->ready++)
		mod_timer(&dev->timer, (jiffies+5));
	return IRQ_HANDLED;
}


static int 
keys_init(void)
{
	int i;
	int value;

	for (i=0; i<KEY_NUM; i++) {
		devs[i] = (struct key_input *)kzalloc(sizeof(*devs[i]), GFP_KERNEL);
		if (NULL == devs[i])
			return -ENOMEM;

		/* 设定按键值分别为A/S/D/F */
		devs[i]->num = i;
		devs[i]->key_code = KEY_A + i;
		set_bit(devs[i]->key_code, idev->keybit);

		/* setup timer_list */
		setup_timer(&devs[i]->timer, key_timer, (unsigned long)devs[i]);
	
		/* interrupt handler init */
		devs[i]->key_irq = IRQ_EINT(i);
		sprintf(devs[i]->irq_name, "key%d", i);
		value = request_irq(devs[i]->key_irq, /* irqnum */
			key_handler,	/* handler */
			IRQF_SHARED | IRQF_TRIGGER_FALLING,
			devs[i]->irq_name, /* name */
			devs[i]);	/* dev_id */
		if (value) {
			printk("Request key irq %d error\n", devs[i]->key_irq);
			kfree(devs[i]);	
			devs[i] = NULL;
			return value;
		}
	}

	return 0;
}


static void 
keys_exit(void)
{
	int i;
	for (i=0; i<KEY_NUM; i++) {
		if (devs[i]) {
			free_irq(devs[i]->key_irq, devs[i]);
			del_timer(&devs[i]->timer);
			kfree(devs[i]);
		}
	}
}


static int __init 
my_init(void)
{
	int ret = 0;

	/* allocate & init input_dev */
	idev = input_allocate_device();
	if (!idev) {
		return -ENOMEM;
	}
	
	idev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY);
	idev->name = "keys";
	idev->phys = "keys-phy";
	idev->id.bustype = BUS_HOST;
	idev->id.vendor = 0xdead;
	idev->id.product = 0xbeaf;
	idev->id.version = 0x0101;

	/* init all keys */
	ret = keys_init();
	if (ret) {
		printk("Alloc key's input_dev failed\n");
		goto err;
	}

	/* 将input_dev注册到input子系统 */
	ret = input_register_device(idev);
	if(ret) {
		printk("Cannot register input_dev(keys)!\n");
		ret = -EIO;
		goto err;
	}

	return 0;
err:
	keys_exit();
	return ret;
}


static void __exit 
my_exit(void)
{
	keys_exit();	
	input_unregister_device(idev);
	input_free_device(idev);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZHANG");


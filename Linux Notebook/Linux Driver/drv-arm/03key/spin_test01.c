/*************************
 * spinlock_t锁的测试例子
 * 内核要求链表等数据结构同一时间只能一个人访问，因此一般会用spinlock_t进行保护；
 * 本例在每次按键按下时记录按键信息，并存入链表中；
 * 用户态打开/proc文件时，遍历链表，显示按下的所有键值；
 * 最好用新的seq_file接口来实现proc文件
 * 尝试将按键的触发模式改为双沿，并用timer_list去抖；
 * Author: zht
 * Date: 2016-08-04
 *************************/
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h> /* spinlock_t */
#include <linux/time.h> //timeval & timespec
#include <linux/gpio.h>
#include <mach/gpio.h>

#define KEY_NUM 4

//按键的描述
struct btn_desc {
	int gpio;
	char value;
	char *name;  //中断处理函数的名字
};

static struct btn_desc buttons[] = {
	{ EXYNOS4_GPX3(2), 'A', "spin0" },
	{ EXYNOS4_GPX3(3), 'B', "spin1" },
	{ EXYNOS4_GPX3(4), 'C', "spin2" },
	{ EXYNOS4_GPX3(5), 'D', "spin3" },
};


/* 按键的私有结构体 */
struct key_priv {
	int key_num;	/* 按键号 */
	int key_irq;	/* 中断号 */
	char key_value; /* 按键对应的ASCII值 */
};


//链表头，只分配一个
struct key_head {
	struct list_head head;
	spinlock_t key_lock;  //谁想访问链表，首先持有锁
	int key_cnt[KEY_NUM]; //记录每个按键按下的次数
};

//链表的数据成员，每次按下按键都要分配一个
struct key_item {
	struct list_head item;
	char value; //按键的值
	struct timeval time; //记录按键按下的时间
};

static struct key_priv *keys[KEY_NUM];
static struct key_head *klist;

/* proc文件的读函数 */
static int 
key_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int i, ret = 0;
	struct key_item *tmp;
	unsigned long flags;

	/* 遍历链表，输出每个按键的值以及发生的时间 */
	spin_lock_irqsave(&klist->key_lock, flags);

	list_for_each_entry(tmp, &klist->head, item) {
		ret += sprintf(page+ret, "%c: %ld--%ldus\n", tmp->value, tmp->time.tv_sec, tmp->time.tv_usec);		
	}
	for (i=0; i<KEY_NUM; i++) {
		ret += sprintf(page+ret, "Captured key %c: %d times\n", keys[i]->key_value, klist->key_cnt[i]);
	}

	spin_unlock_irqrestore(&klist->key_lock, flags);

	return ret;
}


//中断处理函数，4个按键共用
static irqreturn_t 
key_service(int irq, void *dev_id)
{
	struct key_priv *dev = dev_id;
	struct key_item *tmp;
	unsigned long flags;

	//GFP_ATOMIC标志保证不会睡眠
	tmp = kzalloc(sizeof(*tmp), GFP_ATOMIC);
	if (!tmp) {
		printk("key: cannot get memory\n");
		return IRQ_HANDLED;
	}

	//初始化proc_item
	INIT_LIST_HEAD(&tmp->item);
	do_gettimeofday(&tmp->time);
	tmp->value = dev->key_value;

	/* 加锁同时关中断：将成员加入链表 */
	//spin_lock(&klist->key_lock);
	spin_lock_irqsave(&klist->key_lock, flags);
	list_add_tail(&tmp->item, &klist->head); 
	klist->key_cnt[dev->key_num]++;
	spin_unlock_irqrestore(&klist->key_lock, flags);
	//spin_unlock(&klist->key_lock);

	return IRQ_HANDLED;
}


static int __init my_init(void)
{
	int i, ret;
	unsigned long flags;

	//1.分配并初始化key_head
	klist = (struct key_head *)kzalloc(sizeof(*klist), GFP_KERNEL);
	if (!klist)
		return -ENOMEM;
	INIT_LIST_HEAD(&klist->head);
	spin_lock_init(&klist->key_lock);
	for (i=0; i<KEY_NUM; i++)
		klist->key_cnt[i] = 0;

	//2.分配并初始化key_priv(4个)
	for (i=0; i<KEY_NUM; i++) {
		keys[i] = kzalloc(sizeof(*keys[i]), GFP_KERNEL);
		if (!keys[i]) {
			ret = -ENOMEM;
			goto err;
		}
		keys[i]->key_value = buttons[i].value;
		keys[i]->key_irq = gpio_to_irq(buttons[i].gpio);
		keys[i]->key_num = i;

		/* 注册中断处理函数 */
		flags = //IRQF_TRIGGER_LOW;
			IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING;
		ret = request_irq(keys[i]->key_irq,
			key_service, 
			flags, 
			buttons[i].name,
			keys[i]);
		if (ret) {
			kfree(keys[i]);
			printk("Cannot request irq %d\n", keys[i]->key_irq);
			keys[i] = NULL;
			goto err;
		}
	}

	//3.创建/proc文件，在文件中遍历链表
	create_proc_read_entry("proc_spin",
		0, NULL, key_proc_read, NULL);

	return 0;
err:
	for (i=0; i<KEY_NUM; i++) {
		if (keys[i]) {
			free_irq(keys[i]->key_irq, keys[i]);
			kfree(keys[i]);
		}
	}
	kfree(klist);

	return ret;
}

static void __exit my_exit(void)
{
	int i;
	struct key_item *tmp1, *tmp2;

	remove_proc_entry("proc_spin", NULL);
	for (i=0; i<KEY_NUM; i++) {
		if (keys[i]) {
			free_irq(keys[i]->key_irq, keys[i]);
			kfree(keys[i]);
		}
	}

	/* 释放整个链表 */
	list_for_each_entry_safe(tmp1, tmp2, &klist->head, item) {
		list_del(&tmp1->item);
		kfree(tmp1);
	}
	kfree(klist);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZHT");


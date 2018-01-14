/************************
 * 工作队列的测试(work queue)
 * 如果中断中必须调用可能导致睡眠的函数，则可以把下半部推后到进程上下文执行
 * 在给定的内核线程中执行下半部
 * 如果是Redhat，该线程为events/x
 * 如果是android，则该线程为kworker/x:y
 * 在工作队列的下半部执行函数中可以睡眠
 * Author: zht
 * Date: 2015-04-24
 ************************/
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h> //work_struct
#include <linux/smp.h> //smp_processor_id()
#include <linux/gpio.h>
#include <mach/gpio.h>

//按键1的中断号
#define KEY_IRQ 	gpio_to_irq(EXYNOS4_GPX3(2))

//声明工作的封装结构体和其他变量
static struct work_struct mywork;
static int up_cnt, bo_cnt;
static int up_cpu, bo_cpu;

//下半部执行函数
//由于在进程上下文执行，因此可以睡眠(可以调用可能导致睡眠的函数)
static void 
bo_service(struct work_struct *data)
{
	bo_cpu = smp_processor_id();
	printk("Bo %d: cpu(%d); %d--%s\n", \
		++bo_cnt, bo_cpu, \
		current->pid, current->comm);
}


//上半部执行函数
static irqreturn_t 
up_service(int irq, void *dev_id)
{
	up_cpu = smp_processor_id();
	printk("Up %d: cpu(%d);\n", ++up_cnt, up_cpu);

	//默认情况下，上下半部在同一cpu上执行
	//可以唤醒给定cpu上的线程执行下半部
	//1代表cpu1，0代表cpu0
	//schedule_work(&mywork);
	schedule_work_on(1, &mywork);

	return IRQ_HANDLED;
}


static int __init my_init(void)
{
	int ret, flags;

	//初始化工作队列
	INIT_WORK(&mywork, bo_service);

	flags = IRQF_TRIGGER_FALLING;
	//flags = IRQF_TRIGGER_LOW;
	ret = request_irq(KEY_IRQ, up_service, 
		flags, "wq_test1", &mywork);
	if (ret) {
		printk("cannot request irq %d\n", KEY_IRQ);
		return ret;
	}
		
	return 0;
}


static void __exit my_exit(void)
{
	free_irq(KEY_IRQ, &mywork);
	flush_scheduled_work();
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("ZHANG");
MODULE_LICENSE("GPL");


/*****************************
 * 内核时间的测试例子1
 * 输出当前的相对时间和绝对时间
 * Author: zht
 * Date: 2016-10-16
 *****************************/
#include <linux/module.h>
#include <linux/sched.h> //jiffies & jiffies_64
#include <linux/proc_fs.h>
#include <linux/time.h> //timeval & timespec

static int 
time_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int ret = 0;
	u64 j64;
	struct timeval tval;
	struct timespec tspec;

	//获得并打印当前的相对和绝对时间
	j64 = get_jiffies_64();
	ret += sprintf(page+ret, "jiffies    = 0x%lx\n", jiffies);
	ret += sprintf(page+ret, "jiffies_64 = 0x%llx\n", j64);

	do_gettimeofday(&tval);
	getnstimeofday(&tspec);
	ret += sprintf(page+ret, "timeval:  %lds -- %ldus\n", tval.tv_sec, tval.tv_usec);
	ret += sprintf(page+ret, "timespec: %lds -- %ldns\n", tspec.tv_sec, tspec.tv_nsec);

	return ret;
}


static int __init my_init(void)
{
	create_proc_read_entry("proc_time", 0, NULL,
			time_proc_read, NULL);
	return 0;
}

static void __exit my_exit(void)
{
	remove_proc_entry("proc_time", NULL);
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZHANG");


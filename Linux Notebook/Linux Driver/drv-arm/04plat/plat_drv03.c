/***************************
 * 简单的char驱动，功能同plat_drv02.c
 * 实现了/dev/下设备文件的自动创建
 * 需要将驱动选择的设备号，设备文件名等信息广播到用户态，然后由用户态的deamon程序(如udevd)接收内核的广播，然后完成mknod的工作；
 * 驱动需要创建一个class，代表驱动支持设备的类型；
 * 驱动每匹配一个设备，则在class下面增加一个device，该device中封装了设备号即设备文件名；
 * 将device注册到class下面时，设备模型会自动通过netlink协议将该信息广播到用户态；
 * Author: zht
 * Date: 2016-10-10
 ***************************/
#include <linux/module.h>
#include <linux/slab.h> //kmalloc
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h> 
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/platform_device.h>

//定义char驱动需要的主设备号
#define PLAT_MAJOR	85

//定义ioctl命令
struct reg_info {
	int offset; //寄存器的偏移
	int value; //寄存器的值
};

#define PLAT_TYPE	'P'
#define GET_REG		_IOWR(PLAT_TYPE, 1, struct reg_info)
#define SET_REG		_IOW(PLAT_TYPE, 2, struct reg_info)

//由于驱动可以支持任意多个设备，因此应该定义私有结构体，但不需要定义全局的私有结构体指针
struct plat_priv {
	int phy_base; //设备的物理基地址
	int phy_size; //寄存器范围
	void __iomem *vir_base; //虚拟基地址
	dev_t dev_id;
	struct cdev plat_cdev;
	struct device *mydev; //创建一个新的device结构体，封装设备号和设备名等信息
};

//如果某些成员是多个设备共用的，则可以设定为全局变量
//存储设备的次设备号，每匹配一个设备，可以将cur_minor++
static int cur_minor = 10;

//创建一个新的class结构体，用于注册device
//class结构体会出现在/sys/class目录下
struct class *myclass;


//准备file_operations

//open函数中要找到和打开设备对应的私有结构体
static int
plat_open(struct inode *inode, struct file *filp)
{
	struct plat_priv *priv = container_of(inode->i_cdev, struct plat_priv, plat_cdev);
	filp->private_data = priv;
	return 0;
}

static int
plat_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static long
plat_ioctl(struct file *filp, unsigned int req, unsigned long arg)
{
	struct plat_priv *dev = filp->private_data;
	struct reg_info tmp;

	//首先将用户态准备的reg_info结构体拷贝到内核中
	if (copy_from_user(&tmp, (void *)arg, sizeof(tmp)))
		return -EFAULT;
	//检测reg_info参数中寄存器偏移的范围
	if (tmp.offset > (dev->phy_size-4)) {
		printk("Offset should between 0~0x%x\n", (dev->phy_size-4));
		return -1;
	}

	switch (req) {
	case GET_REG: //读寄存器
		//printk("get reg 0x%x\n", tmp.offset);
		tmp.value = readl(dev->vir_base+tmp.offset);
		if (copy_to_user((void *)arg, &tmp, sizeof(tmp)))
			return -EFAULT;
		break;
	case SET_REG: //写寄存器
		//printk("set reg 0x%x to 0x%x", tmp.offset, tmp.value);
		writel(tmp.value, (dev->vir_base+tmp.offset));
		break;
	default:
		printk("Cannot support ioctl 0x%x\n", req);
		return -1;
	}
	return 0;
}

static struct file_operations plat_fops = {
	.owner		= THIS_MODULE,
	.open		= plat_open,
	.release	= plat_release,
	.unlocked_ioctl = plat_ioctl,
};

//准备platform_driver
static int
plat_probe(struct platform_device *pdev)
{
	struct plat_priv *priv;
	struct resource *res;

	printk("%s(%d) : probe()\n", pdev->name, pdev->id);

	//1.首先获取设备的物理地址资源
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		printk("%s(%d): Cannot get enough resource\n", pdev->name, pdev->id);
		return -EIO;
	}

	//2.分配并初始化私有结构体
	priv = (struct plat_priv *)kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv) 
		return -ENOMEM;
	priv->phy_base = res->start;
	priv->phy_size = res->end-res->start+1;

	//3.将物理地址映射为虚拟地址
	priv->vir_base = ioremap(priv->phy_base, priv->phy_size);
	if (!priv->vir_base) {
		kfree(priv);
		return -EIO;
	}

	//4.分配设备号并注册cdev
	priv->dev_id = MKDEV(PLAT_MAJOR, cur_minor++);
	cdev_init(&priv->plat_cdev, &plat_fops);
	cdev_add(&priv->plat_cdev, priv->dev_id, 1);
	printk("%s(%d) : device %d:%d OK\n", pdev->name, pdev->id, MAJOR(priv->dev_id), MINOR(priv->dev_id));

	//5.创建device，封装设备号等信息，然后注册到class中
	priv->mydev = device_create(
		myclass, //class指针
		&pdev->dev, //父device指针
		priv->dev_id, //设备号
		NULL, //drvdata，默认为NULL
		pdev->name); // /dev/目录下的设备文件名

	if (IS_ERR(priv->mydev)) {
		cdev_del(&priv->plat_cdev);
		iounmap(priv->vir_base);
		kfree(priv);
		printk("Cannot create /dev/xxx...\n");
		return PTR_ERR(priv->mydev);
	}

	//6.将分配的私有结构体指针存入pdev中
	platform_set_drvdata(pdev, priv);
	return 0;
}

static int
plat_remove(struct platform_device *pdev)
{
	//从pdev中获得私有结构体的指针
	struct plat_priv *priv = platform_get_drvdata(pdev);
	printk("%s(%d) : remove()\n", pdev->name, pdev->id);
	//注销class下面注册的device
	device_destroy(myclass, priv->dev_id);
	cdev_del(&priv->plat_cdev);
	iounmap(priv->vir_base);
	kfree(priv);
	return 0;
}

//准备支持设备的名字数组
static struct platform_device_id my_ids[] = {
	{ "e4412-wdt", 0 },
	{ "e4412-rtc", 0 },
	{ "e4412-chipID", 0 },
	{},
};

static struct platform_driver my_pdrv = {
	.probe = plat_probe,
	.remove = plat_remove,
	.id_table = my_ids,
	.driver = {
		.name = "my4412",
	},
};

static int __init my_init(void)
{
	int ret;
	//1.创建新的class，第二个参数为class的名字，代表设备的分类，会出现在/sys/class目录下
	myclass = class_create(THIS_MODULE, "directIO");
	if (IS_ERR(myclass)) {
		printk("Cannot register class (my4412)\n");
		return PTR_ERR(myclass);
	}

	//2.注册pdrv
	ret = platform_driver_register(&my_pdrv);
	if (ret) {
		class_destroy(myclass);
		printk("Cannot register platform_driver\n");
		return ret;
	}
	return 0;
}

static void __exit my_exit(void)
{
	//注销pdrv
	platform_driver_unregister(&my_pdrv);
	//销毁class
	class_destroy(myclass);
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


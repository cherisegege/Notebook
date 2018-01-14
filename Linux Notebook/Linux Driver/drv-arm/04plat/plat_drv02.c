/***************************
 * 简单的char驱动
 * 为匹配的设备提供char驱动，利用ioctl命令来访问设备内部的寄存器
 * 用户态可以指定要访问寄存器的偏移，以及要设定的值
 * 驱动可以匹配任意多个设备
 * 如果一个驱动支持一个设备，则用miscdevice注册；
 * 如果一个驱动支持多个设备，则必须用cdev注册；
 * Author: zht
 * Date: 2016-10-10
 ***************************/
#include <linux/module.h>
#include <linux/slab.h> //kmalloc
//char驱动需要的头文件
#include <linux/fs.h>
#include <linux/cdev.h> //miscdevice.h
#include <linux/uaccess.h> 
//寄存器访问需要的头文件(ioremap/readl/writel)
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/platform_device.h>

//定义char驱动需要的主设备号
#define PLAT_MAJOR	80

//定义ioctl命令
struct reg_info {
	int offset; //寄存器的偏移
	int value;	//寄存器的值
};

#define PLAT_TYPE	'P'
#define GET_REG		_IOWR(PLAT_TYPE, 1, struct reg_info)
#define SET_REG		_IOW(PLAT_TYPE, 2, struct reg_info)

//由于驱动可以支持任意多个设备，因此应该定义私有结构体，但不需要定义全局的私有结构体指针
//私有结构体应该在probe中分配，在remove中释放
struct plat_priv {
	int phy_base; //设备的物理基地址
	int phy_size; //寄存器范围
	void __iomem *vir_base; //虚拟基地址
	dev_t dev_id;
	struct cdev plat_cdev;
};

//如果某些成员是多个设备共用的，则可以设定为全局变量
//存储设备的次设备号，每匹配一个设备，可以将cur_minor++
static int cur_minor = 10;


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
	printk("%s(%d) : vir_base--> %p\n", pdev->name, pdev->id, priv->vir_base);

	//4.分配设备号并注册cdev
	priv->dev_id = MKDEV(PLAT_MAJOR, cur_minor++);
	cdev_init(&priv->plat_cdev, &plat_fops);
	cdev_add(&priv->plat_cdev, priv->dev_id, 1);
	printk("%s(%d): device %d:%d OK\n", pdev->name, pdev->id, MAJOR(priv->dev_id), MINOR(priv->dev_id));

	//5.将分配的私有结构体指针存入pdev中
	platform_set_drvdata(pdev, priv);
	return 0;
}

static int
plat_remove(struct platform_device *pdev)
{
	//从pdev中获得私有结构体的指针
	struct plat_priv *priv = platform_get_drvdata(pdev);
	printk("%s(%d) : remove()\n", pdev->name, pdev->id);
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
	.probe		= plat_probe,
	.remove		= plat_remove,
	.id_table	= my_ids,
	.driver		= {
		.name	= "my4412",
	},
};

static int __init my_init(void)
{
	//注册pdrv
	return platform_driver_register(&my_pdrv);
}

static void __exit my_exit(void)
{
	//注销pdrv
	platform_driver_unregister(&my_pdrv);
}
module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("ZHT");
MODULE_LICENSE("GPL");


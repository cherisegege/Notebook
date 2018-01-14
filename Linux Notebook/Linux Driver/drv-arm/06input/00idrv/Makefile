# 用于外部模块的Makefile
# (必须连接到一个编译好的内核)
# Author: zht
# Date: 2014-10-29
obj-m := mma7660_test01.o
KERNEL := /home/class-full/02kernel/kernel/linux-3.0.86/

all:
	make -C $(KERNEL) M=`pwd` modules
install:
	adb remount
	adb push mma7660_test01.ko /system/lib/modules/
clean:
	make -C $(KERNEL) M=`pwd` clean


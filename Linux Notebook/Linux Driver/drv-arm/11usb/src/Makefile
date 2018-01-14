# 用于外部模块的Makefile
# (必须连接到一个编译好的内核)
# Author: zht
# Date: 2014-10-29
obj-m := usb_test01.o
KERNEL := /var/weekend/02kernel/kernel/linux-3.0.86/

all:
	make -C $(KERNEL) M=`pwd` modules
	arm-linux-gcc -static -o usb01 usb01.c
install:
	adb remount
	adb push usb_test01.ko /system/lib/modules/
	adb push usb01 /system/bin
clean:
	make -C $(KERNEL) M=`pwd` clean


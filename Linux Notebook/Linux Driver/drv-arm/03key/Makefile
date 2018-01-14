# Makefile for external module 
# Author: zht
# Date: 2016-09-10
obj-m := spin_test01.o
KERNEL := /var/weekend/02kernel/kernel/linux-3.0.86/

all:
	make -C $(KERNEL) M=`pwd` modules
install:
	adb remount
	adb push spin_test01.ko /system/lib/modules/
clean:
	make -C $(KERNEL) M=`pwd` clean


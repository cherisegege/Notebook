# Makefile for external module 
# Author: zht
# Date: 2016-11-13
obj-m := wdt_dev.o wdt_drv.o
KERNEL := /var/weekend/02kernel/kernel/linux-3.0.86/

all:
	make -C $(KERNEL) M=`pwd` modules
#arm-linux-gcc -static -o wdt01 wdt01.c
install:
	adb remount
	adb push wdt_dev.ko /system/lib/modules/
	adb push wdt_drv.ko /system/lib/modules/
#	adb push wdt01 /system/bin/
clean:
	make -C $(KERNEL) M=`pwd` clean


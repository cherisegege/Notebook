# Makefile for external module 
# Author: zht
# Date: 2016-11-12
obj-m := plat_wdt.o plat_rtc.o plat_drv03.o
KERNEL := /var/weekend/02kernel/kernel/linux-3.0.86/

all:
	make -C $(KERNEL) M=`pwd` modules
	arm-linux-gcc -static -o plat01 plat01.c
install:
	adb remount
	adb push plat_wdt.ko /system/lib/modules/
	adb push plat_rtc.ko /system/lib/modules/
	adb push plat_drv03.ko /system/lib/modules/
	adb push plat01 /system/bin/
clean:
	make -C $(KERNEL) M=`pwd` clean


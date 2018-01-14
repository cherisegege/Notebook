# Makefile for external module 
# Author: zht
# Date: 2016-09-10
obj-m := led_test03.o
KERNEL := /var/weekend/02kernel/kernel/linux-3.0.86/

all:
	make -C $(KERNEL) M=`pwd` modules
	arm-linux-gcc -static -o led02 led02.c
install:
	adb remount
	adb push led_test03.ko /system/lib/modules/
	adb push led02 /system/bin/
clean:
	make -C $(KERNEL) M=`pwd` clean
	rm -f led02



# Makefile for external module 
# Author: zht
# Date: 2016-09-10
obj-m := pwm_test02.o
KERNEL := /var/weekend/02kernel/kernel/linux-3.0.86/

all:
	make -C $(KERNEL) M=`pwd` modules
	arm-linux-gcc -static -o buzz01 buzz01.c
install:
	adb remount
	adb push pwm_test02.ko /system/lib/modules/
	adb push buzz01 /system/bin/
clean:
	make -C $(KERNEL) M=`pwd` clean
	rm -f buzz01


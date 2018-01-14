# Makefile for external module 
# Author: zht
# Date: 2016-09-10
obj-m := char_test01.o
KERNEL := /usr/src/kernels/`uname -r`/

all:
	make -C $(KERNEL) M=`pwd` modules
install:
	make -C $(KERNEL) M=`pwd` modules_install
	depmod -A
clean:
	make -C $(KERNEL) M=`pwd` clean


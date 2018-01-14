# Makefile for external module
obj-m := mutex_test02.o
KERNEL := /usr/src/kernels/`uname -r`/

all:
	make -C $(KERNEL) M=`pwd` modules
clean:
	make -C $(KERNEL) M=`pwd` clean


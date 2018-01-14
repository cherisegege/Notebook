# Makefile for external module
obj-m := timer_test.o
KERNEL := /usr/src/kernels/`uname -r`/

all:
	make -C $(KERNEL) M=`pwd` modules
clean:
	make -C $(KERNEL) M=`pwd` clean
	rm -f user01 user02


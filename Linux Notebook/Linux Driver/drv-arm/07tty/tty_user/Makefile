ARM_CFLAGS := -static 

all:
	arm-linux-gcc -o tty_write tty_write.c $(ARM_CFLAGS)
install:
	adb remount
	adb push tty_write /system/bin/
clean:
	rm -f *~ *.o tty_read tty_write


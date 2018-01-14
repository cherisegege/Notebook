src := $(shell ls *.c)
elf := $(patsubst %.c, %, $(src))

LDFLAGS := -L ./jpeg-so/lib/ -I ./jpeg-so/include/ -ljpeg

%: %.c
	arm-linux-gcc -o $@ $< $(LDFLAGS)

install:
	adb remount
	adb push catch02 /system/bin/
clean:
	rm -f $(elf)


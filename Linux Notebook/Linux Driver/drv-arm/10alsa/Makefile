src := $(shell ls *.c)
elf := $(patsubst %.c, %, $(src))

LDFLAGS = -lasound

clean:
	rm -f $(elf)


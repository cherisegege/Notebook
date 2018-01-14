TARGET	:= itest01
SRC		:= itest01.c

all: $(SRC)
	arm-linux-gcc -static -o $(TARGET) $<
install:
	adb remount
	adb push $(TARGET) /system/bin
clean:
	rm -f $(TARGET)
cleanall:
	rm -f itest01 itest02 itest03


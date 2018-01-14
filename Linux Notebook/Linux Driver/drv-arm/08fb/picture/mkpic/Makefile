
CC	= @gcc 
CFLAGS  = -O2 -Wall -g

SOURCES	= main.c jpeg.c bmp.c mkpic.c

OBJECTS	= ${SOURCES:.c=.o}

OUT	= mkpic

LIBS = -ljpeg

all: $(OUT)
	@echo Build Done.
	@rm -f $(OBJECTS)

.PHONY: $(OUT)
$(OUT): $(OBJECTS)
	@$(CC) $(LDFLAGS) -o $(OUT) $(OBJECTS) $(LIBS)


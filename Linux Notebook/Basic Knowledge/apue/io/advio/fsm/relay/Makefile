
LDFLAGS+=-pthread


relayer:main.o relay.o
	gcc -o $@ $^ $(LDFLAGS)


clean:
	rm *.o relayer -f


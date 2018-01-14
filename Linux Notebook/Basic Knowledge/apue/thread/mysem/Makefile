
LDFLAGS+=-pthread

sem:main.o sem.o
	gcc -o $@ $^ $(LDFLAGS)

clean:
	rm *.o sem


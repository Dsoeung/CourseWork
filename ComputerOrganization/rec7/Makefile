CC=gcc
CFLAGS= -m32 -g
DEPS= rec7.h
OBJ= rec7.o rec7Arch.o

all: runfile

clean:
	rm runfile rec7.o

rec7.o: rec7.s $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

runfile: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)






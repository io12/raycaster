CC=gcc
CFLAGS="-g -Wall -std=gnu99"
LDLIBS="-lm -lncurses"

main.c: exe.out

all: exe.out

clean:
	rm -f exe.out

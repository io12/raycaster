CC = gcc
CFLAGS = -g -Wall -std=gnu99
LDLIBS = -lm -lncurses

.PHONY: all clean

ncaster: ncaster.c input.c | ncaster.h

all: main

clean:
	rm -f main

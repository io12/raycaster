CC = gcc
CFLAGS = -g -Wall -std=gnu99
LDLIBS = -lm -lncurses

.PHONY: all clean

all: main

clean:
	rm -f main

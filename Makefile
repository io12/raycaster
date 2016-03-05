CC = gcc
CFLAGS = -g -Wall -std=gnu99
LDLIBS = -lm -lncurses

.PHONY: all clean

ncaster.out: $(wildcard src/*.c) | $(wildcard src/*.h)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

all: ncaster.out

clean:
	rm -f $(wildcard *.out)

CC = gcc
CFLAGS = -g -Wall -std=gnu99
LDLIBS = -lm -lncurses
C_FILES = $(wildcard src/*.c)
H_FILES = $(wildcard src/*.h)

.PHONY: all clean

ncaster.out: $(C_FILES) $(H_FILES)
	$(CC) $(CFLAGS) $(C_FILES) -o $@ $(LDLIBS)

all: ncaster.out

clean:
	rm -f $(wildcard *.out)

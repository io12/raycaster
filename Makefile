CC = gcc
CFLAGS = -g -Wall -std=c99 -Og
LDLIBS = -lm -lncurses
C_FILES = $(wildcard src/*.c)
H_FILES = $(wildcard src/*.h)
BUILD = ncaster.out

.PHONY: all clean

$(BUILD): $(C_FILES) $(H_FILES)
	$(CC) $(CFLAGS) $(C_FILES) -o $@ $(LDLIBS)

all: ncaster.out

clean:
	rm -f $(BUILD)

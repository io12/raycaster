all:
	gcc -g -Wall -std=gnu99 main.c -o exe.out -lncurses -lm
clean:
	rm -f main

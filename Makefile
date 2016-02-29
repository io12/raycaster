all:
	gcc -g -Wall main.c -o exe.out -lncurses -lm
clean:
	rm -f main

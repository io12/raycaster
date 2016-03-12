#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "ncaster.h"
#include "input.h"
#include "parse.h"
#include "render.h"
#include "gen_maze.h"
#include "write_file.h"

int main(int argc, char* argv[]) {
	// parse arguments
	if (argc == 1)
		quit(1, "Need to specify a file or argument");
	struct player p;
	struct flags f;
	f.r = 0;
	f.c = 0;
	f.o = 0;
	for (GETOPT; f.opt != EOF && f.opt != 255; GETOPT) {
		switch (f.opt) {
			case 'r':
				f.r = 1;
				break;
			case 'c':
				f.c = 1;
				break;
			case 'o':
				f.o = 1;
				filename = strdup(optarg);
				break;
			default:
				quit(1, "");
		}
	}

	// load a map based on the arguments
	if (f.r) {
		p = gen_maze(21, 21, f.c);
		if (f.o) {
			// output to file
			write_map(p, 21, 21);
			quit(0, "");
		}
	}
	else if (f.o)
		quit(1, "Nothing to write to \"%s\"", filename);
	else
		p = parse_map(argv[1]);

	init_raycaster();

	// gameloop
	while(1) {
		gen_frame(p);
		// print the frame
		refresh();
		p = get_input(p);
	}
}

void init_raycaster() {
	// ncurses
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, 1);
	curs_set(0);
	start_color();
	init_color(COLOR_BLACK, 0, 0, 0);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_BLUE, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_CYAN, COLOR_BLACK);
}

// quit(1, "message", ...) for errors
// quit(0, "") for normal exit
void quit(int status, char* message, ...) {
	endwin();

	if (status == 0)
		exit(0);
	else {
		if (strcmp(message, "")) {
			va_list ap;
			va_start(ap, message);
			vfprintf(stderr, message, ap);
			fprintf(stderr, "\n");
			va_end(ap);
		}
		exit(1);
	}
}

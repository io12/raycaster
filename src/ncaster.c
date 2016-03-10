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

int main(int argc, char* argv[]) {
	// parse arguments
	if (argc == 1)
		quit(1, "Need to specify a file or argument");
	struct flags f;
	f.random = 0;
	f.color = 0;
	f.opt = getopt(argc, argv, "rc");;
	while (f.opt != EOF || f.opt != -1) {
		switch (f.opt) {
			case 'r':
				f.random = 1;
				break;
			case 'c':
				f.color = 1;
				break;
			default:
				quit(1, "");
		}
		f.opt = getopt(argc, argv, "rc");;
	}

	// load a map based on the arguments
	struct player p;
	if (f.random)
		p = gen_maze(21, 21, f.color);
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

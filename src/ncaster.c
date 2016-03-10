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
	while ((f.opt = getopt(argc, argv, "rc")) != -1) {
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

// quit(1, "") when errno is set
// quit(1, "message") for program defined errors
// quit(0, "") for normal exit
void quit(int status, char* message, ...) {
	// TODO: add formatting
	endwin();
	va_list ap;
	va_start(ap, message);
	if (status == EXIT_SUCCESS) {
		va_end(ap);
		exit(EXIT_SUCCESS);
	}
	else {
		if (strcmp(message, "")) {
			fprintf(stderr, "%s", message);
			fprintf(stderr, "\n");
		}
		if (strcmp(strerror(errno), "Success") != 0)
			perror("");
		va_end(ap);
		exit(EXIT_FAILURE);
	}
}

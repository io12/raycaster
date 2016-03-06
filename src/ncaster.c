#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

#include "ncaster.h"
#include "input.h"
#include "parse.h"
#include "render.h"
#include "gen_maze.h"

int main(int argc, char* argv[]) {
	init_raycaster();
	if (argc != 2)
		quit("Incorrect amount of arguments. Need to specify a map.");
	struct player p;
	if (strcmp(argv[1], "-r") == 0) {
		p = gen_maze(21, 21);
	}
	else {
		p = parse_map(argv[1]);
	}
	p.quit = 0;
	// gameloop starts here
	while(1) {
		gen_frame(p);
		// print the frame
		refresh();
		p = get_input(p);
		if (p.quit)
			break;
	}
	quit("EXITED");;
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

void quit(char* message) {
	endwin();
	printf(message);
	printf("\n");
	exit(EXIT_SUCCESS);
}

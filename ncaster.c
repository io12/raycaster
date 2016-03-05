#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <curses.h>
#include <error.h>

#include "ncaster.h"

int main(int argc, char* argv[]) {
	init_raycaster();
	if (argc != 2)
		quit("incorrect amount of arguments");
	struct player p = read_map(argv[1]);
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
	free(p.map);
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

void gen_frame(struct player p) {
	struct ray r;
	// loop over columns
	for (int ic = 0; ic < COLS; ic++) {
		// fire ray
		r.x = p.x;
		r.y = p.y;
		r.distance = 0;
		while(1) {
			r.angle = ((p.fov / 2) + p.angle) + (ic * (p.fov / COLS));
			r.x += cos(r.angle) * R_JMP_FACTOR;
			r.y += sin(r.angle) * R_JMP_FACTOR;
			r.distance += R_JMP_FACTOR;
			// check if ray hit a wall
			r.status = p.map[(int) r.y][(int) r.x];
			if (r.status)
				break;
		}
		// create a column
		// height that the wall appears to the player
		int h = (int) ( ( 1 / pow(r.distance, 2) ) * LINES);
		// get the height that the floor appears
		// this may be problematic if (LINES - h) is not even
		int f = ( LINES - h ) / 2;
		for (int ir = 0; ir < LINES; ir++) {
			// draw the walls
			switch (r.status) {
				case 2:
					attron(COLOR_PAIR(2));
					break;
				case 3:
					attron(COLOR_PAIR(3));
					break;
				case 4:
					attron(COLOR_PAIR(4));
					break;
				case 5:
					attron(COLOR_PAIR(5));
					break;
				case 6:
					attron(COLOR_PAIR(6));
					break;
				case 7:
					attron(COLOR_PAIR(7));
					break;
				default:
					break;
			}
			if ((ir < f) || (ir > (f + h))) {
				mvaddch(ir, ic, ' ');
			}
			else if (r.distance < 2) {
				attron(A_STANDOUT);
				mvaddch(ir, ic, ' ');
				attroff(A_STANDOUT);
			}
			else if (r.distance < 4)
				mvaddch(ir, ic, W_DARK);
			else {
				attron(COLOR_PAIR(1));
				mvaddch(ir, ic, W_DARKER);
			}
			attron(COLOR_PAIR(1));
		}
	}
	// update hud
	if (p.hud) {
		mvprintw(0, 0, "coords: %f, %f", p.x, p.y);
		mvprintw(1, 0, "angle: %f rad", p.angle);
		mvprintw(2, 0, "fov: %f rad", p.fov);
	}
	if (p.crosshairs) {
		mvaddch(LINES - LINES / 2 - 1, COLS - COLS / 2, ACS_VLINE);
		mvaddch(LINES - LINES / 2, COLS - COLS / 2 - 2, ACS_HLINE);
		mvaddch(LINES - LINES / 2, COLS - COLS / 2 - 1, ACS_HLINE);
		mvaddch(LINES - LINES / 2, COLS - COLS / 2, ACS_PLUS);
		mvaddch(LINES - LINES / 2, COLS - COLS / 2 + 1, ACS_HLINE);
		mvaddch(LINES - LINES / 2, COLS - COLS / 2 + 2, ACS_HLINE);
		mvaddch(LINES - LINES / 2 + 1, COLS - COLS / 2, ACS_VLINE);
	}
}

struct player read_map(char* filename) {
	// initialize some values
	struct player p;
	p.fov = 2;
	p.lantern = 1;
	p.hud = 1;
	p. crosshairs = 1;

	// open file
	FILE* fp = fopen(filename, "r");
	if (!fp)
		quit("file does not exist");

	// get size of file
	fseek(fp, 0, SEEK_END);
	const long fsize = ftell(fp) - 4;
	if (fsize < 11)
		quit("map file too small");
	rewind(fp);

	char ch;
	char prev_ch;

	// get coordinates
	ch = fgetc(fp);
	if (!isdigit(ch))
		quit("coordinates have to be digits");
	p.x = (double) ch - 48;
	fseek(fp, 2, SEEK_SET);
	ch = fgetc(fp);
	if (!isdigit(ch))
		quit("coordinates have to be digits");
	p.y = (double) ch - 48;
	fseek(fp, 4, SEEK_SET);

	int lines = 0;
	int cols = 0;
	bool inc_cols = 1;
	for (int i = 0; i < fsize; i++) {
		ch = fgetc(fp);
		if (ch == '\n'){
			lines++;
			inc_cols = 0;
		}
		else if (!isdigit(ch))
			quit("map file should only contain digits");
		if (inc_cols)
			cols++;
	}
	if (p.x < 1 || p.x > cols || p.y < 1 || p.y > lines)
		quit("the player can't start outside the map");
	fseek(fp, 4, SEEK_SET);
	p.map = (int**) malloc(lines * sizeof(int*));
	for (int i = 0, j = 0; j < lines;) {
		ch = fgetc(fp);
		if ((j == 0 || j == lines - 1 || i == 0) && ch == '0')
			quit("border of map should be solid");
		if (ch == '\n') {
			if (prev_ch == '\n')
				quit("map files can't have empty lines");
			if (p.map[j][i-1] == 0)
				quit("border of map should be solid");
			if (i != cols)
				quit("map must be a rectangle");
			j++;
			i = 0;
		}
		else {
			p.map[j] = (int*) realloc(p.map[j], sizeof(int) * (i + 1));
			p.map[j][i] = (int) ch - 48;
			i++;
		}
		prev_ch = ch;
	}
	if (p.map[(int) p.y - 1][(int) p.x - 1])
		quit("the player can't start inside a wall");
	fclose(fp);
	return p;
}

void quit(char* message) {
	endwin();
	printf(message);
	printf("\n");
	exit(EXIT_SUCCESS);
}

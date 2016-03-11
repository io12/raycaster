#include <curses.h>
#include <math.h>

#include "ncaster.h"
#include "render.h"

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
			r.x += cos(r.angle) * R_JMP;
			r.y += sin(r.angle) * R_JMP;
			r.distance += R_JMP;

			// vision distance
			// shorter if lantern is off
			if ((r.distance > 10) || (!p.lantern && r.distance > 1.5)) {
				r.status = -1;
				break;
			}

			// check if ray hit a wall
			r.status = p.map[(int) r.y][(int) r.x];
			if (r.status != 0)
				break;
		}
		// create a column
		gen_col(p, r, ic);
	}
	// update hud
	if (p.hud) {
		gen_hud(p);
	}
	// update crosshairs
	if (p.crosshairs) {
		gen_crosshairs();
	}
}

void gen_col(struct player p, struct ray r, int ic) {
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
		if ((ir < f) || (ir > (f + h)) || r.status == -1) {
			mvaddch(ir, ic, ' ');
		}
		else if (r.distance < 2 && p.lantern) {
			attron(A_STANDOUT);
			mvaddch(ir, ic, ' ');
			attroff(A_STANDOUT);
		}
		else if (r.distance < 4 && p.lantern)
			mvaddch(ir, ic, W_DARK);
		else {
			attron(COLOR_PAIR(1));
			mvaddch(ir, ic, W_DARKER);
		}
		attron(COLOR_PAIR(1));
	}

}

void gen_hud(struct player p) {
	mvprintw(0, 0, "coords: %f, %f", p.x, p.y);
	mvprintw(1, 0, "angle: %f rad", p.angle);
	mvprintw(2, 0, "fov: %f rad", p.fov);
	mvprintw(3, 0, "lantern: %s", p.lantern ? "on" : "off");
}

void gen_crosshairs() {
	mvaddch(LINES - LINES / 2 - 1, COLS - COLS / 2, ACS_VLINE);
	mvaddch(LINES - LINES / 2, COLS - COLS / 2 - 2, ACS_HLINE);
	mvaddch(LINES - LINES / 2, COLS - COLS / 2 - 1, ACS_HLINE);
	mvaddch(LINES - LINES / 2, COLS - COLS / 2, ACS_PLUS);
	mvaddch(LINES - LINES / 2, COLS - COLS / 2 + 1, ACS_HLINE);
	mvaddch(LINES - LINES / 2, COLS - COLS / 2 + 2, ACS_HLINE);
	mvaddch(LINES - LINES / 2 + 1, COLS - COLS / 2, ACS_VLINE);
}

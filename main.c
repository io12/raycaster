#include <math.h>
#include <curses.h>

#include "main.h"

double fov = 2;
double pX = 2, pY = 2;
double pAngle = 0;

int main() {
	init_raycaster();
	// gameloop starts here
	while(1) {
		gen_frame(pY, pX, pAngle, MAPLENGTH, map);
		// print the frame
		refresh();
		// get input
		switch (getch()) {
			case KEY_UP:
				// collision checking
				if (!map[(int) (pY + sin(pAngle+0.5*M_PI) * P_MV_FACTOR)][(int) (pX + cos(pAngle+0.5*M_PI) * P_MV_FACTOR)]) {
					pX += cos(pAngle+fov) * P_MV_FACTOR;
					pY += sin(pAngle+fov) * P_MV_FACTOR;
				}
				break;
			case KEY_DOWN:
				if (!map[(int) (pY - sin(pAngle+0.5*M_PI) * P_MV_FACTOR)][(int) (pX - cos(pAngle+0.5*M_PI) * P_MV_FACTOR)]) {
					pX -= cos(pAngle+fov) * P_MV_FACTOR;
					pY -= sin(pAngle+fov) * P_MV_FACTOR;
				}
				break;
			case KEY_RIGHT:
				pAngle += M_PI / 36;
				break;
			case KEY_LEFT:
				pAngle -= M_PI / 36;
				break;
			case '1':
				fov += M_PI / 90;
				break;
			case '2':
				fov -= M_PI / 90;
				break;
			default:
				break;
		}
	}
	endwin();
	return 0;
}

void init_raycaster() {
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

void gen_frame() {
	// used for wall color and presence
	int wStatus;
	// loop over columns
	for (int ic = 0; ic < COLS; ic++) {
		// fire ray
		double rX = pX;
		double rY = pY;
		double rDist = 0;
		while(1) {
			double rAngle = ((fov / 2) + pAngle) + (ic * (fov / COLS));
			rX += cos(rAngle) * R_JMP_FACTOR;
			rY += sin(rAngle) * R_JMP_FACTOR;
			rDist += R_JMP_FACTOR;
			// check if ray hit a wall
			wStatus = map[(int) rY][(int) rX];
			if (wStatus)
				break;
		}
		// create a column
		// height that the wall appears to the player
		int h = (int) ( ( 1 / pow(rDist,2) ) * LINES);
		// get the height that the floor appears
		// this may be problematic if (LINES - h) is not even
		int f = ( LINES - h ) / 2;
		for (int ir = 0; ir < LINES; ir++) {
			// draw the walls
			switch (wStatus) {
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
			else if (rDist < 2) {
				attron(A_STANDOUT);
				mvaddch(ir, ic, ' ');
				attroff(A_STANDOUT);
			}
			else if (rDist < 4)
				mvaddch(ir, ic, W_DARK);
			else {
				attron(COLOR_PAIR(1));
				mvaddch(ir, ic, W_DARKER);
			}
			attron(COLOR_PAIR(1));
		}
	}
	// update coords
	mvprintw(0, 0, "%d.%d, %d.%d", (int) pX, (int) (pX * 10) % 10, (int) pY, (int) (pY * 10) % 10);
}

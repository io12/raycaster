#include <math.h>
#include <curses.h>

#define PI 3.14159265
#define RJMP 0.01
#define WALLMG '#'
#define WALLBG ':'

int main() {
	double pAngle = 0, rAngle;
	double pX = 2, pY=2;
	double rX, rY;
	double fov = 2;
	double dist;
	double mvfactor = 0.1;
	int rows, cols;
	// used for wall color and presence
	int wStatus;
	int map[10][10] = {
		{1,1,2,1,3,1,4,1,5,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,6},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,7},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1}
	};
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, 1);
	curs_set(0);
	start_color();
	init_color(COLOR_BLACK, 0, 0, 0);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	// gameloop starts here
	while(1) {
		getmaxyx(stdscr, rows, cols);
		// loop over columns
		for (int ic = 0; ic < cols; ic++) {
			// fire ray
			rX = pX;
			rY = pY;
			dist = 0;
			while(1) {
				rAngle = ((fov / 2) + pAngle) + (ic * (fov / cols));
				rX += cos(rAngle) * RJMP;
				rY += sin(rAngle) * RJMP;
				dist += RJMP;
				// check if ray hit a wall
				wStatus = map[(int) rY][(int) rX];
				if (wStatus)
					break;
			}
			// create a column
			// height that the wall appears to the player
			int h = (int) ( ( 1 / pow(dist,2) ) * rows );
			// get the height that the floor appears
			// this may be problematic if (rows - h) is not even
			int f = ( rows - h ) / 2;
			for (int ir = 0; ir < rows; ir++) {
				// draw the walls
				switch (wStatus) {
					case 2:
						init_pair(1, COLOR_RED, COLOR_BLACK);
						attron(COLOR_PAIR(1));
						break;
					case 3:
						init_pair(1, COLOR_GREEN, COLOR_BLACK);
						attron(COLOR_PAIR(1));
						break;
					case 4:
						init_pair(1, COLOR_YELLOW, COLOR_BLACK);
						attron(COLOR_PAIR(1));
						break;
					case 5:
						init_pair(1, COLOR_BLUE, COLOR_BLACK);
						attron(COLOR_PAIR(1));
						break;
					case 6:
						init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
						attron(COLOR_PAIR(1));
						break;
					case 7:
						init_pair(1, COLOR_CYAN, COLOR_BLACK);
						attron(COLOR_PAIR(1));
						break;
					default:
						break;
				}
				if ((ir < f) || (ir > (f + h))) {
					mvaddch(ir, ic, ' ');
				}
				else if (dist < 2) {
					attron(A_STANDOUT);
					mvaddch(ir, ic, ' ');
					attroff(A_STANDOUT);
				}
				else if (dist < 4)
					mvaddch(ir, ic, WALLMG);
				else
					mvaddch(ir, ic, WALLBG);
				attroff(COLOR_PAIR(1));
			}
		}
		// print the frame
		refresh();
		// get input
		switch (getch()) {
			case KEY_UP:
				// collision checking
				if (!map[(int) (pY + sin(pAngle+0.5*PI) * mvfactor)][(int) (pX + cos(pAngle+0.5*PI) * mvfactor)]) {
					pX += cos(pAngle+0.5*PI) * mvfactor;
					pY += sin(pAngle+0.5*PI) * mvfactor;
				}
				break;
			case KEY_DOWN:
				if (!map[(int) (pY - sin(pAngle+0.5*PI) * mvfactor)][(int) (pX - cos(pAngle+0.5*PI) * mvfactor)]) {
					pX -= cos(pAngle+0.5*PI) * mvfactor;
					pY -= sin(pAngle+0.5*PI) * mvfactor;
				}
				break;
			case KEY_RIGHT:
				pAngle += PI / 36;
				break;
			case KEY_LEFT:
				pAngle -= PI / 36;
				break;
			default:
				break;
		}
	}
	endwin();
	return 0;
}

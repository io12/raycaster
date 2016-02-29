#include <math.h>
#include <curses.h>

#define PI 3.14159265
#define RJMP 0.01
#define WALLFG (char) 219
#define WALLMG '#'
#define WALLBG ':'

int main() {
	double pAngle = 0;
	double pX = 2;
	double pY = 2;
	double rAngle;
	double rX, rY;
	double fov = 2;
	double dist;
	double mvfactor = 0.1;
	int rows, cols;
	int map[10][10] = {
		{1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,1},
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
				if (map[(int) rY][(int) rX])
					break;
			}
			// create a column
			// height that the wall appears to the player
			int h = (int) ( ( 1 / pow(dist,2) ) * rows );
			// get the height that the floor appears
			// this may be problematic if (rows - h) is not even
			int f = ( rows - h ) / 2;
			for (int ir = 0; ir < rows; ir++) {
				// I have no idea if I even did this part right
				(ir < f) || (ir > (f + h)) ? mvaddch(ir, ic, ' ') :
				dist < 2 ? mvaddch(ir, ic, WALLFG) :
				dist < 4 ? mvaddch(ir, ic, WALLMG) :
				mvaddch(ir, ic, WALLBG);
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

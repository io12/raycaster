#include <math.h>
#include <curses.h>

#define RAY_JUMP_FACTOR 0.1
#define WALL_DARK '#'
#define WALL_DARKER ':'
#define PLAYER_MOVE_FACTOR 0.1
#define FOV 2.0

int main() {
	double pAngle = 0, rAngle;
	double pX = 2, pY = 2, rX, rY;
	double rDist;
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
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_BLUE, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_CYAN, COLOR_BLACK);
	// gameloop starts here
	while(1) {
		// loop over columns
		for (int ic = 0; ic < COLS; ic++) {
			// fire ray
			rX = pX;
			rY = pY;
			rDist = 0;
			while(1) {
				rAngle = ((FOV / 2) + pAngle) + (ic * (FOV / COLS));
				rX += cos(rAngle) * RAY_JUMP_FACTOR;
				rY += sin(rAngle) * RAY_JUMP_FACTOR;
				rDist += RAY_JUMP_FACTOR;
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
					mvaddch(ir, ic, WALL_DARK);
				else {
					attron(COLOR_PAIR(1));
					mvaddch(ir, ic, WALL_DARKER);
				}
				attron(COLOR_PAIR(1));
			}
		}
		// update coords
		mvprintw(0, 0, "%d.%d, %d.%d", (int) pX, (int) (pX * 10) % 10, (int) pY, (int) (pY * 10) % 10);
		// print the frame
		refresh();
		// get input
		switch (getch()) {
			case KEY_UP:
				// collision checking
				if (!map[(int) (pY + sin(pAngle+0.5*M_PI) * PLAYER_MOVE_FACTOR)][(int) (pX + cos(pAngle+0.5*M_PI) * PLAYER_MOVE_FACTOR)]) {
					pX += cos(pAngle+0.5*M_PI) * PLAYER_MOVE_FACTOR;
					pY += sin(pAngle+0.5*M_PI) * PLAYER_MOVE_FACTOR;
				}
				break;
			case KEY_DOWN:
				if (!map[(int) (pY - sin(pAngle+0.5*M_PI) * PLAYER_MOVE_FACTOR)][(int) (pX - cos(pAngle+0.5*M_PI) * PLAYER_MOVE_FACTOR)]) {
					pX -= cos(pAngle+0.5*M_PI) * PLAYER_MOVE_FACTOR;
					pY -= sin(pAngle+0.5*M_PI) * PLAYER_MOVE_FACTOR;
				}
				break;
			case KEY_RIGHT:
				pAngle += M_PI / 36;
				break;
			case KEY_LEFT:
				pAngle -= M_PI / 36;
				break;
			default:
				break;
		}
	}
	endwin();
	return 0;
}

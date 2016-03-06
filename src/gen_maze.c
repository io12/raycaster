#include <stdlib.h>
#include <time.h>
#include <curses.h>

#include "ncaster.h"
#include "stack.h"

struct player gen_maze(const int x, const int y) {
	s.top = -1;
	srand(time(NULL));
	struct player p;
	p.fov = FOV_DEFAULT;
	p.lantern = 1;
	p.hud = 1;
	p.crosshairs = 1;

	// can increase fault tolerance
	// create mapspace
	p.map = malloc(y * sizeof(int*));
	for (int i = 0; i < y; i++)
		p.map[i] = malloc(x * sizeof(int));

	// this can be combined with above
	// set every value of map to 1
	for (int i = 0; i < y; i++)
		for (int j = 0; j < x; j++)
			p.map[i][j] = 1;

	// pick a starting cell
	int cx = ((rand() % (x - 1) / 2) + 1) * 2 - 1;
	int cy = ((rand() % (y - 1) / 2) + 1) * 2 - 1;
	int visited = 1, nb, dirs[4];
	p.map[cy][cx] = 0;
	p.x = cx;
	p.y = cy;

	// while there are unvisited cells
	while (visited < (((y - 1) / 2) * ((x - 1) / 2))) {
		// get the amount of unvisited neighbours
		nb = 0;
		if (cy != 1 && p.map[cy - 2][cx] == 1) {
			dirs[nb] = 1;
			nb++;
		}
		if (cy != (y - 2) && p.map[cy + 2][cx] == 1) {
			dirs[nb] = 2;
			nb++;
		}
		if (cx != 1 && p.map[cy][cx - 2] == 1) {
			dirs[nb] = 3;
			nb++;
		}
		if (cx != (x - 2) && p.map[cy][cx + 2] == 1) {
			dirs[nb] = 4;
			nb++;
		}
		// if there are unvisited neighbours
		if (nb) {
			// choose one
			unsigned short dir = dirs[rand() % nb];
			// push the current cell to the stack
			push(cx);
			push(cy);
			// break wall between current and new cell
			switch (dir) {
				case 1:
					p.map[cy - 1][cx] = 0;
					p.map[cy - 2][cx] = 0;
					cy -= 2;
					break;
				case 2:
					p.map[cy + 1][cx] = 0;
					p.map[cy + 2][cx] = 0;
					cy += 2;
					break;
				case 3:
					p.map[cy][cx - 1] = 0;
					p.map[cy][cx - 2] = 0;
					cx -= 2;
					break;
				case 4:
					p.map[cy][cx + 1] = 0;
					p.map[cy][cx + 2] = 0;
					cx += 2;
					break;
			}
			// mark the cell as visited
			visited++;
		}
		// if the stack is empty
		else if (s.top != -1) {
			cy = pop();
			cx = pop();
		}
	}
	return p;
}

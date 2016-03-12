#include <curses.h>
#include <math.h>

#include "ncaster.h"
#include "input.h"

struct player get_input(struct player p) {
	switch (getin()) {
		case 'q':
			quit(0, "");
		case 'x':
			// TODO: add saving for player coordinates
			quit(0, "");
		case KEY_UP:
			// collision checking
			if (!p.map[(int) (p.y + sin(p.angle+p.fov) * P_MV)][(int) (p.x + cos(p.angle+p.fov) * P_MV)]) {
				p.x += cos(p.angle+p.fov) * P_MV;
				p.y += sin(p.angle+p.fov) * P_MV;
			}
			break;
		case KEY_DOWN:
			if (!p.map[(int) (p.y - sin(p.angle+p.fov) * P_MV)][(int) (p.x - cos(p.angle+p.fov) * P_MV)]) {
				p.x -= cos(p.angle+p.fov) * P_MV;
				p.y -= sin(p.angle+p.fov) * P_MV;
			}
			break;
		case KEY_RIGHT:
			p.angle += M_PI / 36;
			break;
		case KEY_LEFT:
			p.angle -= M_PI / 36;
			break;
		case '1':
			p.fov += M_PI / 90;
			break;
		case '2':
			p.fov -= M_PI / 90;
			break;
		case '3':
			p.hud--;
			break;
		case '4':
			p.crosshairs--;
			break;
		case '5':
			p.lantern--;
			break;
		default:
			break;
	}
	return p;
}

int getin() {
	int ch = getch();
	if (ch == ERR)
		return -1;
	else
		return ch;
}

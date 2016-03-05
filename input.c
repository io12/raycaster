#include <curses.h>
#include <math.h>

#include "ncaster.h"

struct player get_input(struct player p) {
	switch (getch()) {
		case 'q':
			p.quit = 1;
			break;
		case KEY_UP:
			// collision checking
			if (!p.map[(int) (p.y + sin(p.angle+p.fov) * P_MV_FACTOR)][(int) (p.x + cos(p.angle+p.fov) * P_MV_FACTOR)]) {
				p.x += cos(p.angle+p.fov) * P_MV_FACTOR;
				p.y += sin(p.angle+p.fov) * P_MV_FACTOR;
			}
			break;
		case KEY_DOWN:
			if (!p.map[(int) (p.y - sin(p.angle+p.fov) * P_MV_FACTOR)][(int) (p.x - cos(p.angle+p.fov) * P_MV_FACTOR)]) {
				p.x -= cos(p.angle+p.fov) * P_MV_FACTOR;
				p.y -= sin(p.angle+p.fov) * P_MV_FACTOR;
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
		default:
			break;
	}
	return p;
}

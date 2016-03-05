#ifndef _NCASTER_
#define _NCASTER_

struct player {
	int** map;
	double fov;
	double x, y, angle;
	bool lantern;
	bool hud;
	bool crosshairs;
	bool quit;
};

void init_raycaster();

void quit(char* message);

#endif

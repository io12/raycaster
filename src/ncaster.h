#ifndef _NCASTER_
#define _NCASTER_

// curses.h must be included before this
// this probably works with stdbool.h
// will test it later

#define FOV_DEFAULT 2

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
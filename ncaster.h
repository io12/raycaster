#ifndef _MAIN_
#define _MAIN_

#define MAPLENGTH 10
#define R_JMP_FACTOR 0.1
#define W_DARK '#'
#define W_DARKER ':'
#define P_MV_FACTOR 0.1

struct player {
	int** map;
	double fov;
	double x, y, angle;
	bool lantern;
	bool hud;
	bool crosshairs;
	bool quit;
};

struct ray {
	double x, y, angle;
	double distance;
	int status;
};

void init_raycaster();

void gen_frame(struct player p);

struct player get_input(struct player p);

struct player read_map(char* filename);

void quit(char* message);

#endif

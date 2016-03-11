#ifndef _RENDER_
#define _RENDER_

#define R_JMP 0.1
#define W_DARK '#'
#define W_DARKER ':'

struct ray {
	double x, y, angle;
	double distance;
	int status;
};

void gen_frame(struct player p);

void gen_col(struct player p, struct ray r, int col);

void gen_hud();

void gen_crosshairs();

#endif

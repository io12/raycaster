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

#endif

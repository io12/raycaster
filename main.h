#ifndef _MAIN_
#define _MAIN_

#define MAPLENGTH 10
#define R_JMP_FACTOR 0.1
#define W_DARK '#'
#define W_DARKER ':'
#define P_MV_FACTOR 0.1

const int map[][MAPLENGTH] = {
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

void init_raycaster();

void gen_frame();

void get_input();

#endif

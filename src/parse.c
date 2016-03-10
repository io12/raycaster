#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <ctype.h>
#include <math.h>

#include "ncaster.h"

struct player parse_map(char* filename) {
	// initialize some values
	struct player p;
	p.fov = FOV_DEFAULT;
	p.lantern = 1;
	p.hud = 1;
	p.crosshairs = 1;

	// open file
	FILE* fp = fopen(filename, "r");
	if (!fp)
		quit(1, "");

	// get size of file
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	if (fsize < 11)
		quit(1, "Map file too small");
	rewind(fp);

	char ch;
	char prev_ch;

	// get coordinates
	char coord[5];
	for (int i = 0; i < 5; i++) {
		ch = fgetc(fp);
		if (!isdigit(ch) && ch != '.')
			break;
		coord[i] = ch;
	}
	sscanf(coord, "%lf", &p.x);
	for (int i = 0; i < 5; i++) {
		ch = fgetc(fp);
		if (!isdigit(ch) && ch != '.')
			break;
		coord[i] = ch;
	}
	sscanf(coord, "%lf", &p.y);
	fsize -= ftell(fp);
	long coord_end = ftell(fp);

	int lines = 0;
	int cols = 0;
	bool inc_cols = 1;
	for (int i = 0; i < fsize; i++) {
		ch = fgetc(fp);
		if (ch == '\n') {
			lines++;
			inc_cols = 0;
		}
		else if (!isdigit(ch))
			quit(1, "Incorrect Format. Map file should only contain digits");
		if (inc_cols)
			cols++;
	}
	if (p.x < 1 || p.x > cols || p.y < 1 || p.y > lines)
		quit(1, "The player can't start outside the map");
	fseek(fp, coord_end, SEEK_SET);
	p.map = (int**) malloc(lines * sizeof(int*));
	for (int i = 0; i < lines; i++)
		p.map[i] = (int*) malloc(sizeof(int) * cols);
	for (int i = 0, j = 0; j < lines;) {
		ch = fgetc(fp);
		if ((j == 0 || j == lines - 1 || i == 0) && ch == '0')
			quit(1, "The border of the map should be solid");
		if (ch == '\n') {
			if (prev_ch == '\n')
				quit(1, "Map files can't contain empty lines");
			if (p.map[j][i-1] == 0)
				quit(1, "The border of the map should be solid");
			if (i != cols)
				quit(1, "The map must be rectangular");
			j++;
			i = 0;
		}
		else {
			p.map[j][i] = (int) ch - 48;
			i++;
		}
		prev_ch = ch;
	}
	if (p.map[(int) round(p.y - 1)][(int) round(p.x - 1)])
		quit(1, "The player can't start inside a wall");
	fclose(fp);
	return p;
}

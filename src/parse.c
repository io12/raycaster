#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <ctype.h>

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
		quit("File does not exist");

	// get size of file
	fseek(fp, 0, SEEK_END);
	const long fsize = ftell(fp) - 4;
	if (fsize < 11)
		quit("Map file too small");
	rewind(fp);

	char ch;
	char prev_ch;

	// get coordinates
	// TODO: allow multi-digit coords
	ch = fgetc(fp);
	if (!isdigit(ch))
		quit("Incorrect format. Coordinates have to be digits.");
	p.x = (double) ch - 48;
	fseek(fp, 2, SEEK_SET);
	ch = fgetc(fp);
	if (!isdigit(ch))
		quit("Incorrect format. Coordinates have to be digits.");
	p.y = (double) ch - 48;
	fseek(fp, 4, SEEK_SET);

	int lines = 0;
	int cols = 0;
	bool inc_cols = 1;
	for (int i = 0; i < fsize; i++) {
		ch = fgetc(fp);
		if (ch == '\n'){
			lines++;
			inc_cols = 0;
		}
		else if (!isdigit(ch))
			quit("Incorrect Format. Map file should only contain digits");
		if (inc_cols)
			cols++;
	}
	if (p.x < 1 || p.x > cols || p.y < 1 || p.y > lines)
		quit("The player can't start outside the map");
	fseek(fp, 4, SEEK_SET);
	p.map = (int**) malloc(lines * sizeof(int*));
	for (int i = 0; i < lines; i++)
		p.map[i] = (int*) malloc(sizeof(int) * cols);
	for (int i = 0, j = 0; j < lines;) {
		ch = fgetc(fp);
		if ((j == 0 || j == lines - 1 || i == 0) && ch == '0')
			quit("The border of map should be solid");
		if (ch == '\n') {
			if (prev_ch == '\n')
				quit("Map files can't contain empty lines");
			if (p.map[j][i-1] == 0)
				quit("The border of map should be solid");
			if (i != cols)
				quit("The map must be rectangular");
			j++;
			i = 0;
		}
		else {
			p.map[j][i] = (int) ch - 48;
			i++;
		}
		prev_ch = ch;
	}
	if (p.map[(int) p.y - 1][(int) p.x - 1])
		quit("The player can't start inside a wall");
	fclose(fp);
	return p;
}

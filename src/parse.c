#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
		quit(1, "File \"%s\" not found", filename);

	// get size of file
	fseek(fp, 0, SEEK_END);
	int fsize = ftell(fp);
	if (fsize < 11)
		quit(1, "Format error: %d chars in file\nToo small", fsize);
	rewind(fp);

	char ch;
	char prev_ch = 0;

	// get coordinates
	char coord[10];
	for (int i = 0; i < 10; i++) {
		ch = fgetc(fp);
		if (!isdigit(ch) && ch != '.')
			break;
		coord[i] = ch;
	}
	sscanf(coord, "%lf", &p.x);
	for (int i = 0; i < 10; i++) {
		ch = fgetc(fp);
		if (!isdigit(ch) && ch != '.')
			break;
		coord[i] = ch;
	}
	sscanf(coord, "%lf", &p.y);
	fsize -= ftell(fp);
	int coord_end = ftell(fp);

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
			quit(1, "Format error: \"%c\" is not a digit", ch);
		if (inc_cols)
			cols++;
	}
	if (p.x < 1 || p.x > cols || p.y < 1 || p.y > lines)
		quit(1, "Bound error: The player can't start outside the map. Check your coord lines.\nPlayer coords: %f, %f\nMap bounds: (1, 1), (%d, %d)", p.x, p.y, cols, lines);
	fseek(fp, coord_end, SEEK_SET);
	p.map = (int**) malloc(lines * sizeof(int*));
	for (int i = 0; i < lines; i++)
		p.map[i] = (int*) malloc(sizeof(int) * cols);
	for (int i = 0, j = 0; j < lines;) {
		ch = fgetc(fp);
		if ((j == 0 || j == lines - 1 || i == 0 || i + 1 == cols) && ch == '0')
			quit(1, "Format error: Border of the map not solid. Opening at %d, %d.", i, j);
		if (ch == '\n') {
			if (prev_ch == '\n')
				quit(1, "Format error: Line %d empty", j + 1);
			if (i != cols)
				quit(1, "Format error: Map not rectangular. Irrectangularity found at line %d", j);
			j++;
			i = 0;
		}
		else {
			p.map[j][i] = (int) ch - 48;
			i++;
		}
		prev_ch = ch;
	}
	int p_start_status = p.map[(int) round(p.y - 1)][(int) round(p.x - 1)];
	if (p_start_status)
		quit(1, "Format error: Player can't start inside a wall. Wall of type %d at %d, %d", p_start_status, (int) p.x, (int) p.y);
	fclose(fp);
	return p;
}

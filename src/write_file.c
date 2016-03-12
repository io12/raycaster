#include <stdio.h>
#include <stdbool.h>

#include "ncaster.h"

void write_map(struct player p, int map_x, int map_y) {
	FILE* fp = fopen(filename, "w");
	if (!fp)
		quit(1, "Error creating file \"%s\" for writing", filename);
	fprintf(fp, "%d %d\n", (int) p.x, (int) p.y);
	int i, j;
	for (i = 0; i < map_y; i++) {
		for (j = 0; j < map_x; j++) {
			fputc(p.map[i][j] + 48, fp);
		}
		fputc('\n', fp);
	}
	printf("Wrote %ld characters, %d lines to \"%s\"\n", ftell(fp), ++i, filename);
	fclose(fp);
}

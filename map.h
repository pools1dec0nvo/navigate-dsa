#ifndef MAP_H
#define MAP_H

#include <stdio.h>

void populate_map(FILE* in, int** map, int LIN, int COL, int cy, int cx, int k, int top, int left);
void find_map_limits(int limits[6], int LIN, int COL, int x, int y, int k);
int header_check(int limits[6], int LIN, int COL, int TARGET, int y, int x, int k, int INIT);
void map_skip(FILE* in, int LIN, int COL);

#endif
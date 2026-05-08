#ifndef DFS_H
#define DFS_H

#include <stdio.h>

#include "point.h"
#include "dfscontent.h"

int energy_compare(const void* a, const void* b);
Point* populate_Point_vector(FILE* in, int LIN, int COL, int initY, int initX, int k, int top, int left, int* candidateCount);
int bound_calculate(int RSTEPS, Point* candidates, int candidateCount, int lin, int col, int** visited, int LIN, int COL);
int dfs_target_path(DFSContent* mapData, int x, int y, int stepsLeft, int currentSum, int pathIndex);
void dfs_best_path(DFSContent* mapData, int x, int y, int stepsLeft, int currentSum, int pathIndex);


#endif
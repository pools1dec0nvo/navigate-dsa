#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

void free_matrix(int** matrix, int height);
int** matrix_allocate(int LIN, int COL, int isZero);

#endif
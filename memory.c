#include <stdlib.h>

#include "memory.h"

/***********************************************************************
 * free_matrix
 * Descrição: Liberta a memória alocada para uma matriz bidimensional.
 * Argumentos:
 *   - matrix: ponteiro para a matriz a ser libertada.
 *   - height: número de linhas da matriz.
 ***********************************************************************/
void free_matrix(int** matrix, int height) {
    if (matrix != NULL) {
        for (int i = 0; i < height; i++) {
            free(matrix[i]);
        }
        free(matrix);
    } else
        exit(0);
}

/***********************************************************************
 * matrix_allocate
 * Descrição: Aloca memória para uma matriz bidimensional de inteiros.
 * Argumentos:
 *   "LIN": número de linhas.
 *   "COL": número de colunas.
 *   - isZero: indica se a matriz deve ser inicializada a zeros (1) ou não (0).
 * Retorna:
 *   - Ponteiro para a matriz alocada.
 ***********************************************************************/
int** matrix_allocate(int LIN, int COL, int isZero) {
    int** matrix = (int**)malloc(LIN * sizeof(int*));
    if (matrix == NULL) {
        exit(0);
    }
    for (int i = 0; i < LIN; i++) {
        if (isZero == 1) {
            matrix[i] = (int*)calloc(COL, sizeof(int));
        } else {
            matrix[i] = (int*)malloc(COL * sizeof(int));
        }
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            exit(0);
        }
    }
    return matrix;
}

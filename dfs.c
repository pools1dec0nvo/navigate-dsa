#include <stdio.h>
#include <stdlib.h>

#include "dfs.h"

#include "point.h"
#include "dfscontent.h"

/***********************************************************************
 * energy_compare
 * Descrição: Função de comparação para qsort, ordena pontos por valor de energia decrescente.
 * Argumentos:
 *   - a, b: ponteiros para os elementos a comparar.
 * Devolve:
 *   - Valor negativo, zero, ou positivo se o primeiro elemento for menor, igual ou maior que o segundo.
 ***********************************************************************/
int energy_compare(const void* a, const void* b) {
    Point* pointA = (Point*)a;
    Point* pointB = (Point*)b;
    return pointB->energyValue - pointA->energyValue;
}

/***********************************************************************
 * populate_Point_vector
 * Descrição: Cria um array de pontos candidatos dentro do alcance k, ordenados por valor de energia.
 * Argumentos:
 *   - in: ficheiro de entrada.
 *   - "LIN, COL": dimensões do mapa.
 *   - initY, initX: posição inicial.
 *   - k: alcance máximo.
 *   - top, left: limites superiores da submatriz.
 *   - candidateCount: ponteiro para armazenar o número de candidatos.
 * Devolve:
 *   - Array de pontos candidatos ou NULL em caso de mapa sem solução.
 ***********************************************************************/
Point* populate_Point_vector(FILE* in, int LIN, int COL, int initY, int initX, int k, int top, int left, int* candidateCount) {
    initY--;
    initX--;

    int realPointCount = (2 * k + 1) * (2 * k + 1);
    Point* points = (Point*)malloc(realPointCount * sizeof(Point));
    if (points == NULL)
        exit(0);


    int lin, col;
    int value;
    int allocatedPointIndex = 0;

    for (lin = 0; lin < LIN; lin++) {
        for (col = 0; col < COL; col++) {
            if (fscanf(in, "%d", &value) != 1) {
                exit(0);
            }
            if (abs(lin - initY) + abs(col - initX) <= k) {
                points[allocatedPointIndex].lin = lin;
                points[allocatedPointIndex].col = col;
                points[allocatedPointIndex].rLin = lin - top;
                points[allocatedPointIndex].rCol = col - left;
                points[allocatedPointIndex].energyValue = value;
                allocatedPointIndex++;
            }
        }
    }

    if (k > allocatedPointIndex) {
        free(points);
        return NULL;
    }

    *candidateCount = allocatedPointIndex;
    qsort(points, allocatedPointIndex, sizeof(Point), energy_compare);

    return points;
}

/***********************************************************************
 * bound_calculate
 * Descrição: Calcula o melhor cenário possível para a energia restante.
 * Argumentos:
 *   - "RSTEPS": número de passos restantes.
 *   - candidates: array de pontos candidatos ordenados por energia.
 *   - candidateCount: número de candidatos.
 *   - lin, col: posição atual (linha, coluna).
 *   - visited: matriz que indica se um ponto já foi visitado.
 *   - "LIN, COL": dimensões da matriz.
 * Devolve:
 *   - Soma máxima possível de energia que pode ser obtida nos passos restantes.
 ***********************************************************************/
int bound_calculate(int RSTEPS, Point* candidates, int candidateCount, int lin, int col, int** visited, int LIN, int COL) {
    int bestCaseScenario = 0;
    int i = 0, jump = 0;

    while (i < RSTEPS) {
        if (i + jump >= candidateCount)
            break; // Prevent out-of-limits access

        int candLin = candidates[i + jump].rLin;
        int candCol = candidates[i + jump].rCol;

        if (candLin < 0 || candLin >= LIN || candCol < 0 || candCol >= COL) {
            jump++;
            continue;
        }

        if (abs(candLin - lin) + abs(candCol - col) > RSTEPS)
            jump++;
        else if (visited[candLin][candCol] == 1)
            jump++;
        else {
            bestCaseScenario += candidates[i + jump].energyValue;
            i++;
        }
    }

    return bestCaseScenario;
}

/***********************************************************************
 * dfs_target_path
 * Descrição: Realiza uma busca em profundidade (DFS) para encontrar um caminho que atinja ou exceda a energia alvo.
 * Argumentos:
 *   - mapData: estrutura contendo os dados do mapa e estado atual.
 *   - x, y: posição atual no mapa.
 *   - stepsLeft: número de passos restantes.
 *   - currentSum: soma atual de energia.
 *   - pathIndex: índice atual no caminho.
 * Devolve:
 *   - 1 se encontrar um caminho que atinge o alvo; 0 caso contrário.
 ***********************************************************************/
int dfs_target_path(DFSContent* mapData, int x, int y, int stepsLeft, int currentSum, int pathIndex) {
    if (stepsLeft == 0) {
        if (currentSum >= mapData->TARGET) {
            return 1;
        }
        return 0;
    }

    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    for (int dir = 0; dir < 4; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (nx < 0 || nx >= mapData->COL || ny < 0 || ny >= mapData->LIN) {
            continue;
        }

        if (currentSum <= 0)
            continue;

        int distance = abs(nx - mapData->cx) + abs(ny - mapData->cy);
        if (distance > mapData->k) {
            continue;
        }

        int optimal = bound_calculate(stepsLeft, mapData->candidateArray, mapData->candidateArraySize, ny, nx, mapData->visited, mapData->LIN, mapData->COL) + currentSum;
        if (optimal < mapData->TARGET) {
            continue;
        }

        if (!mapData->visited[ny][nx]) {
            mapData->visited[ny][nx] = 1;

            mapData->latestPath[pathIndex].rLin = ny;
            mapData->latestPath[pathIndex].rCol = nx;
            mapData->latestPath[pathIndex].energyValue = mapData->map[ny][nx];

            int newSum = currentSum + mapData->map[ny][nx];
            if (dfs_target_path(mapData, nx, ny, stepsLeft - 1, newSum, pathIndex + 1)) {
                return 1;
            }

            mapData->visited[ny][nx] = 0;
        }
    }

    return 0;
}

/***********************************************************************
 * dfs_best_path
 * Descrição: Realiza uma busca em profundidade (DFS) para encontrar o caminho com a máxima soma de energia.
 * Argumentos:
 *   - mapData: estrutura contendo os dados do mapa e estado atual.
 *   - x, y: posição atual no mapa.
 *   - stepsLeft: número de passos restantes.
 *   - currentSum: soma atual de energia.
 *   - pathIndex: índice atual no caminho.
 ***********************************************************************/
void dfs_best_path(DFSContent* mapData, int x, int y, int stepsLeft, int currentSum, int pathIndex) {

    if (stepsLeft == 0) {
        if (currentSum > mapData->maxSum) {
            mapData->maxSum = currentSum;
            mapData->bestPathLength = pathIndex;
            for (int i = 0; i < pathIndex; i++) {
                mapData->bestPath[i] = mapData->latestPath[i];
            }
        }
        return;
    }

    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    for (int dir = 0; dir < 4; dir++) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (nx < 0 || nx >= mapData->COL || ny < 0 || ny >= mapData->LIN) {
            continue;
        }

        if (currentSum <= 0)
            continue;

        int distance = abs(nx - mapData->cx) + abs(ny - mapData->cy);
        if (distance > mapData->k) {
            continue;
        }

        if (!mapData->visited[ny][nx]) {
            mapData->visited[ny][nx] = 1;

            mapData->latestPath[pathIndex].rLin = ny;
            mapData->latestPath[pathIndex].rCol = nx;
            mapData->latestPath[pathIndex].energyValue = mapData->map[ny][nx];

            int newSum = currentSum + mapData->map[ny][nx];

            int optimal = bound_calculate(stepsLeft - 1, mapData->candidateArray, mapData->candidateArraySize, ny, nx, mapData->visited, mapData->LIN, mapData->COL) + newSum;
            if (optimal <= mapData->maxSum) {
                mapData->visited[ny][nx] = 0;
                continue;
            }

            dfs_best_path(mapData, nx, ny, stepsLeft - 1, newSum, pathIndex + 1);

            mapData->visited[ny][nx] = 0;
        }
    }
}

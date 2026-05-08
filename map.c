#include <stdlib.h>
#include <stdio.h>

#include "map.h"
/***********************************************************************
 * populate_map
 * Descrição: Preenche a matriz do mapa com os valores do ficheiro de entrada, considerando o alcance k.
 * Argumentos:
 *   - in: ficheiro de entrada.
 *   - map: matriz do mapa a ser preenchida.
 *   - "LIN, COL": dimensões do mapa original.
 *   - cy, cx: coordenadas do centro.
 *   - k: alcance máximo a partir do centro.
 *   - top, left: limites superiores da submatriz.
 ***********************************************************************/
void populate_map(FILE* in, int** map, int LIN, int COL, int cy, int cx, int k, int top, int left) {
    int centerRow = cy - 1;
    int centerCol = cx - 1;

    int r, c;
    int val;

    for (r = 0; r < LIN; r++) {
        for (c = 0; c < COL; c++) {
            if (fscanf(in, "%d", &val) != 1) {
                exit(0);
            }
            if (abs(r - centerRow) + abs(c - centerCol) <= k) {
                int mapI = r - top;
                int mapJ = c - left;
                if (mapI >= 0 && mapI < (2 * k + 1) && mapJ >= 0 && mapJ < (2 * k + 1)) {
                    map[mapI][mapJ] = val;
                }
            }
        }
    }
}
/***********************************************************************
 * find_map_limits
 * Descrição: Calcula os limites da submatriz dentro do alcance k a partir de uma posição central.
 * Argumentos:
 *   - limits: array para armazenar os limites calculados (top, bottom, left, right, height, width).
 *   - "LIN, COL" dimensões do mapa original.
 *   - x, y: coordenadas da posição central.
 *   - k: alcance máximo.
 ***********************************************************************/
void find_map_limits(int limits[6], int LIN, int COL, int x, int y, int k) {
    int top, bottom, left, right, height, width;
    top = y - 1 - k;
    if (top < 0)
        top = 0;
    bottom = y - 1 + k;
    if (bottom >= LIN)
        bottom = LIN - 1;
    left = x - 1 - k;
    if (left < 0)
        left = 0;
    right = x - 1 + k;
    if (right >= COL)
        right = COL - 1;

    height = bottom - top + 1;
    width = right - left + 1;
    limits[0] = top;
    limits[1] = bottom;
    limits[2] = left;
    limits[3] = right;
    limits[4] = height;
    limits[5] = width;
}

/***********************************************************************
 * header_check
 * Descrição: Verifica a validade dos parâmetros de entrada e calcula os limites da matriz se válido.
 * Argumentos:
 *   - limits: array para armazenar os limites calculados.
 *   - "LIN, COL" dimensões do mapa.
 *   - "TARGET" energia alvo.
 *   - y, x: posição inicial.
 *   - k: alcance máximo.
 *   - "INIT" energia inicial.
 * Devolve:
 *   - 1 se válido para dfs_target_path; 2 se válido para dfs_best_path; 0 caso inválido.
 ***********************************************************************/
int header_check(int limits[6], int LIN, int COL, int TARGET, int y, int x, int k, int INIT) {
    if ((TARGET <= 0 && TARGET != -2) || y < 1 || y > LIN || x < 1 || x > COL || k <= 0 || INIT <= 0)
        return 0;
    else if (TARGET == -2) {
        find_map_limits(limits, LIN, COL, x, y, k);
        return 2;
    } else {
        find_map_limits(limits, LIN, COL, x, y, k);
        return 1;
    }
}
/***********************************************************************
 * map_skip
 * Descrição: Avança o ponteiro do ficheiro até ao fim do mapa atual.
 * Argumentos:
 *   - in: ficheiro de entrada.
 *   - "LIN, COL": dimensões do mapa.
 ***********************************************************************/
void map_skip(FILE* in, int LIN, int COL) {
    for (int i = 0; i < LIN * COL; i++)
        if (fscanf(in, "%*d") == 1) {
        }
}

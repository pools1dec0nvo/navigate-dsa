#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "point.h"
#include "dfscontent.h"

#include "dfs.h"
#include "file.h"
#include "memory.h"
#include "map.h"

/***********************************************************************
 * IST Navigate (the tower of doom)
 * Carolina João (109587) e Tiago Lopes Carvalho (106396)
 * Algoritmos e Estruturas de Dados LEEC 2024/25
 * 
 * Especiais agradecimentos ao grande João-Ex-Machina e os restantes monitores da cadeira pela ajuda.
 *
 * "If you have any trouble sounding condescending, find a Unix user to show you how it's done"
 *  - Scott Adams
 * Nota: Nenhuma máquina Windows prejudicou o nosso desenvolvimento :D
 ***********************************************************************/

int main(int argc, char* argv[]) {
    FILE *fin, *fout;
    char *outputFilename;
    int lines, columns, initY, relativeInitY, initX, relativeInitX, k, targetEnergy, initEnergy, candidateCount, valid;
    int mapLimits[6];
    int **taskMap, **flagMap;
    long mapStart;
    Point *boundPoints, *latestPath, *bestPath;
    DFSContent state;

    if (argc < 2)
        exit(0);

    fin = fopen(argv[1], "r");
    if (fin == NULL)
        exit(0);

    if (!check_file_extension(argv[1], ".maps"))
        exit(0);

    outputFilename = (char*)malloc((strlen(argv[1]) + 10) * sizeof(char));
    if (outputFilename == NULL) 
        exit(0);
    
    change_file_extension(argv[1], outputFilename, ".solmaps");
    fout = fopen(outputFilename, "w");
    if (fout == NULL)
        exit(0);

    while (fscanf(fin, "%d%d%d%d%d%d%d", &lines, &columns, &targetEnergy, &initY, &initX, &k, &initEnergy) == 7) {
        fprintf(fout, "%d %d %d %d %d %d %d", lines, columns, targetEnergy, initY, initX, k, initEnergy);
        mapStart = ftell(fin);
        if (mapStart == -1L)
            exit(0);

        valid = header_check(mapLimits, lines, columns, targetEnergy, initY, initX, k, initEnergy);
        if (!valid) {
            fprintf(fout, "\n\n");
            map_skip(fin, lines, columns);
            fflush(fout);
            continue;
        } else {
            boundPoints = populate_Point_vector(fin, lines, columns, initY, initX, k, mapLimits[0], mapLimits[2], &candidateCount);
            if (boundPoints == NULL) {
                fprintf(fout, " %d\n", -1);
                fflush(fout);
                continue;
            }

            taskMap = matrix_allocate(mapLimits[4], mapLimits[5], 1);
            flagMap = matrix_allocate(mapLimits[4], mapLimits[5], 1);
            relativeInitY = initY - 1 - mapLimits[0]; // coordenadas ajustadas de absolutas para relativas
            relativeInitX = initX - 1 - mapLimits[2];

            fseek(fin, mapStart, SEEK_SET);
            populate_map(fin, taskMap, lines, columns, initY, initX, k, mapLimits[0], mapLimits[2]);
            flagMap[relativeInitY][relativeInitX] = 1;

            latestPath = malloc(k * sizeof(Point));
            if (latestPath == NULL)
                exit(0);
            
            state.map = taskMap;
            state.LIN = mapLimits[4];
            state.COL = mapLimits[5];
            state.cx = relativeInitX;
            state.cy = relativeInitY;
            state.k = k;
            state.visited = flagMap;
            state.INIT = initEnergy;
            state.latestPath = latestPath;
            state.candidateArray = boundPoints;
            state.candidateArraySize = candidateCount;

            if (valid == 1) {
                state.TARGET = targetEnergy;
                
                if (dfs_target_path(&state, state.cx, state.cy, k, state.INIT, 0)) {
                    int finalSum = initEnergy;
                    for (int i = 0; i < k; i++) {
                        finalSum += state.latestPath[i].energyValue;
                    }
                    fprintf(fout, " %d\n", finalSum);

                    for (int i = 0; i < k; i++) {
                        fprintf(fout, "%d %d %d\n",
                        state.latestPath[i].rLin + mapLimits[0] + 1, // coordenadas reajustadas para absolutas indexadas a 1 (para printar)
                        state.latestPath[i].rCol + mapLimits[2] + 1,
                        state.latestPath[i].energyValue);
                    }
                    fprintf(fout, "\n");
                    fflush(fout);
                } else {
                    fprintf(fout, " %d\n\n", -1);
                    fflush(fout);
                }
            }

            if (valid == 2) {
                bestPath = malloc(k * sizeof(Point));
                if (bestPath == NULL)
                    exit(0);

                state.maxSum = INT_MIN;
                state.bestPath = bestPath;
                state.bestPathLength = 0;

                dfs_best_path(&state, state.cx, state.cy, k, state.INIT, 0);

                if (state.bestPathLength > 0) {
                    fprintf(fout, " %d\n", state.maxSum);
                    for (int i = 0; i < state.bestPathLength; i++) {
                        fprintf(fout, "%d %d %d\n",
                        state.bestPath[i].rLin + mapLimits[0] + 1,
                        state.bestPath[i].rCol + mapLimits[2] + 1,
                        state.bestPath[i].energyValue);
                    }
                    fprintf(fout, "\n");
                    fflush(fout);
                } else {
                    fprintf(fout, " %d\n\n", -1);
                    fflush(fout);
                }
                free(bestPath);
            }
            free_matrix(taskMap, mapLimits[4]);
            free_matrix(flagMap, mapLimits[4]);
            free(latestPath);
            free(boundPoints);
        }
    }
    fclose(fin);
    fclose(fout);
    free(outputFilename);
    return 0;
}
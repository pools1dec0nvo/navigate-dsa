#ifndef DFS_CONTENT_H
#define DFS_CONTENT_H

#include "point.h"

typedef struct {
    int** map;            
    int LIN;              
    int COL;              
    int cx;               
    int cy;               
    int k;                
    int INIT;
    int TARGET;           
    int** visited;        
    Point* latestPath;
    Point* candidateArray;
    int candidateArraySize;   
    int maxSum;           
    Point* bestPath;      
    int bestPathLength; 
} DFSContent;

#endif // DFS_CONTENT_H
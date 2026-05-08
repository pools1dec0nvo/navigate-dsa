#ifndef POINT_H
#define POINT_H

/*
 * r indica que as coordenadas são relativas ou correspondentes ao mapa alocado.
 */
typedef struct {
    int lin;        
    int col;       
    int rLin;    
    int rCol;       
    int energyValue; 
} Point;

#endif // POINT_H
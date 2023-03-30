#ifndef APIG23_H
#define APIG23_H

#include <stdio.h>
#include <stdlib.h>
#include "EstructuraGrafo23.h"

//debe leer desde stdin
Grafo ConstruirGrafo();

void DestruirGrafo(Grafo G);



//funciones para extraer datos del grafo. u32 debe estar definida en el .h de arriba

u32 NumeroDeVertices(Grafo G);
u32 NumeroDeLados(Grafo G);
u32 Delta(Grafo G);


//funciones de extraccion de informacion de vertices 

u32 Nombre(u32 i,Grafo G);
u32 Grado(u32 i,Grafo G);
u32 IndiceVecino(u32 j,u32 i,Grafo G);



#endif

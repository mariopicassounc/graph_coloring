#include "APIG23.h"
#include "APIParte2.h"
#include "EstructuraParte2.h"

u32 Greedy(Grafo G, u32 *Orden, u32 *Color)
{
    ColoresDisponible = calloc(Delta(G) + 1, sizeof(u32));
    /* Seteo sin colores */
    u32 numero_colores = 0;
    u32 vertice_i;
    u32 menor_color_disponible;
    u32 index = 0;
    u32 end = 0;
    
    /* Recorro el array Orden */
    for (u32 i = 0; i < NumeroDeVertices(G); i++)
    {
        vertice_i = Orden[i];

        /* Inicializamos array con NC */
        for (u32 j = 0; j < Delta(G) + 1; j++)
        {
            ColoresDisponible[j] = 0;
        }

        /* Seteamos los colores disponibles segun los colores de los vecinos */
        for (u32 j = 0; j < Grado(vertice_i, G); j++)
        {   
            /* Para evitar segfault, cuando NO esta coloreado seteamos ColoresDisponibles */
            if(Color[IndiceVecino(j, i, G)] != NC){
                ColoresDisponible[Color[IndiceVecino(j, i, G)]] = 1;
            }
            
        }

        /* Buscamos el menor color disponible (primer 0 en el array) */
        while (index < Delta(G) + 1 && !end)
        {
            if (ColoresDisponible[index] == 0)
            {
                menor_color_disponible = index;
                end = 1;
            }
            index++;
        }

        /* Actualizo numero de colores a retornar */
        if (menor_color_disponible > numero_colores)
        {
            numero_colores = menor_color_disponible;
        }
        Color[i] = menor_color_disponible;
    }

    return numero_colores;
}
#include <stdbool.h>
#include <string.h>
#include "APIParte2.h"
#define NC __UINT32_MAX__
u32 *AuxColor;
u32 *resultadoF;

/* DEBUG */
// static void imprimirColoresDisponibles(char *ColoresDisponibles, Grafo g)
// {
//     printf("Colores disponibles: ");
//     for (u32 i = 0; i < Delta(g) + 1; i++)
//     {
//         printf("%u ", ColoresDisponibles[i]);
//     }
//     printf("\n");
// }

static int compararColor(const void *a, const void *b)
{
    u32 a_color = AuxColor[*(u32 *)a];
    u32 b_color = AuxColor[*(u32 *)b];

    return b_color - a_color;
}

static int compararColorParidad(const void *a, const void *b)
{
    /* Primero los impares luego los pares. */
    u32 a_color = AuxColor[*(u32 *)a];
    u32 b_color = AuxColor[*(u32 *)b];
    bool a_paridad_color = (a_color % 2);
    bool b_paridad_color = (b_color % 2);

    /* Si ambos tienen la misma paridad, los ordenamos en base a su valor numerico */
    if (a_paridad_color == b_paridad_color)
        return b_color - a_color;

    else if (a_paridad_color)
        return -1;

    else
        return 1;
}

static int compararResultadoF(const void *a, const void *b)
{
    u32 a_res = resultadoF[*(u32 *)a];
    u32 b_res = resultadoF[*(u32 *)b];

    return b_res - a_res;
}

u32 *crearResultadoF(Grafo G, u32 *Orden, u32 *Color)
{
    /* Devuelve memoria a liberar por el usuario 
    Crea un array de tamaño NumeroDeVertices(G) donde cada posicion es el resultado de F para el color de ese vertice
    O(2n)
    */
    u32 *resultF = calloc(NumeroDeVertices(G), sizeof(u32));
    u32 vertice_i;
    u32 vertice_anterior;
    u32 sumaGrados = 0;
    u32 contadorColores = 0;
    u32 resultado = 0;

    for (u32 i = 1; i < NumeroDeVertices(G); i++)
    {
        vertice_i = Orden[i];
        vertice_anterior = Orden[i - 1];
        /* Acumulo la suma de grados */
        sumaGrados += Grado(vertice_anterior, G);
        contadorColores++;
        
        /* Si cambia de color entonces calculo el resultado de F */
        if (Color[vertice_i] != Color[vertice_anterior])
        {
            /* Calculo el resultado de F */
            resultado = sumaGrados * Color[vertice_anterior];
            /* Guardo el resultado de F para el color para todos los vertices que tengan ese mismo color */
            for (u32 j = 0; j < contadorColores; j++)
            {
                resultF[Orden[(i - contadorColores) + j ]] = resultado;
            }
            /* Reseteo la suma de grados y el contador de colores */
            sumaGrados = 0;
            contadorColores = 0;
        }
        
        /* Calculo el resultado de F para el ultimo color */
        if (i == NumeroDeVertices(G) - 1)
        {
            resultado = sumaGrados * Color[vertice_i];
            for (u32 j = 0; j < contadorColores + 1; j++)
            {
                resultF[Orden[(i - contadorColores) + j ]] = resultado;
            }
        }
    }

    return resultF;
}

u32 Greedy(Grafo G, u32 *Orden, u32 *Color)
{
    /*
    Se nos complico hacer andar Greedy con un bitmap real pero con tiempo se puede y quedaría mas rápido
    Se puede hacer uso de funciones de gcc como __builtin_clz(bitmap) que devuelve la posicion del primer 0
    y estan super optimizadas.

    Declaramos un "bitmap" del tamaño delta de G + 1 */
    bool *ColoresDisponibles = calloc(NumeroDeVertices(G), sizeof(bool));
    if (ColoresDisponibles == NULL)
    {
        printf("Error al reservar memoria para ColoresDisponibles\n");
        return NC;
    }

    /* Seteo sin colores */
    u32 numero_colores = 0;
    u32 vertice_i;
    u32 menor_color_disponible;

    /* Recorro el array Orden */
    for (u32 i = 0; i < NumeroDeVertices(G); i++)
    {
        vertice_i = Orden[i];

        /* En el array los colores libres estan en 0 y los ocupados en 1 */
        memset(ColoresDisponibles, 0, NumeroDeVertices(G) * sizeof(bool));

        /* Seteamos los colores disponibles segun los colores de los vecinos del vertice_i */
        for (u32 j = 0; j < Grado(vertice_i, G); j++)
        {
            /* Si un vertice tiene color entonces marco que color es en el bitmap */
            if (Color[IndiceVecino(j, vertice_i, G)] != NC)
            {
                ColoresDisponibles[Color[IndiceVecino(j, vertice_i, G)]] = 1;
            }
        }
        // imprimirColoresDisponibles(ColoresDisponibles, G);

        /* Buscamos el menor color disponible (primer 0 en el array) */
        for (u32 i = 0; i < Delta(G) + 1; i++)
        {
            if (ColoresDisponibles[i] == 0)
            {
                menor_color_disponible = i;
                break;
            }
        }

        /* Actualizo numero de colores a retornar */
        if (menor_color_disponible + 1 > numero_colores)
        {
            numero_colores = menor_color_disponible + 1;
        }

        /* Seteamos el color */
        Color[vertice_i] = menor_color_disponible;
    }

    return numero_colores;
}

/*
    Ordenes
*/

char OrdenImparPar(u32 n, u32 *Orden, u32 *Color)
{
    AuxColor = Color;
    qsort(Orden, n, sizeof(u32), compararColorParidad);
    return (char)0;
}

char OrdenJedi(Grafo G, u32 *Orden, u32 *Color)
{
    /* Ordeno por color */
    qsort(Orden, NumeroDeVertices(G), sizeof(u32), compararColor);

    /* Guardo para cada vertice el resultado de F segun su color */
    resultadoF = crearResultadoF(G, Orden, Color);

    /* Ordeno por resultado de F */
    qsort(Orden, NumeroDeVertices(G), sizeof(u32), compararResultadoF);

    free(resultadoF);
    return 0;
}
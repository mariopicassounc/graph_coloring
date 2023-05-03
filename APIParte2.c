#include <stdbool.h>
#include <string.h>
#include "APIParte2.h"
#define NC (2 ^ 32) - 1
u32 *AuxColor;

/* DEBUG */
static void imprimirColoresDisponibles(char *ColoresDisponibles, Grafo g)
{
    printf("Colores disponibles: ");
    for (u32 i = 0; i < Delta(g) + 1; i++)
    {
        printf("%u ", ColoresDisponibles[i]);
    }
    printf("\n");
}

static int compararColor(const void *a, const void *b)
{
    /* Primero los impares luego los pares. */
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

u32 Greedy(Grafo G, u32 *Orden, u32 *Color)
{
    /*
    Se nos complico hacer andar Greedy con un bitmap real pero con tiempo se puede y quedaría mas rápido
    Se puede hacer uso de funciones de gcc como __builtin_clz(bitmap) que devuelve la posicion del primer 0
    y estan super optimizadas.

    Declaramos un "bitmap" del tamaño delta de G + 1 */
    char *ColoresDisponibles = calloc(Delta(G) + 1, sizeof(u32));
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
        memset(ColoresDisponibles, 0, Delta(G) + 1);

        /* Seteamos los colores disponibles segun los colores de los vecinos del vertice_i */
        for (u32 j = 0; j < Grado(vertice_i, G); j++)
        {
            if (j == 0)
            {
                ColoresDisponibles[0] = 1;
            }
            /* Si un vertice tiene color entonces marco que color es en el bitmap */
            else if (Color[IndiceVecino(j, vertice_i, G)] != NC)
            {
                ColoresDisponibles[Color[IndiceVecino(j, vertice_i, G)]] = 1;
            }
        }
        imprimirColoresDisponibles(ColoresDisponibles, G);

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
        if (menor_color_disponible > numero_colores)
        {
            numero_colores = menor_color_disponible;
        }

        /* Seteamos el color */
        Color[i] = menor_color_disponible;
    }

    return numero_colores;
}

char OrdenImparPar(u32 n, u32 *Orden, u32 *Color)
{
    AuxColor = Color;
    qsort(Orden, n, sizeof(u32), compararColorParidad);
    return 0;
}

char OrdenColor(u32 n, u32 *Orden, u32 *Color)
{
    AuxColor = Color;
    qsort(Orden, n, sizeof(u32), compararColor);
    return 0;
}
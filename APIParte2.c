#include <stdbool.h>
#include <string.h>
#include "APIG23.h"
#include "APIParte2.h"
#include "EstructuraParte2.h"

#define print_bits(x)                                    \
    do                                                   \
    {                                                    \
        unsigned long long a__ = (x);                    \
        size_t bits__ = sizeof(x) * 8;                   \
        printf(#x ": ");                                 \
        while (bits__--)                                 \
            putchar(a__ & (1ULL << bits__) ? '1' : '0'); \
        putchar('\n');                                   \
    } while (0)

/* DEBUG */
void imprimirColoresDisponibles(long long *ColoresDisponibles, Grafo g)
{
    printf("Colores disponibles: ");
    for (u32 i = 0; i < (Delta(g) / 64) + 1; i++)
    {
        print_bits(ColoresDisponibles[i]);
        printf(" ");
    }
    printf("\n");
}

void imprimirBits(size_t const size, void const *const ptr)
{
    unsigned char *b = (unsigned char *)ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--)
    {
        for (j = 7; j >= 0; j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

/* QSORT */
int comparar_colores(const void *a, const void *b)
{
    u32 *x = (u32 *)a;
    u32 *y = (u32 *)b;
    return Color[*x] - Color[*y];
}

u32 Greedy(Grafo G, u32 *Orden, u32 *Color)
{
    /* Declaramos un bitmap del tamaño delta de G + 1*/
    long long *ColoresDisponibles = calloc((Delta(G) / 64) + 1, sizeof(u32));
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

        /* En este bitmap los colores libres estan en 0 y los ocupados en 1 */
        memset(ColoresDisponibles, 0, (Delta(G) / 64) + 1);

        /* Seteamos los colores disponibles segun los colores de los vecinos del vertice_i */
        for (u32 j = 0; j < Grado(vertice_i, G); j++)
        {
            if (j == 0)
            {
                ColoresDisponibles[0] |= 1 << 31;
            }
            /* Si un vertice tiene color entonces marco que color es en el bitmap*/
            else if (Color[IndiceVecino(j, vertice_i, G)] != NC)
            {
                ColoresDisponibles[Color[IndiceVecino(j, vertice_i, G)] / 64] |= 1 << (Color[IndiceVecino(j, vertice_i, G)] % 64);
            }
        }
        imprimirColoresDisponibles(ColoresDisponibles, G);

        /* Buscamos el menor color disponible (primer 0 en el array) */
        for (u32 i = 0; i < (Delta(G) / 64) + 1; i++)
        {
            if (ColoresDisponibles[i] != 0)
            {
                menor_color_disponible = __builtin_clz(~ColoresDisponibles[i]);
                menor_color_disponible = 64 * i + menor_color_disponible;
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

// char OrdenImparPar(u32 n, u32 *Orden, u32 *Color)
// {
//     /* Crear un array para ordenar los vertices de mayor a menor segun su color */
//     u32 *OrdenAux = calloc(n, sizeof(u32));
//     if (OrdenAux == NULL)
//     {
//         printf("Error al reservar memoria para OrdenAux\n");
//         exit(1);
//     }
//     /* Ordenar los vertices de mayor a menor segun su color con qsort */
//     qsort(OrdenAux, n, sizeof(u32), comparar_colores);

// }
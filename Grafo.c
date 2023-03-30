#include "APIG23.h"

struct LadoSt nuevo_lado(u32 x, u32 y)
{
    struct LadoSt l;
    l.lado_x = x;
    l.lado_y = y;
    return l;
}

/*
Alloca memoria para crear un arreglo de tamaño total_lados
*/
Lados construir_lados(u32 total_lados)
{
    Lados l = (Lados)calloc(total_lados, sizeof(struct LadoSt));
    return (l);
}

/*
Define un lado en la posición par y en la posición impar
*/
Lados crear_lados(Lados l, u32 i, u32 x, u32 y)
{
    l[2 * i].lado_x = x;
    l[2 * i].lado_y = y;
    l[2 * i + 1].lado_x = y; // guardo el lado al revez
    l[2 * i + 1].lado_y = x;
    return (l);
}

Lados cargar_lados()
{
    u32 total_lados = 0, total_vertices = 0;
    u32 x = 0, y = 0;
    u32 index = 0;
    int count = 0;
    Lados lados = NULL;
    char c;

    while (fscanf(stdin, "%c", &c) != EOF)
    {
        if (c == 'p')
        {
            count = fscanf(stdin, " edge %u %u", &total_vertices, &total_lados);
            if (count != 2)
            {
                fprintf(stderr, "Error de lectura 1\n");
                exit(EXIT_FAILURE);
            }

            printf("total_v: %d,\t total_l: %d\n", total_vertices, total_lados);

            lados = construir_lados(2 * total_lados);
        }
        else if (c == 'e')
        {
            count = fscanf(stdin, "%u %u", &x, &y);
            if (count != 2)
            {
                fprintf(stderr, "Error de lectura 2\n");
                exit(EXIT_FAILURE);
            }

            printf("x: %u,\t y:%u \n", x, y);
            lados = crear_lados(lados, index, x, y);
            ++index;
        }
        else if (c == 'c')
        {
            fscanf(stdin, "%*[^\n]"); // ignore the line: %*[^\n]
        }
    }
    return lados;
}

    u32 NumeroDeVertices(Grafo G)
    {
        return G->numero_vertices;
    }

    u32 NumeroDeLados(Grafo G)
    {
        return G->numero_lados;
    }

    u32 Delta(Grafo G)
    {
        return G->delta;
    }

    u32 Nombre(u32 i, Grafo G)
    {
        return G->v[G->orden[i]].nombre;
    }

    u32 Grado(u32 i, Grafo G)
    {
        return G->v[G->orden[i]].grado;
    }


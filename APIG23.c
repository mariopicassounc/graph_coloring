#include "APIG23.h"
#include <stdbool.h>



/* DEBUG */
imprimir_lados(Lados l, u32 m){
    for (u32 i = 0; i < m ; i++){
        fprintf(stdout, "%u \t %u\n", l[i].lado_x, l[i].lado_y);
    }
}

/* FUNCIONES AUXILIARES */

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
    bool m_alcanzado = false; // Una vez alcanzado el m se trunca el grafo (por mas que hayan mas datos)
    Lados lados = NULL;
    char c;
    
    while ((fscanf(stdin, "%c", &c) != EOF) && !(m_alcanzado))
    {   
        /* Leer encabezado */
        if (c == 'p')
        {
            count = fscanf(stdin, " edge %u %u", &total_vertices, &total_lados);
            if (count != 2)
            {
                fprintf(stderr, "Error de lectura 1\n");
                exit(EXIT_FAILURE);
            }
            total_lados = total_lados * 2; // para grafos sin direccion: xy yx
            
            lados = construir_lados(total_lados); 
        }
        /* Leer lados */
        else if (c == 'e')
        {
            count = fscanf(stdin, "%u %u", &x, &y);
            if (count != 2)
            {
                fprintf(stderr, "Error de lectura 2\n");
                exit(EXIT_FAILURE);
            }

            lados = crear_lados(lados, index, x, y);
            index++;

            if (index == total_lados){
                m_alcanzado = true;
            }
        }
        else if (c == 'c')
        {
            fscanf(stdin, "%*[^\n]"); // ignorar la linea con exp reg: %*[^\n]
        }
    }

    imprimir_lados(lados, total_lados);
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

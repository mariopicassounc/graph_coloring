#include "APIG23.h"
#include <stdbool.h>

/* DEBUG */
void imprimir_lados(Lados l, u32 m)
{
    for (u32 i = 0; i < m; i++)
    {
        fprintf(stdout, "%u \t %u\n", l[i].lado_x, l[i].lado_y);
    }
}

/* FUNCIONES AUXILIARES */

/* LADOS */

/* Alloca memoria para crear un arreglo de tamaño total_lados */
Lados construir_lados(u32 total_lados)
{
    Lados l = (Lados)calloc(total_lados, sizeof(struct LadoSt));
    return (l);
}

/* Define un lado en la posición par y en la posición impar */
Lados guardar_lado(Lados l, u32 i, u32 x, u32 y)
{
    l[2 * i].lado_x = x;
    l[2 * i].lado_y = y;
    l[2 * i + 1].lado_x = y; // guardo el lado al revez
    l[2 * i + 1].lado_y = x;
    return (l);
}


/* Usada por qsort */
int comparar_lados(const void *a, const void *b)
{
    LadoSt *x1 = (LadoSt *)a;
    LadoSt *x2 = (LadoSt *)b;
    if (x1->lado_x < x2->lado_x)
    {
        return -1;
    }
    else if (x1->lado_x > x2->lado_x)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* Ordena los lados con respecto a los x */
void ordenar_lados(Lados l, u32 m)
{
    qsort(l, m, sizeof(struct LadoSt), comparar_lados);
}

Lados cargar_lados(u32 *m, u32 *n)
{
    u32 x = 0, y = 0;
    u32 index = 0;
    u32 count = 0;
    bool m_alcanzado = false; // Una vez alcanzado el m se trunca el grafo (por mas que hayan mas datos)
    Lados lados = NULL;
    char c;

    while ((fscanf(stdin, "%c", &c) != EOF) && !(m_alcanzado))
    {
        /* Leer encabezado */
        if (c == 'p')
        {
            count = fscanf(stdin, " edge %u %u", n, m);
            if (count != 2)
            {
                fprintf(stderr, "Error de lectura 1\n");
                exit(EXIT_FAILURE);
            }

            lados = construir_lados(2 * (*m)); // 2*m porque guardo (x,y) y (y,x)
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

            lados = guardar_lado(lados, index, x, y);
            index++;

            if (index == (*m))
            {
                m_alcanzado = true;
            }
        }
        else if (c == 'c')
        {
            count = fscanf(stdin, "%*[^\n]"); // ignorar la linea con exp reg: %*[^\n]
            if (count != 0)
            {
                fprintf(stderr, "Error de lectura 2\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    return lados;
}

/* GRAFO */

Grafo inicializar_grafo(u32 n, u32 m)
{   
    Grafo g = (Grafo)calloc(1, sizeof(struct GrafoSt));
    
    g->numero_vertices = n;
    g->numero_lados = m;
    g->delta = 0;
    g->v = calloc(n, sizeof(struct VerticeSt));

    return (g);
}

void cargar_grafo(Grafo g, Lados l)
{
    u32 grado = 0;
    u32 j = 0;
    for (u32 i = 0; i < g->numero_lados * 2; i++){
        
        grado++;

        // Si el lado actual es distinto al siguiente, entonces es el ultimo lado de un vertice
        if (l[i].lado_x != l[i+1].lado_y){
        
            g->v[j].grado = grado;
            g->v[j].nombre = l[i].lado_x;
            g->v[j].vecinos = calloc(grado, sizeof(u32));
            
            // Asigno los vecinos
            for(u32 k = 0; k < grado; k++){
                g->v[j].vecinos[k] = l[i - (grado - 1) + k].lado_y;
                // Ejemplo de i - (grado - 1) + k: 
                // 4 - (5 - 1) + 0 = 0
                fprintf(stdout, "%u \t %u\n", g->v[j].nombre, g->v[j].vecinos[k]);
            }

            // Asigno el delta del grafo
            if(g->v[j].grado > g->delta){
                g->delta = g->v[j].grado;
            }

            j++;
            grado = 0;
        }
    }
}

/* FUNCIONES DE LA API */

Grafo ConstruirGrafo()
{
    u32 m, n;
    Lados l = cargar_lados(&m, &n);
    ordenar_lados(l, 2*m);
    Grafo G = inicializar_grafo(n, m);
    cargar_grafo(G, l);
    free(l);
    return G;
}

void DestruirGrafo(Grafo G)
{
    
    for (u32 i = 0; i < G->numero_vertices; i++)
    {
        free(G->v[i].vecinos);
    }
    free(G->v);
    free(G);
    
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
    return G->v[i].nombre;
}

u32 Grado(u32 i, Grafo G)
{
    return G->v[i].grado;
}

u32 IndiceVecino(u32 j, u32 i, Grafo G)
{
    return G->v[i].vecinos[j];
}
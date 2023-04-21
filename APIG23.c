#include "APIG23.h"
#include <stdbool.h>
#include <assert.h>

u32 *vecinos;

/* DEBUG */
void imprimir_vertices (Grafo g){
    printf("\nvertices:\n");
    for (u32 i = 0; i < g->numero_vertices; i++)
    {
        printf("Vertice: %u \t Grado: %u \n", g->v[i].nombre, g->v[i].grado);
    }
}
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
    Grafo G = (Grafo)calloc(1, sizeof(struct GrafoSt));

    G->numero_vertices = n;
    G->numero_lados = m;
    G->delta = 0;
    G->v = calloc(n, sizeof(struct VerticeSt));

    return (G);
}

int busqueda_binaria_vertices(u32 numero, Grafo g)
{
    assert(g != NULL);

    u32 inicio = 0;
    u32 medio = g->numero_vertices - 1;
    u32 fin = g->numero_vertices - 1;

    while (inicio <= fin)
    {
        medio = (inicio + fin) / 2;
        if (g->v[medio].nombre == numero)
        {
            return medio;
        }
        else if (g->v[medio].nombre < numero)
        {
            inicio = medio + 1;
        }
        else
        {
            fin = medio - 1;
        }
    }

    return -1;
}

void cargar_grafo(Grafo g, Lados l)
{
    assert(g != NULL);
    
    u32 tamaño_vecinos = (g->numero_lados * 2);
    vecinos = calloc(tamaño_vecinos, sizeof(u32));
    u32 grado = 0;
    u32 vertice_i = 0;
    int indice_vecino = -1;

    /* Completamos el arreglo global vecinos */
    for (u32 i = 0; i < tamaño_vecinos; i++)
    {
        vecinos[i] = l[i].lado_y; // guardo el nombre del vecino
        grado++;

        /* Si estoy en el último vecino de un vertice*/
        if (l[i].lado_x != l[i + 1].lado_x && i < tamaño_vecinos - 1)
        {
            /* Guardamos grado de cada vertice, Nombre, delta y PrimerVecino */
            g->v[vertice_i].nombre = l[i].lado_x;
            g->v[vertice_i].grado = grado;
            g->v[vertice_i].primerVecino = i - (grado - 1);

            /* Actualizamos delta */
            if (grado > g->delta)
            {
                g->delta = grado;
            }

            /* Reseteamos para siguiente tanda de vecinos */
            vertice_i++;
            grado = 0;
        }

        /* Guardo el ultimo vértice */
        else if (i == tamaño_vecinos - 1)
        {
            g->v[vertice_i].nombre = l[i].lado_x;
            g->v[vertice_i].grado = grado;
            g->v[vertice_i].primerVecino = i - (grado - 1);
        }
    }
    /* O(2m + n) */
    /* Tranformamos los nombre del arreglo vecino por sus correspondientes indices */
    for (u32 i = 0; i < tamaño_vecinos; i++)
    {
        indice_vecino = busqueda_binaria_vertices(vecinos[i], g);
        if (indice_vecino == -1)
        {
            printf("%u  %u  %u\n", i, vecinos[i], indice_vecino);
            fprintf(stderr, "Error de busqueda\n");
            exit(EXIT_FAILURE);
        }
        
        vecinos[i] = indice_vecino;
    }
    // O(2m log n)
}

/* FUNCIONES DE LA API */

Grafo ConstruirGrafo()
{
    u32 m, n;
    Lados l = cargar_lados(&m, &n);
    ordenar_lados(l, 2 * m);
    Grafo G = inicializar_grafo(n, m);
    cargar_grafo(G, l);
    free(l);
    return G;
}

void DestruirGrafo(Grafo G)
{
    free(vecinos);
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
    return vecinos[G->v[i].primerVecino + j];
}
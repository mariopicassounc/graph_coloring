#include "APIG23.h"
#include <stdbool.h>

u32 *vertices;
u32 *vecinos;

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
    Grafo G = (Grafo)calloc(1, sizeof(struct GrafoSt));

    G->numero_vertices = n;
    G->numero_lados = m;
    G->delta = 0;
    G->v = calloc(n, sizeof(struct VerticeSt));

    return (G);
}

int busqueda_b(u32 numero, u32 *arreglo, u32 n)
{
    u32 inicio = 0;
    u32 medio = n - 1;
    u32 fin = n - 1;

    while (inicio <= fin) {
        medio = (inicio + fin) / 2;
        if (arreglo[medio] == numero) {
            return medio;
        }
        else if (arreglo[medio] < numero) {
            inicio = medio + 1;
        }
        else {
            fin = medio - 1;
        }
    }

    return -1;
}

void cargar_grafo(Grafo g, Lados l)
{
    u32 tamaño_vecinos = (g->numero_lados * 2) + g->numero_vertices;
    vecinos = calloc(tamaño_vecinos, sizeof(u32));    
    vertices = calloc(g->numero_vertices, sizeof(u32));
    u32 grado = 0;
    u32 i = 0;
    u32 vertice_i = 0;
    u32 flag = 0;
    int result_busqueda = -1;

    /* Completamos el arreglo global vecinos */
    for(u32 index = 0; index < tamaño_vecinos; index++)
    {
        if(flag == 0)
        {
            vecinos[index] = l[i].lado_y;
            grado++;
        }

        /* Si estoy en el último vecino de un vertice*/
        if(l[i].lado_x != l[i+1].lado_x)
        {
            /* Generamos un espacio para el caso (x,x) */
            if(index == i + vertice_i + 1)
            {
                vecinos[index] = l[i].lado_x;

                /* Guardamos grado de cada vertice, Nombre, delta y PrimerVecino */
                g->v[vertice_i].nombre = l[i].lado_x;
                g->v[vertice_i].grado = grado;
                g->v[vertice_i].primerVecino = index - grado;

                /* Actualizamos delta */
                if(grado > g->delta)
                    g->delta = grado;

                /* reseteamos para siguiente tanda de vecinos */
                vertice_i++;
                grado = 0;
                flag = 0;
                i++;
            }
            else
                flag = 1;
        }
        else
            i++;
    }
    // O(2m + n)

    /* Completamos el arreglo global vertices */
    for(u32 i = 0; i < g->numero_vertices; i++)
        vertices[i] = vecinos[g->v[i].primerVecino+g->v[i].grado];
    // O(n)

    /*  Tranformamos los nombre del arreglo vecino por sus correspondientes
        indices  */
    for(u32 i=0; i < tamaño_vecinos; i++){
        result_busqueda = busqueda_b(vecinos[i], vertices, g->numero_vertices);
        if(result_busqueda == -1){
            fprintf(stderr, "Error de busqueda\n");
            exit(EXIT_FAILURE);
        }

        vecinos[i] = result_busqueda;
    }
    // O((2m + n) log n)
    
}
// Si m >= n & Dios quiere --> O(m log n)

/* FUNCIONES DE LA API */

Grafo ConstruirGrafo()
{
    u32 m, n;
    Lados l = cargar_lados(&m, &n);
    ordenar_lados(l, 2 * m);
    imprimir_lados(l, 2 * m);
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
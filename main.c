#include <assert.h>
#include "EstructuraGrafo23.h"
#include "APIG23.h"
#include "APIParte2.h"
#define NC (2 ^ 32) - 1

u32 *Color;
u32 *Orden;

void imprimirOrden(Grafo g)
{
    printf("Orden: ");
    for (u32 i = 0; i < NumeroDeVertices(g); i++)
        printf("%u %u \t", Orden[i], Color[Orden[i]]);
    printf("\n");
}

void imprimir_vecinos(Grafo g)
{
    assert(g != NULL);

    printf("\n\nindicesVecinos:\n");
    for (u32 i = 0; i < g->numero_vertices; i++)
    {
        printf("Vertice %u: ", g->v[i].nombre);
        for (u32 j = 0; j < g->v[i].grado; j++)
            printf("%u ", IndiceVecino(j, i, g));
        printf("\n");
    }
}

void imprimirColores(Grafo g)
{
    assert(Color != NULL && g != NULL);

    printf("\nColores:\n");
    for (u32 i = 0; i < NumeroDeVertices(g); i++)
    {
        printf("Vertice %u: %u\n", i, Color[i]);
    }
}

void OrdenNatural(u32 n)
{
    for (u32 i = 0; i < n; i++)
        Orden[i] = i;
}

// setea todos los colores como no coloreados, que para nosotros seria el valor (2^32-1)
void setearNoColoreado(u32 n)
{
    for (u32 i = 0; i < n; i++)
        Color[i] = NC;
}

int main()
{
    u32 numero_colores = 0;
    Grafo g = ConstruirGrafo();
    printf("Delta: %u\n", g->delta);

    Orden = calloc(g->numero_vertices, sizeof(u32));
    if (Orden == NULL)
    {
        printf("Error al reservar memoria para Orden\n");
        exit(1);
    }

    Color = calloc(g->numero_vertices, sizeof(u32));
    if (Color == NULL)
    {
        printf("Error al reservar memoria para Color\n");
        exit(1);
    }
 
    OrdenNatural(g->numero_vertices);
    /* Corro Greedy con orden natural */
    setearNoColoreado(g->numero_vertices);
    numero_colores = Greedy(g, Orden, Color);

    printf("\nNumero de colores: %u\n", numero_colores);
    
    OrdenImparPar(g->numero_vertices, Orden, Color);

    /* Corro Greedy con orden natural */
    numero_colores = Greedy(g, Orden, Color);
    printf("\nNumero de colores: %u\n", numero_colores);
  
    OrdenImparPar(g->numero_vertices, Orden, Color);

    /* Corro Greedy con orden natural */
    numero_colores = Greedy(g, Orden, Color);
    printf("\nNumero de colores: %u\n", numero_colores);
   
    DestruirGrafo(g);
    return 0;
}
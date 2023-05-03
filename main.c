#include <assert.h>
#include "EstructuraGrafo23.h"
#include "APIG23.h"
#include "APIParte2.h"
#define NC (2 ^ 32) - 1

u32 *Color;
u32 *Orden;

void imprimirOrden(u32 *Orden, Grafo g)
{
    printf("Orden: ");
    for (u32 i = 0; i < NumeroDeVertices(g); i++)
        printf("%u ", Orden[i]);
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

void imprimirColores(u32 *Color, Grafo g)
{
    assert(Color != NULL && g != NULL);

    printf("\nColores:\n");
    for (u32 i = 0; i < NumeroDeVertices(g); i++)
    {
        printf("Vertice %u: %u\n", i, Color[i]);
    }
}

void OrdenNatural(u32 n, u32 *Orden)
{
    for (u32 i = 0; i < n; i++)
        Orden[i] = i;
}

// setea todos los colores como no coloreados, que para nosotros seria el valor (2^32-1)
void setearNoColoreado(u32 n, u32 Color[])
{
    for (u32 i = 0; i < n; i++)
        Color[i] = NC;
}

int main()
{
    u32 numero_colores = 0;
    Grafo g = ConstruirGrafo();
    printf("Delta: %u\n", g->delta);
    imprimir_vecinos(g);

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

    
    OrdenNatural(g->numero_vertices, Orden);
    
    /* Corro Greedy con orden natural */
    setearNoColoreado(g->numero_vertices, Color);
    numero_colores = Greedy(g, Orden, Color);
    printf("\nNumero de colores: %u\n", numero_colores);
    imprimirColores(Color, g);

    // numero_colores = Greedy(g, Orden, Color);

    // printf("\nNumero de colores: %u\n", numero_colores);

    // imprimirColores(Color, g);
    /*
    rellenar Orden[] con Orden natural Orden[0] = 0
    correr greedy en orden natural
    reordenar Orden[] con ImparPar
    Correr greedy en el nuevo orden
    reordenar Orden[] con Jedi
    Correr greedy en el nuevo orden
    */
    DestruirGrafo(g);
    return 0;
}
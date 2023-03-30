#ifndef EGRAFO_H
#define EGRAFO_H

typedef unsigned int u32;

typedef struct VerticeSt *Vertices;

typedef struct GrafoSt * Grafo;

typedef struct LadoSt *Lados;


struct VerticeSt {
    u32 nombre;
    u32 color;
    u32 grado;
    u32 *vecinos;
};

struct GrafoSt {
    u32 delta;
    u32 numero_lados;
    u32 numero_vertices;
    Vertices v;
    u32 *orden;
};

struct LadoSt {
    u32 lado_x;
    u32 lado_y;
};

Lados cargar_lados();
void imprimir_lados();

#endif
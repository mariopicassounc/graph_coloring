#ifndef EGRAFO_H
#define EGRAFO_H

typedef unsigned int u32;

/* Renombres */
typedef struct VerticeSt VerticeSt;
typedef struct LadoSt LadoSt;
typedef struct GrafoSt GrafoSt;

/* Arrays */
typedef VerticeSt *Vertices;
typedef LadoSt *Lados;

/* Estructuras */
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
};

struct LadoSt {
    u32 lado_x;
    u32 lado_y;
};


#endif
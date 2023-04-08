#include "EstructuraGrafo23.h"
#include "APIG23.h"

int main()
{
    Grafo g = ConstruirGrafo();
    DestruirGrafo(g);
    return 0;
}

/*
void print_help(char *program_name)
{
    printf("Usage: %s <input file path>\n\n"
            "\n"
            "El input debe de seguir el siguiente formato:\n"
            " * La primera linea debe de contener un primer caracter igual a "
            "'c'\n\n"
            " * La segunda linea debe empezar con el caracter 'p', seguido de"
            " la palabra 'edge' y acontinuación dos valores que indican la "
            "cantidad de vertices y lados\n\n"
            " * Las siguientes linea debe empezar con el caracter 'e', seguido"
            " de dos valores que indican la realación entre un vertice y otro"
            "\n\n"
            "En otras palabras, el formato del archivo es:\n"
            "c <p-value>\n"
            "p edge <amount nodes> <amount edges> \n"
            "e <node i> <node j>\n...\n\n",
            program_name);
}
*/
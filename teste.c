#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

  FILE *fs = fopen(argv[1], "r");
  grafo g = le_grafo(fs);
  fclose(fs);

  if ( !g )

    return 1;

  printf("nome: %s\n", nome_grafo(g));
  printf("%sdirecionado\n", direcionado(g) ? "" : "não ");
  printf("%sponderado\n", ponderado(g) ? "" : "não ");
  printf("%d vértices\n", numero_vertices(g));
  printf("%d arestas\n", numero_arestas(g));

  escreve_grafo(stdout, g);

  return ! destroi_grafo(g);
}

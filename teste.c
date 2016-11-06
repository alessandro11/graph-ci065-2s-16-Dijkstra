#include <stdio.h>
#include "grafo.h"

#define PATH_FILE_NAME		"../graph-examples/2vg1.dot"
//------------------------------------------------------------------------------

int main(void) {

  FILE fs = fopen(PATH_FILE_NAME, "r");
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

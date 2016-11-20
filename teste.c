#include <stdio.h>
#include <malloc.h>
#include "grafo.h"

typedef unsigned int uint;
typedef long int lint;
typedef struct no *no;
typedef struct vertice* heap;

typedef enum __Estado {
	NaoVisitado = 0,
	Visitado
}Estado;

typedef struct lista *lista;
struct lista {

  unsigned int tamanho;
  int padding; // só pra evitar warning
  no primeiro;
};

typedef struct grafo *grafo;
struct grafo {
	uint	nvertices;
	uint	narestas;
	int 	direcionado;
	int 	ponderado;
	lint	diametro;
	char*	nome;
	lista	vertices;
};


int destroi_vertice(void *v);
vertice busca_vertice(const char*, lista);

#ifdef DEBUG

#define UNUSED(x)				(void)(x)

void print_a(vertice, lista);
void print_v(grafo);
void print_vattr(grafo);
void print_vbylista(lista);
void print_heap(heap*);
void print_mat(lista**, grafo);
void print_mat_dist(lint**, grafo);

#else

#define print_a(vertice, lista)		(void)0
#define print_v(grafo)				(void)0
#define print_vattr(grafo)			(void)0
#define print_vbylista(lista)		(void)0
#define print_heap(heap)			(void)0
#define print_mat(lista, grafo)		(void)0
#define print_mat_dist(dist_m, grafo)(void)0

#endif /* DEBUG */


//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {

  FILE *fs = fopen(argv[1], "r");
  grafo g = le_grafo(fs);
  fclose(fs);

//  if( !g )
//    return 1;


	//https://www.youtube.com/watch?v=gdmfOwyQlcI
	//binary heap
	//http://www.cs.princeton.edu/~wayne/cs423/lectures/heaps-4up.pdf

    vertice u, v;
    lista T, **T2;

    uint i, j;

	fprintf(stderr, "%d\n", direcionado(g));
	fprintf(stderr, "%ld\n", diametro(g));

    u = busca_vertice("A", g->vertices);
    v = busca_vertice("G", g->vertices);
	fprintf(stderr, "%ld\n", distancia(u, v, g));

    T = caminho_minimo(u, v, g);
    print_vbylista(T);
    destroi_lista(T, NULL);

	T2 = (lista**)calloc(g->nvertices, sizeof(lista**));
	lista **p = T2;
	for( no n=primeiro_no(g->vertices); n; n=proximo_no(n) )
		*p++ = (lista*)calloc(g->nvertices, sizeof(lista*));

	caminhos_minimos(T2, g);
	print_mat(T2, g);

	p = T2;
	for( i=0; i < g->nvertices; i++  ) {
		for( j=0; j< g->nvertices; j++ ) {
			destroi_lista(T2[i][j], destroi_vertice);
		}
		free(*p++);
	}
	free(T2);

	lint **dist = (lint**)calloc(g->nvertices, sizeof(lint**));
	for( i=0; i < g->nvertices; i++ ) {
		dist[i] = (lint*)calloc(g->nvertices, sizeof(lint*));
	}

	distancias(dist, g);
	print_mat_dist(dist, g);

	for( i=0; i < g->nvertices; i++ ) {
		free(dist[i]);
	}
	free(dist);



  printf("nome: %s\n", nome_grafo(g));
  printf("%sdirecionado\n", direcionado(g) ? "" : "não ");
  printf("%sponderado\n", ponderado(g) ? "" : "não ");
  printf("%d vértices\n", numero_vertices(g));
  printf("%d arestas\n", numero_arestas(g));

  escreve_grafo(stdout, g);

  return ! destroi_grafo(g);
}

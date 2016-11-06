/*
 * debug.c
 *
 *  Created on: Nov 6, 2016
 *      Author: m3cool
 */

#include <stdio.h>

/*
 * DO NOT DECLARE IN THE HEADER SINCE NEEDS TO KNOW TYPES vertices, lista and
 * grafo. DO NOT DEVCLARE THIS TYPES IN debug.h, AFTER ALL THIS ARE DUPLICATED
 * CODE AND WILL RISE REDECLARATION BY THE COMPILER.
 */
typedef unsigned int uint;
typedef long int lint;
typedef struct no *no;
typedef struct lista *lista;

struct grafo {
	uint 	nvertices;
	uint 	narestas;
	int		direcionado;
	int		ponderado;
	char*	nome;
	lista	vertices;
};
typedef struct grafo *grafo;

struct vertice {
	char*	nome;
	lista	vizinhos_esq;
	lista	vizinhos_dir;
};
typedef struct vertice* vertice;

struct aresta {
	int		ponderado;
	int		padding;
	lint	peso;
	vertice	destino;
};
typedef struct aresta* aresta;

no primeiro_no(lista l);
no proximo_no(no n);
void *conteudo(no n);

/*
 * DO NOT DECLARE IN THE HEADER SINCE NEEDS TO KNOW TYPES vertices, lista and
 * grafo. DO NOT DEVCLARE THIS TYPES IN debug.h, AFTER ALL THIS ARE DUPLICATED
 * CODE AND WILL RISE REDECLARATION BY THE COMPILER.
 */
void print_a(vertice, lista);
void print_v(grafo g);

/*____________________________________________________________________________*/
void print_v(grafo g) {
        no n;
        struct vertice* v;
        lista l = g->vertices;

        printf("Grafo %s=%p\n", g->nome, g);
        for( n=primeiro_no(l); n; n=proximo_no(n) ) {
                v = conteudo(n);
                printf("%s=%p\n", v->nome, v);
                printf("\tV.:\n");
                print_a(v, v->vizinhos_esq);
        }
        fflush(stdout);
}

void print_a(vertice v, lista l) {
    no n;
    struct aresta* a;

    n=primeiro_no(l);
    if( n ) {
        a = conteudo(n);
        printf("\taresta=%p\n", a);
        printf( "\t(%s=%p, %s=%p)\n", v->nome, v, a->destino->nome, a->destino);
        for( n=proximo_no(n); n; n=proximo_no(n) ) {
            a = conteudo(n);
            printf("\taresta=%p\n", a);
            printf( "\t(%s=%p, %s=%p)\n", v->nome, v, a->destino->nome, a->destino);
        }
    }
    fflush(stdout);
}

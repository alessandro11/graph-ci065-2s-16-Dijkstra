/*
 * =====================================================================================
 *
 *       Filename:  grafo.c

 *
 *    Description:  Trabalho1 de grafos Professor Renato.
 *
 *        Version:  1.0
 *        Created:  08/27/2016 07:15:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alessandro Elias, GRR20110589, ae11@inf.ufpr.br
 *
 * =====================================================================================
 */

#include <malloc.h>
#include "grafo.h"

//-----------------------------------------------------------------------------
// (apontador para) lista encadeada
typedef struct lista *lista;

//-----------------------------------------------------------------------------
// (apontador para) nó da lista encadeada cujo conteúdo é um void *
typedef struct no *no;

//------------------------------------------------------------------------------
// devolve o número de nós da lista l
unsigned int tamanho_lista(lista l);

//------------------------------------------------------------------------------
// devolve o primeiro nó da lista l,
//      ou NULL, se l é vazia
no primeiro_no(lista l);

//------------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da lista
no proximo_no(no n);

//------------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n = NULL
void *conteudo(no n);

//------------------------------------------------------------------------------
// insere um novo nó na lista l cujo conteúdo é p
//
// devolve o no recém-criado
//      ou NULL em caso de falha
no insere_lista(void *conteudo, lista l);

//------------------------------------------------------------------------------
// cria uma lista vazia e a devolve
//
// devolve NULL em caso de falha
lista constroi_lista(void);
//------------------------------------------------------------------------------
// desaloca a lista l e todos os seus nós
//
// se destroi != NULL invoca
//
//     destroi(conteudo(n))
//
// para cada nó n da lista.
//
// devolve 1 em caso de sucesso,
//      ou 0 em caso de falha
int destroi_lista(lista l, int destroi(void *));

//------------------------------------------------------------------------------
// remove o no de endereço rno de l
// se destroi != NULL, executa destroi(conteudo(rno))
// devolve 1, em caso de sucesso
//         0, se rno não for um no de l
int remove_no(struct lista *l, struct no *rno, int destroi(void *));
//______________________________________________________________________________




/*
 * Meus includes
 */
#include <graphviz/cgraph.h>
#define _XOPEN_SOURCE
#include <string.h>
#include <stdlib.h>

typedef unsigned int uint;
typedef long int lint;
typedef unsigned short ushort;
typedef int bool;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

#ifndef NULL
#define NULL			0
#endif

#define ERRO(fmt, ...)      	(fprintf(stderr, (fmt), ## __VA_ARGS__))
#define UNUSED(x)				(void)(x)


struct grafo {
	uint 	nvertices;
	uint 	narestas;
	int		direcionado;
	int		ponderado;
	char*	nome;
	lista	vertices;
};

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

int 	destroi_vertice(void*);


//------------------------------------------------------------------------------
// devolve o nome do grafo g
char *nome_grafo(grafo g) {
	return g->nome;
}

//------------------------------------------------------------------------------
// devolve o nome do vertice v
char *nome_vertice(vertice v) {
	return v->nome;
}

//------------------------------------------------------------------------------
// devolve 1, se g tem pesos nas arestas/arcos,
//      ou 0, caso contrário
int ponderado(grafo g) {
	return g->ponderado;
}

//------------------------------------------------------------------------------
// devolve 1, se g tem pesos nas arestas/arcos,
//      ou 0, caso contrário
int direcionado(grafo g) {
	return g->direcionado;
}

//------------------------------------------------------------------------------
// devolve o número de vértices do grafo g
unsigned int numero_vertices(grafo g) {
	return g->nvertices;
}

//------------------------------------------------------------------------------
// devolve o número de arestas/arcos do grafo g
unsigned int numero_arestas(grafo g) {
	return g->narestas;
}

//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot.
//
// o peso das arestas/arcos (quando houver) é escrito como o atributo
// de nome "peso"
//
// devolve o grafo escrito
//      ou NULL em caso de erro
grafo escreve_grafo(FILE *output, grafo g) {
    vertice v;
    aresta  a;
    char    ch;
    no      n, na;

    if( !g ) return NULL;
    fprintf( output, "strict %sgraph \"%s\" {\n\n", \
            direcionado(g) ? "di" : "", g->nome );

    for( n=primeiro_no(g->vertices); n; n=proximo_no(n) )
        fprintf(output, "    \"%s\"\n", ((vertice)conteudo(n))->nome);
    fprintf( output, "\n" );

    ch = direcionado(g) ? '>' : '-';
    for( n=primeiro_no(g->vertices); n; n=proximo_no(n) ) {
        v = (vertice)conteudo(n);
        for( na=primeiro_no(v->vizinhos_esq); na; na=proximo_no(na) ) {
            a = (aresta)conteudo(na);
            fprintf(output, "    \"%s\" -%c \"%s\"", v->nome, ch, a->destino->nome);

            if ( g->ponderado )
                fprintf( output, " [peso=%ld]", a->peso );
            fprintf( output, "\n" );
        }
    }
    fprintf( output, "}\n" );

    return g;
}

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
//
// devolve 1 em caso de sucesso,
//      ou 0 caso contrário
int destroi_grafo(grafo g) {
	int ret = 1;

	free(g->nome);
	ret = destroi_lista(g->vertices, destroi_vertice);
	free(g);

	return ret;
}

int destroi_vertice(void *v) {
	int ret = 1;

	free( ((vertice)v)->nome );
	ret = destroi_lista( ((vertice)v)->vizinhos_esq, NULL );
	free(v);

	return ret;
}

//______________________________________________________________________________
//
// Definição das funções que trata a lista.
//
// nó de lista encadeada cujo conteúdo é um void *
struct no {

  void*	conteudo;
  no	anterior;
  no 	proximo;
};

//---------------------------------------------------------------------------
// lista encadeada
struct lista {

  unsigned int tamanho;
  int padding; // só pra evitar warning
  no primeiro;
};

//---------------------------------------------------------------------------
// devolve o número de nós da lista l
unsigned int tamanho_lista(lista l) { return l->tamanho; }

//---------------------------------------------------------------------------
// devolve o primeiro nó da lista l,
//      ou NULL, se l é vazia
no primeiro_no(lista l) { return l->primeiro; }

//---------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n = NULL
void *conteudo(no n) { return n->conteudo; }

//---------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da lista
no proximo_no(no n) { return n->proximo; }

//---------------------------------------------------------------------------
// cria uma lista vazia e a devolve
//
// devolve NULL em caso de falha
lista constroi_lista(void) {

  lista l = malloc(sizeof(struct lista));

  if ( ! l )
    return NULL;

  l->primeiro = NULL;
  l->tamanho = 0;

  return l;
}

//---------------------------------------------------------------------------
// desaloca a lista l e todos os seus nós
//
// se destroi != NULL invoca
//
//     destroi(conteudo(n))
//
// para cada nó n da lista.
//
// devolve 1 em caso de sucesso,
//      ou 0 em caso de falha
int destroi_lista(lista l, int destroi(void *)) {

  no p;
  int ok=1;

  while ( (p = primeiro_no(l)) ) {

    l->primeiro = proximo_no(p);

    if ( destroi )
      ok &= destroi(conteudo(p));

    free(p);
  }

  free(l);

  return ok;
}

//---------------------------------------------------------------------------
// insere um novo nó na lista l cujo conteúdo é p
//
// devolve o no recém-criado
//      ou NULL em caso de falha
no insere_lista(void *conteudo, lista l) {

  no novo = malloc(sizeof(struct no));

  if ( ! novo )
    return NULL;

  novo->conteudo = conteudo;
  novo->proximo = primeiro_no(l);
  ++l->tamanho;

  return l->primeiro = novo;
}

//------------------------------------------------------------------------------
// remove o no de endereço rno de l
// se destroi != NULL, executa destroi(conteudo(rno))
// devolve 1, em caso de sucesso
//         0, se rno não for um no de l
int remove_no(struct lista *l, struct no *rno, int destroi(void *)) {
	int r = 1;
	if (l->primeiro == rno) {
		l->primeiro = rno->proximo;
		if (destroi != NULL) {
			r = destroi(conteudo(rno));
		}
		free(rno);
		l->tamanho--;
		return r;
	}
	for (no n = primeiro_no(l); n->proximo; n = proximo_no(n)) {
		if (n->proximo == rno) {
			n->proximo = rno->proximo;
			if (destroi != NULL) {
				r = destroi(conteudo(rno));
			}
			free(rno);
			l->tamanho--;
			return r;
		}
	}
	return 0;
}

/*
 *******************************************************************************
 * Meu código começa aqui.
 *******************************************************************************
 */
/*
 * Protótipos
 */
void 	guarda_arcos(Agraph_t*, Agnode_t*, grafo);
void 	guarda_arestas(Agraph_t*, Agnode_t*, grafo, vertice);
void 	constroi_grafo(Agraph_t*, grafo);
grafo	alloc_grafo(void);
vertice alloc_vertice(const char*);
aresta 	alloc_aresta(void);
aresta 	dup_aresta(aresta);
char* 	str_dup(const char*);
vertice busca_vertice(const char*, lista);

void print_a(vertice, lista);
void print_v(grafo g);
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

grafo alloc_grafo(void) {
	grafo g = (grafo)calloc(1, sizeof(struct grafo));
	return g;
}

vertice alloc_vertice(const char* nome) {
	vertice v = (vertice)calloc(1, sizeof(struct vertice));
	if( v ) {
		v->nome = str_dup(nome);
		v->vizinhos_esq = constroi_lista();
//		v->vizinhos_dir = constroi_lista();
	}

	return v;
}

aresta alloc_aresta(void) {
	aresta a = (aresta)calloc(1, sizeof(struct aresta));
//	if( a ) {
//		a->nome = str_dup(nome);
//		a->peso = 0L;
//	}
	return a;
}

aresta dup_aresta(aresta a) {
	aresta p = (aresta)malloc(sizeof(struct aresta));
	if( p ) {
		memcpy(p, a, sizeof(struct aresta));
	}

	return p;
}

char* str_dup(const char* str) {
	char* p = (char*)calloc(1, strlen(str)+1);
	if( p )
		strcpy(p, str);

	return p;
}
//------------------------------------------------------------------------------
// lê um grafo no formato dot de input, usando as rotinas de libcgraph
//
// desconsidera todos os atributos do grafo lido exceto o atributo
// "peso" quando ocorrer; neste caso o valor do atributo é o peso da
// aresta/arco que é um long int
//
// num grafo com pesos todas as arestas/arcos tem peso
//
// o peso default é 0
//
// todas as estruturas de dados alocadas pela libcgraph são
// desalocadas ao final da execução
//
// devolve o grafo lido
//      ou NULL em caso de erro
grafo le_grafo(FILE *input) {
    Agraph_t*	Ag_g;
    grafo       g;

    g = (grafo)alloc_grafo();
    Ag_g = agread(input, NULL);
    if ( !Ag_g ) {
        free(g);
        ERRO("Não foi possível ler o grado!\n");
        return NULL;
    }

    g->nome = str_dup(agnameof(Ag_g));
    g->direcionado = agisdirected(Ag_g);
    g->nvertices= (uint)agnnodes(Ag_g);
    g->narestas = (uint)agnedges(Ag_g);
    g->vertices = constroi_lista();
    constroi_grafo(Ag_g, g);

    agclose(Ag_g);
    return g;
}

vertice busca_vertice(const char* nome, lista l) {
	no n;
	vertice v;

	for( n=primeiro_no(l); n; n=proximo_no(n) ) {
		v = (vertice)conteudo(n);
		if( strcmp(nome, v->nome) == 0 )
			return v;
	}

	return NULL;
}

void guarda_arcos(Agraph_t* ag, Agnode_t* av, grafo g) {
	UNUSED(ag); UNUSED(av); UNUSED(g);
}

void guarda_arestas(Agraph_t* ag, Agnode_t* agn, grafo g, vertice v) {
	Agedge_t 	*age;
    aresta		a;
    vertice 	cabeca;
    void		*tail, *head;
    char*		peso;
    char		str_peso[5] = "peso";

	for( age=agfstedge(ag, agn); age; age=agnxtedge(ag, age, agn) ) {
		tail = agtail(age);
		head = aghead(age);
		fprintf(stderr, "v=%s\n", agnameof(agn));
		fprintf(stderr, "t=%s\n", agnameof(tail));
		fprintf(stderr, "h=%s\n\n", agnameof(head));

		if( (Agnode_t*)head == agn )
			cabeca = busca_vertice(agnameof(tail), g->vertices);
		else
			cabeca = busca_vertice(agnameof(head), g->vertices);

		a = alloc_aresta();
		peso = agget(age, str_peso);
		if( peso ) {
			a->peso = atol(peso);
			a->ponderado = TRUE;
		}

		a->destino = cabeca;
		insere_lista(a, v->vizinhos_esq);
	}
}

void constroi_grafo(Agraph_t* ag, grafo g) {
	Agnode_t*	agn;
	vertice		v;

	// Armazene a lista de vértices; deste modo podemos
	// apenas apontar as arestas para os respectivos vértices.
	for( agn=agfstnode(ag); agn; agn=agnxtnode(ag, agn) ) {
		v = alloc_vertice(agnameof(agn));
		insere_lista(v, g->vertices);
	}

	for( agn=agfstnode(ag); agn; agn=agnxtnode(ag, agn) ) {
		if( g->direcionado )
			guarda_arcos(ag, agn, g);
		else
			guarda_arestas( ag, agn, g, busca_vertice(agnameof(agn), g->vertices) );

		print_v(g);
	}
}

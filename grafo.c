/*
 * =====================================================================================
 *
 *       Filename:  grafo.c
 *
 *    Description:  Trabalho2; Professor Renato.
 *    				O objetivo deste trabalho é implementar rotinas que computam
 *    				caminhos mínimos e distâncias entre vértices de um grafo,
 *    				usando o Algoritmo de Dijkstra.
 *
 *        Version:  1.0
 *        Created:  11/06/2016 09:15:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alessandro Elias, GRR20110589, ae11@c3sl.ufpr.br
 *
 * =====================================================================================
 */

#include <malloc.h>
#include <stdio.h>
#include <limits.h>

//------------------------------------------------------------------------------
// o valor representando "infinito"

const long int infinito = LONG_MAX;

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
//      ou NULL se n == NULL

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

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
//
// o grafo pode ser
// - direcionado ou não
// - com pesos nas arestas/arcos ou não
//
// além dos vértices e arestas, o grafo tem um nome, que é uma "string"
//
// num grafo com pesos todas as arestas/arcos tem peso, que é um long int
//
// o peso default é 0

typedef struct grafo *grafo;

//------------------------------------------------------------------------------
// devolve o nome do grafo g

char *nome_grafo(grafo g);

//------------------------------------------------------------------------------
// devolve 1, se g é direcionado,
//      ou 0, caso contrário

int direcionado(grafo g);

//------------------------------------------------------------------------------
// devolve 1, se g tem pesos nas arestas/arcos,
//      ou 0, caso contrário

int ponderado(grafo g);

//------------------------------------------------------------------------------
// devolve o número de vértices do grafo g

unsigned int numero_vertices(grafo g);

//------------------------------------------------------------------------------
// devolve o número de arestas/arcos do grafo g

unsigned int numero_arestas(grafo g);

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados que representa um vértice do grafo
//
// cada vértice tem um nome que é uma "string"

typedef struct vertice *vertice;

//------------------------------------------------------------------------------
// devolve o nome do vertice v

char *nome_vertice(vertice v);

//------------------------------------------------------------------------------
// devolve o vertice de nome s no grafo g,
//      ou NULL caso não exista em g um vertice de nome s

vertice vertice_nome(char *s, grafo g);

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

grafo le_grafo(FILE *input);

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
//
// devolve 1 em caso de sucesso,
//      ou 0 caso contrário

int destroi_grafo(grafo g);

//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot.
//
// o peso das arestas/arcos (quando houver) é escrito como o atributo
// de nome "peso"
//
// devolve o grafo escrito
//      ou NULL em caso de erro

grafo escreve_grafo(FILE *output, grafo g);

//------------------------------------------------------------------------------
// devolve o grau de v no grafo g, se não é direcionado ou se direcao == 0,
//      ou o grau de entrada de v no grafo g, se direcao == -1,
//      ou o grau de saída de v no grafo g, se direcao == +1

unsigned int grau(vertice v, int direcao, grafo g);

//------------------------------------------------------------------------------
// devolve uma lista de vértices de g representando o caminho mínimo
// de u a v em g
//
// a lista é vazia se u e v estão em componentes diferentes de g

lista caminho_minimo(vertice u, vertice v, grafo g);

//------------------------------------------------------------------------------
// devolve a distância de u a v em g

long int distancia(vertice u, vertice v, grafo g);

//------------------------------------------------------------------------------
// devolve um número entre 0 e numero_vertices(g)
//
// este número é único e distinto para cada vértice de g e deve servir
// para indexar vetores e matrizes a partir dos vértices de g

unsigned int indice(vertice v, grafo g);

//------------------------------------------------------------------------------
// preenche a matriz d com as distâncias entre os vértices de g de
// maneira que d[indice(u,g)][indice(v,g)] tenha o valor da distância
// entre os vértices u e v em g
//
// devolve d

long int **distancias(unsigned int **d, grafo g);

//------------------------------------------------------------------------------
// preenche a matriz c com caminhos mínimos entre os vértices de g de
// maneira que c[indice(u,g)][indice(v,g)] tenha um caminho mínimo
// entre os vértices u e v em g
//
// devolve d

lista **caminhos_minimos(lista **c, grafo g);

//------------------------------------------------------------------------------
// devolve o diâmetro de g

long int diametro(grafo g);

//______________________________________________________________________________


/***
 * Meus includes.
 ******************************************************************************/
#include <graphviz/cgraph.h>
//#define _XOPEN_SOURCE
#include <string.h>
#include <stdlib.h>

#define UNUSED(x)				(void)(x)

#ifdef DEBUG

#define UNUSED(x)				(void)(x)

void print_a(vertice, lista);
void print_v(grafo g);
void print_vattr(grafo g);

#else

#define print_a(vertice, lista)		(void)0
#define print_v(grafo)				(void)0
#define print_vattr(grafo)			(void)0

#endif /* DEBUG */



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

typedef enum __Estado {
	NaoVisitado = 0,
	Visitado
}Estado;

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
	lint	id;
	lint	distancia;
	Estado	estado;
	lista	vizinhos_dir;
};
typedef struct vertice* vertice;

struct aresta {
	int		ponderado;
	lint	peso;
	vertice origem;
	vertice	destino;
};
typedef struct aresta* aresta;

int 	destroi_vertice(void*);
int 	destroi_aresta(void*);

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

            if ( a->ponderado )
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
	ret = destroi_lista( ((vertice)v)->vizinhos_esq, destroi_aresta );
	free(v);

	return ret;
}

int destroi_aresta(void *a) {
    free(a);
    return 1;
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
        ERRO("Não foi possível ler o grafo!\n");
        return NULL;
    }

    g->nome = str_dup(agnameof(Ag_g));
    g->direcionado = agisdirected(Ag_g);
    g->nvertices= (uint)agnnodes(Ag_g);
    g->narestas = (uint)agnedges(Ag_g);
    g->vertices = constroi_lista();
    constroi_grafo(Ag_g, g);

    agclose(Ag_g);

    vertice u, v;
    u = busca_vertice("a", g->vertices);
    v = busca_vertice("e", g->vertices);
    caminho_minimo(u, v, g);

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

typedef struct __vertices {
	vertice origem, destino;
}vertices;
vertices busca_vertices(const char* nome_orig, const char* nome_dest, lista l) {
	no n;
	vertice v;
	vertices vs = {NULL};

	for( n=primeiro_no(l); n && (vs.origem == NULL || vs.destino == NULL); n=proximo_no(n) ) {
		v = (vertice)conteudo(n);
		if( strcmp(nome_orig, v->nome) == 0 )
			vs.origem = v;
		else if( strcmp(nome_dest, v->nome) == 0 )
			vs.destino = v;
	}

	return vs;
}

void guarda_arcos(Agraph_t* ag, Agnode_t* av, grafo g) {
	UNUSED(ag); UNUSED(av); UNUSED(g);
}

void guarda_arestas(Agraph_t* ag, Agnode_t* agn, grafo g, vertice v) {
	Agedge_t 	*age;
    aresta		a;
    vertice 	vtmp;
    void		*tail, *head;
    char*		peso;
    char		str_peso[5] = "peso";
    vertices	vs;

	for( age=agfstedge(ag, agn); age; age=agnxtedge(ag, age, agn) ) {
		tail = agtail(age);
		head = aghead(age);

		vs = busca_vertices(agnameof(tail), agnameof(head), g->vertices);
		if( (Agnode_t*)head == agn ) {
			vtmp = vs.origem;
			vs.origem = vs.destino;
			vs.destino = vtmp;
		}

		a = alloc_aresta();
		peso = agget(age, str_peso);
		if( peso ) {
			a->peso = atol(peso);
			a->ponderado = TRUE;
		}

		a->origem = vs.origem;
		a->destino = vs.destino;
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

		// funciona somente em modo de DEBUG, link com debug.c
		print_v(g);
	}
}

//------------------------------------------------------------------------------
// devolve uma lista de vértices de g representando o caminho mínimo
// de u a v em g
//
// a lista é vazia se u e v estão em componentes diferentes de g

lista caminho_minimo(vertice u, vertice v, grafo g) {
	no n;
	aresta a;
	vertice vcurr;
	lint dist, dist_min;

	lista T = constroi_lista();
	insere_lista(u, T);

	for( n=primeiro_no(g->vertices); n; n=proximo_no(n) ) {
		vcurr = conteudo(n);
		vcurr->estado = NaoVisitado;
		vcurr->distancia = infinito;
	}
	u->estado = Visitado;
	u->distancia = 0;

	print_vattr(g);

	vcurr = conteudo(primeiro_no(T));
	while( vcurr ) {
		dist_min = LONG_MAX;
		for( n=primeiro_no(vcurr->vizinhos_esq); n; n=proximo_no(n) ) {
			a = conteudo(n);
			dist = a->origem->distancia + a->peso;
			if( dist < a->destino->distancia )
				a->destino->distancia = dist;
			if( dist < dist_min ) dist_min = dist;
		}
	}

	return T;
}

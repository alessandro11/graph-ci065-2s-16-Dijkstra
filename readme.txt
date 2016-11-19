lista **caminhos_minimos(lista **c, grafo g);
	Nesta função preenchi a matriz com uma lista que representa os caminhos
mínimos, porém toda operação feita pela função Dijkstra é feita sempre nos mesmos
vértices (mesmos ponteiros), logo para cada vértice (c[indice(u,g)][indice(v,g)])
da lista ele é duplicado para armazenar a lista de vértices que representa o
caminho mínimo. Os únicos membros válidos de cada vértice destes caminhos são:
	id - identificador único de cada vértice, também indica a posição i ou j na
		 matriz.
	distancia - distância da raiz u até o vértice v.
	



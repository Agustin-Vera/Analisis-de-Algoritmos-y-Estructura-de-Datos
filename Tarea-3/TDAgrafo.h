#include <stdio.h>
#include <stdlib.h>
#include "TDAlista.h"

#define INFINITO 123456789  //o podria ser un numero mayor a la suma de todas las aristas

/*------------- estructura de datos -------------*/

typedef struct matrizGrafo {
	int cvertices;
	int** adyacencias;
} TDAgrafo;

typedef struct tda_dijkstra {
	int vertices;
	int verticeInicial;
	int *distancia;
	int *padre;
	int *visitados;
} TDAdijkstraResultado;


/*----------------- operaciones -----------------*/

TDAgrafo* crearGrafoVacio(int vertices)
{
	TDAgrafo* grafo = (TDAgrafo*)malloc(sizeof(TDAgrafo));
	grafo->cvertices = vertices;
	grafo->adyacencias = (int**)malloc(vertices * sizeof(int*));
	int i,j;

	for (i = 0; i < vertices; i++) {
		grafo->adyacencias[i] = (int*)malloc(vertices * sizeof(int));
		//Inicializa en cero
		for(j=0;j<vertices;j++){
			grafo->adyacencias[i][j] = 0;
		}
	}

	return grafo;
}

void imprimirMatrizAdyacencia(TDAgrafo* grafo)
{
	int i, j;

	for (i = 0; i < grafo->cvertices; i++) {
		for (j = 0; j < grafo->cvertices; j++) {
			printf("%2d ", grafo->adyacencias[i][j]);
		}
		printf("\n");
	}
}

int adyacenciaNodos(TDAgrafo * grafo, int vertA, int vertB)
{
	return grafo->adyacencias[vertA][vertB];
}


/* Crea un grafo no dirigido ponderado a partir de datos de un archivo.
 * @param {char*} nombreArchivo     String que contiene el nombre del archivo.
 * @return {TDAgrafo*}              TDA del grafo. */
TDAgrafo* leerGrafoDirigidoPonderado(char *nombreArchivo, int*entrada, int*salida){
	//para abrir archivo
	FILE*pf;
	pf = fopen(nombreArchivo, "r");
	int n_vertices, m_aristas;
	int i, j, k, p, e, s;
	if (pf == NULL){
		return NULL;
	}
	else{
		//Cantidad de nodos y aristas
		fscanf(pf, "%d %d", &n_vertices, &m_aristas);
		//Posiciones de entrada y salida (se les resta 1 porque es el formato que se utilizo en las experiencias de laboratorio)
		fscanf(pf, "%d %d", &e, &s);
		*entrada = e-1;
		*salida = s-1;
		TDAgrafo *G = crearGrafoVacio(n_vertices);
		
		//Desde la tercera fila hacia abajo
		for(k=0; k < m_aristas; ++k){
			fscanf(pf,"%d %d %d",&i, &j, &p);
			G->adyacencias[i-1][j-1] = p; //Para ir al vertice "i-1" cuesta "p", y en "j-1" son todos los adyacentes a "i-1"
		}
		fclose(pf);
		return  G;
	}
}

/* Devuelve la lista de adyacencias un vertice dado.
 * @param {TDAgrafo*} grafo         Grafo a utilizar.
 * @param {int}       vertice       Vertice en el cual buscar las adyacencias.
 * @return {TDAlista*}              Lista de adyacencias.*/
TDAlista* obtenerAdyacentes(TDAgrafo* grafo, int vertice, int*visitados)
{
	if(vertice > grafo->cvertices)
		return NULL;

	TDAlista *lista = crearListaVacia();

	for(int i = 0; i < grafo->cvertices; ++i){
		if(adyacenciaNodos(grafo, vertice, i) > 0 && visitados[i] == -1){
			insertarInicio(lista, i);
		}
	}

	return(lista);
}


/* Libera de la memoria un grafo dado.
 * @param {TDAgrafo*} grafo         Grafo a liberar. */
void liberarGrafo(TDAgrafo* grafo)
{
	for(int i = 0; i < grafo->cvertices; ++i){
		free(grafo->adyacencias[i]);
	}
	free(grafo->adyacencias);
	free(grafo);
}


/* Libera de la memoria una instancia del la estructura que contiene
 * los resultados del algoritmo Prim.
 * @param {TDAprimResultado*} prim         Instancia de una estructura que contiene los resultados del algoritmo Prim.
 */

//Funcion quedanPorVisitar: Función que verifica si en un arreglo de visitados existe algun valor "-1" en una posición i-ésima (la cual representa un nodo o vértice)
//Entradas:
//largo: largo del arreglo
//arreglo: arreglo de visitados
//Salida: "1" si ya se visitaron todos y "0" en caso de que el arreglo poseea una posición con -1 (NO visitado)
int quedanPorVisitar(int largo, int*arreglo){
	int i;
	for(i=0;i<largo;i++){
		if(arreglo[i] != 1){ //Si en la posición i-esima tengo distinto a 1, significa que el vértice i-ésimo aún no fue visitado
			return 1;
		}
	}
	return 0;
}


//Entradas:
//cada indice representa un nodo, por ej: el indice 0 representa al nodo con peso 5 (el de la esquina izquierda arriba en la foto del enunciado), el indice 1 con peso 1 representa al nodo de entrada para el nivel propuesto.
//distancia: arreglo que posee en cada en cada indice la distancia recorrida hasta llegar al nodo, si esque ya se puede llegar a el (!= INFINITO)
//vistos: arreglo que posee en cada indice si el nodo respectivo ya fue visitado (visitado = "1", no visitado = "-1")
//Salida: Retorna el inidice de vértice que posea el menor peso
int extrearMinimo(int*distancia, int*vistos, int largo){
	int i,a,minimo, pos;
	for(i=0;i<largo;i++){ //Recorremos buscando los NO vistos y los adyacentes (!= INFINITO)
		if(vistos[i] == -1 && distancia[i] != INFINITO){ //Consulta si el vertice no esta visitado "-1" y si es adyacente (!= "INFINITO")
			minimo = distancia[i]; //Se almacena la primera distancia encontrada, para luego comparar con las demás si es distancia (o peso) menor
			a = i; //Se marca el indice del nodo al cual se le guardo su distancia (o peso) 
			pos = i; //
			i = largo +1; //Dejo de cumplir el ciclo
		}
	}
	i=a;
	for(a=i; a<largo; a++){ //Recorremos buscando los NO vistos y los adyacentes (!= INFINITO)
		if(vistos[a] == -1 && distancia[i] != INFINITO){ //Consulta si el vertice no esta visitado "-1" y si es adyacente (!= "INFINITO")
			if(minimo > distancia[a]){ //Realiza la comparación, si "minimo" almacenado anteriormente es mayor que otra distancia a-ésima cambio el mínimo
				minimo = distancia[a]; //Cambio la distancia minimo
				pos = a; //El peso minimo viene dado por el nodo a-ésimo, entonces guardo el indice del nodo con menor peso
			}
		}
	}
	return pos; //Retorno el indice del nodo con menor peso
}


//Funncion Dijkstra: Función que dado un vértice inicial encuentra el camino menos costoso hacia todos los demás vértices
//Entradas:
//grafo: Es el grafo a ser analizado
//vert_in: Es el vértice inicial del grafo
//Salida: dij: Es la estructura para dijkstra con sus elementos que este necesita para su funcionamiento, elementos ya completados a la hora de retornar
TDAdijkstraResultado * dijkstra(TDAgrafo *grafo, int vert_in){

	int n = grafo->cvertices; //Cantidad de vétices
	int i, u;
	//Nodo y listas auxiliares para almacenar los vértices adyacentes al vértice analizado
	nodo *nodoAux = NULL;
	TDAlista *auxLista;
	
	//Pidiendo memoria y almacenando lo que necesita dijkstra
	TDAdijkstraResultado *dij = (TDAdijkstraResultado*)malloc(sizeof(TDAdijkstraResultado));
	dij->vertices = n;
	dij->verticeInicial = vert_in;
	dij->distancia = (int*)malloc(n * sizeof(int));
	dij->padre = (int*)malloc(n * sizeof(int));
	dij->visitados = (int*)malloc(n * sizeof(int));
	
	for(i=0;i<n;i++){ 
		dij->visitados[i] = -1; //"-1" no se a visitado ningún nodo todavía
		if((grafo->adyacencias[dij->verticeInicial][i]) > 0){ //Este if encuentra los nodos ayacentes al nodo de entrada
				dij->distancia[i] = grafo->adyacencias[dij->verticeInicial][i]; //Se marca el peso que estos tengan
				dij->padre[i] = dij->verticeInicial; //Se marca en "padres" el nodo desde donde vengo, vengo del inicial, por ende, padre[i] = vertice inicial
		}
		else{ //Si no es adyacente no tengo como llegar a ese nodo todavía, por ende, la distancia a ese nodo es "INFINITO"
			dij->distancia[i] = INFINITO;
		}
	}
	dij->distancia[dij->verticeInicial] = 0; //La distancia de nodo inicial a si mismo es 0
	dij->visitados[dij->verticeInicial] = 1; //Marco la visita a nodo inicial
	
	//Mientras queden nodos por visitar
	while(quedanPorVisitar(dij->vertices, dij->visitados) != 0){
		u = extrearMinimo(dij->distancia, dij->visitados, n); //En "u" almacenamos el indice del nodo "NO visitado" que poseea la MENOR distancia
		dij->visitados[u] = 1; //Marcamos ese nodo como visitado
		auxLista = obtenerAdyacentes(grafo, u, dij->visitados);	//Buscamos los adyacentes a "u" (adyacentes NO visitados)
		nodoAux = auxLista->inicio;
		
		while(nodoAux != NULL){ //Recorremos la lista de vertices adyacentes a "u"
			//Si la distancia del nodo en la lista adyacentes es mayor que la distancia del ultimo visitado + un adyacente a este (distancia acumulada)
			if(dij->distancia[nodoAux->dato] > dij->distancia[u] + grafo->adyacencias[u][nodoAux->dato]){
				dij->distancia[nodoAux->dato] = dij->distancia[u] + grafo->adyacencias[u][nodoAux->dato]; //Entonces cambiamos la distancia
				dij->padre[nodoAux->dato] = u; //Y se cambia el nodo del que vengo
			}
			nodoAux = nodoAux->siguiente;
		}
		liberarLista(auxLista);
	}
	return dij; //Retorno el TDA con sus elementos llenos

}
void liberarDijkstraResultado(TDAdijkstraResultado *dijkstra)
{
	free(dijkstra->distancia);
	free(dijkstra->padre);
	free(dijkstra);
}
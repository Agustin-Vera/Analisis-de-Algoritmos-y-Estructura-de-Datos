#include <stdio.h>
#include <stdlib.h>

/*------------- Estructura de Datos -------------*/

typedef struct nodoGenerico {
	int dato;
	struct nodoGenerico* siguiente;
} nodo;

/* Crea una nueva instancia de la estructura "nodo".
 * @param {int} dato            Dato que debe contener el nodo.
 * @return {nodo*}              Instancia del nuevo nodo.
 */
nodo* crearNodo(int dato)
{
	nodo* nuevo = (nodo*)malloc(sizeof(nodo));
	nuevo->dato = dato;
	nuevo->siguiente = NULL;
	return nuevo;
}


typedef struct listaGenerica {
	nodo* inicio;
} TDAlista;


/*------------- Operaciones -------------*/

TDAlista* crearListaVacia()
{
	TDAlista* lista=(TDAlista*)malloc(sizeof(TDAlista));
	lista->inicio=NULL;
	return lista;
}

int esListaVacia(TDAlista* lista)
{
	if (lista->inicio == NULL)
		return 1;
	else
		return 0;
}

void recorrerLista(TDAlista* lista)
{
	if (!esListaVacia(lista))
	{
		nodo* auxiliar=lista->inicio;
		while (auxiliar!=NULL)
		{
			printf("%d / ",auxiliar->dato);
			auxiliar=auxiliar->siguiente;
		}
		printf("\n");
	}
	else
		printf("La lista está vacía\n");
}

void insertarInicio(TDAlista* lista, int dato)
{
	nodo* nuevo = crearNodo(dato);
	nuevo->siguiente = lista->inicio;
	lista->inicio = nuevo;
}

void eliminarInicio(TDAlista* lista)
{
	nodo* auxiliar;
	if(!esListaVacia(lista))
	{
		auxiliar=lista->inicio;
		lista->inicio=lista->inicio->siguiente;
		free(auxiliar);
	}
}


/* Libera de la memoria los struct de lista y los nodos.
 * @param {TDAlista} lista      Lista a liberar.
 */
void liberarLista(TDAlista* lista)
{
	while(!esListaVacia(lista)){
		eliminarInicio(lista);
	}
	free(lista);
}


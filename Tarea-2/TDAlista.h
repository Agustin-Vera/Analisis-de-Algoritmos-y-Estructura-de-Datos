#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Nombre: Agustín Vera  ,  Sección: C-3
//En este archivo solamente estarán almacenadas las funciones que trabajen directamente con el TDA: lista enlazada
//Las demás funciones (no trabajan directamente con el TDA) estarán en el archivo "T2_C3_AgustínVera.c"

/*------------- estructura de datos -------------*/
//etiqueta: es una linea del archivo
//nro_linea: es el número de la linea en que se enecuentra "etiqueta"
//puntero: es la dirección del nodo siguiente (puntero que apunta al siguiente nodo)
typedef struct nodoGenerico
{
  char*etiqueta;
  int dato;
  int nro_linea;
  struct nodoGenerico* puntero;
}nodo;

//inicio: es la dirección del nodo inicial de la lista (puntero al nodo inicial de la lista)
typedef struct listaGenerica
{
  nodo* inicio;
}TDAlista;


/*------------- operaciones -------------*/

//Función que crea una lista vacía de tipo TDAlista
TDAlista* crearListaVacia()
{
  TDAlista* lista=(TDAlista*)malloc(sizeof(TDAlista));
  lista->inicio=NULL;
  return lista;
}

//Función que comprueba si una lista esta vacía (no posee elementos) o no lo está
int esListaVacia(TDAlista* lista)
{
  if (lista->inicio == NULL)
    return 1;
  else
    return 0;
}

//Función que recorre una lista e imprime el contenido de los nodos
void recorrerLista(TDAlista* lista)
{
  if (!esListaVacia(lista))
  {
    nodo* auxiliar=lista->inicio;
 
    while (auxiliar!=NULL)
    {
      printf("%d) %s",auxiliar->nro_linea,auxiliar->etiqueta);
      auxiliar=auxiliar->puntero;
    }
    
  }
  else
    printf("La lista esta vacia\n");
}

//Función que crea un nodo con el contenido de las entradas y lo ingresa al inicio de una lista
void insertarInicio(TDAlista* lista, int dato, char*linea)
{
  nodo* nuevo=(nodo*)malloc(sizeof(nodo));
  nuevo->etiqueta = linea;
  nuevo->nro_linea=dato;
  nuevo->puntero = lista->inicio;
  lista->inicio=nuevo;
}

//Función que elimina el nodo inicial de una lista
void eliminarInicio(TDAlista* lista)
{
  nodo* auxiliar;
  if(!esListaVacia(lista))
  {
    auxiliar=lista->inicio;
    lista->inicio=lista->inicio->puntero;
    free(auxiliar);
  }
}


//Función que elimina una lista, liberando la memoria nodo por nodo
void liberarLista(TDAlista* lista){
	while(!esListaVacia(lista)){
		eliminarInicio(lista);
	}
	
}

//Función que crea un nodo con las entradas y lo inserta al final de una lista
void insertarNodoFinal(TDAlista* lista, char*linea,int dato){
	nodo* nuevo=(nodo*)malloc(sizeof(nodo));
	nuevo->etiqueta = (char*)malloc(sizeof(char)*150);
	 if (!esListaVacia(lista)){
	 	
		nodo* auxiliar=lista->inicio;
		while(auxiliar->puntero != NULL){
			auxiliar=auxiliar->puntero;
		}
		strcpy(nuevo->etiqueta,linea);
		nuevo->nro_linea = dato;
		nuevo->puntero = NULL;
		auxiliar->puntero = nuevo;
	 }
	 else{
		strcpy(nuevo->etiqueta,linea);
		nuevo->nro_linea=dato;
		nuevo->puntero = lista->inicio;
		lista->inicio=nuevo;
	 }
}

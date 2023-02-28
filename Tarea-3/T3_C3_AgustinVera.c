#include <stdio.h>
#include <stdlib.h>
#include "TDAgrafo.h"
//#include "TDAlista.h" //Si lo activo me da error, pero funciona si no lo activo, pense en activarlo porque uso TDAlista en este archivo tambien.

//Tarea 3
//Nombre: Agustín Vera Rojas  //  Sección: C-3



//Función principal
//Entradas:
//1er argumento: Nombre del archivo (NIVEL) a analizar.
//2do argumento: Cantidad de energía enfrentar el nivel.
int main(int argc, char *entradas[]){
	
	char*nivel = entradas[1]; //Nombre del nivel
	char*energia_ingresada = entradas[2]; //Energía ingresa para dicho nivel
	int energia_total = atoi(energia_ingresada);
	int entrada, salida; //Indices de vértices de entrada y salida
	
	TDAgrafo* grafo = leerGrafoDirigidoPonderado(nivel, &entrada, &salida); //Se crea el grafo asociado al nivel (con el modelamiento planteado)
	
	//Se busca la salida menos costosa con el algoritmo planteado en clases de teoría. Dijkstra
	TDAdijkstraResultado* resultado = dijkstra(grafo,entrada); //Moldeado para el modelamiento planteado

	int i;
	//Este for muestra los pesos acumulados, los padres y si fue visitado o no (Muestra los inidices de los vértices)
	//for(i=0;i<resultado->vertices;i++){
		//printf("V i-esimo: %d  Padre: %d  Distancia: = %d  Visitado: %d\n",i,resultado->padre[i], resultado->distancia[i], resultado->visitados[i]);
	//}
	int vengo, energia;
	for(i=0; i<resultado->vertices; i++){ //Busco el indice del vértice de salida para saber "por el arreglo de padres" que vértice fue el que me llevo al vértice de salida
		
		if(i == salida){
			vengo = resultado->padre[i]; //Almaceno el vértice mencionado arriba en "vengo"
			energia = resultado->distancia[i] + grafo->adyacencias[i][vengo]; //Sumo la distancia recorrida (peso acumulado) hasta antes de llegar al vértice final + el peso del vértice final
			i = 100;
			
		}
	}
	
	TDAlista *camino = crearListaVacia(); //Lista que alamacenará el camino (indices de vértices) que se deben recorrer para llegar de un vértice inicial a uno final con el menor gasto de energía
	insertarInicio(camino, salida); //Almacenamos el indice del vértice de salida
	insertarInicio(camino, vengo); //Almacenamos el indice del vértice con el que se llega al vértice final

	energia_total = energia_total - energia; //Realizamos el calculo de la energía resultante despues de llegar al final (dada la energía inicial)
	
	printf("Terminaste el nivel con %d de energia.\n",energia_total);
	printf("El camino que debes recorrer para salir del laberinto es: \n"); //Más abajo se imprimen los indices de los vértices que se deben recorrer para poder encontrar la salida
	
	for(i=0; i<resultado->vertices; i++){ //Se recorre el arreglo de padres (el cual me indica desde donde vengo)
		if(i == vengo){ //Si i es igual al indice del vértice del que vengo
			vengo = resultado->padre[vengo]; //Cambio el indice del que vengo
			insertarInicio(camino, vengo); //Almaceno al inicio de lista el indice del vértice del que vengo
			i = 0; //Vuelvo a empezar (esto se repite (el for) hasta que verifiquemos que venimos de la entrada, el if de abajo)
		}
		if(resultado->padre[vengo] == entrada){ //Si vengo desde el vértice inicial (la "entrada")
			vengo = resultado->padre[vengo]; //Almaceno dicho indice "vengo"
			insertarInicio(camino, vengo); //Y lo inserto al inicio de la lista
			i = resultado->vertices+1; //Cambio la condición para que el for no se cumpla
		}
		
	}
	
	recorrerLista(camino); //Imprime el camino a realizar (indices de vértices) de "inicio = 1" hasta "salida = 31" (No son las mismas posiciones que en el archivo porque en la funcion de leer archivo se les resta 1)
	
	//Liberando las memorias pedidas
	liberarLista(camino);
	liberarGrafo(grafo);	
	liberarDijkstraResultado(resultado);
	return 0;
}
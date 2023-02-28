#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "TDAlista.h"

//Nombre: Agustín Vera  ,  Sección: C-3
//Consideraciones para editar el documento LaTeX:
//No identar con "tab", el programa no detecta los fallos identando de esta forma (identar con barra espaciadora)
//No copiar y pegar lineas, el programa no detecta fallos escribiendo de esa forma (escribir las lineas a mano)

//Las funciones definidas en este archivo no trabajan directamente con el TDA, por eso se definen aquí y no en el archivo "TDAlista.h".

//Funcion compara_etiquetas: Compara si la ultima etiqueta abierta es la primera en ser cerrada
//Entradas
//cierre: Es la primera etiqueta en cerrarse
//apertura: Es la última etiqueta en ser abierta
//condicion: Es la dirección de la variable que evalua si ya fue encontrado el error, si es alterada, termina el ciclo while que llama a esta función
//linea: Es el número de la linea de la etiqueta de cierre
void compara_etiquetas(char*cierre,char*apertura, int*condicion,int linea){
	int i;
	int j = 0;
	int cuenta_cierre = 0; //Cuenta el largo dentro de {}, Ej: \end{document} se cuenta el largo de "document" = 8 
	int cuenta_apertura = 0; //Realiza lo mismo que la variable de arriba, pero para la etiqueta de apertura
	char cierre_aux[20]; //Variables donde se almacenará "algo" que solamente va entre las llaves {algo}
	char apertura_aux[20]; //Lo mismo que el puntero cierre_aux
	
	for(i=0; cierre[i] != '}' && cierre[i-1] != '}'; i++){ //Recorre la linea de cierre hasta encontrar el final de llave '}'
		if(cierre[i] == '{'){
			for(i=i+1; cierre[i] != '}'; i++){ //Se recorre "cierre" hasta el final de la llave
				cierre_aux[j] = cierre[i]; //Ej: \end{itemize} almacena en cierre_aux = "itemize"
				cuenta_cierre++;
				j++;
			}	
		}
	}
	j = 0;
	for(i=0; apertura[i] != '}' && apertura[i-1] != '}'; i++){ //Recorre la linea de cierre hasta encontrar el final de llave '}'
		if(apertura[i] == '{'){
			for(i=i+1; apertura[i] != '}'; i++){ //Se recorre "apertura" hasta el final de la llave
				apertura_aux[j] = apertura[i]; //Ej; \begin{itemize} almacena en apertura_aux = "itemize"
				cuenta_apertura++;
				j++;
			}
		}	
	}
	
	if(cuenta_cierre == cuenta_apertura){ //Compara si los largos de las palabras entre llaves son iguales
		for(i=0;i<cuenta_cierre;i++){ //Recorre las palabras hasta encontrar una diferencia
			if(cierre_aux[i] != apertura_aux[i]){ //Si existe diferencia imprime el error correspondiente
				*condicion = 1;
				printf("Error en la linea %d: se esperaba \\end{",linea); //Se imprime el error correspondiente
				for(j=0;j<cuenta_apertura;j++){
					printf("%c",apertura_aux[j]); //Se tuvo que imprimir por caracteres debido a fallas al imprimir cadena (%s) en windows
				}
				printf("} en lugar de \\end{");
				for(j=0;j<cuenta_cierre;j++){
					printf("%c",cierre_aux[j]);
				}
				printf("}.\n");
			}
		}	
	}
	else{ //Si los largos no son iguales, las palabras entre llaves tampoco, por ende, escribe el error correspondiente
		*condicion = 1;
		
		printf("Error en la linea %d: se esperaba \\end{",linea); //Se imprime el error correspondiente
		for(j=0;j<cuenta_apertura;j++){
			printf("%c",apertura_aux[j]); //Se tuvo que imprimir por caracteres debido a fallas al imprimir cadena (%s) en windows
		}
		printf("} en lugar de \\end{");
		for(j=0;j<cuenta_cierre;j++){
			printf("%c",cierre_aux[j]);
		}
		printf("}.\n");
	}
}


//Funcion esImportante: Revisa si la linea es "\begin{algo}", "\end{algo}", "\item" ó posee "$" ("importantes")
//Entradas:
//linea: linea del archivo a analizar
//retorno: retorna 1 si la linea es begin, retorna 2 si la linea es end, y retorna 0 si no es "importante"
int esImportante(char *linea){
	int i;
	int j = 0;
	char palabra[200]; //String donde se almacenara la linea
	int condicion = 1; //Condicion que indica que aun no llegamos a '{'
	
	for(i=0; condicion != 0 && linea[i] != '\0'; i++){ //Recorremos "linea" hasta que encuentre != a " ", encontrando eso, copia en "palabra" hasta '{'
		if(linea[i] != ' '){
			palabra[j] = linea[i];
			j++;
		}
		if(linea[i] == '{'){ //Si llega al tope, salimos del for (cambio condicion)
			condicion = 0; //Ej: begin{ , se copia (if de arriba) hasta antes de llegar a esto '{'
		}
		if(linea[i] == '$'){ //Si la linea posee "$" es importante
			//printf("Encontre signo peso: %s\n",palabra);
			return 4;
		}
	}
	if(palabra[1] == 'b' && palabra[2] == 'e' && palabra[3] == 'g' && palabra[6] == '{'){ //Consulta si palabra presenta comportamiento "\begin{"
		return 1;
	}
	if(palabra[1] == 'e' && palabra[2] == 'n' && palabra[3] == 'd' && palabra[4] == '{'){ //Consulta si palabra presenta comportamiento "\end{"
		return 2;
	}
	if(palabra[0] == '\\' && palabra[1] == 'i' && palabra[2] == 't' && palabra[4] == 'm'){ //Consulta si palabra presenta comportamiento "\item"
		//printf("encontre item: %s",palabra);
		return 3;
	}
	//Si no es importante, retorna 0
	return 0;
}


//Funcion que recorre una linea y cuenta la cantidad de "$"
//Entradas:
//linea: Es la linea que se desea analizar
//*condicion: Es la dirección de la variable (condicion) que será alterada en caso de haber error
//nro_linea: Es el numero de la linea a analizar
void cuenta_dolar(char*linea, int*condicion, int nro_linea){
	int i;
	int contador = 0;
	for(i=0;linea[i] != '\0';i++){
		if(linea[i] == '$'){
			contador++;
		}
	}
	if(contador%2 != 0){ //Si la cantidad de "$" es impar significa que un $ quedo sin cerrar
		*condicion = 1;
		printf("Error en la linea %d: Se esperaba un cierre para la pertura del tipo $.\n",nro_linea);
	}	


}


//Funcion que revisa si antes de un "\item" se ha abierto un "itemize" ó "enumerate"
//Entradas:
//apertura: Es la última linea (etiqueta) en ser abierta
//*condicion: Es la dirección de la variable (condicion) que será alterada en caso de haber error
//nro_linea: Es el numero de la linea a analizar
void revisaItem(char*apertura, int*condicion, int nro_linea){
	int i;
	int j = 0;
	char a[20]; //Variable donde se almacenará "algo" que solamente va entre las llaves {algo}
	
	for(i=0; apertura[i] != '}' && apertura[i-1] != '}'; i++){ //Recorre la linea de cierre hasta encontrar el final de llave '}'
		if(apertura[i] == '{'){
			for(i=i+1; apertura[i] != '}'; i++){ //Se recorre "apertura" hasta el final de la llave
				a[j] = apertura[i]; //Ej: \begin{itemize} almacena en a = "itemize"
				j++;
			}	
		}
	}
	if(a[0] == 'i'){ //En los dos casos se ocupa "if" para saber que es la palabra exacta
		//Si posee alguna letra distinta a "itemize" entonces "\item" no cumple los requisitos para ser usado 
		if(a[1] == 't' && a[2] == 'e' && a[3] == 'm' && a[4] == 'i' && a[5] == 'z' && a[6] == 'e'){
		}
		else{
			printf("Error en la linea %d: Se esperaban aperturas de tipo \\begin{itemize} o \\begin{enumerate} antes de \\item.\n",nro_linea);
			*condicion = 1;
		}
	}
	else{
		//Si posee alguna letra distinta a "enumerate" entonces "\item" no cumple los requisitos para ser usado 
		if(a[0] == 'e' && a[1] == 'n' && a[2] == 'u' && a[3] == 'm' && a[4] == 'e' && a[5] == 'r' && a[6] == 'a' && a[7] == 't' && a[8] == 'e'){
		}
		else{
			printf("Error en la linea %d: Se esperaban aperturas de tipo: \\begin{itemize} o \\begin{enumerate} antes de \\item.\n",nro_linea);
			*condicion = 1;
		}
	}
}


//Funcion principal
int main(int argc, char *entradas[]){
	
	//Variables para tiempos de ejecucion
	//clock_t cini, cfin;
	//cini=clock();
	
	char*entrada; //Se alamcenará el nombre del archivo ingresado por consola
	int i = 1;
	int cant_lineas; //Cantidad de lineas del archivo
	char a;
	char linea[200]; //Puntero donde se almacenará la linea i-ésima del archivo
	entrada = entradas[1];
	
	FILE*archivo = fopen(entrada,"r");
	if(archivo==NULL){
		printf("Se habrio mal\n");
	}
	
	while(a != EOF){ //Contando la cantidad de lineas que posee el archivo
		a = fgetc(archivo);
		if(a == '\n'){
			i++;
		}
	}
	fseek(archivo,0,SEEK_SET); //Devolvemos el cursor al inicio del archivo
	cant_lineas = i;
	
	//lista: Se almacenaran todas las lineas que se consideren "importantes" en su recorrido, en el mismo orden que el archivo
	//lista_comparativa: Lista donde se iran almacenado los nodos iniciales de "lista"
	TDAlista* lista = crearListaVacia();
	TDAlista* lista_comparativa = crearListaVacia();
	
	//Con este for almacenamos todas las lineas importantes en lista, en el mismo orden que vayan apareciendo en el archivo
	for(i=1; i<cant_lineas; i++){
		fgets(linea, 200, archivo); //Almacenamos la linea del archivo en "linea"
		
		if(esImportante(linea) != 0){ //Si la linea es importante se agrega al final de "lista"
			insertarNodoFinal(lista, linea, i);
		}
	}
	
	int error = 0; //Variable que será alterada a "error = 1" si se encuentra algun error. Terminará el ciclo while si se encuentra error
	
	
	//Si la lista no esta vacía y no hay errores
	while(!esListaVacia(lista) && error != 1){
		//en "esImportante" vemos el tipo de linea que tenemos al inicio de "lista"
		
		//Si corresponde a "begin" debemos colocar el nodo inicial de "lista" al final de "lista_comparativa"
		//y eliminar el nodo inicial de "lista"
		if(esImportante(lista->inicio->etiqueta) == 1){
			insertarInicio(lista_comparativa, lista->inicio->nro_linea, lista->inicio->etiqueta);
			eliminarInicio(lista);
			
		}
		//Si corresponde a "end" debemos comparar el nodo inicial de "lista" con el nodo inicial de "lista_comparativa"
		else if(esImportante(lista->inicio->etiqueta) == 2){
			//compara_etiquetas: Compara si el nodo incial de "lista" que esta conteniendo un "\end{algo}" es efectivamente el cierre
			//de "\begin{algo}" el cual estará almacenado en el nodo incial de "lista_comparativa".
			//Si las etiquetas no son correspondientes, "error" es 1, se imprime el tipo de error y sale del while
			compara_etiquetas(lista->inicio->etiqueta, lista_comparativa->inicio->etiqueta, &error, lista->inicio->nro_linea);
			
			//De ser o no correspondientes se eliminan los nodos iniciales de cada lista
			eliminarInicio(lista_comparativa);
			eliminarInicio(lista);
		}
		//Si corresponde a "\item"
		else if(esImportante(lista->inicio->etiqueta) == 3){
			
			//Se revisa si la ultima etiqueta en ser abierta (almacenada al inicio de lista_comparativa) corresponde a "\begin{enumerate}" o "\begin{itemize}"
			revisaItem(lista_comparativa->inicio->etiqueta, &error, lista->inicio->nro_linea);
			//De haber o no error, elimina el nodo inicial de lista
			eliminarInicio(lista);
			
		}
		//Si no corresponde a niguno, corresponderá a el caso de linea tipo "$"
		else if(esImportante(lista->inicio->etiqueta) == 4){
			//Se cuentan la cantidad de "$", si es impar imprime el error y altera la condición, termina el ciclo while
			cuenta_dolar(lista->inicio->etiqueta, &error, lista->inicio->nro_linea);
			//De haber o no error, elimina el nodo incial de lista
			eliminarInicio(lista);
		}
	}
	if(error == 0){ //Si la variable "error" no fue alterada anteoriormente, el archivo no presenta errores
		printf("El archivo es valido.\n");
	}
	
	//Para imprimir tiempos de ejecución
	//cfin=clock();
	//printf("Tiempo para archivo: %s\n",entrada);
	//printf("clock_t algoritmo:  %.15f\n",(double)(cfin-cini)/CLOCKS_PER_SEC);
	
	//Cerrando archivo y liberando memorias
	fclose(archivo);
	liberarLista(lista);
	liberarLista(lista_comparativa);
	return 0;	
}

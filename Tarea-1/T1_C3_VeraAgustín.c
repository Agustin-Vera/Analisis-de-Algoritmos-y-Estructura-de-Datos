#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Tarea 1
//Nombre: Agustín Vera Rojas , Sección: C-3

//Funcion que calcula el binario i-esimo (Los "R" representan 1 y los "N" representan 0, para lo binario
//Entradas:
//binario_anterior: Es el numero binario anterior (en el contexto: es el caso anterior de azulejos)
//largo: Es el ancho de la pared
//*cant_dorado: Es la cantidad de aplicaciones doradas hasta el llamado. Si es el caso, se aumentan
//Salida: El numero binario i-esimo (en el contexto: es el caso actual de azulejos)
char*numero_binario(char*binario_anterior,int largo,int*cant_dorado){
	int i,j;
	int caso = 1;
	//binario_actual: Binario a ser calculado y retornado
	char*binario_actual = (char*)malloc(sizeof(char)*largo);
	char rojo;
	//Pivote almacenará la posición del primer "R" de izquierda a derecha (del binario anterior)
	int pivote;
	for(i=0;i<largo;i++){
		if(binario_anterior[i] == 'R'){
			pivote = i;
			rojo = binario_anterior[i];
			i = largo+1;
		}
	}
	
	//Ciclo que evalua si el binario anterior corresponde al caso 0
	for(i=pivote; i<largo; i++){
		if (binario_anterior[i] != 'R'){
			caso = 0;
		}
	}
	
	//Caso = 0: El binario anterior posee un "N" dentro ("Pensado desde pivote hasta el final")
	if(caso == 0){
	
		//Se copia el binario anterior y se almacena en binario_actual
		for(i=0; i<largo; i++){
			binario_actual[i] = binario_anterior[i];
		}
		
		//Se recorre de derecha a izquierda (de largo-1 hasta antes de pivote)
		for(i=largo-1; i>pivote; i=i-1){
		
			//Si tenemos un 0 en la posicion i-esima, colocamos un 1 y terminamos el ciclo (Fue colocada la reserva)
			if(binario_actual[i] == 'N'){
				binario_actual[i] = 'R';
				i =  pivote-1;
			}
			//Si no tenemos un "N" en la posicion i-esima, colocamos un "N" (Esto simula la "reserva" en la suma de binarios)
			else{
				binario_actual[i] = 'N';
				
			}
		}
		
	}
	//Si no es caso = 0, tenemos el caso 1
	//Caso 1: El binario anterior posee solamente "R" desde pivote hasta el final
	else{		
		//for que rellena de "N" el binario actual
		for(i=0; i <largo; i++){
			binario_actual[i] = 'N';
		}
		//Se coloca el pivote una posicion a la izquierda
		binario_actual[pivote-1] = 'R';
	}
	//Se busca la cantidad de aplicaciones doradas dentro del caso creado y se suma a la cantidad acumulada
	for(i=1;i<largo;i++){
		if((binario_actual[i-1] == 'R') && (binario_actual[i] == 'R')){
			*cant_dorado = *cant_dorado+1;
		}
	}
	return binario_actual;
}


//Funcion que genera el archivo de salida con las aplicaciones doradas y los casos respectivos
//Entradas:
//muestrario: Son todas las combinaciones almacenadas de los casos
//ancho: Es el ancho de la pared. Lo ingresa el usuario
//Las demas entras poseen nombres representativos
//Salida: No retorna nada (void). Crea un archivo
void generar_muestrario(char*nombre_archivo,int cantidad_casos,int cantidad_dorado, int ancho, char**muestrario){
	int i,j;
	//Contador: Llevara la cuenta si hay dos "R" consecutivos
	int contador = 0;
	//Se crea y abre el archivo con su nombre respectivo
	FILE *archivo = fopen(nombre_archivo,"w");
	
	//Primera linea del archivo
	fprintf(archivo,"%d %d\n",cantidad_dorado,cantidad_casos);
	
	//For que itera hasta cantidad de casos
	for(i=0; i<cantidad_casos;i++){
		//For que imprime en el archivo de salida las "N" , "R" y "D" segun corresponda
		for(j=0;j<ancho; j++){
		
			if(muestrario[i][j] == 'R'){
				contador++;
				if(contador == 2){
					fputc('D',archivo);
					contador = contador-1;
					fputc(' ',archivo);
				}
			}
			if(muestrario[i][j] == 'N'){
				contador = 0;
			}
			fputc(muestrario[i][j],archivo);
			fputc(' ',archivo);
			
		}
		fputc('\n',archivo);
		contador = 0;
	}
	//Cerrando el archivo
	fflush(archivo);
	fclose(archivo);
}

//Funcion principal
int main(int argc, char *entradas[]){

	int cantidad_azulejos,i,j;
	char *nombre_salida;
	int largo = 1;
	int aplicaciones_doradas = 0;
	//Cantidad de azulejos
	cantidad_azulejos = atoi(entradas[1]);
	
	//Nombre salida
	nombre_salida = entradas[2];
	
	//Cantidad de casos posibles en la distribución de los azulejos
	double entrada = pow(2,cantidad_azulejos);
	int casos_posibles = (int)entrada;
	
	//Matriz que almacenara todas las combinaciones de azulejos para una entrada n
	char**matriz_casos = (char**)malloc(sizeof(char*)*casos_posibles);
	
	//Se rellenan todos los casos con "N"
	for(i=0;i<casos_posibles;i++){
			matriz_casos[i] = (char*)malloc(sizeof(char*)*cantidad_azulejos);
			for(j=0;j<cantidad_azulejos;j++){
				matriz_casos[i][j] = 'N';
			}
	}
	//Se coloca el binario de 1 (en el contexto: es el caso 1 en la distribucion de azulejos)
	matriz_casos[1][cantidad_azulejos-1] = 'R';
	
	//Variables para tiempos de ejecucion
	clock_t cini, cfin;
	
	cini=clock();
	//Se inicia de 2, los casos anteriores estan creados
	for(i=2;i<casos_posibles;i++){
		matriz_casos[i] = numero_binario(matriz_casos[i-1],cantidad_azulejos,&aplicaciones_doradas);
	}
	generar_muestrario(nombre_salida,casos_posibles, aplicaciones_doradas, cantidad_azulejos, matriz_casos);
	
	cfin=clock();

	for(i=0;i<casos_posibles;i++){
	
		/**for(j=0; j<cantidad_azulejos; j++){ //Si se activa esto, se muestran los casos por consola
			printf("%c ",matriz_casos[i][j]);
		}
		printf("\n");**/
		free(matriz_casos[i]);
	}
	printf("Cantidad aplicaciones doradas: %d\n",aplicaciones_doradas);
	
	//Para analizar tiempos de ejecución
	//printf("Tiempo para una entrada: %d\n",cantidad_azulejos);
	//printf("clock_t algoritmo:  %.15f\n",(double)(cfin-cini)/CLOCKS_PER_SEC);
	
	return 0;
}

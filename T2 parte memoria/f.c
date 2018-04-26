#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>

void numeros_nivel(int* arreglo_resultados, int * arreglo_optimo, int numeros_nivel, char* binario);

int main(int argc, char *argv[]){
int arreglo_optimo[5];
int niveles;
niveles = 2;
int arreglo_resultados[5];
char* binario;
arreglo_optimo[0] = 10;
arreglo_optimo[1] = 10;
arreglo_optimo[2] = 0;
arreglo_optimo[3] = 0;
arreglo_optimo[4] = 0;
binario = "01101110000101000101";
numeros_nivel( arreglo_resultados, arreglo_optimo, niveles, binario);

return 0;
}


void numeros_nivel(int* arreglo_resultados, int * arreglo_optimo, int numeros_nivel, char* binario){
	int w1;
	int w2;
	//int acumulador;
	//acumulador = 0;
	int s=0;
	for(w1 = 0 ; w1 < numeros_nivel ; w1++){
		printf ( "ao: %i     ",arreglo_optimo[w1]);
		char* string_acotado = calloc((arreglo_optimo[w1]+1), sizeof(char));
		//har string_acotado[arreglo_optimo[w1]];
		string_acotado[w1]= '\0';
		for(w2 = 0; w2 < arreglo_optimo[w1];w2++){
			string_acotado[w2]=binario[s];
			s++;
		}
		//acumulador = acumulador + w1;
		//printf("string acotado: %s   ",string_acotado);
		int a = strtol(string_acotado, NULL, 2);
		printf("aaaa :%d   ",a);
		free(string_acotado);
		
	}
}


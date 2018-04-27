#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

typedef struct fila_tlb {
	char* entrada;
	char* salida;
} Fila_tlb;

typedef struct fila {
	struct fila** tabla_apuntada;
	int es_ultimo_nivel;
	int marco;
  int  validez;
}Fila;

int* calcular_optimos(int cantidad_de_niveles);//funciona
Fila**crear_tabla( int nivel_actual, int* bits_nivel, int total_niveles);
void destruir_tablas(Fila **tabla,int nivel_actual, int* bits_nivel, int total_niveles);
int buscar_marco(Fila**tabla, int* segmentos);
void int2binstr(int num, char* bin);//funciona
void numeros_nivel(int* arreglo_resultados, int * arreglo_optimo, int numeros_nivel, char* binario);//funciona



int main(int argc, char *argv[]){
  //inicializacion del programa leer archivo
  int niveles = atoi(argv[1]);
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  fp = fopen(argv[2], "r");
  int cantidad_lineas=0;
  while ( getline(&line, &len, fp)!=-1) {
    cantidad_lineas++;
    }
  fclose(fp);
  fp = fopen(argv[2], "r");
  int *direcciones_a_revisar=malloc(sizeof(int)*cantidad_lineas);
  int i=0;

  for(i=0;i<cantidad_lineas;i++) {
      int numero;
      fscanf(fp, "%d", &numero);
      direcciones_a_revisar[i]=numero;
  }
  fclose(fp);
  /*for(l=0;l<cantidad_lineas;l++){
    printf("valor: %i\n",direcciones_a_revisar[l]);
  }*/
//fin leer texto

//inicializar sistema de tablas
  int *optimo = calcular_optimos(niveles);
/* //mostrar optimos
  for (i=0;i<niveles;i++){
    printf("optimo: %i ",optimo[i]);
  }*/

  //creacion de arrays de tablas por nivel
  Fila** instancia_tabla = crear_tabla( 0, optimo,  niveles);
  //estructura para soportar binario
  char* bin=malloc(sizeof(char)*29);
  for (i=0;i<28;i++){
    bin[i]='0';
  }
  bin[28]='\0';


  //ejemplos
  int2binstr( 3655,bin);
  int *arreglo_resultados=calloc(niveles,sizeof(int));
  numeros_nivel( arreglo_resultados, optimo,  niveles,  bin);
  int a=buscar_marco(instancia_tabla, arreglo_resultados);
 //printf("marco: %i ",a);

//simulacion






//fin simulacion









//liberar memoria
  destruir_tablas( instancia_tabla,0, optimo,  5);
  free(optimo);
  free(bin);
  free(arreglo_resultados);
  free(direcciones_a_revisar);
  free(line);
  return 0;
}//fin main

int* calcular_optimos(int cantidad_de_niveles){
	// defino el array
	int* array_ans = (int*) calloc(5, sizeof(int));
	array_ans[0] = 0;
	array_ans[1] = 0;
	array_ans[2] = 0;
	array_ans[3] = 0;
	array_ans[4] = 0;

	int i, j, k, l;
	//defino sumas de eficiencia
	int suma_eficiencia_global = 2147483647;
	int suma_eficiencia_actual = 0;
	//comienzo
	int max_nivel_1 = 21 - cantidad_de_niveles;
	for( i=1;i < max_nivel_1 + 1 ;i++){
		if (cantidad_de_niveles > 1){
		   int max_nivel_2 = 21 - cantidad_de_niveles - i;
			 for(j = 1; j < max_nivel_2 + 1; j++){
				 if (cantidad_de_niveles > 2){
					 int max_nivel_3 = 21 - cantidad_de_niveles - i - j;
					 for(k = 1; k < max_nivel_3 + 1; k++){
						 if (cantidad_de_niveles > 3){
							 int max_nivel_4 = 21 - cantidad_de_niveles - i - j - k;
							 for(l = 1 ; l < 21; l++){
								 if(cantidad_de_niveles > 4){
									 int max_nivel_5 = 21 - cantidad_de_niveles - i - j - k - l;
									 //for(m = 1; m < max_nivel_5 + 1; m++){
										 suma_eficiencia_actual = pow(2,i)*j + pow(2,j)*k + pow(2,k)*l + pow(2,l)*(20 - i - j - k - l) + pow(2,(20 - i - j -k - l))*11;
										 //printf("Ef: %d . %d %d %d %d %d \n", suma_eficiencia_actual, i , j , k, l, (20 - i - j - k - l));
										 if ((suma_eficiencia_actual < suma_eficiencia_global) && (suma_eficiencia_actual > 0) && (20 > i + j + k + l)){
											 array_ans[0] = i;
											 array_ans[1] = j;
											 array_ans[2] = k;
											 array_ans[3] = l;
											 array_ans[4] = 20 - i - j - k - l;
											 suma_eficiencia_global = suma_eficiencia_actual;
											 //printf("Entre. La suma de eficiencia actual es %d !\n", suma_eficiencia_actual);
											 //printf("Los numeros son  %d , %d, %d , %d y %d \n", i, j, k, l, 20 - i - j - k - l );
											 //printf("\n" );
									 }
								 //}
							//cantidad de niveles > 4
						 }
						 else if(cantidad_de_niveles == 4){
							 suma_eficiencia_actual = pow(2,i)*j + pow(2,j)*k + pow(2,k)*(20 - i - j - k) + pow(2,(20 - i - j - k))*11;
							 if ((suma_eficiencia_actual < suma_eficiencia_global) && (suma_eficiencia_actual > 0)){
								 array_ans[0] = i;
								 array_ans[1] = j;
								 array_ans[2] = k;
								 array_ans[3] = 20 - i  - j - k;
								 suma_eficiencia_global = suma_eficiencia_actual;
								 //printf("Entre. La suma de eficiencia actual es %d !\n", suma_eficiencia_actual);
								 //printf("Los numeros son  %d , %d, %d y %d \n", i, j, k, 20 - i - j - k );
								 //printf("\n" );
						 }
					 }
						//for l
					 }
				//cantida de  niveles > 3
				 }

				else if(cantidad_de_niveles == 3){
					suma_eficiencia_actual = pow(2,i)*j + pow(2,j)*(20-i -j) + pow(2,(20-i-j))*11;
					if ((suma_eficiencia_actual < suma_eficiencia_global) && (suma_eficiencia_actual > 0)){
						array_ans[0] = i;
						array_ans[1] = j;
						array_ans[2] = 20 - i - j;
						suma_eficiencia_global = suma_eficiencia_actual;
						//printf("Entre. La suma de eficiencia actual es %d !\n", suma_eficiencia_actual);
						//printf("Los numeros son  %d , %d y %d \n", i, j, 20 - i - j );
						//printf("\n" );
				  }
				}
// for k
		   }
	//cantidad de niveles > 2
		}
	else if(cantidad_de_niveles == 2){
		suma_eficiencia_actual = pow(2,i)*(20-i) + pow(2,(20-i))*11;
		if ((suma_eficiencia_actual < suma_eficiencia_global) && (suma_eficiencia_actual > 0)){
			array_ans[0] = i;
			array_ans[1] = 20 - i;
			suma_eficiencia_global = suma_eficiencia_actual;
			//printf("Entre. La suma de eficiencia actual es %d !\n", suma_eficiencia_actual);
			//printf("Los numeros son  %d y %d\n", i, (20-i) );
			//printf("\n" );
	}
}
// for j
	}
//cantidad de niveles > 1
}
else if(cantidad_de_niveles == 1){
	array_ans[0] = 20;
}
//for i
}
return array_ans;
//funcion principal
}

void int2binstr(int num, char* bin){
	int i;
	for(i = 0; i<28; i++){
		bin[i] = '0';
	}

	int resto;
	i = 27;
	while(num != 0){
		if(i >= 0){
		resto = num %2;
		num = (num - resto) / 2;
		if(resto == 0){
			bin[i] = '0';
		}
		else{
			bin[i] = '1';
		}
		i--;
	}
}
//printf("binario:%s ",bin);
}

void numeros_nivel(int* arreglo_resultados, int * arreglo_optimo, int numeros_nivel, char* binario){
	int w1;
	int w2;
	//int acumulador;
	//acumulador = 0;
	int s=0;
	for(w1 = 0 ; w1 < numeros_nivel ; w1++){
		printf ( "ao: %i     ",arreglo_optimo[w1]);
		char* string_acotado = calloc((arreglo_optimo[w1]+5), sizeof(char));
		//har string_acotado[arreglo_optimo[w1]];
		string_acotado[w1]= '\0';
		for(w2 = 0; w2 < arreglo_optimo[w1];w2++){
      //printf("w2: %i nivel:%i ",w2, w1);
			string_acotado[w2]=binario[s];
			s++;
		}
		//acumulador = acumulador + w1;
		printf("string acotado: %s   ",string_acotado);
		int a = strtol(string_acotado, NULL, 2);
    arreglo_resultados[w1]=a;
		//printf("aaaa :%d   ",a);
		free(string_acotado);

	}
}


Fila**crear_tabla(int nivel_actual, int* bits_nivel, int total_niveles){
  int largo=pow(2,bits_nivel[nivel_actual]);
  Fila** tabla=(Fila**) calloc(largo,sizeof(Fila*));
  int i;
  if( nivel_actual<total_niveles-1){
    for(i=0;i<largo;i++){
      tabla[i]=(Fila*) calloc(1,sizeof(Fila));
      tabla[i]->es_ultimo_nivel=0;
      tabla[i]->validez=0;
      tabla[i]->marco=55;
      tabla[i]->tabla_apuntada=crear_tabla( nivel_actual+1,bits_nivel, total_niveles );
    }
  }
  else{
    for(i=0;i<largo;i++){
      tabla[i]=(Fila*) calloc(1,sizeof(Fila));
      tabla[i]->es_ultimo_nivel=1;
      tabla[i]->validez=0;
      tabla[i]->marco=77;

    }
  }
  return tabla;
}

void destruir_tablas(Fila **tabla,int nivel_actual, int* bits_nivel, int total_niveles){
  int cantidad_filas=pow(2,bits_nivel[nivel_actual]);
  if(tabla[0]->es_ultimo_nivel==1){
    int i=0;
    for(i=0;i<cantidad_filas;i++){
      free(tabla[i]);
    }
    free(tabla);
  }
  else{
    int i=0;
    for(i=0;i<cantidad_filas;i++){
      destruir_tablas(tabla[i]->tabla_apuntada, nivel_actual+1,  bits_nivel,  total_niveles);
      free(tabla[i]);
    }
  free(tabla);
}
}

int buscar_marco(Fila**tabla, int* segmentos){
	int i;

	i= 0;
  while(tabla[0]->es_ultimo_nivel != 1){
  	tabla = tabla[segmentos[i]]->tabla_apuntada;

  }
  return tabla[segmentos[i]]->marco;
}

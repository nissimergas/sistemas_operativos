#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>



//TLB
// typedef struct fila {
// 	int salida; //puede ser un marco o una direccion a una tabla
// 	int valido; //1 si tabla a la que apunta esta en memoria
// } Fila;

// typedef struct tabla {
// 	int ultimo_nivel; //1 si tabla es de ultimo nivel
// 	Fila** arreglo;
// } Tabla ;

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

//declaraciones
int* calcular_optimos(int cantidad_de_niveles);
Fila**crear_tabla( int nivel_actual, int* bits_nivel, int total_niveles);
void destruir_tablas(Fila **tabla,int nivel_actual, int* bits_nivel, int total_niveles);
int buscar_marco(Fila**tabla, int* segmentos);
void int2binstr(int num, char* bin);
void numeros_nivel(int* arreglo_resultados, int * arreglo_optimo, int numeros_nivel, char* binario);

// Tabla* crear_tabla(int n, int ultimo_nivel);

//////////////////////////////////////////////////////begin_main///////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){
	//declaracion de variables de contador
	// int i;
	//
	int c1;
	int direccion_actual;

	int page_fault;
	page_fault = 0;
	int tlb_hits;
	tlb_hits = 0;

	double porcentaje_page_fault;
	double porcentaje_tlb_hits;

	int c2;
	int c3;

	int esta_en_tlb;

  char * bits_direccionamiento = malloc(20);
	char * bits_offset  = malloc(8);
	// int c4;
	//
	// int c5;
	// int c6;
	// int c7;
	// int c8;
	//
	// int c9;
	// int c10;
	// int c11;
	// int c12;
	//
	// int c13;

	//declaracion de otras variables
	// Tabla* nueva_tabla;
	// Tabla* tabla_n1;

	//crear TLB
	Fila_tlb** tlb = (Fila_tlb**) calloc(64,sizeof(Fila_tlb*));
	//Fila** tlb = (Fila**) calloc(64, sizeof(Fila*));

	//crear memoria virtual
	char* memoria = (char*) calloc(256, sizeof(char));

	//creo el binario
	char* bin =  malloc(28 * sizeof(char));

//arreglo de arreglo_resultados
int* arreglo_resultados_valor = malloc(5*sizeof(int));

	/// direccionamiento eficiente
	int cantidad_de_niveles;
	cantidad_de_niveles = atoi(argv[1]);
	printf("Cantidad de niveles: %d\n",cantidad_de_niveles);

//encontrar optimo
int* optimo;
optimo = calcular_optimos(cantidad_de_niveles);
printf("El optimo encontrado es: %d %d %d %d %d \n", optimo[0], optimo[1], optimo[2], optimo[3], optimo[4]);





//creacion de arrays de tablas por nivel
Fila** instancia_tabla = crear_tabla( 0, optimo,  cantidad_de_niveles);








///  lectura
char * nombre_archivo = argv[2];
printf("El archivo leido fue: %s\n", nombre_archivo);

FILE* fp;
char* linea = NULL;
size_t len = 0;
ssize_t read;
fp = fopen(nombre_archivo, "r");
c1 = 0;
//Leo el archivo y al mismo tiempo ejecuto el loop principal
//char*a=int2binstr(23333);
//printf("%s\n", a);


//arreglo_resultados_valor_dummy = mall
//buscar_marco(instancia_tabla, arreglo_re=sultados_valor);
numeros_nivel(arreglo_resultados_valor, optimo, cantidad_de_niveles, bin);
buscar_marco(instancia_tabla, arreglo_resultados_valor);

while ((read = getline(&linea, &len, fp)) != -1) {
  direccion_actual = atoi(linea);
	printf("Leyendo direccion: %d \n",direccion_actual);
	int2binstr(direccion_actual, bin);
	strncpy(bits_direccionamiento, bin, 20);
	//strncpy(bits_offset, bin + 19, 8);
	numeros_nivel(arreglo_resultados_valor, optimo, cantidad_de_niveles, bin);
    //buscar_marco(instancia_tabla, arreglo_resultados_valor);
	esta_en_tlb = 0;

	//Buscar en TLB
	for(c3 = 0; c3 < 64; c3++){
		 if(tlb[c3]->entrada == bits_direccionamiento){
		 	printf("HIT");
		 	tlb_hits ++;
		 	esta_en_tlb = 1;
		 }
		//int numero = strtol(string, NULL, 2)
	}
	if(esta_en_tlb != 1){

	}

}
fclose(fp);



///Impresion de estadisticas
porcentaje_tlb_hits = tlb_hits / (c1 + 1);
porcentaje_page_fault = page_fault / (c1 + 1);


destruir_tablas( instancia_tabla,0, optimo,  cantidad_de_niveles);
free(bits_offset);
free(arreglo_resultados_valor);
free(bits_direccionamiento);
free(optimo);
free(memoria);
free(tlb);
free(bin);
if (linea){
		free(linea);
}



	return 0;
}
//////////////////////////////////////////////////////end_main///////////////////////////////////////////////////////////////



int* calcular_optimos(int cantidad_de_niveles){
	// defino el array
	int* array_ans = (int*) calloc(5, sizeof(int));
	array_ans[0] = 0;
	array_ans[1] = 0;
	array_ans[2] = 0;
	array_ans[3] = 0;
	array_ans[4] = 0;

	int i, j, k, l, m;
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
											 printf("Entre. La suma de eficiencia actual es %d !\n", suma_eficiencia_actual);
											 printf("Los numeros son  %d , %d, %d , %d y %d \n", i, j, k, l, 20 - i - j - k - l );
											 printf("\n" );
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
								 printf("Entre. La suma de eficiencia actual es %d !\n", suma_eficiencia_actual);
								 printf("Los numeros son  %d , %d, %d y %d \n", i, j, k, 20 - i - j - k );
								 printf("\n" );
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
						printf("Entre. La suma de eficiencia actual es %d !\n", suma_eficiencia_actual);
						printf("Los numeros son  %d , %d y %d \n", i, j, 20 - i - j );
						printf("\n" );
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
			printf("Entre. La suma de eficiencia actual es %d !\n", suma_eficiencia_actual);
			printf("Los numeros son  %d y %d\n", i, (20-i) );
			printf("\n" );
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

//cortesia de https://codereview.stackexchange.com/questions/43256/binary-string-to-integer-and-integer-to-binary-string
void int2binstr(int num, char* bin){

	//char* bin =  malloc(28 * sizeof(char));


	int i;
	for(i = 0; i<28; i++){
		bin[i] = '0';
	}
	int div_entera;
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
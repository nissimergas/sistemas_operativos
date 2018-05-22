#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

typedef struct fila_tlb {
  int pagina;
	int  marco;
  int valido;
  int ultimo_uso;
} Fila_tlb;

typedef struct fila {
	struct fila** tabla_apuntada;
	int es_ultimo_nivel;
	int marco;
  int  validez;
}Fila;

typedef struct marco {
  int tiempo_sin_usar;
  char arreglo_datos[256];
  int validez;
  int pagina_correspondiente;
} Marco;

int* calcular_optimos(int cantidad_de_niveles);//funciona
Fila**crear_tabla( int nivel_actual, int* bits_nivel, int total_niveles);
Fila_tlb**crear_tlb( );
int buscar_en_tlb(Fila_tlb**tlb, unsigned int pagina );//devuelve un -1 en caso de no encontrar, devuelve el marco en caso de encontrar
int insertar_en_tlb(Fila_tlb**tlb, unsigned int pagina , unsigned int marco);//si importa el return, va a ser el numero de entrada en que introduje la pagina
void destruir_tlb(Fila_tlb**tlb);
void destruir_tablas(Fila **tabla,int nivel_actual, int* bits_nivel, int total_niveles);
int buscar_marco(Fila**tabla, int* segmentos);
void insertar_marco(Fila**tabla, int* segmentos, int marco);
void int2binstr(unsigned int num, char* bin);//funciona
void numeros_nivel(int* arreglo_resultados, int * arreglo_optimo, int numeros_nivel, char* binario);//funciona
int calcular_offset(char* binario);
void leer_disco(unsigned direccion,char *recuperar_disco);
int buscar_en_tlb_entrada_empleada(Fila_tlb**tlb, unsigned int pagina );
double calcular_peso(int* arreglo_optimo, int cantidad_de_niveles);
void actualizar_validez_marco(Fila**tabla, int* segmentos, int validez);
void actualizar_validez_tlb(Fila_tlb**tlb,unsigned int pagina );


int main(int argc, char *argv[]){
  //inicializacion del programa leer archivo

  //Cantidad de niveles
  int niveles = atoi(argv[1]);

  //fp es el archivo en el cual voy a trabajar
  //primero lo abro para contar la cantidad de lineas, es decir, cuantas direcciones tengo que leer.
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  fp = fopen(argv[2], "r");
  int cantidad_lineas=0;
  while ( getline(&line, &len, fp)!=-1) {
    cantidad_lineas++;
    }
  fclose(fp);

  //creo un array con las direcciones que debo revisar
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

//calculo el optimo
  int *optimo = calcular_optimos(niveles);


  //creacion de arrays de tablas por nivel
  Fila** instancia_tabla = crear_tabla( 0, optimo,  niveles);


  //estructura para soportar binario
  char* bin=malloc(sizeof(char)*29);
  for (i=0;i<28;i++){
    bin[i]='0';
  }
  bin[28]='\0';

  //
  char* bin2=malloc(sizeof(char)*29);
  for (i=0;i<28;i++){
    bin2[i]='0';
  }
  bin2[28]='\0';

  //creacion de la memoria fisica
Marco ** memoria_fisica=malloc(sizeof(Marco*)*256);
int w5;
for(w5 = 0; w5 < 256; w5++){
  memoria_fisica[w5]= malloc(sizeof(Marco));
  memoria_fisica[w5]-> tiempo_sin_usar = 0;
  memoria_fisica[w5]->validez = -1;
  memoria_fisica[w5]->pagina_correspondiente = -1;
}


  //Creacion de tlb
 Fila_tlb**tlb=crear_tlb();

 //Impresion de estadisticas iniciales
 printf("\n");
 printf("BITS NIVEL 1: %d \n", optimo[0]);
 if(niveles > 1){
 printf("BITS NIVEL 2: %d \n", optimo[1]);
   if(niveles > 2){
   printf("BITS NIVEL 3: %d \n", optimo[2]);
     if(niveles > 3){
     printf("BITS NIVEL 4: %d \n", optimo[3]);
       if(niveles > 4){
       printf("BITS NIVEL 5: %d \n", optimo[4]);

       }
     }
   }
 }
 double peso = calcular_peso(optimo, niveles);
 printf("ESPACIO UTILIZADO: %.6f KB \n", peso);
 printf("\n");

  //inicio simulacion con paso del tiempo
  int tiempo = 0;
  int c1;
  int resultado_busqueda_en_tlb;
  int *arreglo_resultados=calloc(niveles,sizeof(int));
  int *arreglo_resultados2=calloc(niveles,sizeof(int));
  int se_actualizo_memfis = 0; //Condicional para actualizacion ene memoria fisica
  int se_actualizo_tlb = 0; //Condicional para actualizacion en tlb
  int posicion_menos_usado = 0; //se usa solo en worst case es para memoria fisica
  int posicion_menos_usado_tlb = 0;
  int indice_entrada_tlb;

  int cantidad_page_faults = 0;
  int cantidad_tlb_hits = 0;
  char *recuperar_disco=calloc(256,sizeof(char));

  for(c1 = 0; c1 < cantidad_lineas; c1++ ){
    printf("- %d -\n", direcciones_a_revisar[c1] );
    //convierto direccion de busqueda en binario
    int2binstr(direcciones_a_revisar[c1], bin);
    //printf("Equivalente en binario a direccion que sera revisada: %s\n", bin);

    //Busco el offset
    int offset = calcular_offset(bin);
    //printf("El offset es de: %d\n", offset);

    //creo el arreglo de los indices de direcciones en los cuales buscare
    numeros_nivel( arreglo_resultados, optimo,  niveles,  bin);
    //Busco el marco en la TLB

    //TODO ver tema de validez, si funcion de Nissim funciona y que hacer ante invalidez. Este esquema fue replicado abajo con una funcion complementaria. ANte cualquier modificiacion, cambiar parte de abajao tambien
    // if(niveles == 1){
    int pag2 = (direcciones_a_revisar[c1]-(direcciones_a_revisar[c1]%256))/256;
      indice_entrada_tlb=buscar_en_tlb_entrada_empleada(tlb, pag2 );
      resultado_busqueda_en_tlb=buscar_en_tlb(tlb, pag2 );
    // }
    // else if(niveles == 2){
    //   indice_entrada_tlb=buscar_en_tlb_entrada_empleada(tlb, direcciones_a_revisar[c1] );
    //   resultado_busqueda_en_tlb=buscar_en_tlb(tlb, direcciones_a_revisar[c1] );
    // }
    // else if(niveles == 3){
    //   indice_entrada_tlb=buscar_en_tlb_entrada_empleada(tlb, direcciones_a_revisar[c1] );
    //   resultado_busqueda_en_tlb=buscar_en_tlb(tlb, direcciones_a_revisar[c1] );
    // }
    // else if(niveles == 4){
    //     indice_entrada_tlb=buscar_en_tlb_entrada_empleada(tlb, direcciones_a_revisar[c1] );
    //   resultado_busqueda_en_tlb=buscar_en_tlb(tlb, direcciones_a_revisar[c1] );
    // }
    // else {
    //   indice_entrada_tlb=buscar_en_tlb_entrada_empleada(tlb, direcciones_a_revisar[c1] );
    //   resultado_busqueda_en_tlb=buscar_en_tlb(tlb, direcciones_a_revisar[c1] );
    // }


    //No se encontro la pagina buscada en la tlb
    if(resultado_busqueda_en_tlb == -1){
      //printf("Pagina no encontrada en la tlb\n");


      int busqueda_marco = buscar_marco(instancia_tabla, arreglo_resultados);
      //printf("Busqueda marco %d\n", busqueda_marco);
      //worst path, no esta en memoria
      if(busqueda_marco == -1){
        //printf("El marco no esta en memoria \n");
        cantidad_page_faults ++;
        //primero debo leer el data.bin y extraer el marco que corresponda
        leer_disco(direcciones_a_revisar[c1],recuperar_disco);
        //Inserto el marco en la memoria_fisica
        int w8;
        for(w8=0; w8 < 256; w8++){
          if(memoria_fisica[w8]->tiempo_sin_usar > memoria_fisica[posicion_menos_usado]->tiempo_sin_usar){
            posicion_menos_usado = w8;
          }
        }
        //printf("Posicion menos usada %d\n", posicion_menos_usado);
        int w9;
        for(w9 = 0; w9 < 256; w9++){
          memoria_fisica[posicion_menos_usado]->arreglo_datos[w9] = recuperar_disco[w9];
          memoria_fisica[posicion_menos_usado]->validez = 1;
        }
        se_actualizo_memfis = 1;
        //TODO meterme con bits de validez, al sacar cosas
        //Actualizar validez
        int p =  memoria_fisica[posicion_menos_usado]->pagina_correspondiente;
        if(p != -1){
          int2binstr(p, bin2);
          numeros_nivel( arreglo_resultados2, optimo,  niveles,  bin2);
          actualizar_validez_marco(instancia_tabla, arreglo_resultados2 , -1);
          actualizar_validez_tlb(tlb,p );
        }


        //Inserto el marco correpondiente en la tabla
        insertar_marco(instancia_tabla, arreglo_resultados,posicion_menos_usado);

        //TODO algoritmos de reemplazo cuando cosas estan llenas

        //inserto marco en tlb
        int pag = (direcciones_a_revisar[c1]-(direcciones_a_revisar[c1]%256))/256;
        //printf("Pagina: %d\n", pag );
        //printf("Marco: %d\n", posicion_menos_usado );
        indice_entrada_tlb = insertar_en_tlb (tlb, pag , posicion_menos_usado);
        se_actualizo_tlb = 1;

        //Imprimo el Contenido
        printf("DIRECCION FISICA: %d \n", posicion_menos_usado*256+offset);
        unsigned char contenido = memoria_fisica[posicion_menos_usado]->arreglo_datos[offset];
        printf("CONTENIDO: %d \n", contenido);

        //int insertar_en_tlb(Fila_tlb**tlb,int pagina , int marco);
        //insertar_en_tlb(tlb, indice_entrada_tlb , posicion_menos_usado);//no importa el return

      }

      //No se encontro en tlb, pero esta en memoria (segundo happiest path)
      else {
        //printf("El marco se encuentra en memoria \n");
        //Asumo que la validez es correcta
        printf("DIRECCION FISICA: %d \n", busqueda_marco*256+offset);
        unsigned char contenido = memoria_fisica[busqueda_marco]->arreglo_datos[offset];
        printf("CONTENIDO: %d \n", contenido);

        //Inserto entrada en tlb
        int pag = (direcciones_a_revisar[c1]-(direcciones_a_revisar[c1]%256))/256;
        indice_entrada_tlb = insertar_en_tlb (tlb, pag , busqueda_marco);
        se_actualizo_tlb = 1;
      }
    }
    //Se encontro la pagina buscada en la tlb (Happiest path)
    else{
      //printf("Pagina encontrada en la tlb \n");
      cantidad_tlb_hits ++;
      //Asumo que la validez es correcta
      printf("DIRECCION FISICA: %d \n", resultado_busqueda_en_tlb*256+offset);
      unsigned char contenido = memoria_fisica[resultado_busqueda_en_tlb]->arreglo_datos[offset];
      printf("CONTENIDO: %d \n", contenido);
      se_actualizo_tlb = 1;
    }

    //Aumentar el tiempo que llevan en memoria las entradas de la memoria fisica
    int w7;
    for(w7 = 0; w7 < 256; w7++){
      memoria_fisica[w7]-> tiempo_sin_usar ++;
    }
    //El que acaba de ser usado actualiza su tiempo
    if(se_actualizo_memfis == 1){
      memoria_fisica[posicion_menos_usado]->tiempo_sin_usar = 0;
      se_actualizo_memfis = 0;
    }
    //Aumentar el tiempo que llevan en la tlb las entradas
    int w10;
    for(w10 = 0; w10 < 64; w10++){
      tlb[w10]->ultimo_uso ++;
    }
    //El que acaba de ser usado actualiza su tiempo
    if(se_actualizo_tlb == 1){
      tlb[indice_entrada_tlb]->ultimo_uso = 0;
      se_actualizo_tlb = 0;
    }


    printf("\n");
    printf("\n");
    printf("\n");
    tiempo ++;
  }


//impresion de estadisticas
float porcentaje_page_faults = (float)cantidad_page_faults/(float)tiempo*100;
float porcentaje_hits = (float)cantidad_tlb_hits/(float)tiempo*100;
printf("PORCENTAJE_PAGE_FAULTS = %.0f %% \n", porcentaje_page_faults);
printf("PORCENTAJE_TLB_HITS = %.0f %% \n", porcentaje_hits);
printf("TLB \n");
printf("i   ");
printf("n1_number ");
if(niveles > 1){
printf("n2_number ");
  if(niveles > 2){
  printf("n3_number ");
    if(niveles > 3){
    printf("n4_number ");
      if(niveles > 4){
      printf("n5_number ");

      }
    }
  }
}
printf("frame_number");
printf("\n" );


//impresion de TLB
int fila_print;
for(fila_print = 0; fila_print < 64; fila_print ++){
  printf("%d      ", fila_print);

  int2binstr(256 * tlb[fila_print]->pagina, bin);
  numeros_nivel( arreglo_resultados, optimo,  niveles,  bin);
  printf("%d      ", arreglo_resultados[0]);
  if(niveles > 1){
  printf("%d      ", arreglo_resultados[1]);
    if(niveles > 2){
    printf("%d     ", arreglo_resultados[2]);
      if(niveles > 3){
      printf("%d      ", arreglo_resultados[3]);
        if(niveles > 4){
        printf("%d      ", arreglo_resultados[3]);

        }
      }
    }
  }
  printf("%d", tlb[fila_print]->marco);
  printf("\n");
}

//liberar memoria
  destruir_tlb(tlb);
  destruir_tablas( instancia_tabla,0, optimo,  5);
  int w6;
  for(w6 = 0; w6 < 256; w6++){
    free(memoria_fisica[w6]);
  }
  free(optimo);
  free(bin);
  free(memoria_fisica);
  free(arreglo_resultados);
  free(direcciones_a_revisar);
  free(line);
  free(recuperar_disco);
  free(bin2);
  free(arreglo_resultados2);
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

void int2binstr(unsigned int num, char* bin){
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
		//printf ( "ao: %i     ",arreglo_optimo[w1]);
		char* string_acotado = calloc((arreglo_optimo[w1]+5), sizeof(char));
		//har string_acotado[arreglo_optimo[w1]];
		string_acotado[w1]= '\0';
		for(w2 = 0; w2 < arreglo_optimo[w1];w2++){
      //printf("w2: %i nivel:%i ",w2, w1);
			string_acotado[w2]=binario[s];
			s++;
		}
		//acumulador = acumulador + w1;
		//printf("string acotado: %s   \n",string_acotado);
		int a = strtol(string_acotado, NULL, 2);
    arreglo_resultados[w1]=a;
		//printf("dir por nivel :%d   ",a);
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
      tabla[i]->validez=-1;
      tabla[i]->marco=0;
      tabla[i]->tabla_apuntada=crear_tabla( nivel_actual+1,bits_nivel, total_niveles );
    }
  }
  else{
    for(i=0;i<largo;i++){
      tabla[i]=(Fila*) calloc(1,sizeof(Fila));
      tabla[i]->es_ultimo_nivel=1;
      tabla[i]->validez=-1;
      tabla[i]->marco=0;

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
  //TODO creo que esta funcion (y posiblemente las siguientes) son las que arrojan error con direcciones altas
  //printf("La longitud de los segmentos es de %lu\n", sizeof(segmentos));
	int i;
	i= 0;
  while(tabla[0]->es_ultimo_nivel != 1){
  	tabla = tabla[segmentos[i]]->tabla_apuntada;
    i++;
  }
  if(tabla[segmentos[i]]->validez==1){
    return tabla[segmentos[i]]->marco;
  }
  return -1;
}

void insertar_marco(Fila**tabla, int* segmentos, int nmarco){
  int i;

  i= 0;
  while(tabla[0]->es_ultimo_nivel != 1){
    tabla = tabla[segmentos[i]]->tabla_apuntada;
    i++;
  }
   tabla[segmentos[i]]->marco=nmarco;
   tabla[segmentos[i]]->validez=1;
  }

  Fila_tlb**crear_tlb(){
    Fila_tlb** tabla=(Fila_tlb**) calloc(64,sizeof(Fila_tlb*));
    int i;
    for(i=0;i<64;i++){
      tabla[i]=(Fila_tlb*) calloc(1,sizeof(Fila_tlb));
      tabla[i]->marco=0;
      tabla[i]->pagina=-1;
      tabla[i]->valido=-1;
      tabla[i]->ultimo_uso=0;
    }
    return tabla;
  }
  int buscar_en_tlb(Fila_tlb**tlb, unsigned int pagina ){
    int i;
    for(i=0;i<64;i++){
      if(tlb[i]->pagina==pagina){
        if(tlb[i]->valido==1){
          return tlb[i]->marco;
        }
      }
    }
    return -1;
  }
//complemento a la funcion anterior, me dice que entrada use
  int buscar_en_tlb_entrada_empleada(Fila_tlb**tlb, unsigned int pagina ){
    int i;
    for(i=0;i<64;i++){
      if(tlb[i]->pagina==pagina){
        if(tlb[i]->valido==1){
          return i;
        }
      }
    }
    return -1;
  }


int insertar_en_tlb(Fila_tlb**tlb,unsigned int pagina, unsigned int marco ){

  int i;
  for(i=0;i<64;i++){
    if(tlb[i]->valido==-1){
      tlb[i]->valido=1;
      tlb[i]->pagina=pagina;
      tlb[i]->marco=marco;
      //printf("Marco tlb: %d y pagina tlb: %d \n", tlb[i]->marco, tlb[i]->pagina);
      return i;
    }
  }
  int min=100000000;//lo inserto en el lru
  for(i=0;i<64;i++){
    if(tlb[i]->ultimo_uso<min){
      min=i;
    }
  }
  //printf("Minimo  %d \n", min);
  tlb[min]->valido=1;
  //printf("M \n");
  tlb[min]->pagina=pagina;
  //printf("M2 \n");
  tlb[min]->marco=marco;
  //printf("Marco tlb: %d y pagina tlb: %d \n", tlb[min]->marco, tlb[min]->pagina);
  return min;
}
  void destruir_tlb(Fila_tlb**tlb ){
    int i;
    for(i=0;i<64;i++){
      free(tlb[i]);
    }
    free(tlb);
  }

  int calcular_offset(char* binario){
  	char* string_acotado = calloc(9, sizeof(char));
    int w3;
    int w4 = 0;
    for(w3 = 20; w3<28; w3++){
      string_acotado[w4] = binario[w3];
      w4 ++;
    }
    int a = strtol(string_acotado, NULL, 2);
    free(string_acotado);
    return a;
  }

  void leer_disco(unsigned int direccion,char *recuperar_disco){
    direccion=direccion-(direccion%256);
    //printf("direccion: %i \n",direccion );
    unsigned char* buffer=calloc(256,sizeof(char));
    FILE *fp;
    fp = fopen("data.bin", "r");
    int i=0;
    fseek(fp, direccion, SEEK_SET);
    //fread(buffer, sizeof(buffer), 1, fp);
    //fread(buffer, sizeof(buffer), 256, fp);
    for(i=0;i<256;i++){
    fseek(fp, direccion+i, SEEK_SET);
    fread(buffer, sizeof(buffer), 1, fp);
      //fseek(fp, direccion, SEEK_SET);
      //fread(buffer, sizeof(buffer), 256, fp);
      uint a=buffer[0];
     //printf("%direccion i :%i\n",direccion+i, a);
      recuperar_disco[i]=a;
    }
    free(buffer);
    fclose(fp);
  }
double calcular_peso(int* arreglo_optimo, int cantidad_de_niveles){
  float w13 = 7.5;
  if(cantidad_de_niveles == 1){
    w13 = pow(2,arreglo_optimo[0])*11;
  }
  else if(cantidad_de_niveles == 2){
		w13 = pow(2,arreglo_optimo[0])*arreglo_optimo[1] + pow(2,arreglo_optimo[1])*11;
  }
  else if(cantidad_de_niveles == 3){
    w13 = pow(2,arreglo_optimo[0])*arreglo_optimo[1] + pow(2,arreglo_optimo[1])*arreglo_optimo[2]+pow(2,arreglo_optimo[2])*11;
  }
  else if(cantidad_de_niveles == 4){
    w13 = pow(2,arreglo_optimo[0])*arreglo_optimo[1] + pow(2,arreglo_optimo[1])*arreglo_optimo[2]+pow(2,arreglo_optimo[2])*arreglo_optimo[3]+pow(2,arreglo_optimo[3])*11;
  }
  else {
    w13 = pow(2,arreglo_optimo[0])*arreglo_optimo[1] + pow(2,arreglo_optimo[1])*arreglo_optimo[2]+pow(2,arreglo_optimo[2])*arreglo_optimo[3]+pow(2,arreglo_optimo[3])*arreglo_optimo[4]+pow(2,arreglo_optimo[4])*11;
  }
  w13 = w13*0.000125;
  return w13;
}

void actualizar_validez_marco(Fila**tabla, int* segmentos, int validez){
  //TODO creo que esta funcion (y posiblemente las siguientes) son las que arrojan error con direcciones altas
  //printf("La longitud de los segmentos es de %lu\n", sizeof(segmentos));
	int i;
	i= 0;
  while(tabla[0]->es_ultimo_nivel != 1){
  	tabla = tabla[segmentos[i]]->tabla_apuntada;
    i++;
  }
  tabla[segmentos[i]]->validez= validez;
}
void actualizar_validez_tlb(Fila_tlb**tlb,unsigned int pagina ){
  int encontrado=-1;
  int i;
  for(i=0;i<64;i++){
    if(tlb[i]->pagina==pagina){
      if(tlb[i]->valido==1){
        encontrado=i;
      }
    }
  }
  if (encontrado !=-1){
  tlb[encontrado]->valido=-1;
}
}

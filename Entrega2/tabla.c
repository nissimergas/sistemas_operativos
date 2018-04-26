#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

typedef struct fila {
	struct fila** tabla_apuntada;
	int es_ultimo_nivel;
	int marco;
  int  validez;
}Fila;

Fila**crear_tabla( int nivel_actual, int* bits_nivel, int total_niveles);
void destruir_tablas(Fila **tabla,int nivel_actual, int* bits_nivel, int total_niveles);
int main(){

  int bitspornivel[3];
  bitspornivel[0]=3;
  bitspornivel[1]=2;
  bitspornivel[2]=4;
  int total_niveles=3;
  Fila** t=crear_tabla( 0, bitspornivel,  total_niveles);
  destruir_tablas( t,0, bitspornivel,  total_niveles);
return 0;
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
      tabla[i]->tabla_apuntada=crear_tabla( nivel_actual+1,bits_nivel, total_niveles );
    }
  }
  else{
    for(i=0;i<largo;i++){
      tabla[i]=(Fila*) calloc(1,sizeof(Fila));
      tabla[i]->es_ultimo_nivel=1;
      tabla[i]->validez=0;
      tabla[i]->marco=-1;

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

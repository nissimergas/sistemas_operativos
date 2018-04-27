#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  //inicializacion del programa
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
  int l=0;
  int i=0;

  for(i=0;i<cantidad_lineas;i++) {
      int numero;
      fscanf(fp, "%d", &numero);
      direcciones_a_revisar[i]=numero;


  }

  fclose(fp);
  for(l=0;l<cantidad_lineas;l++){
    printf("valor: %i\n",direcciones_a_revisar[l]);
  }
  free(direcciones_a_revisar);
  free(line);
  return 0;
}

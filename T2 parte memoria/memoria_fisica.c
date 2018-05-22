#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

typedef struct marco {
  int pagina;
  char* arreglo_datos;
} Marco;

int main(){
  Marco * memoria_fisica=malloc(sizeof(Marco)*256);
  return 0;
}

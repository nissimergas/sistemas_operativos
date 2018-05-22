#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

int calcular_offset(char* binario);

int main(){
  char* bin1;
  char* bin2;
  bin1 = "0000000000000000000011111111";
  bin2 = "1111111111111111111100000000";
  int offset = 3;
  offset = calcular_offset(bin1);
  printf("%d \n", offset);
  offset = calcular_offset(bin2);
  printf("%d\n", offset);
  return 0;
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

#include <stdio.h>
#include <stdlib.h>

void leer_disco(int direccion,int *recuperar_disco);
int main(){
int *recuperar_disco=calloc(256,sizeof(int));
leer_disco(10105,recuperar_disco);
int i=0;
for(i=0;i<256;i++){
  //printf(" valor: %i \n", recuperar_disco[i]);
}
//leer_disco(26843545,recuperar_disco);
 i=0;
for(i=0;i<256;i++){
 //printf(" valor: %i \n", recuperar_disco[i]);
}
free(recuperar_disco);

return 0;
}

void leer_disco(int direccion,int *recuperar_disco){
  direccion=direccion-(direccion%256);
  printf("direccion: %i",direccion );
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
    int a=buffer[0];
   printf("%direccion i :%i\n",direccion+i, a);
    recuperar_disco[i]=a;
  }
  free(buffer);
  fclose(fp);
}

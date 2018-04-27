#include <stdio.h>
#include <stdlib.h>

void leer_disco(int direccion,int *recuperar_disco);
int main(){
int *recuperar_disco=calloc(256,sizeof(int));
//leer_disco(5,recuperar_disco);
int i=0;
for(i=0;i<256;i++){
  //printf(" valor: %i \n", recuperar_disco[i]);
}
leer_disco(26843545,recuperar_disco);
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
  unsigned char buffer[1];
  FILE *fp;
  fp = fopen("data.bin", "r");
  int i=0;
  fseek(fp, direccion, SEEK_SET);
  for(i=0;i<256;i++){
    //fseek(fp, direccion, SEEK_SET);
    fread(buffer, sizeof(buffer), 1, fp);
    int a=*buffer;
   printf("e %u ue\n", *buffer);
    recuperar_disco[i]=a;
  }
  fclose(fp);
}

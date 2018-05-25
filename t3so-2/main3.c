#include <stdio.h>
#include "t3.h"

int main(int argc, char *argv[])
{
  //Conservar
  fp = fopen(argv[1], "rb+");
  bloques_bm=abrir_bloques_bitmap(fp);
  bl_direc=abrir_bloque_directorio(fp);

  //Testing


  cz_ls();

  // czFILE* archivo = cz_open("texto.txt",'r');
  // if (archivo!=NULL){
  //   printf("sadasdsadsadsa");
  //   cz_read( archivo, ch, 2000);
  //   printf("text: %s\n",ch);
  //  //cz_write(archivo, ch, 2048);
  //   if(archivo!=NULL){
  //     printf("not null\n");
  //   cz_close(archivo);
  //   }
  // }
  cz_rm("texto.txt");
  cz_ls();

  //Conservar
  copiar_bloque_directorio_disco(fp, bl_direc , 0);
  cerrar_bloque_directorio(bl_direc);
  escribir_bloques_bitmap(fp);
  free(bloques_bm->bits);
  free(bloques_bm);
  fclose(fp);
  return 0;
}

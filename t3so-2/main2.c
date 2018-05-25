// Libreria de input y output (para leer y escribir archivos o leer y escribir en consola)
#include <stdio.h>

// Importo los archivos .h de las listas
#include "t3.h"


// Recibe como input los parametros del programa en un arreglo de strings y un numero que indica cuantos argumentos son
//Directorios*bl_direc;
int main(int argc, char *argv[])
{
  fp = fopen(argv[1], "rb+");
  bloques_bm=abrir_bloques_bitmap(fp);
  bl_direc=abrir_bloque_directorio(fp);

  char *ch=(char*)calloc(2000,1);
  int i;
  for (i=0;i<100;i++){
    //ch[i]='a';
  }
  czFILE* archivo = cz_open("p5.txt",'r');
  if (archivo!=NULL){

    //cz_write(archivo, ch, 2048);
    for (i=0;i<400;i++){
    cz_read( archivo, ch, 1020);

    printf("text: %s\n",ch);
    }
   //cz_write(archivo, ch, 2048);
    //free(ch);
    if(archivo!=NULL){
      //printf("not null\n");
    cz_close(archivo);
    }
  }
free(ch);
  //cz_close(archivo);
  //Esto no cambia

  copiar_bloque_directorio_disco(fp, bl_direc , 0);
  cerrar_bloque_directorio(bl_direc);
  escribir_bloques_bitmap(fp);
  free(bloques_bm->bits);
  free(bloques_bm);
  fclose(fp);
  return 0;
}
//871424

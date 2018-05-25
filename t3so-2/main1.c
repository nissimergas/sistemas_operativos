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
 char *ch=(char*)calloc(2000,sizeof(char));
  ch[1999]='\0';
 int i;
  for (i=0;i<100;i++){
    ch[i]='s';
  }
  for (i=100;i<250;i++){
    ch[i]='s';
  }
  for (i=250;i<1800;i++){
    ch[i]='s';
  }
/*  for (i=3000;i<3200;i++){
    ch[i]='d';
  }*/

    ch[i]='\0';
  cz_rm("p1.txt");
  czFILE* archivo = cz_open("p1.txt",'w');

  if (archivo!=NULL){
  for(i=0;i<500;i++){
    printf("%i: \n",i);


   cz_write(archivo, ch, 1024);
}
    if(archivo!=NULL){
      printf("not null\n");
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

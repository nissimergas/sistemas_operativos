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


  cz_ls();


  copiar_bloque_directorio_disco(fp, bl_direc , 0);
  cerrar_bloque_directorio(bl_direc);
  escribir_bloques_bitmap(fp);
  free(bloques_bm->bits);
  free(bloques_bm);
  fclose(fp);
  return 0;
}

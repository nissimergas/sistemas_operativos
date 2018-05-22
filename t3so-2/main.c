// Libreria de input y output (para leer y escribir archivos o leer y escribir en consola)
#include <stdio.h>

// Importo los archivos .h de las listas
#include "t3.h"


// Recibe como input los parametros del programa en un arreglo de strings y un numero que indica cuantos argumentos son
//Directorios*bl_direc;
int main(int argc, char *argv[])
{
  // Retorna 0 cuando la ejecucion del programa es correcta
  //TODO: preguntar a Nissim si estoy compilando bien , pues puede afectar que voy a abrir
  //TODO: preguntar a Nissim, me parece que nuestro archivo de test debiera ser un .txt y no un .bin (puede que de lo msimo en realidad)
  //TODO: preguntar a Nissim, como cambiar el bitmap de acuerdo con el  disco incial que me dan (fp?)
  //TODO: preguntar a Nissim, Es fp un disco o un archivo segun su mail? Entiendo que debiera ser un leer_int_disco
  //TODO: preguntar a Nissim como funciona el remove con bloque de direccionamiento indiecto

  //Esta linea no se puede quitar
  fp = fopen(argv[1], "rb+");
  bloques_bitmap=abrir_bloques_bitmap(fp);
  bl_direc=abrir_bloque_directorio(fp);
  //Estas lineas son las que cambian
  czFILE * file_desc;
  file_desc = cz_open("ejemplo.txt", 'w');
  cz_ls();
  cz_rm("23456789AB");
  cz_close(file_desc);

  //Esto no se puede cambiar
  fclose(fp);
  cerrar_bloque_directorio(bl_direc);
  free(file_desc->punteros);
  free(file_desc);
  free(bloques_bitmap);

  return 0;
}

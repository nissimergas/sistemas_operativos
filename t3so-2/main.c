// Libreria de input y output (para leer y escribir archivos o leer y escribir en consola)
#include <stdio.h>

// Importo los archivos .h de las listas
#include "t3.h"


// Recibe como input los parametros del programa en un arreglo de strings y un numero que indica cuantos argumentos son
//Directorios*bl_direc;
int main(int argc, char *argv[])
{
  // Retorna 0 cuando la ejecucion del programa es correcta
  //FILE *fp;
  fp = fopen("j2.bin", "rb+");
  char s[11]="sdsds";

  bl_direc=abrir_bloque_directorio(fp);
  cz_exists(s);
  escribir_int_disco(fp, 0,  0);
  escribir_int_disco(fp, 4,  0);
  escribir_int_disco(fp, 8,  0);
  escribir_int_disco(fp, 0,  0);
  escribir_int_disco(fp, 10,  0);
  escribir_int_disco(fp, 9,  0);
  char str[12]="123456789AB\0";
  escribir_string_disco( fp,  0,  str,  12);
  escribir_char_disco( fp,  0,  1);
  escribir_char_disco( fp,  30,  2);
  fclose(fp);
  fp = fopen("j2.bin", "rb+");

char* s2 =leer_string_disco( fp,  0,  12);
printf("string: %s \n",str);
free(s2);
  fclose(fp);
  //cz_directories("test.bin");
  cerrar_bloque_directorio(bl_direc);
  return 0;
}

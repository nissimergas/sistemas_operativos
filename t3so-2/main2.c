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
  /*
  fp = fopen("j4.bin", "rb+");
   escribir_int_disco(  fp,  1, 557060437);
   int n=leer_int_disco( fp,  12);
   printf("n: %i    ...",n);
   fclose(fp);
   */

  fp = fopen(argv[1], "rb+");
  bloques_bm=abrir_bloques_bitmap(fp);
  bl_direc=abrir_bloque_directorio(fp);

  //Estas lineas son las que cambian
  //cz_rm("alberto2.txt");
  cz_ls();
//czFILE* archivo = cz_open("ejemplo.txt",'r');
//  cz_write( archivo, "12345678901234567890\0", 21);
  char *ch=(char*)calloc(2048,1);
  //ch[100]='\0';
  int i;
  for (i=0;i<100;i++){
    //ch[i]='a';
  }
  czFILE* archivo = cz_open("hhh.txt",'r');
  if (archivo!=NULL){
    printf("sadasdsadsadsa");
    cz_read( archivo, ch, 2000);
    printf("text: %s\n",ch);
   //cz_write(archivo, ch, 2048);
    free(ch);
    if(archivo!=NULL){
      printf("not null\n");
    cz_close(archivo);
    }
  }

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

// Esta linea sirve para que el código de este archivo solo se importe una vez
#include <stdio.h>
#include <stdlib.h>
#pragma once

struct bloque_datos{
  char *datos;//1024
  int posicion_a_escribir;
};

struct bloque_dir_indirecto{
  int* punteros;//256
  int pos_a_escribir;
};
/** Estructura de una arraylist */
struct czfile
{
  unsigned int size;
  int indice; //bloque indice
  int f_cracion;
  int f_modificacion;
  int *punteros;
  int puntero_dir_indirecto;
  int ultimo_read;
  char mode;
  int leyendo;
  struct bloque_datos* *bloques_de_datos;


};

struct directorio
{
  char validez;
  char* nombre;//11
  int bloque_indice;

};


struct bloque_directorios
{
  struct directorio* *directorios; //debe tener 64 directorios

};

//considero los 8 bloques de bitmap como una unidad
struct conjunto_bloques_bitmap
{
  char* bits ;//[65536]; //debe tener 65536 bits

};

// Aquí le estoy poniendo un nombre más simple a la lista para no tener que
//
/** Estructura de una arraylist */
typedef struct czfile czFILE;
typedef struct directorio Directorio;
typedef struct bloque_directorios Directorios;
typedef struct bloque_datos b_datos;
//typedef struct bit_for_bitmap Bit_for_bitmap;
typedef struct conjunto_bloques_bitmap Conjunto_bloques_bitmap;
//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

//OJO: No se debe modificar nada de esto
/** abrir archivo, modo:r, w, si se escoge w el nombre no debe estar ocupado */

/** abrir archivo, modo:r, w, si se escoge w el nombre no debe estar ocupado */
void escribir_int_disco( FILE *fp, int pos, int numero);
void escribir_char_disco( FILE *fp, int pos, char caracter);
void escribir_string_disco( FILE *fp, int pos, char* str, int size_of_str);
int leer_int_disco( FILE *fp, int pos);
int espacio_libre_directorio( );//devuelve el espacio de directorio libre
int espacio_libre_bitmap( );// devuelve el primer bloque libre
char leer_char_disco( FILE *fp, int pos);
char* leer_string_disco( FILE *fp, int pos, int size_of_str);
void copiar_bloque_directorio_disco(FILE *fp,Directorios* bloque_dir ,int pos);
int devolver_bloque_indice(char*filename);
Directorios* abrir_bloque_directorio(FILE *fp);
void cerrar_bloque_directorio(Directorios*bloque);
czFILE* cz_open(char*filename, char mode);
//Crecaion de bloques bitmap
Conjunto_bloques_bitmap* abrir_bloques_bitmap(FILE *fp);

/** retorn 1 si filname existe, 0 en caso contrario*/
int cz_exists2(char*filename,Directorios*bloque);
int cz_exists(char*filename);

/** lee n_bytes desde el archivo y los copia en buffer*/
int cz_read(czFILE*file_desc, char* buffer, int nbytes);

/** escribe en el archivo lo que hay en buffer (nbytes) */
int cz_write(czFILE*file_desc, char* buffer, int nbytes);

/** cierra el archivo */
int cz_close(czFILE* file_desc);

/** cambia el nombre del archivo */
int cz_mv(char*orig, char*dest);

/**copia */
int cz_cp(char*orig, char*dest);

/** borra */
int cz_rm(char* filename);

void char2binstr(char num, char* bin);
int cz_read_cp(czFILE*file_desc, char* buffer, int nbytes);
void escribir_bloques_bitmap(FILE *fp);

/** ls */
void cz_ls();
Directorios*bl_direc;
FILE *fp;
Conjunto_bloques_bitmap* bloques_bitmap;

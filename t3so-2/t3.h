// Esta linea sirve para que el código de este archivo solo se importe una vez
#include <stdio.h>
#include <stdlib.h>
#pragma once


/** Estructura de una arraylist */
struct czfile
{
  int size;
  int indice; //bloque indice
  int f_cracion;
  int f_modificacion;
  int *punteros;
  int puntero_dir_indirecto;
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

// Aquí le estoy poniendo un nombre más simple a la lista para no tener que
//
/** Estructura de una arraylist */
typedef struct czfile czFILE;
typedef struct directorio Directorio;
typedef struct bloque_directorios Directorios;
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
char leer_char_disco( FILE *fp, int pos);
char* leer_string_disco( FILE *fp, int pos, int size_of_str);
void copiar_bloque_directorio_disco(FILE *fp,Directorios* bloque_dir ,int pos);
int devolver_bloque_indice(char*filename);
Directorios* abrir_bloque_directorio(FILE *fp);
void cerrar_bloque_directorio(Directorios*bloque);
czFILE* cz_open(char*filename, char mode);

/** retorn 1 si filname existe, 0 en caso contrario*/
int cz_exists2(char*filename,Directorios*bloque);
int cz_exists(char*filename);

/** lee n_bytes desde el archivo y los copia en buffer*/
int cz_read(czFILE*file_desc, void* buffer, int nbytes);

/** escribe en el archivo lo que hay en buffer (nbytes) */
int cz_write(czFILE*file_desc, void* buffer, int nbytes);

/** cierra el archivo */
int cz_close(czFILE* file_desc);

/** cambia el nombre del archivo */
int cz_mv(char*orig, char*dest);

/**copia */
int cz_cp(char*orig, char*dest);

/** borra */
int cz_rm(char* filename);

/** ls */
void cz_ls();
Directorios*bl_direc;
FILE *fp;

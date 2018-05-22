// Importo el archivo arraylist.h
#include "t3.h"
// Libreria estandar de C
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

// TODO: Debes completar las siguientes funciones
// Puedes crear otras funciones aca para el
// funcionamiento interno del arreglo dinamico
Directorios*bl_direc;
FILE* fp;

void escribir_int_disco( FILE *fp, int pos, int numero){
unsigned int buffer[1];
  buffer[0]=numero;
  fseek(fp, pos, SEEK_SET);
  fwrite(buffer,4,1,fp);

}
void escribir_char_disco( FILE *fp, int pos, char caracter){
  unsigned char buffer[1];
    buffer[0]=caracter;
    fseek(fp, pos, SEEK_SET);
    fwrite(buffer,1,1,fp);
  }

void escribir_string_disco( FILE *fp, int pos, char* str, int size_of_str){
      fseek(fp, pos, SEEK_SET);
      fwrite(str,1,size_of_str,fp);
    }

int leer_int_disco( FILE *fp, int pos){
  unsigned int buffer[1];
  fseek(fp, pos, SEEK_SET);
  fread(buffer, 4, 1, fp);
  return buffer[0];
}
char leer_char_disco( FILE *fp, int pos){
  unsigned char buffer[1];
  fseek(fp, pos, SEEK_SET);
  fread(buffer, 1, 1, fp);
  return buffer[0];
  }

char* leer_string_disco( FILE *fp, int pos, int size_of_str){
  char* buffer=(char*)calloc(size_of_str,sizeof(char));
  fseek(fp, pos, SEEK_SET);
  fread(buffer, 1, size_of_str, fp);
  return buffer;
}

Directorios* abrir_bloque_directorio(FILE *fp){
  Directorios* bloque_dir=(Directorios*)calloc(1,sizeof(Directorios));
  int i=0;
  int pos=0;
  bloque_dir->directorios=(Directorio**)calloc(64,sizeof(Directorio*));
  for (i=0;i<64;i++){
    Directorio* d=(Directorio*)calloc(1,sizeof(Directorio));
    d->validez=leer_char_disco( fp,  pos);
    pos=pos+1;
    d->nombre=leer_string_disco(fp,  pos, 11);//podria ser 12
    pos+=11;
    d->bloque_indice=leer_int_disco( fp,  pos);
    pos+=4;

    bloque_dir->directorios[i]=d;
  }
  return bloque_dir;
}

Conjunto_bloques_bitmap* abrir_bloques_bitmap(FILE *fp){
  Conjunto_bloques_bitmap* bloques_bm=(Conjunto_bloques_bitmap*)calloc(1,sizeof(Conjunto_bloques_bitmap));
  //Asumimos que bloques estan ya con los 9 primeros bits no disponibles para escribir
  int i=0;
  for (i=0;i<9;i++){
    bloques_bm->bits[i] = 1;
  }
  for (i=9;i<65536;i++){
    bloques_bm->bits[i] = 0;
  }
  return bloques_bm;
}


void cerrar_bloque_directorio(Directorios*bloque){
  int i=0;
  for (i=0;i<64;i++){
    free(bloque->directorios[i]->nombre);
    free(bloque->directorios[i]);
  }
  free(bloque->directorios);
  free(bloque);

}
void copiar_bloque_directorio_disco(FILE *fp,Directorios* bloque_dir ,int pos){
  int i=0;

  for (i=0;i<64;i++){
    Directorio* d=bloque_dir->directorios[i];
    //Directorio* d=(Directorio*)calloc(1,sizeof(Directorio));
    escribir_char_disco(  fp,  pos,d->validez);
    pos=pos+1;
    escribir_string_disco(fp,  pos, d->nombre,  11);
    pos+=11;
    escribir_char_disco(  fp,  pos,d->bloque_indice);
    //d->bloque_indice=leer_int_disco( fp,  pos);
    pos+=4;
    //bloque_dir->directorios[i]=d;
  }
}
int comparar_string(char *s2 , char*s1,  int largo){
  int i=0;
  for(i=0;i<largo;i++){
    if(s1[i]!=s2[i]){
      return 0;
    }
  }
  return 1;
}

int cz_exists2(char*filename,Directorios*bloque){
  int i=0;

  for (i=0;i<64;i++){
    Directorio* d=bloque->directorios[i];
    int s=comparar_string(filename , d->nombre,  11);
    if(s==1 && d->validez==1){
      return 1;
    }
  }
  return 0;
}

int devolver_bloque_indice(char*filename){
  int i=0;

  Directorios*bloque=bl_direc;
  for (i=0;i<64;i++){
    Directorio* d=bloque->directorios[i];
    int s=comparar_string(filename , d->nombre,  11);
    if(s==1 && d->validez==1){
      return d->bloque_indice;
    }
  }
  return 0;
}
/** retorn 1 si filname existe, 0 en caso contrario*/
int cz_exists(char*filename){
  printf("exists:%s \n",bl_direc->directorios[0]->nombre);
  return cz_exists2(filename,bl_direc);
}

/** abrir archivo, modo:r, w, si se escoge w el nombre no debe estar ocupado */
czFILE* cz_open(char*filename, char mode){
bloques_bitmap=abrir_bloques_bitmap(fp);
bl_direc=abrir_bloque_directorio(fp);
if(mode=='r'){
  int exist=cz_exists(filename);
  if(exist==1){
    czFILE* f_index =(czFILE*)calloc(1,sizeof(czFILE));
      int indice = devolver_bloque_indice(filename);
      int pos= indice*1024;

      int tamao= leer_int_disco(fp,pos);
      pos+=4;
      int creacion=leer_int_disco(fp,pos);
      pos+=4;
      int modificacion=leer_int_disco(fp,pos);
      pos+=4;
      f_index->indice=indice;
      f_index->size=tamao;
      f_index->f_cracion=creacion;
      f_index->f_modificacion=modificacion;
      int j=0;
      f_index->punteros=(int*)calloc(252,sizeof(int));
      for (j=0;j<252;j++){
        f_index->punteros[j]=leer_int_disco(fp,pos);
        pos+=4;
      }
      f_index->puntero_dir_indirecto=leer_int_disco(fp,pos);
      return f_index;

  }
  else {
      return NULL;
  }
}
if(mode=='w'){
  int exist=cz_exists(filename);
  if(exist==0){
    czFILE* f_index =(czFILE*)calloc(1,sizeof(czFILE));

      int tamao= 0;
      int creacion=1;
      int modificacion=1;
      f_index->indice=-1;
      f_index->size=tamao;
      f_index->f_cracion=creacion;
      f_index->f_modificacion=modificacion;
      int j=0;
      f_index->punteros=(int*)calloc(252,sizeof(int));
      for (j=0;j<252;j++){
        f_index->punteros[j]=-1;

      }
      f_index->puntero_dir_indirecto=-1;
      return f_index;
  }
  else {
    return NULL;
  }
}
}

/** lee n_bytes desde el archivo y los copia en buffer*/
/** DOMINGO*/
int cz_read(czFILE*file_desc, void* buffer, int nbytes){

}

/** escribe en el archivo lo que hay en buffer (nbytes) */
int cz_write(czFILE*file_desc, void* buffer, int nbytes){

}

/** cierra el archivo */
int cz_close(czFILE* file_desc)
{
  fclose(fp);
  cerrar_bloque_directorio(bl_direc);
  free(file_desc->punteros);
  free(file_desc);
  free(bloques_bitmap);
  return 0;
}

/** cambia el nombre del archivo */
int cz_mv(char*orig, char*dest){

}

/**copia */
int cz_cp(char*orig, char*dest){

}

/** borra */
int cz_rm(char* filename){
  int contador_rm;
  for(contador_rm = 0 ; contador_rm < 64; contador_rm++){
      if (bl_direc->directorios[contador_rm]->nombre == filename && bl_direc->directorios[contador_rm]->validez == 1){
        printf("Eliminando archivo %s \n", filename);
        bl_direc->directorios[contador_rm]->bloque_indice;
      }
  }
  return 0;
}

/** ls */
void cz_ls(){
  int contador_ls;
  printf("LS: \n");
  for(contador_ls = 0 ; contador_ls < 64; contador_ls++){
    if(bl_direc->directorios[contador_ls]->validez == 1){
      printf("%s \n",bl_direc->directorios[contador_ls]->nombre);
    }
  }
}

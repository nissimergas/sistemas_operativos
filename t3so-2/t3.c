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
Conjunto_bloques_bitmap* bloques_bm;
FILE* fp;

void char2binstr(char num, char* bin){
	int i;
	for(i = 0; i<8; i++){
		bin[i] = 0;
	}

	int resto;
	i = 7;
	while(num != 0){
		if(i >= 0){
		resto = num %2;
		num = (num - resto) / 2;
		if(resto == 0){
			bin[i] = 0;
		}
		else{
			bin[i] = 1;
		}
		i--;
	}
}
}

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
int espacio_libre_directorio( ){
  //bloques_bitmap=abrir_bloques_bitmap(fp);
  bl_direc=abrir_bloque_directorio(fp);
  int i=0;
  for (i=0;i<64;i++){
    Directorio* d=bl_direc->directorios[i];
    if (d->validez==0){
      return i;
    }
  }
  return -1;
}
int espacio_libre_bitmap( ){
  bloques_bitmap=abrir_bloques_bitmap(fp);
  //bl_direc=abrir_bloque_directorio(fp);
  int i=0;
  for (i=0;i<65536;i++){

    if (bloques_bitmap->bits[i]==0){
      return i;
    }
  }
  return -1;
}

void escribir_bloques_bitmap(FILE *fp){
  int i;
  for(i = 0; i<1024*8; i++){
      int a0 = Conjunto_bloques_bitmap->bits[i*8]  * pow(2,7);
      int a1 = Conjunto_bloques_bitmap->bits[i*8+1] * pow(2,6);
      int a2 = Conjunto_bloques_bitmap->bits[i*8+2] * pow(2,5);
      int a3 = Conjunto_bloques_bitmap->bits[i*8+3] * pow(2,4);
      int a4 = Conjunto_bloques_bitmap->bits[i*8+4] * pow(2,3);
      int a5 = Conjunto_bloques_bitmap->bits[i*8+5] * pow(2,2);
      int a6 = Conjunto_bloques_bitmap->bits[i*8+6] * pow(2,1);
      int a7 = Conjunto_bloques_bitmap->bits[i*8+7] * pow(2,0);
      int a = a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7;
      //Asumo que tengo hecho el byte_array de 8 bits
      char char_de_bin = a;
      escribir_char_disco(fp, 1024 + i, char_de_bin);
    }
  }
}

Conjunto_bloques_bitmap* abrir_bloques_bitmap(FILE *fp){
  Conjunto_bloques_bitmap* bloques_bm=(Conjunto_bloques_bitmap*)calloc(1,sizeof(Conjunto_bloques_bitmap));
  //Asumimos que bloques estan ya con los 9 primeros bits no disponibles para escribir
  char * arreglo_chars = (char *) calloc(1024*8, sizeof(char));
  int i;
  for(i = 0; i < 1024*8; i++){
    arreglo_chars[i] = leer_char_disco(fp, i + 1024);
  }
  char * cada_char = (char *) calloc(1024*8, sizeof(char));
  for(i = 0; i < 1024*8; i++){
    //printf("%i\n", arreglo_chars[i] );
    char2binstr(arreglo_chars[i], cada_char);
    bloques_bm->bits [8*i] = cada_char[0];
    bloques_bm->bits [8*i+1] = cada_char[1];
    bloques_bm->bits [8*i+2] = cada_char[2];
    bloques_bm->bits [8*i+3] = cada_char[3];
    bloques_bm->bits [8*i+4] = cada_char[4];
    bloques_bm->bits [8*i+5] = cada_char[5];
    bloques_bm->bits [8*i+6] = cada_char[6];
    bloques_bm->bits [8*i+7] = cada_char[7];
  }
  free(arreglo_chars);
  free(cada_char);
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
    char filename2[11]="00000000000";
    int i=0;
    while (i<11){
      if(filename[i]=='\0'){
        break;
      }
      filename2[i]=filename[i];
      i++;
    }
    Directorio* d=bloque->directorios[i];
    int s=comparar_string(filename2 , d->nombre,  11);
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
      f_index->ultimo_read=0;
      f_index->bloques_de_datos=NULL;
      f_index->f_cracion=creacion;
      f_index->leyendo=0;
      f_index->mode='r';
      f_index->f_modificacion=modificacion;

      int j=0;
      f_index->punteros=(int*)calloc(508,sizeof(int));
      for (j=0;j<252;j++){
        f_index->punteros[j]=leer_int_disco(fp,pos);
        pos+=4;
      }
      f_index->puntero_dir_indirecto=leer_int_disco(fp,pos);
      pos=f_index->puntero_dir_indirecto*1024;
      for (j=252;j<508;j++){
        f_index->punteros[j]=leer_int_disco(fp,pos);
        pos+=4;
      }
      return f_index;

  }
  else {
      return NULL;
  }
}
if(mode=='w'){
  int exist=cz_exists(filename);
  if(exist==0){
    int espacio_libre=espacio_libre_directorio();
    if(espacio_libre!=-1){
      int b_libre=espacio_libre_bitmap();
      if (b_libre!=-1){
        bloques_bitmap->bits[b_libre]=1;
          bl_direc->directorios[espacio_libre]->validez=1;
          char* n=(char*)calloc(11,sizeof(char));
          int lll=0;
          while(lll<11){
            if(filename[lll]=='\0'){
              break;
            }
            n[lll]=filename[lll];
          }
          bl_direc->directorios[espacio_libre]->nombre=n;

            czFILE* f_index =(czFILE*)calloc(1,sizeof(czFILE));

              int tamao= 0;
              int creacion=1;
              int modificacion=1;
              f_index->indice=b_libre;
              f_index->size=tamao;
              f_index->ultimo_read=0;
              f_index->f_cracion=creacion;
              f_index->f_modificacion=modificacion;
              f_index->mode='w';
              f_index->leyendo=0;
              int j=0;
              f_index->punteros=(int*)calloc(252,sizeof(int));
              for (j=0;j<508;j++){
                f_index->punteros[j]=-1;
              }
              f_index->bloques_de_datos=(b_datos**)calloc(252,sizeof(b_datos*));
              int i=0;
              for (i=0;i<508;i++){
                f_index->bloques_de_datos[i]=(b_datos*)calloc(1,sizeof(b_datos));

                  f_index->bloques_de_datos[i]->datos=(char*)calloc(1024,sizeof(char));
                  f_index->bloques_de_datos[i]->posicion_a_escribir=0;

              }
              f_index->puntero_dir_indirecto=-1;
              return f_index;
            }
      }

  }
}
return NULL;
}

/** lee n_bytes desde el archivo y los copia en buffer*/
/** DOMINGO*/
int cz_read(czFILE*file_desc, char* buffer, int nbytes){
  //buffer=(char*)calloc(nbytes,sizeof(char));
  if(file_desc->size==0 ||file_desc->mode=='w' || file_desc->ultimo_read>=file_desc->size){
    return -1;
  }
//int fin_lectura=nbytes+file_desc->ultimo_read;
//int numero_bloques_datos=(nbytes-(nbytes%1024)/1024)+1;
//int bloque_inicio=(file_desc->ultimo_read-(file_desc->ultimo_read%1024)/1024);
//int offset=file_desc->ultimo_read%1024;
int j=0;
while(nbytes>0){
  if(file_desc->leyendo>=file_desc->size){
    break;
  }
  int bloque_actual= file_desc->leyendo-(file_desc->leyendo%1024)/1024;
  int offset=file_desc->leyendo%1024;
  int bloque_en_disco;
   bloque_en_disco=file_desc->punteros[bloque_actual];
  if(bloque_actual<252){
     bloque_en_disco=file_desc->punteros[bloque_actual];
  }
  else{
    int p=file_desc->puntero_dir_indirecto*1024+((bloque_actual-252)*4);
    bloque_en_disco=leer_int_disco(fp,  p);
  }
  int pos=bloque_en_disco*1024+offset;

  buffer[j]=leer_char_disco( fp,  pos);
  j++;
  file_desc->leyendo++;
}
return 0;
}

/** escribe en el archivo lo que hay en buffer (nbytes) */
int cz_write(czFILE*file_desc, char* buffer, int nbytes){
  int j=0;
  while(nbytes>0){
    int bloque_a_escribir=(file_desc->size-(file_desc->size%1024))/1024;
    int offset=file_desc->size%1024;
    if(offset==0){
      int espacio_libre=espacio_libre_bitmap();
      if (espacio_libre==-1){
        return j;//error espacio
      }
      else{
        if(bloque_a_escribir==252){
          int espacio_libre2=espacio_libre_bitmap();
          if(espacio_libre2==-1){
            return j;//error espacio
          }
          else{
            file_desc->puntero_dir_indirecto=espacio_libre2;
          }
        }
        bloques_bm->bits[espacio_libre]=1;
        file_desc->punteros[bloque_a_escribir]=espacio_libre;
      }

    }
    file_desc->bloques_de_datos[bloque_a_escribir]->datos[offset]=buffer[j];
    file_desc->size++;
    j++;
  }
return j;
}

/** cierra el archivo */
int cz_close(czFILE* file_desc){
  if(file_desc->mode=='w'){
    int pos=file_desc->indice*1024;
    escribir_int_disco( fp,  pos, file_desc->size);
    pos+=4;
    escribir_int_disco( fp,  pos, file_desc->f_cracion);
    pos+=4;
    escribir_int_disco( fp,  pos, file_desc->f_modificacion);
    int i=0;
    for(i=0;i<252;i++){
      escribir_int_disco( fp,  pos, file_desc->punteros[i]);
      pos+=4;
    }
    escribir_int_disco( fp,  pos, file_desc->puntero_dir_indirecto);
    pos+=4;
    for(i=0;i<508;i++){
      if(file_desc->punteros[i]!=-1){
        pos=file_desc->punteros[i]*1024;
        escribir_string_disco( fp, pos,  file_desc->bloques_de_datos[i]->datos, 1024);
      }
    }
    pos=file_desc->puntero_dir_indirecto*1024;
    for(i=252;i<508;i++){
      escribir_int_disco( fp,  pos, file_desc->punteros[i]);
      pos+=4;
    }
  }
int i;
  free(file_desc->punteros);
  if(file_desc->mode=='w'){
    for(i=0;i<508;i++){
      free(file_desc->bloques_de_datos[i]->datos);
      free(file_desc->bloques_de_datos[i]);
    }
    free(file_desc->bloques_de_datos);
  }
  free(file_desc->bloques_de_datos);





    //escribir_char_disco( FILE *fp, int pos, char caracter);
  fclose(fp);
  cerrar_bloque_directorio(bl_direc);
  free(file_desc->punteros);
  free(file_desc);
  free(bloques_bitmap);
  return 0;
}

/** cambia el nombre del archivo */
int cz_mv(char*orig, char*dest){
  int j;
  if(cz_exists(dest) == 1){
      return 1;
  }
  int contador_mv;
  for(contador_mv = 0 ; contador_mv < 64; contador_mv++){
    if (bl_direc->directorios[contador_mv]->nombre == orig){
      bl_direc->directorios[contador_mv]->nombre = dest;
    }
  }
  return 0;
}
//Asumimos que todo lo que leemos son chars

//Leer sin modificar copy
int cz_read_cp(czFILE*file_desc, char* buffer, int nbytes){
  //if(file_desc->size==0 ||file_desc->mode=='w' || file_desc->ultimo_read>=file_desc->size){
  //  return -1;
  //}
int j=0;
int contador_cp = 0;
while(nbytes>0){
  if(contador_cp>=file_desc->size){
    break;
  }
  int bloque_actual= contador_cp-(contador_cp%1024)/1024;
  int offset=contador_cp%1024;
  int bloque_en_disco;
   bloque_en_disco=file_desc->punteros[bloque_actual];
  if(bloque_actual<252){
     bloque_en_disco=file_desc->punteros[bloque_actual];
  }
  else{
    int p=file_desc->puntero_dir_indirecto*1024+((bloque_actual-252)*4);
    bloque_en_disco=leer_int_disco(fp,  p);
  }
  int pos=bloque_en_disco*1024+offset;

  buffer[j]=leer_char_disco( fp,  pos);
  j++;
  contador_cp++;
}
return 0;
}





/**copia */
int cz_cp(char*orig, char*dest){
  if(cz_exists(dest) == 1){
      return 1;
  }
  if(strcmp(orig, dest) != 0){
      return 2;
  }
  czFILE* archivo_original = cz_open(orig, 'r');
  char * buffer_copia = (char *) calloc(archivo_original->size, sizeof(char));
  cz_read_cp(archivo_original, buffer_copia, archivo_original->size);

  czFILE* nuevo_archivo = cz_open(dest, 'w');
  cz_write( nuevo_archivo, buffer_copia , archivo_original->size);

  free(buffer_copia);

  return 0;
}

/** borra */
int cz_rm(char* filename){
  int contador_rm;
  for(contador_rm = 0 ; contador_rm < 64; contador_rm++){
      if (bl_direc->directorios[contador_rm]->nombre == filename && bl_direc->directorios[contador_rm]->validez == 1){
        printf("Eliminando archivo %s \n", filename);
        //indice = bl_direc->directorios[contador_rm]->bloque_indice;
        czFILE* archivo = cz_open(filename, 'r');
        int i;
        for(i = 0; i < 252; i++){
          if(archivo->punteros[i] != -1){
            bloques_bitmap->bits[archivo->punteros[i]] = 0;
          }
        }
        if(archivo->puntero_dir_indirecto != -1){
          bloques_bitmap->bits[archivo->puntero_dir_indirecto] = 0;
          for(i = 0; i < 256; i++){
            int posicion = leer_int_disco(fp, 1024 * archivo->puntero_dir_indirecto + 4*i);
            bloques_bitmap->bits[posicion] = 0;
          }
        }
        cz_close(archivo);
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

// Importo el archivo arraylist.h
#include "t3.h"
// Libreria estandar de C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

// TODO: Debes completar las siguientes funciones
// Puedes crear otras funciones aca para el
// funcionamiento interno del arreglo dinamico
Directorios*bl_direc;
Conjunto_bloques_bitmap* bloques_bm;
FILE* fp;

void char2binstr(unsigned char num, char* bin){
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
unsigned char buffer[4];
  buffer[0]=numero>>24;
  buffer[1]=numero>>16;
  buffer[2]=numero>>8;
  buffer[3]=numero;
  fseek(fp, pos, SEEK_SET);
  fwrite(buffer,1,4,fp);

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
	unsigned char c1=leer_char_disco( fp,  pos);
	pos+=1;
	unsigned char c2=leer_char_disco( fp,  pos);
	pos+=1;
	unsigned char c3=leer_char_disco( fp,  pos);
	pos+=1;
	unsigned char c4=leer_char_disco( fp,  pos);
//	printf("ies: %i %i %i %i\n",c1,c2,c3,c4);


	//char c2= p;
	//printf("c1 %i c2 %i",c1,c2);
	//unsigned int p2= c1<<24 | c2<<16 | c3<<8 | c4;
	unsigned int p2=c4+c3*pow(2,8)+c2*pow(2,16)+c1*pow(2,24);

	/*
  unsigned int buffer[1];
  fseek(fp, pos, SEEK_SET);
  fread(buffer, 4, 1, fp);
	int i=buffer[0];*/
	///printf(" int %i",buffer[0]);
  return p2;
}
char leer_char_disco( FILE *fp, int pos){
  unsigned char buffer[1];
  fseek(fp, pos, SEEK_SET);
  fread(buffer, 1, 1, fp);
	char c=buffer[0];
	//c=0;
  return c;
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
    pos=pos+11;
		pos+=2;
		//int p=leer_int_disco(fp,pos);
		//printf("p:%i   ",p);
		//char c1= p >>8;
		char c1=leer_char_disco( fp,  pos);
		pos+=1;
		char c2=leer_char_disco( fp,  pos);
		pos+=1;

		//char c2= p;
		//printf("c1 %i c2 %i",c1,c2);
		int p2= c1<<8 | c2;
		d->bloque_indice=p2;
    //pos+=4;
		//printf("%i: validez: %i  nombre: %s indice: %i \n",i,d->validez,d->nombre,d->bloque_indice);

    bloque_dir->directorios[i]=d;
  }
  return bloque_dir;
}
int espacio_libre_directorio( ){
  //bloques_bitmap=abrir_bloques_bitmap(fp);
  //bl_direc=abrir_bloque_directorio(fp);
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
  //bloques_bitmap=abrir_bloques_bitmap(fp);
  //bl_direc=abrir_bloque_directorio(fp);
  int i=0;
  for (i=0;i<65536;i++){
		//printf("i: %i ",bloques_bm->bits[i]);

    if (bloques_bm->bits[i]==0){
      return i;
    }
  }
  return -1;
}

void escribir_bloques_bitmap(FILE *fp){
  int i;
  for(i = 0; i<1024*8; i++){
      int a0 = bloques_bm->bits[i*8]  * pow(2,7);
      int a1 = bloques_bm->bits[i*8+1] * pow(2,6);
      int a2 = bloques_bm->bits[i*8+2] * pow(2,5);
      int a3 = bloques_bm->bits[i*8+3] * pow(2,4);
      int a4 = bloques_bm->bits[i*8+4] * pow(2,3);
      int a5 = bloques_bm->bits[i*8+5] * pow(2,2);
      int a6 = bloques_bm->bits[i*8+6] * pow(2,1);
      int a7 = bloques_bm->bits[i*8+7] * pow(2,0);
      int a = a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7;
      //Asumo que tengo hecho el byte_array de 8 bits
      char char_de_bin = a;
      escribir_char_disco(fp, 1024 + i, char_de_bin);
    }
  }


Conjunto_bloques_bitmap* abrir_bloques_bitmap(FILE *fp){
  bloques_bm=(Conjunto_bloques_bitmap*)calloc(1,sizeof(Conjunto_bloques_bitmap));
	bloques_bm->bits=(char*)calloc(65536,sizeof(char));
  //Asumimos que bloques estan ya con los 9 primeros bits no disponibles para escribir
  char * arreglo_chars = (char *) calloc(1024*8, sizeof(char));
  int i;
  for(i = 0; i < 1024*8; i++){
		arreglo_chars[i]=0;
    arreglo_chars[i] = leer_char_disco(fp, i +1024);
  }
  char * cada_char = (char *) calloc(8, sizeof(char));
	int s;
	for (s=0;s<8;s++){
		cada_char[s]=0;
	}
	printf("char1: %i....",arreglo_chars[0]);
	char2binstr(arreglo_chars[0], cada_char);
	for(s=0;s<8;s++){
		printf("%i",cada_char[s]);
	}

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
    escribir_int_disco(  fp,  pos,d->bloque_indice);
    //d->bloque_indice=leer_int_disco( fp,  pos);
    pos+=4;
    //bloque_dir->directorios[i]=d;
  }
}
int comparar_string(char *s2 , char*s1,  int largo){
	//printf("_______________");
  int i=0;
  for(i=0;i<largo;i++){
		printf("c1: %c c2:%c \n",s1[i],s2[i]);
    if(s1[i]!=s2[i]){
			printf("diferencia: %c %c",s1[i],s2[i]);
      return 0;
    }
  }
  return 1;
}

int cz_exists2(char*filename,Directorios*bloque){
  int i=0;

  for (i=0;i<64;i++){
    Directorio* d=bloque->directorios[i];
    int s=strncmp(filename , d->nombre,  11);
    if(s==0 && d->validez==1){
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
    //int s=comparar_string(filename , d->nombre,  11);
		int s=strncmp(filename,d->nombre,11);
    if(s==0 && d->validez==1){
      return d->bloque_indice;
    }
  }
  return 0;
}

/** retorn 1 si filname existe, 0 en caso contrario*/
int cz_exists(char*filename){
  return cz_exists2(filename,bl_direc);
}

/** abrir archivo, modo:r, w, si se escoge w el nombre no debe estar ocupado */
czFILE* cz_open(char*filename, char mode){
//filename[10]='\0';
//bloques_bitmap=abrir_bloques_bitmap(fp);
//bl_direc=abrir_bloque_directorio(fp);
if(mode=='r'){
	char* n=(char*)calloc(12,sizeof(char));
	int l=0;
	for(l=0;l<11;l++){
		n[l]=0;
	}
	n[11]='\0';
	int lll=0;
	while(lll<11){
		if(filename[lll]=='\0'){
			break;
		}
		n[lll]=filename[lll];
		lll++;
	}
	//int exist=cz_exists(n);
  int exist=cz_exists(filename);
	printf("existe?: %i ",exist);

  if(exist==1){
    czFILE* f_index =(czFILE*)calloc(1,sizeof(czFILE));
      int indice = devolver_bloque_indice(n);
			free(n);
      int pos= indice*1024;
			unsigned char c1=leer_char_disco( fp,  pos);
			pos+=1;
			unsigned char c2=leer_char_disco( fp,  pos);
			pos+=1;
			unsigned char c3=leer_char_disco( fp,  pos);
			pos+=1;
			unsigned char c4=leer_char_disco( fp,  pos);
			pos+=1;
			//printf("ies: %i %i %i %i\n",c1,c2,c3,c4);


			//char c2= p;
			//printf("c1 %i c2 %i",c1,c2);
			//unsigned int p2= c1<<24 | c2<<16 | c3<<8 | c4;
			unsigned int p2=c4+c3*pow(2,8)+c2*pow(2,16)+c1*pow(2,24);


    //  int tamao= leer_int_disco(fp,pos);
      //pos+=4;
      int creacion=leer_int_disco(fp,pos);
      pos+=4;
      int modificacion=leer_int_disco(fp,pos);
      pos+=4;
      f_index->indice=indice;
			printf("indice %u\n",f_index->indice);
      f_index->size=p2;
			printf("%s\n",filename);
			printf("size: %u\n",p2);
      f_index->ultimo_read=0;
      f_index->bloques_de_datos=NULL;
      f_index->f_cracion=creacion;
      f_index->leyendo=0;
      f_index->mode='r';
      f_index->f_modificacion=modificacion;

      int j=0;
      f_index->punteros=(int*)calloc(508,sizeof(int));
      for (j=0;j<252;j++){
				int p=leer_int_disco(fp,pos);
				char c1= p >>8;
				char c2= p;
				int p2= c1<<8 | c2;
				f_index->punteros[j]=p2;
        //f_index->punteros[j]=leer_int_disco(fp,pos);
				//printf("puntero: %i\n ",f_index->punteros[j]);
        pos+=4;
      }
			//f_index->bm=(int*)calloc(509,sizeof(int));
      f_index->puntero_dir_indirecto=leer_int_disco(fp,pos);
      pos=f_index->puntero_dir_indirecto*1024;
      for (j=252;j<508;j++){
        f_index->punteros[j]=leer_int_disco(fp,pos);
        pos+=4;
      }
      return f_index;

  }
  else {
		free(n);
      return NULL;
  }
}
if(mode=='w'){
	char* n=(char*)calloc(12,sizeof(char));
	int l=0;
	for(l=0;l<11;l++){
		n[l]=0;
	}
	n[11]='\0';
	int lll=0;
	while(lll<11){
		printf("caracter: %c ",filename[lll]);
		//if(filename[lll]=='\0'){
			//break;
	//	}
		n[lll]=filename[lll];
		lll++;
	}
  int exist=cz_exists(filename);
  if(exist==0){
    int espacio_libre=espacio_libre_directorio();
    if(espacio_libre!=-1){
      int b_libre=espacio_libre_bitmap();
			printf("bloque libre: %i\n",b_libre);
      if (b_libre!=-1){
				printf("b_libre: %i \n",b_libre);
				printf("nombre: %s \n",n);

				bloques_bm->bits[b_libre]=1;
				  bl_direc->directorios[espacio_libre]->validez=1;
				free(bl_direc->directorios[espacio_libre]->nombre);
				bl_direc->directorios[espacio_libre]->nombre=n;

            czFILE* f_index =(czFILE*)calloc(1,sizeof(czFILE));
							f_index->nombre=n;
							f_index->b_libre=b_libre;
							f_index->espacio_libre=espacio_libre;
              int tamao= 0;
							f_index->bm=(int*)calloc(509,sizeof(int));
							f_index->ultimo_b=0;
              //int creacion=1;
              //int modificacion=1;
              f_index->indice=b_libre;
              f_index->size=tamao;
              f_index->ultimo_read=0;
              f_index->f_cracion=(int)time(NULL);
              f_index->f_modificacion=(int)time(NULL);
              f_index->mode='w';
              f_index->leyendo=0;
              int j=0;
              f_index->punteros=(int*)calloc(508,sizeof(int));
              for (j=0;j<508;j++){
                f_index->punteros[j]=-1;
              }
              f_index->bloques_de_datos=(b_datos**)calloc(508,sizeof(b_datos*));
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
	free(n);
}
return NULL;
}

/** lee n_bytes desde el archivo y los copia en buffer*/
/** DOMINGO*/
int cz_read(czFILE*file_desc, char* buffer, int nbytes){
  //buffer=(char*)calloc(nbytes,sizeof(char));
	printf("ultimo read: %i",file_desc->ultimo_read);
  if(file_desc->size==0 ||file_desc->mode=='w' || file_desc->ultimo_read>=file_desc->size){
		if(file_desc->mode=='w' ){
			fprintf(stderr,"El archivo esta en modo escritura");
		}
		else{
			fprintf(stderr,"leyendo fuera de limite");

	}
		printf("size: %i\n",file_desc->size);
		printf("chao");
    return -1;
  }
//int fin_lectura=nbytes+file_desc->ultimo_read;
//int numero_bloques_datos=(nbytes-(nbytes%1024)/1024)+1;
//int bloque_inicio=(file_desc->ultimo_read-(file_desc->ultimo_read%1024)/1024);
//int offset=file_desc->ultimo_read%1024;
int j=0;
while(nbytes>0){
  if(file_desc->leyendo>=file_desc->size){
		fprintf(stderr,"leyendo fuera de limite");
    return j;
  }
  int bloque_actual= (file_desc->leyendo-(file_desc->leyendo%1024))/1024;


	//printf("bloque_actual:%i",bloque_actual);
	//printf("b:%i\n",bloque_actual);
  int offset=file_desc->leyendo%1024;
	//printf("of:%i\n",offset);
  int bloque_en_disco;
   bloque_en_disco=file_desc->punteros[bloque_actual];
	 //printf("bl: %i  ",bloque_en_disco);
  if(bloque_actual<252){
     bloque_en_disco=file_desc->punteros[bloque_actual];
		//printf("bloque_en_disco:%i\n",bloque_en_disco);
  }
  else{
    int p=file_desc->puntero_dir_indirecto*1024+((bloque_actual-252)*4);
    bloque_en_disco=leer_int_disco(fp,  p);

  }

  int pos=bloque_en_disco*1024+offset;
  //printf("pos:%i\n",pos);
  buffer[j]=leer_char_disco( fp,  pos);
	//printf("buf: %c",buffer[j]);
  j++;
  file_desc->leyendo++;
	nbytes--;
}
//printf("leyendo: %i ..",file_desc->leyendo);
return j;
}

/** escribe en el archivo lo que hay en buffer (nbytes) */
int cz_write(czFILE*file_desc, char* buffer, int nbytes){
	if(file_desc->mode=='r'){
		fprintf(stderr,"El archivo esta en modo lectura");
		return -1;
	}
	file_desc->f_modificacion=(int)time(NULL);
  int j=0;
  while(nbytes>0){
		//printf("nb: %i\n",nbytes);
    int bloque_a_escribir=(file_desc->size-(file_desc->size%1024))/1024;
    int offset=file_desc->size%1024;
    if(offset==0){
      int espacio_libre=espacio_libre_bitmap();

			printf("espaciol: %i\n",espacio_libre);
      if (espacio_libre==-1){
				fprintf(stderr,"No queda espacio");
        return j;//error espacio
      }
      else{
				file_desc->bm[file_desc->ultimo_b]=espacio_libre;
				file_desc->ultimo_b++;
				bloques_bm->bits[espacio_libre]=1;
        if(bloque_a_escribir==252){
          int espacio_libre2=espacio_libre_bitmap();

          if(espacio_libre2==-1){
						fprintf(stderr,"No queda espacio");
            return j;//error espacio
          }
          else{
						bloques_bm->bits[espacio_libre2]=1;
						file_desc->bm[file_desc->ultimo_b]=espacio_libre2;
						file_desc->ultimo_b++;
            file_desc->puntero_dir_indirecto=espacio_libre2;
          }
        }
				//printf("holsa\n");
				bloques_bm->bits[espacio_libre]=1;
      //  bloques_bm->bits[espacio_libre]=1;
				//printf("hola\n");
        file_desc->punteros[bloque_a_escribir]=espacio_libre;
      }

    }
		//printf("aca\n");
    file_desc->bloques_de_datos[bloque_a_escribir]->datos[offset]=buffer[j];
    file_desc->size++;
    j++;
		nbytes--;
  }

return j;
}

/** cierra el archivo */
int cz_close(czFILE* file_desc){
	//printf("closing\n");
  if(file_desc->mode=='w'){
		bloques_bm->bits[file_desc->b_libre]=1;
		  bl_direc->directorios[file_desc->espacio_libre]->validez=1;
		//free(bl_direc->directorios[file_desc->espacio_libre]->nombre);
		bl_direc->directorios[file_desc->espacio_libre]->nombre=file_desc->nombre;
		bl_direc->directorios[file_desc->espacio_libre]->bloque_indice=file_desc->indice;
		//printf("indice::::%i\n",file_desc->indice);
    int pos=file_desc->indice*1024;
		//printf("pos: %i \n",pos);
    escribir_int_disco( fp,  pos, file_desc->size);
    pos+=4;
    escribir_int_disco( fp,  pos, file_desc->f_cracion);
    pos+=4;
    escribir_int_disco( fp,  pos, file_desc->f_modificacion);
		pos+=4;
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
				printf("%i\n",pos);
				//printf("%s",file_desc->bloques_de_datos[i]->datos);
        escribir_string_disco( fp, pos,  file_desc->bloques_de_datos[i]->datos, 1024);
      }
    }
    pos=file_desc->puntero_dir_indirecto*1024;
    for(i=252;i<508;i++){
      escribir_int_disco( fp,  pos, file_desc->punteros[i]);
      pos+=4;
    }
		free(file_desc->bm);
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
  free(file_desc);





    //escribir_char_disco( FILE *fp, int pos, char caracter);
  //fclose(fp);
  //cerrar_bloque_directorio(bl_direc);
  //free(file_desc->punteros);
  //free(file_desc);
  //free(bloques_bitmap);
  return 0;
}

/** cambia el nombre del archivo */
int cz_mv(char*orig, char*dest){
  //int j;
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
		char filename2[11];
		int nn=0;
		for(nn=0;nn<11;nn++){
			filename2[nn]=0;
		}
	    int i=0;
	    while (i<11){
	      if(filename[i]=='\0'){
	        break;
	      }
	      filename2[i]=filename[i];
	      i++;
			}
	    //
			//printf("nom: %s\n",filename2);
			int s=strncmp(filename2 , bl_direc->directorios[contador_rm]->nombre,  11);
			//printf("s: %i \n",s);
			if (s==1 && bl_direc->directorios[contador_rm]->validez == 1){
      //if (bl_direc->directorios[contador_rm]->nombre == filename && bl_direc->directorios[contador_rm]->validez == 1){
        printf("Eliminando archivo %s \n", filename);
        //indice = bl_direc->directorios[contador_rm]->bloque_indice;
        czFILE* archivo = cz_open(filename, 'r');
        int i;
        for(i = 0; i < 252; i++){
          if(archivo->punteros[i] != -1){
            bloques_bm->bits[archivo->punteros[i]] = 0;
          }
        }
        if(archivo->puntero_dir_indirecto != -1){
          bloques_bm->bits[archivo->puntero_dir_indirecto] = 0;
          for(i = 0; i < 256; i++){
            int posicion = leer_int_disco(fp, 1024 * archivo->puntero_dir_indirecto + 4*i);
            bloques_bm->bits[posicion] = 0;
          }
        }

        cz_close(archivo);
				bl_direc->directorios[contador_rm]->validez = 0;
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
      printf("%s  \n",bl_direc->directorios[contador_ls]->nombre);
    }
  }
}

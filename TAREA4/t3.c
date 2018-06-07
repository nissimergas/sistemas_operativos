// Importo el archivo arraylist.h
#include "t3.h"
// Libreria estandar de C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/socket.h>
//////////////////////////////////////////////////////////////////////////
//                             Funciones                                //
//////////////////////////////////////////////////////////////////////////

// TODO: Debes completar las siguientes funciones
// Puedes crear otras funciones aca para el
// funcionamiento interno del arreglo dinamico
void start_conection(int sock,char*buffer){
  buffer[0]=1;
  int i=1;
  for (i=1;i<258;i++){
    buffer[i]=0;
  }
  send(sock, buffer, strlen(buffer), 0);
}

void conection_establish(int sock, char*buffer){
  buffer[0]=2;
  int i=1;
  for (i=1;i<258;i++){
    buffer[i]=0;
  }
  send(sock, buffer, strlen(buffer), 0);
}

void ask_nickname(int sock, char*buffer){
  buffer[0]=3;
  int i=1;
  for (i=1;i<258;i++){
    buffer[i]=0;
  }
  send(sock, buffer, strlen(buffer), 0);
}


void return_nickname(int sock,char*buffer){
  int i=0;
  char buf[257];
  for (i=0;i<257;i++){
    buf[i]=0;
  }
  
  printf( "Enter a value :");
  fgets(buf, 200, stdin);
  printf( "\n");
  buffer[0]=4;
  
  for (i=1;i<258;i++){
    buffer[i]=buf[i-1];
  }
  send(sock, buffer, strlen(buffer), 0);
}

void casos_respuestas(int sock, char* buffer){
  int i=0;
  for (i=0;i<258;i++){
    buffer[i]=0;
  }
  recv(sock, buffer, 258, 0);
    char a=buffer[0];
    switch(a){
        case 1:
            printf("Started new conexion\n" );
            conection_establish( sock, buffer);
            sleep(1);
            ask_nickname(sock,buffer);
            break;
        case 2:
            printf("Connection Established\n" );
            break;
        case 3:
            printf("Cual es tu nickname?\n" );
            return_nickname(sock,buffer);
            break;

        case 4:
            printf("Nickname jugador:\n" );
            char buf[257];
            int i;
            for (i=1;i<258;i++){
              buf[i-1]=buffer[i];
            }
            printf("%s\n",buf);
            break;
        case 5:
            printf("Oponent found\n" );
            break;

        case 6:
            printf("Initial pot\n" );
            break;

        case 7:
            printf("Game Start\n" );
            break;

        case 8:
            printf("Start round\n" );
            break;
        case 9:
            printf("Start round\n" );
            break;
        case 10:
            printf("Start round\n" );
            break;

        case 11:
            printf("Start round\n" );
            break;
        case 12:
            printf("Start round\n" );
            break;
        case 13:
            printf("Start round\n" );
            break;
        case 14:
            printf("Start round\n" );
            break;
        case 15:
            printf("Start round\n" );
            break;
        case 16:
            printf("Start round\n" );
            break;
        case 17:
            printf("Start round\n" );
            break;
        case 18:
            printf("Start round\n" );
            break;
        case 19:
            printf("Start round\n" );
            break;
        case 20:
            printf("Start round\n" );
            break;
        case 21:
            printf("Start round\n" );
            break;
        case 22:
            printf("Start round\n" );
            break;
        case 23:
            printf("Start round\n" );
            break;
        case 24:
            printf("Start round\n" );
            break;
        default :
            printf("Invalid \n" );
            break;
    }
}
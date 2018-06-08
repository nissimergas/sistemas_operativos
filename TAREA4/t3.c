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
void ret_bet(int sock,char*buffer){
    buffer[0]=15;
    int i;
    int res;
    printf("ingrese apuesta(1:fold, 2:0, 3:100, 4:200, 5:500):");
    scanf("%d", &res);
    printf("\n");
    if(res>5 || res<1){
      res=1;
    }
    buffer[1]=res;
    for (i=1;i<258;i++){
      buffer[i]=0;
    }
    send(sock, buffer, strlen(buffer), 0);  
}

void error_bet(int sock,char*buffer){
    buffer[0]=16;
    int i;
    for (i=1;i<258;i++){
      buffer[i]=0;
    }
    send(sock, buffer, strlen(buffer), 0);  
}
/*int manejar_bet(int sock,char*buffer, Player* jugador, int bet){
  recv(sock, buffer, 258, 0);
  int apuesta;
  if(buffer[1]==1){
    apuesta=-1;

  }
  if(buffer[1]==2){
    apuesta=0;
    
  }
  if(buffer[1]==3){
    apuesta=100;
    
  }
  if(buffer[1]==4){
    apuesta=200;
    
  }
  if(buffer[1]==5){
    apuesta=-300;
  }
  if(apuesta>jugador->pot || (apuesta<bet && apuesta!=-1 )){
    error_bet(sock,buffer);
    manejar_bet(sock,buffer,  jugador);
  }
  jugador->pot-=apuesta;
  return apuesta;
}*/

void get_bet(int sock,char*buffer){
    buffer[0]=14;
    buffer[1]=5;
    int i;
    for (i=2;i<258;i++){
      buffer[i]=0;
    }
    for (i=2;i<7;i++){
      buffer[i]=i-2;
    }
    send(sock, buffer, strlen(buffer), 0);
     
}
// TODO: Debes completar las siguientes funciones
// Puedes crear otras funciones aca para el
// funcionamiento interno del arreglo dinamico
/*char carta_a_numero(char number){
  char num;
  switch(number){
    case 'A':
            num=1;
          break;
    case '2':
            num=2;
          break;
    case '3':
            num=3;
          break;
    case '4':
            num=4;
          break;
    case '5':
            num=5;
          break;
    case '6':
            num=6;
          break;

    case '7':
            num=7;
          break;
    case '8':
            num=8;
          break;
    case '9':
            num=9;
          break;
    case '10':
            num=10;
          break;
    case 'J':
            num=11;
          break;
    case 'Q':
            num=12;
          break;
    case 'K':
            num=13;
          break;
    default :
            num=0;
            break;
  }
  return num;
}
char pinta_a_numero(char pinta){
  char num;
  switch(pinta){
    case '♥':
            num=1;
          break;
    case '♦':
            num=2;
          break;
    case '♣':
            num=3;
          break;
    case '♠':
            num=4;
            break;
    default :
            num=0;
            break;
  }
  return num;
}*/
void cambiar(char number,char pinta,char *cartas, char*mazo, char *ocupado ){
  int i=0;
  int pos=100;
  //obtengo posicion de la carta
  while(i<5){
    if (cartas[2*i]==number){
      if (cartas[2*i+1]==pinta){
        pos=i;
        int r;
    r = rand() % 52;
    while(ocupado[r]==1){
      r = rand() % 52;
    }
    ocupado[r]=1;
    int numero2= r%13;
    int pinta2= ((r-numero2)/13)+1;
    int carta2=numero2+1;
    cartas[2*pos]=carta2;
    cartas[2*pos+1]=pinta2;
        break;
      } 
    }
    i++;
  }
  /*if( pos!=100){
    int r;
    r = rand() % 52;
    while(ocupado[r]==1){
      r = rand() % 52;
    }
    ocupado[r]=1;
    int numero2= r%13;
    int pinta2= ((r-numero2)/13)+1;
    int carta2=numero2+1;
    cartas[2*pos]=carta2;
    cartas[2*pos+1]=pinta2;
  }*/

}
void send_new_cards(int sock,char*buffer, char *cartas, char*mazo, char *ocupado){
  printf("aca\n");
  recv(sock, buffer, 258, 0);
  printf("buffer:%i\n",buffer[1]);
  int i;
  for(i=0;i<5;i++){
    printf("buf: %i %i \n",buffer[2*i+2],buffer[2*i+1+2]);
  }
  if(buffer[0]==13){
    int nc=buffer[1]/2;
    int i=0;
    printf("_________\n");
    for (i=0;i<5;i++){
      
      if(buffer[i+2]!=0){
        printf("cambiando\n");
          cambiar(buffer[i+2],buffer[i+3],cartas, mazo, ocupado);
    }
    }

    buffer[0]=10;
    buffer[1]=10;
    for(i=2;i<12;i++){
      buffer[i]=cartas[i-2];
    }
    send(sock, buffer, 258, 0);

  }
  else{
    printf("  Invalid package\n");
    buffer[0]=100;
    send(sock, buffer, 258, 0);
  }


}
void send_get_cards_to_change(int sock, char*buffer, char *cartas){
 buffer[0]=12;
 buffer[1]=10;
    int i;
    for (i=2;i<12;i++){
      buffer[i]=cartas[i-2];
    }
    for (i=12;i<258;i++){
      buffer[i]=0;
    }
    printf("int: %lu \n",strlen(buffer));
    send(sock, buffer, strlen(buffer), 0);
    printf("holaalals");
}

void return_cards_to_change(int sock, char*buffer){
  int i=0;
  
  int nc=0;
  /*printf( "Seleccione numero de cartas a cambiar :");
  scanf("%d", &nc);
  printf( "\n");
  if(nc>5){
  printf( "Invalido Seleccione numero de cartas a cambiar :");
    scanf("%d", &nc);
    printf( "\n");

  }
  printf( "Seleccione cartas a cambiar :");
  fgets(buf, 200, stdin);*/
  char buf[10];
  for(i=0;i<10;i++){
    buf[i]=0;
  }
  int indice=0;
  int res;
  for(i=0;i<5;i++){
    printf("Desea cambiar ");
    if (buffer[2*i+2]==1){
        printf("A");
      }
      else if(buffer[2*i+2]==11){
        printf("J");
      }
      else if(buffer[2*i+2]==12){
        printf("Q");
      }
      else if(buffer[2*i+2]==13){
        printf("K");
      }
      else{
        printf("%i",buffer[2*i+2]);
      }
       if(buffer[2*i+3]==1){
          printf("♥");
        }
        else if(buffer[2*i+3]==2){
          printf("♦");
        }
        else if(buffer[2*i+3]==3){
          printf("♣");
        }
        else if(buffer[2*i+3]==4){
          printf("♠");
        }
        else{
          printf("?");
        }
        printf("?(1/si 2/no)");
        scanf("%d", &res);
        if (res==1){
          buf[indice]=buffer[2*i+2];
          buf[indice+1]=buffer[2*i+3];
          indice=indice+2;
          nc+=1;
        }
        else{
          buf[indice]=0;
          buf[indice+1]=0;
          indice=indice+2;
          nc+=1;
        }
  }
  for(i=0;i<5;i++){
    printf("buf: %i %i ",buf[2*i],buf[2*i+1]);
  }

  printf( "\n");
  buffer[0]=13;
  buffer[1]=2*nc;
  for (i=2;i<12;i++){
    buffer[i]=buf[i-2];
  }
  for (i=12;i<258;i++){
    buffer[i]=0;
  }

  send(sock, buffer, 258, 0);
}

void send_turn(int sock, char*buffer, int turn){
    buffer[0]=11;
    buffer[1]=1;
    buffer[2]=turn;
    int i;
    for (i=3;i<258;i++){
      buffer[i]=0;
    }
    send(sock, buffer, 258, 0);
}
void dar_cartas(char* cartas, char* mazo,char*ocupado){
  int r;
  int i;
  for(i=0;i<5;i++){
    r = rand() % 52;
    while(ocupado[r]==1){
      r = rand() % 52;
    }
    ocupado[r]=1;
    int numero= r%13;
    int pinta= ((r-numero)/13)+1;
    int carta=numero+1;
    cartas[2*i]=carta;
    cartas[2*i+1]=pinta;
  }
}

void send_cards(int sock, char*buffer, char*cards){
  buffer[0]=10;
  buffer[1]=10;
  int i=1;
  for (i=2;i<12;i++){
    buffer[i]=cards[i-2];
  }
  for (i=12;i<258;i++){
    buffer[i]=0;
  }
  send(sock, buffer, 258, 0);
}
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

void send_oponent_found(int sock, char*buffer){
  buffer[0]=5;
  int i=1;
  for (i=1;i<258;i++){
    buffer[i]=0;
  }
  send(sock, buffer, strlen(buffer), 0);
}

void send_initial_pot(int sock, char*buffer, int pot){
  int numero=pot;
  buffer[0]=6;
  buffer[1]=4;
  buffer[2]=numero>>24;
  buffer[3]=numero>>16;
  buffer[4]=numero>>8;
  buffer[5]=numero;

  int i=6;
  for (i=6;i<258;i++){
    buffer[i]=0;
  }
  send(sock, buffer, 258, 0);
}


void send_game_start(int sock, char*buffer){
  buffer[0]=7;
  int i=1;
  for (i=6;i<258;i++){
    buffer[i]=0;
  }
  send(sock, buffer, 258, 0);
}


void send_round_start(int sock, char*buffer, int pot){
  int numero=pot;
  buffer[0]=8;
  buffer[1]=4;
  buffer[2]=numero>>24;
  buffer[3]=numero>>16;
  buffer[4]=numero>>8;
  buffer[5]=numero;

  int i=6;
  for (i=6;i<258;i++){
    buffer[i]=0;
  }
  send(sock, buffer, 258, 0);
}

int casos_respuestas(int sock, char* buffer){
  int respuesta=1;
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
            char buf[258];
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
            printf("__________________________________________\n" );
            printf("Initial pot\n" );
            unsigned char a5=buffer[5];
            unsigned char a4=buffer[4];
            unsigned char a3=buffer[3];
            unsigned char a2=buffer[2];
            int pot2=a5+a4*pow(2,8)+a3*pow(2,16)+a2*pow(2,24);
            printf(" pot: %i \n",pot2);

            //printf(" pot: %i %i %i %i %i %i",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
            break;

        case 7:
        printf("__________________________________________\n" );
            printf("Game started\n" );
            break;

        case 8:
        printf("__________________________________________\n" );
            printf("New round\n" );
            a5=buffer[5];
            a4=buffer[4];
            a3=buffer[3];
            a2=buffer[2];
            pot2=a5+a4*pow(2,8)+a3*pow(2,16)+a2*pow(2,24);
            printf(" apuesta inicial: %i \n",pot2);
            break;
        case 9:
        printf("__________________________________________\n" );
            printf("Start round\n" );
            a5=buffer[5];
            a4=buffer[4];
            a3=buffer[3];
            a2=buffer[2];
            pot2=a5+a4*pow(2,8)+a3*pow(2,16)+a2*pow(2,24);
            printf(" monto actual del jugador: %i \n",pot2);
            break;
        case 10:
         printf("__________________________________________\n" );
           
            printf("cartas: ");
            for(i=2;i<12;i++){
              if(i%2==0){
                if (buffer[i]==1){
                  printf("A");
                }
                else if(buffer[i]==11){
                  printf("J");
                }
                else if(buffer[i]==12){
                  printf("Q");
                }
                else if(buffer[i]==13){
                  printf("K");
                }
                else{
                  printf("%i",buffer[i]);
                }
                
              }
              else{
                if(buffer[i]==1){
                  printf("♥");
                }
                else if(buffer[i]==2){
                  printf("♦");
                }
                else if(buffer[i]==3){
                  printf("♣");
                }
                else if(buffer[i]==4){
                  printf("♠");
                }
                else{
                  printf("?");
                }
              }
            }
             printf("\n" );
            
            break;

        case 11:
            printf("__________________________________________\n" );
            printf("Turno\n" );
            if(buffer[2]==1){
              printf("Juegas primero!\n");
            }
            else if(buffer[2]==2){
              printf("Juegas segundo !\n");
            }
            else{
              printf("Ha ocurrido un error\n");
            }
            
            break;
        case 12:
            return_cards_to_change(sock,buffer);
            break;
        case 13:
            printf("Start round\n" );
            break;
        case 14:
            printf("Apuesta\n" );
            ret_bet( sock,buffer);
            break;
        case 15:
            printf("bet:\n" );
            int bet;

            break;
        case 16:
            printf("Error bet\n" );
            ret_bet( sock,buffer);
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
            respuesta=0;
            break;
    }
    return respuesta;
}
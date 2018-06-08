
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "t3.h"
#include <unistd.h>

#define PORT 4474

int main(){
  //estructura mantenida por servidor
  char* nombre1=(char*)calloc(200,1);
  char* nombre2=(char*)calloc(200,1);
  char* mazo=(char*)calloc(52,1);
  char* ocupado=(char*)calloc(52,1);

  int j=0; //inicializo mazo
  for (j=0;j<52;j++){
    mazo[j]=j;
    ocupado[j]=0;

  }
  //ocupado[0]=1;
  Player* player1=(Player*)calloc(1,sizeof(Player));
  Player* player2=(Player*)calloc(1,sizeof(Player));
  player1->pot=1000;
  player2->pot=1000;
  player1->nombre=nombre1;
  player2->nombre=nombre1;
  char*cartas1=(char*)calloc(10,1);
  char*cartas2=(char*)calloc(10,1);
  player1->cartas=cartas1;
  player2->cartas=cartas2;
  
  int sockfd;
  struct sockaddr_in serverAddr;

  int cliente1, cliente2;
  struct sockaddr_in newAddr;

  socklen_t addr_size;
  char buffer[258];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("[+]Server Socket Created Sucessfully.\n");
  memset(&serverAddr, '\0', sizeof(serverAddr));

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  printf("[+]Bind to Port number %d.\n", 4455);
 
    listen(sockfd, 5);
    printf("[+]Listening...\n");
 

    cliente1 = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
    
    

///recivo un mensaje c1
     casos_respuestas(cliente1, buffer);
     casos_respuestas(cliente1, buffer);
     int i;
     for (i=1;i<200;i++){
        nombre1[i-1]=buffer[i];
      }



//recivo client2
cliente2 = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
///recivo un mensaje c2
    casos_respuestas(cliente2, buffer);
    casos_respuestas(cliente2, buffer);
    for (i=1;i<200;i++){
        nombre2[i-1]=buffer[i];
      }
    sleep(1);
    printf("los nombres de los jugadores son : %s, %s\n",nombre1,nombre2);
    //En este momento tengo guardado los dos jugadores
    //se envian mensajes "cliente encontrado"
    send_oponent_found(cliente1, buffer);
    send_oponent_found(cliente2, buffer);
    //
    int jugando=5;
    usleep(10);
    //initial pot
    send_initial_pot(cliente1, buffer, player1->pot);
    sleep(1);
    send_initial_pot(cliente2, buffer, player2->pot);

    //mensaje game start
    send_game_start(cliente1, buffer);
    send_game_start(cliente2, buffer);

    //Game start
    int table_pot=0;
    int turn1=2;
    int turn2=1;
    while(jugando>0){//parte el juego
      int bet1=0;
      int bet2=0;
      if(turn1==1){
        turn1=2;
        turn2=1;
      }
      else{
        turn2=2;
        turn1=1;

      }
      //inic¡cializo mazo;
      for (j=0;j<52;j++){
          ocupado[j]=0;
      }
      if(player1->pot>=10){
        send_round_start( cliente1, buffer, 10);
        player1->pot-=10;
      }
      else{
        jugando=0;
        break;

      }
     if(player2->pot>=10){
        send_round_start( cliente2, buffer, 10);
        player2->pot-=10;
      }
      else{
        jugando=0;
        break;
      }
      table_pot=20;
      dar_cartas( cartas1, mazo,ocupado);
      dar_cartas( cartas2, mazo,ocupado);
      send_cards(cliente1, buffer, cartas1);
      send_cards(cliente2, buffer, cartas2);
      send_turn( cliente1, buffer, turn1);
      send_turn( cliente2, buffer, turn2);
      //cambiar cartas
      if (turn1==1){
        send_get_cards_to_change(cliente1,buffer,cartas1);
        printf(" hola\n");
        send_new_cards(cliente1, buffer, cartas1,mazo,ocupado);
        sleep(1);
        send_get_cards_to_change(cliente2,buffer,cartas2);
        
        send_new_cards(cliente2, buffer, cartas2,mazo, ocupado);
//apuesta
        ////////////////////////apuesta////////////////////////////////////
        int error=1;
        while(error=1){
          get_bet(cliente1,buffer);
          recv(cliente1, buffer, 258, 0);
          int apuesta;
          if(buffer[1]==1){
            apuesta=-1;

          }
          if(buffer[1]==2){
            apuesta=0;
            
          }
          if(buffer[1]==3{
            apuesta=100;
            
          }
          if(buffer[1]==4){
            apuesta=200;
            
          }
          if(buffer[1]==5){
            apuesta=-300;
          }
          if(apuesta>jugador1->pot ){
            error_bet(cliente1,buffer);
            
          }
          error=0;
      }
      if(apuesta==-1){
        continue;
      }
        jugador1->pot-=apuesta;
        bet1=apuesta;

      ////////////////////////apuesta j2////////////////////////////////////
      get_bet(cliente2,buffer);
      error=1;
      while(error=1){
          get_bet(cliente2,buffer);
          recv(cliente2, buffer, 258, 0);
          int apuesta;
          if(buffer[1]==1){
            apuesta=-1;

          }
          if(buffer[1]==2){
            apuesta=0;
            
          }
          if(buffer[1]==3{
            apuesta=100;
            
          }
          if(buffer[1]==4){
            apuesta=200;
            
          }
          if(buffer[1]==5){
            apuesta=-300;
          }
          if(apuesta>jugador2->pot ||  bet1>apuesta){
            error_bet(cliente2,buffer);
            
          }
          error=0;
      }
      if(apuesta==-1){
        jugador1->pot+=(bet1+bet2)
        continue;
      }
       jugador2->pot-=apuesta;
        bet2=apuesta;

      if(bet2>bet1){
        int error=1;
        while(error=1){
          get_bet(cliente1,buffer);
          recv(cliente1, buffer, 258, 0);
          int apuesta;
          if(buffer[1]==1){
            apuesta=-1;

          }
          if(buffer[1]==2){
            apuesta=0;
            
          }
          if(buffer[1]==3{
            apuesta=100;
            
          }
          if(buffer[1]==4){
            apuesta=200;
            
          }
          if(buffer[1]==5){
            apuesta=-300;
          }
          if(apuesta>jugador1->pot+bet1||apuesta<bet2 ){
            error_bet(cliente1,buffer);        
          }
          error=0;
      }
      if(apuesta==-1){
        jugador2->pot+=(bet1+bet2)
        continue;
      }
        jugador1->pot+=bet1;
        jugador1->pot-=apuesta;
        bet1=apuesta;
 ////////////////////apuesta /////////////////////////////////////

error=1;

      while(error=1){
          get_bet(cliente2,buffer);
          recv(cliente2, buffer, 258, 0);
          int apuesta;
          if(buffer[1]==1){
            apuesta=-1;

          }
          if(buffer[1]==2){
            apuesta=0;
            
          }
          if(buffer[1]==3{
            apuesta=100;
            
          }
          if(buffer[1]==4){
            apuesta=200;
            
          }
          if(buffer[1]==5){
            apuesta=-300;
          }
          if(apuesta>jugador2->pot+bet2 ||  bet1!=apuesta{
            error_bet(cliente2,buffer);
            
          }
          error=0;
      }
      if(apuesta==-1){
        jugador1->pot+=(bet1+bet2)
        continue;
      }
       jugador2->pot+=bet2;
       jugador2->pot-=apuesta;
       bet2=apuesta;


      }
        ////////////////////////apuesta////////////////////////////////////





        





      }
      else{
        send_get_cards_to_change(cliente2,buffer,cartas2);
        
        send_new_cards(cliente2, buffer, cartas2,mazo, ocupado);
        send_get_cards_to_change(cliente1,buffer,cartas1);
        
        send_new_cards(cliente1, buffer, cartas1, mazo, ocupado);


         ////////////////////////apuesta////////////////////////////////////
        int error=1;
        while(error=1){
          get_bet(cliente2,buffer);
          recv(cliente2, buffer, 258, 0);
          int apuesta;
          if(buffer[1]==1){
            apuesta=-1;

          }
          if(buffer[1]==2){
            apuesta=0;
            
          }
          if(buffer[1]==3{
            apuesta=100;
            
          }
          if(buffer[1]==4){
            apuesta=200;
            
          }
          if(buffer[1]==5){
            apuesta=-300;
          }
          if(apuesta>jugador2->pot ){
            error_bet(cliente2,buffer);
            
          }
          error=0;
      }
      if(apuesta==-1){
        continue;
      }
        jugador2->pot-=apuesta;
        bet2=apuesta;

      ////////////////////////apuesta j2////////////////////////////////////
      get_bet(cliente1,buffer);
      error=1;
      while(error=1){
          get_bet(cliente1,buffer);
          recv(cliente1, buffer, 258, 0);
          int apuesta;
          if(buffer[1]==1){
            apuesta=-1;

          }
          if(buffer[1]==2){
            apuesta=0;
            
          }
          if(buffer[1]==3{
            apuesta=100;
            
          }
          if(buffer[1]==4){
            apuesta=200;
            
          }
          if(buffer[1]==5){
            apuesta=-300;
          }
          if(apuesta>jugador1->pot ||  bet2>apuesta){
            error_bet(cliente1,buffer);
            
          }
          error=0;
      }
      if(apuesta==-1){
        jugador2->pot+=(bet1+bet2)
        continue;
      }
       jugador1->pot-=apuesta;
        bet1=apuesta;

      if(bet1>bet2){
        int error=1;
        while(error=1){
          get_bet(cliente2,buffer);
          recv(cliente2, buffer, 258, 0);
          int apuesta;
          if(buffer[1]==1){
            apuesta=-1;

          }
          if(buffer[1]==2){
            apuesta=0;
            
          }
          if(buffer[1]==3{
            apuesta=100;
            
          }
          if(buffer[1]==4){
            apuesta=200;
            
          }
          if(buffer[1]==5){
            apuesta=-300;
          }
          if(apuesta>jugador2->pot+bet2||apuesta<bet1 ){
            error_bet(cliente2,buffer);        
          }
          error=0;
      }
      if(apuesta==-1){
        jugador1->pot+=(bet1+bet2)
        continue;
      }
        jugador2->pot+=bet2;
        jugador2->pot-=apuesta;
        bet2=apuesta;
 ////////////////////apuesta /////////////////////////////////////

error=1;

      while(error=1){
          get_bet(cliente1,buffer);
          recv(cliente1, buffer, 258, 0);
          int apuesta;
          if(buffer[1]==1){
            apuesta=-1;

          }
          if(buffer[1]==2){
            apuesta=0;
            
          }
          if(buffer[1]==3{
            apuesta=100;
            
          }
          if(buffer[1]==4){
            apuesta=200;
            
          }
          if(buffer[1]==5){
            apuesta=-300;
          }
          if(apuesta>jugador1->pot+bet1 ||  bet2!=apuesta{
            error_bet(cliente1,buffer);
            
          }
          error=0;
      }
      if(apuesta==-1){
        jugador2->pot+=(bet1+bet2)
        continue;
      }
       jugador1->pot+=bet1;
       jugador1->pot-=apuesta;
       bet1=apuesta;


      }

      }
     

      jugando--;
    }


    //strcpy(buffer, "Hello");
    //send(cliente2, buffer, strlen(buffer), 0);

     
   
  printf("[+]Closing the connection.\n");
  free(nombre1);
  free(nombre2);
  free(player1);
  free(player2);
  free(cartas1);
  free(cartas2);
  free(mazo);
  free(ocupado);
  return 0;



}
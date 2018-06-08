// Esta linea sirve para que el código de este archivo solo se importe una vez
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#pragma once

struct palyer
{
  
  char* nombre;//11
  char* cartas;
  int pot;

};

typedef struct palyer Player;
void start_conection(int sock,char*buffer);
void conection_establish(int sock, char*buffer);
void ask_nickname(int sock, char*buffer);
void return_nickname(int sock,char*buffer);
void send_oponent_found(int sock, char*buffer);
void send_initial_pot(int sock, char*buffer,int pot);
void send_game_start(int sock, char*buffer);
void send_round_start(int sock, char*buffer, int pot);
void dar_cartas(char* cartas, char* mazo,char*ocupado);
void send_cards(int sock, char*buffer, char*cards);
void send_turn(int sock, char*buffer, int turn);
void send_get_cards_to_change(int sock, char*buffer,char *cartas);
void send_new_cards(int sock,char*buffer, char *cartas, char*mazo, char *ocupado);
char carta_a_numero(char number);
char pinta_a_numero(char pinta);
void cambiar(char number,char pinta,char *cartas, char*mazo, char *ocupado );
void get_bet(int sock,char*buffer);
void ret_bet(int sock,char*buffer);
int manejar_bet(int sock,char*buffer, Player*jugador, int bet);
void error_bet(int sock,char*buffer);



int casos_respuestas(int sock, char* buffer);
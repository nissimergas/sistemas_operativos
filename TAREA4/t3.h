// Esta linea sirve para que el código de este archivo solo se importe una vez
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#pragma once

void start_conection(int sock,char*buffer);
void conection_establish(int sock, char*buffer);
void ask_nickname(int sock, char*buffer);
void return_nickname(int sock,char*buffer);
void casos_respuestas(int sock, char* buffer);

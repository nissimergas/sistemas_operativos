#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "t3.h"
#include <unistd.h>

#define PORT 4475


int main(){


	char*buffer=( char*)calloc(258,1);
	int clientSocket;
	struct sockaddr_in serverAddr;
	
	

	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	printf("[+]Client Socket Created Sucessfully.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	start_conection(clientSocket,buffer);
	casos_respuestas(clientSocket, buffer);
    casos_respuestas(clientSocket, buffer);
    //recivo respuesta oponente encontrado
    casos_respuestas(clientSocket, buffer);

    //recivo respuesta pot
    sleep(1);
    casos_respuestas(clientSocket, buffer);
     //recivo  comienzo del juego
    casos_respuestas(clientSocket, buffer);
    int jugando=1;
    while(jugando==1){
    	jugando=casos_respuestas(clientSocket, buffer);

  

    }


	
	
	//strcpy(buffer, "soy cliente1");

	//send(clientSocket, buffer, strlen(buffer), 0);

	//recv(clientSocket, buffer, 1024, 0);
	
	printf("[+]Closing the connection.\n");
	free(buffer);
return 0;
}


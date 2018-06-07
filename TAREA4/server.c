
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "t3.h"

#define PORT 4445

int main(){
  
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
    cliente2 = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
    

///recivo un mensaje c1
     casos_respuestas(cliente1, buffer);
     casos_respuestas(cliente1, buffer);

//Envio a c1
    //strcpy(buffer, "Hello");
    //send(cliente1, buffer, strlen(buffer), 0);



///recivo un mensaje c2
    casos_respuestas(cliente2, buffer);
    casos_respuestas(cliente2, buffer);
//Envio a c2

    strcpy(buffer, "Hello");
    send(cliente2, buffer, strlen(buffer), 0);
     
   
  printf("[+]Closing the connection.\n");
  return 0;



}
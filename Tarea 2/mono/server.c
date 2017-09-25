#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 255 /*Longitud máxima de mensaje*/

int main(int argc, char *argv[])
{
    int sock = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    int rc, n;
    char buffer[MAXLINE];

    //Creación del socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(7500);

    bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(sock, 5);

    // Inicio
  	printf("=== SERVER ===\n");

    while(1){
      connfd = accept(sock, (struct sockaddr*)NULL, NULL);
      if (connfd < 0){
        perror("ERROR on accept");
        exit(1);
      }

      bzero(buffer, MAXLINE);
      n = read(connfd, buffer, MAXLINE);
      if (n < 0){
        perror("ERROR reading from socket");
        exit(1);
      }

      printf("\rMensaje recibido: %s", buffer);

      rc = write(connfd,buffer,strlen(buffer));
      if(rc<=0) perror( "send error");

      close(connfd);
      sleep(1);
   }
}

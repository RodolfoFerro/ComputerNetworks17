#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAXLINE 255 /*Longitud máxima de mensaje*/

void lee_clientes(int sock);
void lee_clientes(int sock){
  char buffer[MAXLINE], user[MAXLINE];
  int rc;

  while (1) {
    // Recibimos usuario y mensaje:
    bzero(buffer, MAXLINE);
    bzero(user, MAXLINE);
    rc = recv(sock, user, MAXLINE, 0);
    if (rc < 0){
      printf("Usuario desconectado.\n");
      exit(1);
    }

    rc = recv(sock, buffer, MAXLINE, 0);
    if (rc < 0){
      printf("Error recibiendo mensaje.\n");
      exit(1);
    }
    if (rc > 0){
      // Imprimimos mensaje recibido:
      printf("Usuario %s dice: %s", user, buffer);
    }

    // Confirmamos de recibido con ECHO:
    rc = send(sock, buffer, MAXLINE, 0);
    if (rc <= 0) perror( "Error en send" );
  }

  return;
}

int main(int argc, char **argv)
{
    int sock = 0, connfd = 0;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;
    int rc, n, status;
    char buffer[MAXLINE];
    pid_t pid, wpid;

    // Inicio
    printf("=== SERVER ===\n");
    printf("Receptor de mensajes.\n");

    // Creamos el socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(7500);

    rc = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (rc < 0){
      perror("Error en bind");
      exit(1);
    }
    rc = listen(sock, 5);
    if (rc < 0){
      perror("Error en listen");
      exit(1);
    }
    cli_len = sizeof(cli_addr);

    // Obtenemos e imprimimos PID
    printf("PID: %u\n\n", getpid());

    while(1){
      // Aceptamos conexión
      connfd = accept(sock, (struct sockaddr*)&cli_addr, &cli_len);
      if (connfd < 0){
        perror("Error al aceptar conexión");
        exit(1);
      }

      // Hacemos un fork
      pid = fork();
      if (pid == (pid_t)-1){
        close(connfd);
        printf("Error en fork(): %s\n", strerror(errno));
        exit(1);
      }
      if (pid == 0){
        close(sock);
        lee_clientes(connfd);
        exit(0);
      }
      close(connfd);
    }

    // Cerramos socket:
    close(connfd);
    printf("Gracias por usar este servicio.\n");
   	return 0;
}

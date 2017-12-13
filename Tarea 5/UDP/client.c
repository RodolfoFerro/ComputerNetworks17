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

int main(int argc, char **argv)
{
    int sock = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    int rc, n;
    char buffer[MAXLINE];

    // Inicio
    printf("=== CLIENT ===\n");
    printf("Creador de mensajes.\n");

    // Obtenemos tty para crear usuario
    char *tty = ttyname(STDIN_FILENO);
    char *user;
    user = &tty[9];
    // int user = 100*(tty[9] - '0') + 10*(tty[10] - '0') + (tty[11] - '0');
    printf("Usuario %s.\n\n", user);

    // Si sock < 0 hay un error en la creación del socket:
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) <0) {
			perror("Problema creando el Socket");
			exit(2);
		}

    // Creación del socket
  	memset(&serv_addr, 0, sizeof(serv_addr));
  	serv_addr.sin_family = AF_INET;
  	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  	serv_addr.sin_port = htons(7500);

    // Conexión del clinte al servidor:
		if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
			perror("Problema al intentar la conexión con el servidor");
			exit(3);
		}
    
    // Ciclo para leer texto:
    while (1) {
      // Limpiamos y escribimos un mensaje:
      bzero(buffer, MAXLINE);
      printf("Introduce un mensaje: ");
      fgets(buffer, MAXLINE, stdin);

      // Enviamos usuario y mensaje:
      rc = send(sock, user, MAXLINE, 0);
      rc = send(sock, buffer, MAXLINE, 0);
      if (rc <= 0) perror( "Error en send" );

      // Si escribimos 'EXIT', salimos:
      if (strcmp(buffer, "EXIT\n") == 0) {
        printf("¡Hasta luego!\n");
        break;
      }

      // Recibimos respuesta del servidor:
      bzero(buffer, MAXLINE);
      rc = recv(sock, buffer, MAXLINE, 0);
      if (rc < 0){
        printf("Error leyendo buffer.\n");
        exit(1);
      }
      if (rc > 0){
        // Imprimimos mensaje recibido:
        printf("ECHO: %s\n", buffer);
      }
    }

    // Cerramos socket:
		close(sock);
		printf("Gracias por usar este servicio.\n");
		return 0;
}

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>

#define MAXLINE 255 /*Longitud máxima de mensaje*/

int main(int argc, char **argv)
{
		int sock = 0, connfd = 0;
		struct sockaddr_in serv_addr;
		int rc, n;
		char buffer[MAXLINE];

		// Creación del socket:
		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		serv_addr.sin_port = htons(7500);

		// Si sock < 0 hay un error en la creación del socket:
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) <0) {
			perror("Problema creando el Socket.");
			exit(2);
		}

		// Inicio:
		printf("=== CLIENT ===\n");
		// Conexión del clinte al servidor:
		if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
			perror("Problema al intentar la conexión con el servidor.");
			exit(3);
		}
		printf("Conectado con el servidor.\n\n");
		printf("Introduce un mensaje: ");
		fflush(stdout);

		while (1){
			// Definimos tiempo de espera:
			struct timeval tmo;
		  fd_set readfds;
			FD_ZERO(&readfds);
			FD_SET(0, &readfds);
			tmo.tv_sec = 0;
			tmo.tv_usec = 1;

			// Espera de input de texto:
      n = select(1, &readfds, NULL, NULL, &tmo);
      if (n == -1) printf("ERROR en select.\n");
      else if (n == 0){
        rc = recv(sock, buffer, MAXLINE, MSG_DONTWAIT);
        if (rc > 0){
          // Imprimimos mensaje recibido:
          fflush(stdout);
          printf("\rMensaje recibido: %s", buffer);
					printf("Introduce un mensaje: ");
          fflush(stdout);

          // Si escribimos 'EXIT', salimos:
          if (strcmp(buffer, "EXIT\n") == 0) {
            printf("\r¡Hasta luego!         \n");
            break;
          }
        }
			}
      else if (n > 0){
        // Limpiamos y escribimos un mensaje:
        printf("Introduce un mensaje: ");
        fflush(stdout);
        fgets(buffer, MAXLINE, stdin);

    		rc = send(sock, buffer, MAXLINE, MSG_DONTWAIT);
    		if (rc <= 0) perror( "send call failed" );

  			// Si escribimos 'EXIT', salimos:
  			if (strcmp(buffer, "EXIT\n") == 0) {
  				printf("\r¡Hasta luego!         \n");
  				break;
  			}
      }
		}

		// Cerramos socket:
		close(sock);
		printf("Gracias por usar este servicio.\n");

		return 0;
}

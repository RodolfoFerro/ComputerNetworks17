#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXLINE 255 /*Longitud máxima de mensaje*/

void *recvmg(void *sock)
{
		int their_sock = *((int *)sock);
		char msg[MAXLINE];
		int len;
		while((len = recv(their_sock, msg, MAXLINE, 0)) > 0) {
				msg[len] = '\0';
				fputs(msg, stdout);
				memset(msg, '\0', sizeof(msg));
		}
}

int main(int argc, char *argv[])
{
		struct sockaddr_in their_addr;
		int my_sock, their_sock, their_addr_size;
		int portno, len;
		pthread_t sendt, recvt;
		char msg[MAXLINE], res[MAXLINE];
		char username[MAXLINE];
		char ip[INET_ADDRSTRLEN];

    // Inicio:
    printf("=== CLIENT ===\n");
    printf("Creador de mensajes.\n\n");

    // Guardamos el nombre de usuario:
		if(argc < 2) {
				printf("Error, debes introducir tu nombre de usuario.");
				exit(1);
		}
		strcpy(username, argv[1]);

    // Creamos socket:
		my_sock = socket(AF_INET, SOCK_STREAM, 0);
		memset(their_addr.sin_zero, '\0', sizeof(their_addr.sin_zero));
		their_addr.sin_family = AF_INET;
		their_addr.sin_port = htons(7500);
		their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Establecemos conexión:
		if(connect(my_sock,(struct sockaddr *)&their_addr,sizeof(their_addr)) < 0) {
			perror("connection not esatablished");
			exit(1);
		}
		inet_ntop(AF_INET, (struct sockaddr *)&their_addr, ip, INET_ADDRSTRLEN);

    // Imprimimos estado de conexión:
		// printf("Connected to %s, start chatting\n\n", ip);
		printf("Conectado al servidor, puedes comenzar a chatear.\n\n");
		// printf("Introduce tu mensaje: ");
		pthread_create(&recvt, NULL, recvmg, &my_sock);

		while(fgets(msg, MAXLINE, stdin) > 0) {
				// fflush(stdout);
				strcpy(res, "\n\n");
				strcpy(res, username);
				strcat(res, " dice: ");
				strcat(res, msg);
				// printf("Introduce tu mensaje: ");
				// fflush(stdout);
				len = write(my_sock, res, strlen(res));
				if(len < 0) {
						perror("Error al enviar mensaje");
						exit(1);
				}

	      // Si escribimos 'EXIT', salimos:
	      if (strcmp(msg, "EXIT\n") == 0) {
		        printf("\r¡Hasta luego!         \n");
						close(my_sock);
		        break;
	      }

				memset(msg, '\0', sizeof(msg));
				memset(res, '\0', sizeof(res));
		}
		pthread_join(recvt, NULL);
		close(my_sock);
		printf("Gracias por usar este servicio.\n");
		return 0;
}

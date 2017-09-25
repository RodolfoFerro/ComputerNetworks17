#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 255 /*Longitud máxima de mensaje*/

int main(int argc, char **argv)
{
	int sock;
	int fin = 0, n;
	int bytecount;
	struct sockaddr_in servaddr;
	char buffer[MAXLINE];

	//Creación del socket
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(7500);

	// Inicio
	printf("=== CLIENT ===\n");

	// Ciclo para leer constantemente el texto:
	while(1){
		//Creamos socket para el cliente
		//Si sock < 0 hay un error en la creación del socket.
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) <0) {
			perror("Problema creando el Socket.");
			exit(2);
		}

		//Conexión del clinte al socket del servidor
		if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr))<0){
			perror("Problema al intentar la conexión con el servidor.");
			exit(3);
		}

		bzero(buffer, MAXLINE);
		printf("Introduce un mensaje: ");
	  fgets(buffer, MAXLINE, stdin);

		// Si escribimos 'EXIT', salimos:
		if (strcmp(buffer, "EXIT\n") == 0){
			printf("Gracias por usar este servicio.\n");
			break;
		}

		// Le escribimos al servidor a través del socket
		n = write(sock, buffer, strlen(buffer));
		if (n < 0){
	    perror("ERROR: No se pudo enviar al socket.");
	    exit(4);
	  }

		// Leemos respuesta del servidor:
	  bzero(buffer, MAXLINE);
	  n = read(sock, buffer, MAXLINE);
	  if (n < 0){
	    perror("ERROR: No se pudeo leer del socket.");
	    exit(5);
	  }

		// Cerramos socket:
		close(sock);
	}
	exit(0);
}

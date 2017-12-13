#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define PACK_SIZE 1440 /*Tamaño de paquetes*/
#define BUFFER_SZ 256   /*Tamaño de buffer*/

int main(int argc, char **argv)
{
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len;
    int rc, n, sock = 0;
    char buffer[BUFFER_SZ];

    // Inicio:
    printf("=== SERVER ===\n");
    printf("Encargado del envío de paquetes.\n");
    printf("Paquetes por enviar: 10000.\n");
    printf("Tamaño de buffer: %d\n", BUFFER_SZ);

    // Creamos el socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("Problema creando el Socket");
			exit(1);
		}
    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_port = htons(7500);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    rc = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (rc < 0){
      perror("Error en bind");
      exit(1);
    }

    // Enviamos paquetes:
    for (int i=0; i<10000; i++){
      rc = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&cli_addr,sizeof(cli_addr));
      if (rc < 0){
        perror("Error al enviar paquetes");
        exit(1);
      }
    }
    rc = sendto(sock, NULL, 0, 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));

    // Cerramos socket:
    close(sock);
    printf("Gracias por usar este servicio.\n");
   	return 0;
}

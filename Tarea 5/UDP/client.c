#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define PACK_SIZE 1440 /*Tamaño de paquetes*/
#define BUFFER_SZ 256   /*Tamaño de buffer*/

int main(int argc, char **argv)
{
    struct sockaddr_in serv_addr;
    socklen_t len;
    int sock = 0, rc, n = 0;
    char buffer[BUFFER_SZ];
    unsigned t_i, t_f;
    double ttime, transf;

    // Localhost:
    char server_address[INET_ADDRSTRLEN] = {"127.0.0.1"};
    // Red local (mi IP):
    // char server_address[INET_ADDRSTRLEN] = {"192.168.100.7"};

    // Inicio
    printf("=== CLIENT ===\n");
    printf("Receptor de paquetes.\n");

    // Si sock < 0 hay un error en la creación del socket:
		if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("Problema creando el Socket");
			exit(1);
		}

    // Creación del socket
  	serv_addr.sin_family = AF_INET;
  	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  	serv_addr.sin_port = htons(7500);

    // Verificamos conexión:
    memset(buffer, 'X', 256);
    if ((rc = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0){
       perror("Error al intentar conectar");
       exit(1);
    }

    // Recepción de paquetes:
    t_i = clock();
    while (1) {
 	    rc = recvfrom(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, &len);
 	    if (rc < 0){
        perror("Problema al recibir paquetes del servidor");
        exit(1);
 		  }
 	    if (rc == 0)
        break;
 	    if (rc > 0)
        n++;
    }

    // Medimos tiempo y transferencia:
    t_f = clock();
    ttime = ((double)(t_f-t_i)/CLOCKS_PER_SEC);
    transf = (double)(1440*n)/(ttime*1024);

    printf("Tiempo de ejecución: %f \n", ttime);
    printf("Paquetes recibidos: %d \n", n);
    printf("Mb/seg: %f\n", transf);

    // Cerramos socket:
		close(sock);
		printf("Gracias por usar este servicio.\n");
		return 0;
}

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
#define BUFFER_SZ 5000*1024   /*Tamaño de buffer*/

int sock_client(){
	  struct sockaddr_in serv_addr;
    int s, rc;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7500);
    // Localhost:
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Red local (mi IP):
    // serv_addr.sin_addr.s_addr = inet_addr("192.168.100.7");

    // Si sock < 0 hay un error en la creación del socket:
    s = socket( AF_INET, SOCK_STREAM, 0 );
    if (s < 0){
      perror("Problema creando el socket");
      exit(1);
    }

    // Conexión del cliente al servidor:
    rc = connect(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (rc){
      perror("Problema al intentar la conexión con el servidor");
      exit(1);
    }

    return s;
}

int main(int argc, char **argv)
{
    int sock = 0, rc, n = 0;
    char buffer[BUFFER_SZ];
    unsigned t_i, t_f;
    double ttime, transf;

    // Inicio
    printf("=== CLIENT ===\n");
    printf("Receptor de paquetes.\n");


    // Si sock < 0 hay un error en la creación del socket:
		sock = sock_client();
    t_i = clock();

    // Recibimos paquetes:
    for (int i=0; i<10000; i++){
      rc = recv(sock, buffer, BUFFER_SZ, 0);
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

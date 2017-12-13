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
#define BUFFER_SZ 5000*1024   /*Tamaño de buffer*/

int sock_server(){
    struct sockaddr_in serv_addr;
    int s, rc;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7500);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Si sock < 0 hay un error en la creación del socket:
    s = socket( AF_INET, SOCK_STREAM, 0 );
    if (s < 0){
      perror("Problema creando el socket");
      exit(1);
    }

    // Bind del servidor al cliente:
    rc = bind(s, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (rc < 0){
      perror("Problema en bind");
      exit(1);
    }

    // Listen al cliente:
    rc = listen(s, 5);
    if (rc){
      perror("Problema en listen");
      exit(1);
    }

    return s;
}

int main(int argc, char **argv)
{
    int sock = 0, s, rc;
    char buffer[BUFFER_SZ];
    pid_t pid, wpid;

    // Inicio:
    printf("=== SERVER ===\n");
    printf("Encargado del envío de paquetes.\n");
    printf("Paquetes por enviar: 10000.\n");
    printf("Tamaño de buffer: %d\n", BUFFER_SZ);

    // Creamos el socket:
    sock = sock_server();
    s = accept(sock, NULL, NULL);
    if (s < 0){
      perror("Problema aceptando conexión");
      exit(1);
    }

    // Enviamos paquetes:
    for (int i=0; i<10000; i++){
      rc = send(s, buffer, BUFFER_SZ, 0);
    }
    send(s, NULL, 0, 0);

    // Cerramos socket:
    close(sock);
    printf("Gracias por usar este servicio.\n");
   	return 0;
}

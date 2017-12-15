#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SZ 256   /*Tamaño de buffer*/

int sock_client(){
	  struct sockaddr_in serv_addr;
    int s, rc;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7500);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

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

void recv_file(int sock, char *filename){
		// Recibimos paquetes:
    char buffer[BUFFER_SZ];
		int rc;
		FILE *f;
		f = fopen(filename, "w");

		rc = recv(sock, buffer, sizeof(buffer), 0);
		while (rc){
			fprintf(f, "%s", buffer);
			bzero(buffer, BUFFER_SZ);
			rc = read(sock, buffer, BUFFER_SZ);
		}
		fclose(f);

		printf("The file was received successfully!\n\n");
}

int main(int argc, char **argv)
{
    int sock = 0, rc, n = 0;
		char opt;
    char buffer[BUFFER_SZ];
		char filename[BUFFER_SZ];
    FILE *f;

    // Inicio
    printf("=== CLIENT ===\n");
    printf("Receptor de archivos.\n");


    // Creamos el socket:
		sock = sock_client();
		printf("Menu:  \n");
		printf("1. Desargar archivo \n");
		printf("2. Introducir comando del sistema \n");
		printf("3. Salir \n\n");

    // Ciclo de control:
		while (1) {
			printf("Opción: ");
			scanf("%c", &opt);
			rc = send(sock, &opt, 1, 0);

			opt = (int) opt;
			if (opt == 49) {
				recv_file(sock, "new.txt");
				break;
			}
			if (opt == 50) {
				printf("Inroduce comando: \n");
				scanf("%s", &buffer);
				// printf("%s\n", buffer);
				send(sock, buffer, BUFFER_SZ, 0);
				system(buffer);
			}
			if (opt == 51) {
				break;
			}
		}

    // Cerramos socket:
		close(sock);
		printf("Gracias por usar este servicio.\n");
		return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sysctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>

#define MAXLINE 255 /*Longitud máxima de mensaje*/

void print_instructions();

void print_instructions(){
	printf("=== Instrucciones ===\n");
	printf("here \t\t- Imprime dirección actual.\n");
	printf("clear \t\t- Limpia pantalla.\n");
	printf("ls \t\t- Enlista archivos en la dirección actual.\n");
	printf("more <filename> - Imprime contenido del archivo.\n");
	printf("get <filename> \t- Descarga <filename> de la dirección actual.\n");
	printf("cd <path> \t- Cambia dirección actual a <path>.\n");
	printf("exit \t\t- Termina el programa del lado del cliente.\n");
	printf("\n");
}

int main(int argc, char **argv)
{
		int sockfd;
		int n = 0;
		struct sockaddr_in servaddr;
		char recvline[MAXLINE];
		char line[MAXLINE];

		// Creación del socket:
		memset(&servaddr, 0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr= inet_addr("127.0.0.1");
		servaddr.sin_port = htons(7500);

    // Inicio
    printf("=== CLIENT ===\n");
		print_instructions();

		while (1) {
				// Si sockfd<0 hay un error en la creación del socket.
				if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
						perror("Problema creando el Socket.");
						exit(2);
				}

				// Conexión del clinte al socket del servidor:
				if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
				{
						perror("Problema al intentar la conexión con el servidor");
						exit(3);
				}

				// Introducimos comando:
				bzero(recvline, MAXLINE);
				bzero(line, MAXLINE);
				printf("Introduce comando: ");
			  fgets(recvline, MAXLINE, stdin);
				strcpy(line, &recvline[0]);
				line[3] = '\0';

				// Le escribimos al servidor a través del socket:
				n = write(sockfd, recvline, strlen(recvline));
				if (n < 0) {
			      perror("ERROR: No se pudo enviar al socket.");
			      exit(4);
			  }

				// Si escribimos 'exit', salimos:
    		if (strcmp(recvline, "exit\n") == 0){
	    			printf("Bye.\n");
	    			break;
    		}

				// Si escribimos 'clear':
    		if (strcmp(recvline, "clear\n") == 0){
						system("clear");
						print_instructions();
    		}

				// Si escribimos 'ls':
    		else if (strncmp(recvline, "ls", 2) == 0){
						bzero(recvline, MAXLINE);
						while ((n = read(sockfd, recvline, MAXLINE)) > 0){
								printf("%s", recvline);
								bzero(recvline, MAXLINE);
						}
						printf("\n");
    		}

				// Si escribimos 'here':
				else if (strcmp(recvline, "here\n") == 0){
						bzero(recvline,MAXLINE);
						n = read(sockfd, recvline, MAXLINE);
						if (n < 0) {
								perror("ERROR al leer del socket.");
								exit(1);
						}
						printf("%s\n\n", recvline);
    		}

				// Si escribimos 'get':
				else if (strncmp(recvline, "get", 3) == 0){
						FILE *f;
						// f = fopen(recvline, "w");
						recvline[strlen(recvline)-1] = '\0';
						f = fopen(&(recvline[4]), "w");
						printf("Archivo descargado: %s\n", &(recvline[4]));

						// Imprimimos la respuesta del servidor:
						bzero(recvline, MAXLINE);
						n = read(sockfd, recvline, MAXLINE);
						if (n < 0) {
								perror("ERROR al leer del socket.");
								exit(1);
						}
						// printf("File opened: %s\n", recvline);
						bzero(recvline, MAXLINE);
						while ((n = read(sockfd, recvline, MAXLINE)) > 0){
								fprintf(f, "%s", recvline);
								bzero(recvline,MAXLINE);
						}
						fclose(f);
    		}

				// Si los primeros 3 caracteres son 'cd ':
    		else if (strcmp(line, "cd ") == 0){
						bzero(recvline, MAXLINE);
						n = read(sockfd, recvline, MAXLINE);
						if (n < 0) {
                perror("ERROR al leer del socket.");
                exit(1);
            }
						printf("Nuevo directorio: %s\n\n", recvline);
    		}

				// Si escribimos 'more':
    		else if (strncmp(recvline, "more", 4) == 0){
						// Imprimimos la respuesta del servidor:
						bzero(recvline, MAXLINE);
						n = read(sockfd, recvline, MAXLINE);
						if (n < 0) {
                perror("ERROR al leer del socket.");
                exit(1);
            }
						printf("File opened: %s\n", recvline);
					 	bzero(recvline, MAXLINE);
						while ((n = read(sockfd, recvline, MAXLINE)) > 0){
								printf("%s", recvline);
								bzero(recvline,MAXLINE);
						}
						printf("\n");
    		}
		}
		exit(0);
}

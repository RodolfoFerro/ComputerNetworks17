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

int sock_server(){
    struct sockaddr_in serv_addr;
    int s, rc;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7500);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Si sock < 0 hay un error en la creación del socket:
    s = socket(AF_INET, SOCK_STREAM, 0);
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

pid_t create_child(){
	pid_t pid;
  pid = fork();
  if (pid == (pid_t)-1){
      perror("Error en child");
      exit(1);
  }
  return pid;
}

void send_file(int s, FILE *fp, char * filename){
    // Enviamos archivo:
    char buffer[BUFFER_SZ];
    int rc;
    fp = fopen(filename, "r");

    while (!feof(fp)){
      fscanf(fp, "%s", buffer);
      rc = write(s, buffer, BUFFER_SZ);
      bzero(buffer, BUFFER_SZ);
    }

    printf("The file was sent successfully!\n\n");
    return;
}

int main(int argc, char **argv)
{
    int sock = 0, s, rc;
    char opt;
    char buffer[BUFFER_SZ];
		char filename[BUFFER_SZ];
    FILE *fp;
    pid_t child;

    // Inicio:
    printf("=== SERVER ===\n");
    printf("Encargado del envío de archivos.\n");

    // Creamos el socket:
    sock = sock_server();

    // Ciclo de control:
    while (1){
        // Aceptamos conexiones:
        s = accept(sock, NULL, NULL);
        if (s < 0){
          perror("Problema aceptando conexión");
          exit(1);
        }

        // Hacemos fork:
        child = create_child();

        if (child == 0){
          while (1) {
            rc = recv(s, &buffer, 1, 0);
            if(rc < 0) {
              perror("Error recibiendo opción");
              exit(1);
            }

            opt = buffer[0];
            printf("Opción recibida: %c\n", opt);
            switch (opt) {
              case '1':
                send_file(s, fp, "test.txt");
                break;

      				default:
      					printf("HERE!\n");
                break;
            }
          }
        }
    }



    // Cerramos socket:
    fclose(fp);
    close(sock);
    printf("Gracias por usar este servicio.\n");
    // system("ls");
   	return 0;
}

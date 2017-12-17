#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAXLINE 255 /*Longitud máxima de mensaje*/

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
	  int rc, n, nl;
    char buffer[MAXLINE];
    char buff[MAXLINE];
    char line[MAXLINE];
    FILE *ptr_file;
    struct dirent **namelist;
    char *list;
    char cwd[MAXLINE];
    char new_path[MAXLINE];
    char dir[MAXLINE];
    pid_t cpid, w;
    int status;

    // Creación del socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(7500);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);

    // Inicio:
    printf("=== SERVER ===\n");
    printf("Encargado de la atención de peticiones.\n");

    // Obtenemos directorio donde estamos:
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("ERROR al obtener directorio");
    strcpy(dir, cwd);

    while(1)
    {
        // Aceptamos conexión:
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        if (connfd < 0) {
            perror("ERROR al aceptar");
            exit(1);
        }

        // Creamos un subproceso:
        if ((cpid = fork()) == 0){
            // Leemos a través del socket:
            bzero(buffer, MAXLINE);
            n = read(connfd, buffer, MAXLINE);
            if (n < 0) {
                perror("ERROR al leer del socket");
                exit(1);
            }
            printf("Request: %s", buffer);
    				bzero(line, MAXLINE);
    				strcpy(line, &buffer[0]);
    				line[3] = '\0';

            // Si recibimos 'exit', salimos:
        		if (strcmp(buffer, "exit\n") == 0){
        		    break;
        		}

            // Si recibimos "ls" - Mostrar listado de archivos:
            if (strcmp(buffer, "ls\n") == 0){
                nl = scandir(dir, &namelist, 0, alphasort);
                if (nl < 0)
                    perror("Error al escanear contenido de directorio");
                else{
                    for (int i = 0; i < nl; i++){
                        list = namelist[i]->d_name;
                        strcat(list, "\n");
                        rc = write(connfd, list, strlen(list));
                        if(rc<=0) perror("ERROR al imprimir ls");
                    }
                    for (int i = 0; i < nl; i++){
                        free(namelist[i]);
                    }
                    free(namelist);
                }
            }

            // Si recibimos "here" - Mostramos directorio actual:
            else if (strcmp(buffer, "here\n") == 0){
                n = write(connfd, dir, strlen(dir));
                if (n < 0) {
                    perror("ERROR al enviar al socket");
                    exit(4);
                }
        		}

            // Si recibimos "get" - Descargamos archivo del directorio actual:
            else if (strncmp(buffer, "get", 3) == 0){
              // Abrimos archivo seleccionado:
              bzero(buff, MAXLINE);
              strcpy(buff, &(buffer[4]));
              buff[strlen(buff) - 1] = '\0';
              ptr_file = fopen(buff, "r");
              if (ptr_file == NULL){
                  if (errno == 2){
                      n = write(connfd, "No existe ese archivo. :(", strlen("No existe ese archivo. :("));
                      if (n < 0) {
                          perror("ERROR al enviar al socket.");
                          exit(4);
                      }
                  }
                  else{
                      n = write(connfd, "Archivo en uso, intente después.", strlen("Archivo en uso, intente después."));
                      if (n < 0) {
                          perror("ERROR al enviar al socket.");
                          exit(4);
                      }
                  }
                  fclose(ptr_file);
              }
              else{
                  // Avisamos qué archivo abrimos:
                  // printf("Opening file: %s\n", buff);
                  n = write(connfd, buff, strlen(buff));
                  if (n < 0) {
                      perror("ERROR al enviar al socket.");
                      exit(4);
                  }
                  while (fgets(buff, MAXLINE, ptr_file) != NULL){
                      n = write(connfd, buff, strlen(buff));
                      if (n < 0) {
                          perror("ERROR al enviar al socket.");
                          exit(4);
                      }
                  }
                  fclose(ptr_file);
              }
        		}

            // Si recibimos "cd dir" - Cambiamos directorio:
            else if (strcmp(line, "cd ") == 0){
    				    // bzero(new_path,MAXLINE);
    						// strcat(new_path, &(buffer[3]));
                // new_path[strlen(new_path) - 1] = '\0';
                // setPath(dir, new_path);
                chdir(&(buffer[3]));
                chroot(&(buffer[3]));
                if (getcwd(cwd, sizeof(cwd)) == NULL)
                    perror("ERROR al obtener directorio");
                strcpy(dir, cwd);
    						n = write(connfd, dir, strlen(dir));
    						if (n < 0) {
    					      perror("ERROR al enviar al socket.");
    					      exit(4);
    					  }
                printf("New dir: %s", dir);
        		}

            // Si recibimos "more" - Abrimos archivo:
            else if (strncmp(buffer, "more", 4) == 0){
                // Abrimos archivo seleccionado:
                bzero(buff, MAXLINE);
                strcpy(buff, &(buffer[5]));
                buff[strlen(buff) - 1] = '\0';
                ptr_file = fopen(buff, "r");
                if (ptr_file == NULL){
                    if (errno == 2){
                        n = write(connfd, "No existe ese archivo. :(", strlen("No existe ese archivo. :("));
                        if (n < 0) {
            					      perror("ERROR al enviar al socket.");
            					      exit(4);
            					  }
                    }
                    else{
                        n = write(connfd, "Archivo en uso, intente después.", strlen("Archivo en uso, intente después."));
                        if (n < 0) {
                            perror("ERROR al enviar al socket.");
                            exit(4);
                        }
                    }
                    fclose(ptr_file);
                }
                else{
                    // Avisamos qué archivo abrimos:
                    // printf("Opening file: %s\n", buff);
                    n = write(connfd, buff, strlen(buff));
                    if (n < 0) {
        					      perror("ERROR al enviar al socket.");
        					      exit(4);
        					  }
                    while (fgets(buff, MAXLINE, ptr_file) != NULL){
                        n = write(connfd, buff, strlen(buff));
                        if (n < 0) {
            					      perror("ERROR al enviar al socket.");
            					      exit(4);
            					  }
                    }
                    fclose(ptr_file);
                }
        		}
        }

        // Proceso padre:
        close(connfd);
        sleep(1);
     }
}

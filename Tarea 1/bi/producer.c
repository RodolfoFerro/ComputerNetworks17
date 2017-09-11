#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>

#define MAXLINE 255 /*Longitud máxima de mensaje*/

int main()
{
  // Declaración de variables a usar:
	const int SIZE = 4096;
	const char *name = "MI CHAT";
	char message[MAXLINE];
	bzero(message, MAXLINE);

	int shm_fd;
	void *ptr;
  char temp[MAXLINE];

	// Creamos memoria ocmpartida:
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	// Configuramos el tamaño de memoria:
	ftruncate(shm_fd, SIZE);

	// Mapeamos el segmento de memoria compartida en la dirección del proceso:
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("¡Falló el mapeo!\n");
		return -1;
	}

	// Mensaje de bienvenida:
	printf("=== PRODUCER ===\n");
	printf("Bienvenido a %s.\n\n", name);
	// printf("Siéntete libre de enviar cualquier mensaje.\n\n");
	printf("Introduce un mensaje: \n");
	fflush(stdout);

	INIT:
	fpurge(stdout);

	// Definimos tiempo de espera:
	struct timeval tmo;
  fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(0, &readfds);
	tmo.tv_sec = 0;
	tmo.tv_usec = 5;

	// Espera de input de texto:
	switch (select(1, &readfds, NULL, NULL, &tmo)) {
    case -1:
      err(1, "select");
      break;
    case 0:
			// Imprimimos mensajes de la memoria compartida:
			if (strcmp(ptr, temp) != 0) {
				// Si escribimos 'EXIT', salimos:
				if (strcmp(ptr, "EXIT\n") == 0) {
					printf("¡Hasta luego!\n");
					goto END;
				}
				else {
					printf("\rMensaje recibido: %s", ptr);
					printf("Introduce un mensaje: \n");
					fflush(stdout);
					bzero(temp, MAXLINE);
		      strcpy(temp, ptr);
				}
	    }
			sleep(1);
			goto INIT;
  }

	// Limpiamos y escribimos un mensaje:
	printf("Introduce un mensaje: \n");
	fflush(stdout);
	fgets(message, MAXLINE, stdin);

	if (strcmp(message, "") != 0) {
		// bzero(temp, MAXLINE);
		sprintf(ptr, "%s", message);
		strcpy(temp, ptr);

		// Si escribimos 'EXIT', salimos:
		if (strcmp(message, "EXIT\n") == 0) {
			printf("¡Hasta luego!\n");
			goto END;
		} else goto INIT;
	}

  // Cerramos conexión y removemos el segmento de memoria compartida:
	END:
  close(shm_fd);
  if (shm_unlink(name) == -1) {
    printf("Gracias por usar el servicio de %s.\n",name);
    exit(-1);
  }

	return 0;
}

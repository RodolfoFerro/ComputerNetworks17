#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#define MAXLINE 255 /*Longitud máxima de mensaje*/

int main()
{
	const int SIZE = 4096;
	const char *name = "MI CHAT";
	char message[MAXLINE];

	int shm_fd;
	void *ptr;

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

  // Ahora escribimos en la región de memoria:
  while (1) {
		// Limpiamos y escribimos un mensaje:
    bzero(message, MAXLINE);
    printf("Introduce mensaje: ");
    fgets(message, MAXLINE, stdin);

  	sprintf(ptr, "%s", message);
    sleep(1);

    // Si escribimos 'EXIT', salimos:
		if (strcmp(message, "EXIT\n") == 0){
			printf("Sayonara!\n");
			break;
		}
  }

  // Removemos el segmento de memoria compartida:
  close(shm_fd);
  if (shm_unlink(name) == -1) {
    printf("Gracias por usar el servicio de %s.\n",name);
    exit(-1);
  }
	return 0;
}

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
	const char *name = "MI CHAT";
	const int SIZE = 4096;
  char message[MAXLINE];

	int shm_fd;
	void *ptr;
  char temp[MAXLINE];

	// Abrimos el segmento de memoria compartida:
  shm_fd = shm_open(name, O_RDONLY, 0666);
  if (shm_fd == -1) {
    printf("¡Falla en memoria compartida!\n");
    exit(-1);
  }

  // Mapeamos el segmento de memoria compartida en la dirección del proceso:
	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("¡Falló el mapeo!\n");
		exit(-1);
	}

  while (1) {
		// Leemos de la región de memoria compartida:
    if (strcmp(ptr, temp) != 0){
			printf("Mensaje recibido: %s", ptr);
			bzero(temp, MAXLINE);
      strcpy(temp, ptr);
		}

    // Si escribimos 'EXIT', salimos:
		if (strcmp(ptr, "EXIT\n") == 0){
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

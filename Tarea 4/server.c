#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXLINE 255 /*Longitud máxima de mensaje*/

struct client_info {
		int sockno;
		char ip[INET_ADDRSTRLEN];
};

int clients[100];
int n = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void sendtoall(char *msg, int curr) {
		pthread_mutex_lock(&mutex);
		for(int i = 0; i < n; i++) {
				if(clients[i] != curr) {
						if(send(clients[i], msg, strlen(msg), 0) < 0) {
							perror("Error al enviar");
							continue;
						}
				}
		}
		pthread_mutex_unlock(&mutex);
}

void *recvmg(void *sock) {
		struct client_info cl = *((struct client_info *)sock);
		char msg[MAXLINE];
		int len, i, j;
		while((len = recv(cl.sockno, msg, MAXLINE, 0)) > 0) {
				msg[len] = '\0';
				sendtoall(msg, cl.sockno);
				memset(msg, '\0', sizeof(msg));
		}

		pthread_mutex_lock(&mutex);
		printf("%s disconnected\n", cl.ip);
		// printf("Usuario desconectado.");
		for(i = 0; i < n; i++) {
				if(clients[i] == cl.sockno) {
						j = i;
						while(j < n-1) {
								clients[j] = clients[j+1];
								j++;
						}
				}
		}
		n--;
		pthread_mutex_unlock(&mutex);
}

int main(int argc,char *argv[])
{
		struct sockaddr_in my_addr,their_addr;
		struct client_info cl;
		socklen_t their_addr_size;
		int my_sock, their_sock, portno, len;
		pthread_t sendt,recvt;
		char msg[MAXLINE], ip[INET_ADDRSTRLEN];

		// Inicio
		printf("=== SERVER ===\n");
		printf("Gestor de mensajes.\n");

	  // Creamos socket:
		my_sock = socket(AF_INET, SOCK_STREAM, 0);
		memset(my_addr.sin_zero, '\0', sizeof(my_addr.sin_zero));
		my_addr.sin_family = AF_INET;
		my_addr.sin_port = htons(7500);
		my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		their_addr_size = sizeof(their_addr);

	  // Hacemos bind
		if(bind(my_sock, (struct sockaddr *)&my_addr, sizeof(my_addr)) != 0) {
				perror("Error en bind");
				exit(1);
		}

	  // Hacemos listen:
		if(listen(my_sock, 5) != 0) {
				perror("Error en listen");
				exit(1);
		}

		while(1) {
				if((their_sock = accept(my_sock, (struct sockaddr *)&their_addr, &their_addr_size)) < 0) {
					perror("Error en accept");
					exit(1);
				}
				pthread_mutex_lock(&mutex);
				inet_ntop(AF_INET, (struct sockaddr *)&their_addr, ip, INET_ADDRSTRLEN);
				printf("%s connected\n", ip);
				// printf("Nuevo usuario conectado.");
				cl.sockno = their_sock;
				strcpy(cl.ip, ip);
				clients[n] = their_sock;
				n++;
				pthread_create(&recvt, NULL, recvmg, &cl);
				pthread_mutex_unlock(&mutex);
		}
		return 0;
}

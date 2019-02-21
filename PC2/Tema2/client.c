#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include "server.h"
#include "erori.h"

#define BUFLEN 256

int findCard(char *message) {
	char *p;
	p = strtok(message, " ");
	p = strtok(NULL, " ");
	return atoi(p);
}

int main(int argc, char **argv) {
	struct sockaddr_in tcp_addr, udp_addr;
	fd_set read_fds, aux_read;
	char buffer[BUFLEN];
	int tcp_fd, udp_fd, portno;
	int fdmax, sock_len;

	int card_unlock = -1;
	int login = 0;

	FILE *out;
	char file_name[100];
	char aux_str[100];

	if(argc != 3) {
		perror("Numar gresit de argumente!");
		exit(1);
	}

	/*  Deschiderea socketilor  */
	tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(tcp_fd < 0) {
		perror("Eroare deschidere socket tcp!");
		exit(1);
	}

	udp_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(udp_fd < 0) {
		perror("Eroare deschidere socket udp!");
		exit(1);
	}

	/*  Golire descriptori  */
	FD_ZERO(&read_fds);
	FD_ZERO(&aux_read);

	/*  Setare sockaddr_in  */
	memset((char *)&tcp_addr, 0, sizeof(tcp_addr));
	tcp_addr.sin_family = AF_INET;
	tcp_addr.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1], &tcp_addr.sin_addr);

	memset((char *)&udp_addr, 0, sizeof(udp_addr));
	udp_addr.sin_family = AF_INET;
	udp_addr.sin_port = ntohs(atoi(argv[2]));
	inet_aton(argv[1], &udp_addr.sin_addr);

	/*  Setare file descriptori  */
	FD_SET(0, &read_fds);
	FD_SET(tcp_fd, &read_fds);
	FD_SET(udp_fd, &read_fds);
	fdmax = tcp_fd > udp_fd ? tcp_fd : udp_fd;
	sock_len = sizeof(struct sockaddr_in);

	/*  Deschidem fisierul de log  */
	strcpy(file_name, "client-");
	sprintf(aux_str, "%d", getpid());
	strcat(file_name, aux_str);
	strcat(file_name, ".log");
	out = fopen(file_name, "w");

	/*  Conectare prin tcp  */
	if(connect(tcp_fd, (struct sockaddr*) &tcp_addr, sizeof(tcp_addr)) < 0) {
		perror("Eroare la conectare prin tcp!");
		exit(1);
	}

	while(1) {
		aux_read = read_fds;
		memset(buffer, 0, sizeof(buffer));
		select(fdmax + 1, &aux_read, NULL, NULL, NULL);

		/*  Daca este input de la stdin  */
		if(FD_ISSET(0, &aux_read)) {
			fgets(buffer, BUFLEN-1, stdin);
			buffer[strlen(buffer) - 1] = '\0';

			/*  Daca inchid procesul  */
			if(strstr(buffer, "quit") != NULL) {
				fprintf(out, "quit\n");
				send(tcp_fd, buffer, strlen(buffer), 0);
				break;
			}

			/*  Daca primesc 'transfer' mai trimit un mesaj cu y/n  */
			if(strstr(buffer, "transfer") != NULL) {
				send(tcp_fd, buffer, strlen(buffer), 0);
				memset(buffer, 0, sizeof(buffer));
				recv(tcp_fd, buffer, BUFLEN, 0);
				fprintf(out, "%s\n", buffer);
				printf("%s\n", buffer);
				fgets(buffer, BUFLEN-1, stdin);
				buffer[strlen(buffer) -1] = '\0';
				send(tcp_fd, buffer, strlen(buffer), 0);

		  /*  Cazul in care primesc o comanda de unlock  */
			} else if( strstr(buffer, "unlock") != NULL) {
				sprintf(buffer, "unlock %d", card_unlock);

				sendto(udp_fd, buffer, BUFLEN, 0, (struct sockaddr*) &udp_addr, sizeof(udp_addr));
				recvfrom(udp_fd, buffer, BUFLEN, 0, (struct sockaddr*) &udp_addr, &sock_len);

				/*  Afisez mesajul la consola si in logfile  */
				printf("%s\n", buffer);
				fprintf(out, "%s\n", buffer);

				/*  Citesc de la stdin parola secreta si o trimit catre server  */
				memset(buffer, 0, BUFLEN);
				fgets(buffer, BUFLEN-1, stdin);
				buffer[strlen(buffer)-1] = '\0';
				fprintf(out, "%s\n", buffer);

				sendto(udp_fd, buffer, strlen(buffer), 0, (struct sockaddr*) &udp_addr, sizeof(udp_addr));
				recvfrom(udp_fd, buffer, BUFLEN, 0, (struct sockaddr*) &udp_addr, &sock_len);

				/*  Afisez mesajul la consola si in logfile  */
				printf("%s\n", buffer);
				fprintf(out, "%s\n", buffer);

			/*  Daca este login trimite prin tcp  */
			} else if ( strstr(buffer, "login") != NULL) {
				if(login == 1) {
					printf("%s\n", ERR2);
					fprintf(out, "%s\n", ERR2);
					continue;
				}

				card_unlock = findCard(strdup(buffer));
				send(tcp_fd, buffer, strlen(buffer), 0);
				fprintf(out, "%s\n", buffer);

			/*  Daca este logout trimite prin tcp  */
			} else if ( strstr(buffer, "logout") != NULL) {
				if(login == 0) {
					printf("%s\n", ERR1);
					fprintf(out, "%s\n", ERR1);
					continue;
				}
				login = 0;
				send(tcp_fd, buffer, strlen(buffer), 0);
				fprintf(out, "%s\n", buffer);

			/*  Daca este orice altceva trimite prin tcp  */
			} else {
				if (send(tcp_fd, buffer, strlen(buffer), 0) < 0) {
					perror("Eroare trimitere prin tcp!");
					exit(1);
				} else {
					fprintf(out, "%s\n", buffer);
				}
			}
		}

		/*  Daca am primit de la socketul tcp  */
		if(FD_ISSET(tcp_fd, &aux_read)) {
			if(recv(tcp_fd, buffer, BUFLEN, 0) < 0) {
				perror("Eroare primire prin tcp!");
				exit(1);
			} else {

			/*  Daca am primit quit de la server inchid clientul  */
			if(strcmp(buffer, "quit") == 0) {
				fprintf(out, "quit\n");
				break;
			} else if (strstr(buffer, "Welcome") != NULL) {
				login = 1;
			}

			/*  Afisez mesajul la consola si in logfile  */
			printf("%s\n", buffer);
			fprintf(out, "%s\n", buffer);
			}
		}

		/*  Daca am primit de la socketul udp  */
		if(FD_ISSET(udp_fd, &aux_read)) {
			if(recvfrom(udp_fd, buffer, BUFLEN, 0, (struct sockaddr*) &udp_addr, &sock_len) < 0) {
				perror("Eroare primire prin udp!");
				exit(1);
			} else {
				printf("%s\n", buffer);
				fprintf(out, "%s\n", buffer);
			}
		}
	}

	close(tcp_fd);
	close(udp_fd);
	fclose(out);

	return 0;
}

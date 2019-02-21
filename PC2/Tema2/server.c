#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "erori.h"

char* take_message(int nr_conturi, cont* conturi, int fd, char * message);
char* unlock(int fd, char* buf, struct sockaddr_in client_addr, struct sockaddr_in udp_addr, cont* conturim, int n);

int main(int argc, char **argv) {
	int udp_fd, tcp_fd, new_sock;
	struct sockaddr_in tcp_addr, udp_addr;
	struct sockaddr_in client_addr;
	FILE *in;
	cont *conturi;
	int number_clients;
	int i, j, aux_check;

	int tcp_fds[10];
	int tcp_no = 0;

	char buffer[BUFLEN];

	fd_set read_fds, aux_read;
	int fdmax, cli_len;

	if(argc != 3) {
		perror("Numar gresit de argumente!");
		exit(1);
	}

	/*  Citirea datelor din fisierul dat ca argument  */
	in = fopen(argv[2], "r");
	fscanf(in, "%d", &number_clients);

	conturi = (cont*) malloc(number_clients * sizeof(cont));
	for(i = 0; i < number_clients; i++) {
		fscanf(in, "%s%s%d%d%s%lf", conturi[i].nume, conturi[i].prenume,
				&conturi[i].nr_card, &conturi[i].pin,
				conturi[i].parola_secreta, &conturi[i].sold);

		conturi[i].sockfd = -1;
		for(j = 0; j < 100; j++) {
			conturi[i].incercari[j] = 0;
		}
	}

	fclose(in);

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

	/*  Golirea descriptorilor pentru TCP  */
	FD_ZERO(&read_fds);
	FD_ZERO(&aux_read);

	/*  Setare sockaddr_in  */
	memset((char*)&tcp_addr, 0, sizeof(tcp_addr));
	tcp_addr.sin_family = AF_INET;
	tcp_addr.sin_addr.s_addr = INADDR_ANY;
	tcp_addr.sin_port = htons(atoi(argv[1]));

	memset((char*)&udp_addr, 0, sizeof(udp_addr));
	udp_addr.sin_family = AF_INET;
	udp_addr.sin_addr.s_addr = INADDR_ANY;
	udp_addr.sin_port = htons(atoi(argv[1]));

	/*  Bind socketi  */
	if(bind(tcp_fd, (struct sockaddr *) &tcp_addr, sizeof(struct sockaddr)) < 0) {
		perror("Eroare bind tcp!");
		exit(1);
	}

	if(bind(udp_fd, (struct sockaddr *) &udp_addr, sizeof(struct sockaddr)) < 0) {
		perror("Eroare bind udp!");
		exit(1);
	}

	/*  Incepe listen pe MAX_CLIENTS  */
	listen(tcp_fd, MAX_CLIENTS);

	/*  Setare socketi pentru select  */
	FD_SET(0, &read_fds);
	FD_SET(tcp_fd, &read_fds);
	FD_SET(udp_fd, &read_fds);
	fdmax = tcp_fd > udp_fd ? tcp_fd : udp_fd;

	/*  Incepe primirea datelor  */
	while(1) {
		aux_read = read_fds;
		if(select(fdmax + 1, &aux_read, NULL, NULL, NULL) == -1)
			perror("Error in select!");

		/*  Daca am primit de la stdin verific singurul mesaj posibil -> 'quit'  */
		if(FD_ISSET(0, &aux_read)) {
			fgets(buffer, BUFLEN-1, stdin);
			buffer[strlen(buffer) -1] = '\0';
			if(strcmp(buffer, "quit") == 0) {
				for(i = 0; i < tcp_no; i++) {
					send(tcp_fds[i], buffer, strlen(buffer), 0);
				}
				break;
			}
		}

		/*  Dupa select verific modificarea din fiecare file descriptor  */
		for(i = 1; i <= fdmax; i++) {
			if(FD_ISSET(i, &aux_read)) {

				/*  Daca este in socketul setat pe listen atunci a venit o conexiune noua  */
				if (i == tcp_fd) {
					cli_len = sizeof(struct sockaddr_in);
					if((new_sock = accept(tcp_fd, (struct sockaddr*) &client_addr, &cli_len)) == -1) {
						perror("Eroare la accept");
					} else {
						tcp_fds[tcp_no++] = new_sock;
						FD_SET(new_sock, &read_fds);
						fdmax = fdmax > new_sock ? fdmax : new_sock;
					}
					printf("Noua conexiune de la %s, port %d, socket_client %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), new_sock);

				/*  Daca am primit pe socketul de udp  */
			  } else if (i == udp_fd) {
					memset(buffer, 0, BUFLEN);
					if(recvfrom(i, buffer, BUFLEN, 0, (struct sockaddr*) &client_addr, &cli_len) < 0) {
						perror("Eroare primire prin udp");
					} else {
						char buf[BUFLEN];
						strcpy(buf, unlock(i, buffer, client_addr, udp_addr, conturi, number_clients));
						sendto(i, buf, strlen(buf), 0, (struct sockaddr *) &client_addr, sizeof(udp_addr));
					}

				/*  Altfel primesc mesajul de la client si il procesez  */
				} else {

					/*  Daca sunt probleme la primirea mesajului  */
					memset(buffer, 0, BUFLEN);
					if(recv(i, buffer, sizeof(buffer), 0) < 0) {
						perror("Eroare la primire mesaj");

					/*  De aici sunt procesate comenzile de la clienti  */
					} else {
						strcpy(buffer, take_message(number_clients, conturi, i, buffer));
						if(strstr(buffer, "Deconectat") != NULL) {
							sprintf(buffer, "Deconectat %d!", i);
							puts(buffer);
							//perror("Deconectat!");
							FD_CLR(i, &read_fds);
						} else {
							send(i, buffer, strlen(buffer), 0);
						}
					}
				}

			}  //  exit IS_SET
		}  //  exit for verificare select
	}  //  exit while(1)

	/*  Sfarsitul executiei  */
	close(tcp_fd);
	close(udp_fd);

	return 0;
}

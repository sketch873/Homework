#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

char mark, seq, maxl, timeout;

int main(int argc, char** argv) {
	unsigned short crc, crc_aux;
    int seq_time = 0, time = 0;
	char name[100], data[250];
	FILE* fout;
	msg *message, *initMessage;
	msg ack;

	init(HOST, PORT);

	/*
	 * Setez datele temporar, pana la primitrea
	 * unui pachet init.
	*/
	seq = 0;
	mark = 0;
	timeout = 5;

	/* Astept init cu timeout. la trei consecutive opresc conexiunea */
	while(1){
		initMessage = receive_message_timeout(timeout * 1000);
		if(initMessage == NULL) {
			seq_time++;
			continue;
		}

		crc = crc16_ccitt(initMessage->payload, initMessage->len - 3);
		memcpy(&crc_aux, initMessage->payload + initMessage->len - 3, 2);

		if(crc != crc_aux || initMessage->payload[3] != 'S') {

			ack = make_N(seq, mark);
			send_message(&ack);
			seq_time = 0;
		
		} else {
			maxl = initMessage->payload[4];
			timeout = initMessage->payload[5];
			seq = initMessage->payload[2];
			mark = initMessage->payload[8];
			ack = make_YY(seq++, mark, *initMessage);
			send_message(&ack);
			break;
		}
	}

	if(seq_time == 3) {
		perror("Init message gone!");
		return -1;
	}

	/*
	 * Primirea pachetelor de la sender, cu exceptia celui S.
	 * -> Pachet F: deschide fisierul cu numele primit
	 * -> Pachet D: scrie in fisierul deschis datele
	 * -> Pachet Z: inchide fisierul
	*/
	while(1) {
		seq %= 64;
		message = receive_message_timeout(timeout*1000);
		if(message == NULL) {
			ack = make_N(seq, mark);
			send_message(&ack);
			
			time++;
			if(time == 3) {
				perror("Did not receive 3 time!");
				return -1;
			}
			continue;
		
		} else {
			time = 0;
		}
		
		crc = crc16_ccitt(message->payload, message->len - 3);
		memcpy(&crc_aux, message->payload + message->len - 3, 2);

		/* Daca e checksum-ul gresit */
		if(crc != crc_aux) {
			ack = make_N(seq, mark);
			send_message(&ack);
			continue;
		
		/* Daca numarul de secventa e gresit */
		} else if(seq != message->payload[2]) {
			ack = make_N(seq, mark);
			send_message(&ack);
			continue;

		/* Daca pachetul este corect */
		} else {
			ack = make_YY(seq++, mark, *message);
			send_message(&ack);
		}

		/* Verificarea fiecarui tip de pachet */
		if(message->payload[3] == 'B') {
			break;
		
		} else if(message->payload[3] == 'Z') {
			fclose(fout);
		
		} else if(message->payload[3] == 'F') {
			strcpy(name, message->payload + 4);
			name[message->len - 7] = '\0';
			fout = fopen(name, "w");

		} else if(message->payload[3] == 'D') {
			memcpy(data, message->payload + 4, message->len - 4);
			fwrite(data, sizeof(char), message->len - 7, fout);
		}

		free(message);
	}
	
	free(message);
	return 0;
}

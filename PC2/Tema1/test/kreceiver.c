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
    msg initMessage;
	msg *message;
	msg ack;
	int time = 0;
	FILE* fout;
	char name[101];
	char data[251];

	seq = 0;

    init(HOST, PORT);

/*
	//primesc init pana este corect
	while(1) {
		if(recv_message(&initMessage) < 0) {
			perror("Receive message!");
			return -1;
		}
		//verific init
		crc = crc16_ccitt(initMessage.payload, initMessage.len - 3);
		memcpy(&crc_aux, initMessage.payload + initMessage.len - 3, 2);
	
		if(crc != crc_aux) {
			ack = make_N(seq, 0x0D);
			send_message(&ack);
		} else {
	
			maxl = initMessage.payload[4];
			timeout = initMessage.payload[5];
			mark = initMessage.payload[8];
			ack = make_YY(seq++, 0x0D, initMessage);
			send_message(&ack);
			break;
		}


	}
*/

	//de aici de transmit date importante
	while(1) {
		seq %=64;
		//primesc mesaj cu timeout
		message = receive_message_timeout(5000);
		if(message == NULL) {
			printf("NU AM PRIMIT DATE\n");
			ack = make_N(seq, 0x0D);
			send_message(&ack);
			time++;
			if(time == 3)
				return -1;
			continue;
		} //din cauza asta receiverul sta in viata si nu pica procesul
	time = 0;
		//prima data e recv_message pentru S si dupa doar cu timeout ca sa faca stop
		crc = crc16_ccitt(message->payload, message->len - 3);
		memcpy(&crc_aux, message->payload + message->len - 3, 2);
	
		//verific crc si intor ack sau nak
		if(crc != crc_aux || (message->payload[3] != 'B' && message->payload[3] != 'Z' && message->payload[3] != 'F' && message->payload[3] != 'S' && message->payload[3] != 'D')) {
			ack = make_N(seq, 0x0D);
			printf("CRC GRESIT LA %c -- %d\n", message->payload[3], message->payload[2]);
			send_message(&ack);
			continue;
		}

		ack = make_YY(seq++, 0x0D, *message);
		send_message(&ack);
		printf("AM PRIMIT DATE  --  %c  --  %d\n", message->payload[3], message->payload[2]);

		//in functie de tipul pachetului fac modificari
		if(message->payload[3] == 'B') {
			printf("Am terminat executia!\n");

			ack = make_YY(seq++, 0x0D, *message);
			send_message(&ack);
			free(message);
			break;
		
		} else if(message->payload[3] == 'F') {
			strcpy(name, message->payload + 4);
			name[message->len - 7] = '\0';
			fout = fopen(name, "w");
			free(message);
//			printf("Am deschis %s\n", name);
		
		} else if(message->payload[3] == 'Z') {
			fclose(fout);
			free(message);
//			printf("Am inchis fisierul\n");
		
		} else if(message->payload[3] == 'D') {

			memcpy(data, message->payload + 4, message->len - 4);
			fwrite(data, sizeof(char), message->len - 7, fout);
			free(message);

		} else if(message->payload[3] == 'S') {
			maxl = initMessage.payload[4];
			timeout = initMessage.payload[5];
			mark = initMessage.payload[8];
			free(message);
		}


	}
	
	return 0;
}

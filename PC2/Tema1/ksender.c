#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib.h"
#include "constante.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc, char** argv) {
	char seq = 0;
	int i;
	FILE* fin;
	int len;
	char data[250];

	init(HOST, PORT);

	/* Trimit pachet de tip S */
	if(send_my_message(make_S(seq++)) == -1) {
		perror("Init message sent 3 times!");
		return 0;
	}

	/*
	 * Pentru fiecare fisier trimit F, deschid fisierul
	 * si trimit datele din fisier. La final trimit Z
	*/
	for(i = 1; i < argc; i++) {
		seq %= 64;
		
		if(send_my_message(make_F(seq++, argv[i])) == -1) {
			perror("Header sent 3 times!");
			return 0;
		}

		fin = fopen(argv[i], "r");
		if(fin  == NULL) {
			continue;
		}

		fseek(fin, 0, SEEK_END);
		len = ftell(fin);
		fseek(fin, 0, SEEK_SET);

		while(len > MAXL) {
			seq %= 64;
			fread(data, sizeof(char), MAXL, fin);
			if(send_my_message(make_D(seq++, data, MAXL)) == -1) {
				perror("Data sent 3 times!");
				return 0;
			}

			len -= MAXL;
		}
		
		fread(data, sizeof(char), len, fin);
		if(send_my_message(make_D(seq++, data, len)) == -1) {
			perror("Data sent 3 times!");
			printf("In file %s\n", argv[i]);
			return 0;
		}
		
		if(send_my_message(make_Z(seq++)) == -1) {
			perror("EOF sent 3 times!");
			return 0;
		}

		fclose(fin);
	}

	/* Trimit end of text */
	if(send_my_message(make_B(seq++)) == -1) {
		perror("EOT sent 3 times!");
		return 0;
	}
	
	return 0;
}

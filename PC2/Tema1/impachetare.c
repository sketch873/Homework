#include "lib.h"
#include "constante.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


	/* Face un pachet de tip S adaugand datele
	 * importante in fiecare camp */
msg make_S(char seq) {
	unsigned short us;
	msg t;

	t.payload[0] = SOH;
	t.payload[1] = LEN_S;
	t.payload[2] = seq;
	t.payload[3] = 'S';

	/* Campul data */
	t.payload[4] = MAXL;
	t.payload[5] = TIMEOUT;
	t.payload[6] = NPAD;
	t.payload[7] = PADC;
	t.payload[8] = EOL;
	t.payload[9] = QCTL;
	t.payload[10] = QBIN;
	t.payload[11] = CHKT;
	t.payload[12] = REPT;
	t.payload[13] = CAPA;
	t.payload[14] = R;

	/* Pozitiile 15 si 16 sunt ocupate de checksum */
	us =  crc16_ccitt(t.payload, 15);
	memcpy(t.payload + 15, &us, 2);

	t.payload[17] = MARK;

	t.len = LEN_S + 2;


	return t;
}

	/* Face un pachet de tip F */
msg make_F(char seq, char *name) {
	unsigned int str_len;
	unsigned short us;
	char *final_name;
	msg t;

	final_name = (char *) calloc (LEN_RECV + strlen(name), sizeof(char));
	strcpy(final_name, "recv_");
	strcat(final_name, name);
	str_len = strlen(final_name);

	t.payload[0] = SOH;
	t.payload[1] = str_len + LEN_F;
	t.payload[2] = seq;
	t.payload[3] = 'F';
	t.payload[4] = '\0';

	strcpy(t.payload + 4, final_name);

	us = crc16_ccitt(t.payload, 4 + str_len);
	memcpy(t.payload + 4 + str_len, &us, 2);
	t.payload[6 + str_len] = MARK;

	t.len = str_len + LEN_F + 2;

	return t;
}

	/* Face un pachet de tip D */
msg make_D(char seq, char *data, int len) {
	unsigned short us;
	msg t;

	t.payload[0] = SOH;
	t.payload[1] = len + LEN_D;
	t.payload[2] = seq;
	t.payload[3] = 'D';

	memcpy(t.payload + 4, data, len);

	us = crc16_ccitt(t.payload, 4 + len);
	memcpy(t.payload + 4 + len, &us, 2);

	t.payload[6 + len] = MARK;

	t.len = len + LEN_D + 2;


	return t;
}

	/* Face un pachet de tip Z */
msg make_Z(char seq) {
	unsigned short us;
	msg t;

	t.payload[0] = SOH;
	t.payload[1] = LEN_Z;
	t.payload[2] = seq;
	t.payload[3] = 'Z';

	us = crc16_ccitt(t.payload, 4);
	memcpy(t.payload + 4, &us, 2);

	t.payload[6] = MARK;

	t.len = LEN_Z + 2;

	return t;
}

	/* Face un pachet de tip B */
msg make_B(char seq) {
	unsigned short us;
	msg t;

	t.payload[0] = SOH;
	t.payload[1] = LEN_B;
	t.payload[2] = seq;
	t.payload[3] = 'B';

	us = crc16_ccitt(t.payload, 4);
	memcpy(t.payload + 4, &us, 2);

	t.payload[6] = MARK;

	t.len = LEN_B + 2;
	return t;
}

	/* Face un pachet de tip Y in functie de isS
	 *   - Daca este de tip S intoarce data din el
	 *   - Daca este de oricare alt tip data este vida
	*/
msg make_YY(char seq, char mark, msg isS) {
	unsigned short us;
	msg t;

	t.payload[0] = SOH;
	t.payload[2] = seq;
	t.payload[3] = 'Y';

	if(isS.payload[3] == 'S') {
		t.payload[1] = LEN_Y_TRUE;
		memcpy(t.payload + 4, isS.payload + 4, 11);

		us = crc16_ccitt(t.payload, 15);
		memcpy(t.payload + 15, &us, 2);

		t.payload[17] = MARK;

		t.len = LEN_Y_TRUE + 2;

	} else {
		t.payload[1] = LEN_Y_FALSE;

		us = crc16_ccitt(t.payload, 4);
		memcpy(t.payload + 4, &us, 2);

		t.payload[6] = MARK;

		t.len = LEN_Y_FALSE + 2;
	}

	return t;
}

	/* Face un pachet de tip N */
msg make_N(char seq, char mark) {
	unsigned short us;
	msg t;

	t.payload[0] = SOH;
	t.payload[1] = LEN_N;
	t.payload[2] = seq;
	t.payload[3] = 'N';

	us = crc16_ccitt(t.payload, 4);
	memcpy(t.payload + 4, &us, 2);

	t.payload[6] = MARK;

	t.len = LEN_N + 2;

	return t;
}

	/*
	 * Functie generala care trimite un pachet:
	 * -> Trimite si asteapta raspuns cu timeout
	 * -> La trei timeout-uri consecutive inchide conexiunea
	*/
int send_my_message(msg t) {
	unsigned short crc, crc_aux;
	int time = 0;
	msg *res;

	while(1) {
		send_message(&t);
		res = receive_message_timeout(TIMEOUT * 1000);
		if(res == NULL) {
			time++;
			if(time == 4) {
				return -1;
			}
			continue;
		}

		crc = crc16_ccitt(res->payload, res->len - 3);
		memcpy(&crc_aux, res->payload + res->len - 3, 2);

		if(crc != crc_aux) {
			free(res);
			time = 0;
			continue;
		}
		if(t.payload[2] != res->payload[2]) {
			free(res);
			time = 0;
		}
		if(res->payload[3] == 'Y') {
			return 1;
		} else {
			time = 0;
			continue;
		}
	}
	return -1;
}

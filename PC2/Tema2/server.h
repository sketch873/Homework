
#define BUFLEN 256
#define MAX_CLIENTS 5

/*  Structura de baza a unei entitati  */
typedef struct {
	char nume[13];
	char prenume[13];
	int nr_card;
	int pin;
	char parola_secreta[9];
	double sold;
	int incercari[100];
	int sockfd;
} cont;

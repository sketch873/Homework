#include "info.h"


/*	citire.c	*/
char *fraza_jucata(int nr_argumente, char **argumente, int *nr, int *nr_caractere);

/*	verificare_argument.c	*/
bool ver_argum(int argc, char **argv);
char **impartire_pe_cuvinte(char *v, int *nr);
int **initializare_joaca(char **cuvinte, int nr_cuvinte);

/*	grafica.c	*/
void afisare_meniu(int k);
void afisare_stadiu(propozitie hangman);
void spanzuratoare_joc(int k);
void initializare_culori();
void afisare_setari(int k);
void afisare_culoare(int k);

/*	hangman.c	*/
void functie_free(int nr, int **ascunse, char **cuvinte);
int verificare_litera(unsigned char c, propozitie *hangman,
bool *final_joc, bool *contor_ratare );
char caracter_random(propozitie *hangman);
void adaugare_fisier(propozitie *hangman);
int citire_fisier(propozitie *hangman);
void meniu_setari();
void alege_culoare(int *x, int y, int z);

/*
	La 'Resume Game', inca este fraza completa, elimina prima citire
	a unui caracter de la tastatura.
*/
int verificare_final(propozitie *hangman)
{
	int i, j;
	
	for(i = 0 ; i < hangman->numar_cuvinte; i++)
	{
		for(j = 0; j < strlen(hangman->cuvinte[i]); j++)
		{
			if(hangman->ascunse[i][j] == 0)
				return 1;
		}
	}
	
	return 0;
}

/*
	Partea propiu-zisa a jocului in care jucatorul alege litere
	pentru a fi incercate.

	Daca alegerea se face in 7 secunde atunci se apeleaza verificare_litera
	cu alegerea facuta, altfel se apeleaza cu un caracter aleator care nu
	s-a mai intalnit pana atunci in jocul curent.

	contor_ratare tine retine daca a fost o alegere proasta si acel caracter
	nu s-a mai intalnit

	final_joc tine minte daca sunt afisate toate caracterele
*/
int game(propozitie *hangman)
{
	unsigned char c;
	bool final_joc = FALSE;
	bool contor_ratare;
	int nfds, sel;
	fd_set read_descriptors;
	struct timeval timeout;
	int apasN;
	
	nfds = 1;
	FD_ZERO(&read_descriptors);
	FD_SET(0, &read_descriptors);
	
	timeout.tv_sec = SEC;
	timeout.tv_usec = USEC;

	afisare_stadiu(*hangman);
	spanzuratoare_joc(hangman->vieti);

	while( hangman->vieti != 0 && final_joc == FALSE && verificare_final(hangman) )
	
	{
		sel = select(nfds, &read_descriptors, NULL, NULL, &timeout);

		switch(sel)
		{
			case 1:
				c = getchar();
	
				apasN = verificare_litera(c, hangman, 
				&final_joc, &contor_ratare);

				break;

			case 0:
				c = caracter_random(hangman);

				verificare_litera(c, hangman,
				 &final_joc, &contor_ratare);

				break;
		}

		FD_SET(0, &read_descriptors);
		timeout.tv_sec = SEC;
		timeout.tv_usec = USEC;

	}

	if(c != 'Q')
	{
		c = getchar();
	}

	return apasN;
}

/*
	La selectarea 'New Game' se apeleaza functiile care
	aleg fraza de joc si completeaza structura <propozitie>
*/
propozitie descoperire_fraza(int argc, char **argv)
{
	propozitie aux;
	int cuvinte;
	char *v;
	int i,j,k;
	
	v = fraza_jucata(argc, argv, &aux.numar_fraze_jucate, &aux.numar_total_caractere);

	aux.cuvinte = impartire_pe_cuvinte(v, &cuvinte);
	free(v);

	aux.numar_cuvinte = cuvinte;
	aux.ascunse = initializare_joaca(aux.cuvinte, cuvinte);
	aux.vieti = 6;
	

	aux.caractere_speciale[0] = ',';
	aux.caractere_speciale[1] = ';';
	aux.caractere_speciale[2] = '{';
	aux.caractere_speciale[3] = '}';
	aux.caractere_speciale[4] = '\\';
	aux.caractere_speciale[5] = '\'';

	for(i = 0; i < 42; i++)
	{
		aux.litere[i] = FALSE;
	}
	
	for(i = 0; i < cuvinte; i++)
	{	

		for(j = 0; j < strlen(aux.cuvinte[i]); j++)
		{
			if(aux.ascunse[i][j] == 1 && isalpha(aux.cuvinte[i][j]))
				aux.litere[aux.cuvinte[i][j] - 87] = TRUE;

			else if(aux.ascunse[i][j] == 1 && isdigit(aux.cuvinte[i][j]))
				aux.litere[aux.cuvinte[i][j] - 48] = TRUE;

			for(k = 0; k < 6; k++)
			{
				if(aux.ascunse[i][j] == 1 && aux.cuvinte[i][j] == aux.caractere_speciale[k])
				{
					aux.litere[36 + k] = TRUE;
				}
			}
		}

	}

	return aux;
}
/*
	Meniul principal al jocului.
*/
void meniu_joc(int argc, char **argv)
{
	int contor = 0;
	int pas;
	int apasN = 1;

	afisare_meniu(contor);
	keypad(stdscr, TRUE);	
	propozitie hangman;

	while( TRUE )
	{
		pas = getch();
		switch( tolower(pas) )
		{
			case KEY_UP:	
				if(contor != 0)
					afisare_meniu(--contor);
				else
				{
					contor = 3;
					afisare_meniu(contor);
				}
				break;

			case KEY_DOWN:
				if(contor != 3)
					afisare_meniu(++contor);
				else
				{
					contor = 0;
					afisare_meniu(contor);
				}
				break;

			case 10:
				if(contor == 0)
				{
					while(apasN)
					{
						hangman = descoperire_fraza(argc, argv);
						apasN = game(&hangman);
					}
					
					afisare_meniu(contor);
					adaugare_fisier(&hangman);
					functie_free(hangman.numar_cuvinte, hangman.ascunse, hangman.cuvinte);
					apasN = 1;
				}
				else if(contor == 1 && citire_fisier(&hangman) )
				{
					game(&hangman);
					afisare_meniu(contor);
					adaugare_fisier(&hangman);
					functie_free(hangman.numar_cuvinte, hangman.ascunse, hangman.cuvinte);
				}
				else if(contor == 2)
				{
					meniu_setari();
					initializare_culori();
					afisare_meniu(contor);
				}
				else if(contor == 3)
					return;
		}


	}

}

/*
	MAIN
	La inceput verifica argumentele. Daca totul e ok
	deschide meniul jocului.
*/
int main(int argc, char **argv)
{
	if( argc == 1 )
	{
		fprintf(stderr, "Nu s-au dat argumente de comanda.\n");
		return 1;
	}

	else if( ver_argum(argc, argv) == FALSE )
		return 1;

	initscr();
	cbreak();
	noecho();
	clear();
	curs_set(0);
	start_color();

	initializare_culori();

	meniu_joc(argc, argv);

	endwin();
	return 0;
}
/*
	Elibereaza memoria din <propozitie>
*/
void functie_free(int nr, int **ascunse, char**cuvinte)
{
	int i;
	for(i = 0; i < nr; i++)
	{
		free(cuvinte[i]);
		free(ascunse[i]);
	}
	free(cuvinte);
	free(ascunse);

}

/*
	Daca 'c' este unul din cele 6 caractere speciale, se determina pozitia
	lui in vectorul caractere_speciale (README).

	Daca este un caracter din lista celor posibile de incercat atunci ia
	pe cazuri:
		1.litera
		2.cifra
		3.caracter special
	
	In fiecare caz daca nu s-a mai introdus acel caracter (<propozitie>.litere)
	il cauta si il face vizibil pentru jucator (daca este cazul).
	
	Altfel, verifica doar daca mai sunt caractere ascunse.
*/
int verificare_litera(unsigned char c, propozitie *hangman,
bool *final_joc, bool *contor_ratare )

{
	int i, j;
	int poz_speciala;
	*final_joc = TRUE;
	int apasN = 0;

	if(strchr("\\,;{}'", c) )
	{
		for(i = 0; i < 6; i++)
		{
			if(hangman->caractere_speciale[i] == c)
			{
				poz_speciala = i;
				break;
			}	
		}
	}

	else
	{
		poz_speciala = 10;
	}

	if(c == 'Q')
	{
                return 0;
	}
	
	else if(c == 'N')
	{
		apasN = 1;
	}

	else if( (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || strchr(",;\\'{}", c) )
	{
		*contor_ratare = TRUE;
	
		if(hangman->litere[c-87] == FALSE && isalpha(c) )
		{
			for(i = 0; i < hangman->numar_cuvinte; i++)
			{
				for(j = 0; j < strlen(hangman->cuvinte[i]); j++)
				{
					if(hangman->cuvinte[i][j] == c)
					{
						hangman->ascunse[i][j] = TRUE;
						*contor_ratare = FALSE;
					}
	
					else if(hangman->ascunse[i][j] == 0)
					{
						*final_joc = FALSE;
					}
				}
			}
	
			hangman->litere[c-87] = TRUE;
		}
	
		else if(hangman->litere[c-48] == FALSE && isdigit(c) )
		{
			for(i = 0; i < hangman->numar_cuvinte; i++)
			{
				for(j = 0; j < strlen(hangman->cuvinte[i]); j++)
				{
					if(hangman->cuvinte[i][j] == c)
					{
						hangman->ascunse[i][j] = TRUE;
						*contor_ratare = FALSE;
					}
	
					else if(hangman->ascunse[i][j] == 0)
					{
						*final_joc = FALSE;
					}
				}
	
			}

			hangman->litere[c-48] = TRUE;
		}

		else if(hangman->litere[poz_speciala + 36] == FALSE )
		{
			for(i = 0; i < hangman->numar_cuvinte; i++)
			{
				for(j = 0; j < strlen(hangman->cuvinte[i]); j++)
				{
					if(hangman->cuvinte[i][j] == c)
					{
						hangman->ascunse[i][j] = TRUE;
						*contor_ratare = FALSE;
					}

					else if(hangman->ascunse[i][j] == 0)
					{
						*final_joc = FALSE;
					}
				}
			}
			
			hangman->litere[poz_speciala + 36] = TRUE;
		}

		else
		{
			for(i = 0; i < hangman->numar_cuvinte; i++)
			{
				for(j = 0; j < strlen(hangman->cuvinte[i]); j++)
				{
					if(hangman->ascunse[i][j] == 0)
					{
						*final_joc = FALSE;
					}
				}
			}
	
			*contor_ratare = FALSE;
		}
	
		if(*contor_ratare == TRUE)
		{
			hangman->vieti--;
		}
	
		afisare_stadiu(*hangman);
		spanzuratoare_joc(hangman->vieti);
	}

	else
	{
		*final_joc = FALSE;
	}

	return apasN;

}

/*
	Ia un caracter random pana cand il gaseste pe primul care
	nu a mai fost folosit pana atunci.
	
	In functie de pozitia lui in <propozitie>.litere returneaza
	codul ASCII specific numarului, cifrei sau caracterului special
	(README).
*/
char caracter_random(propozitie *hangman)
{
	int c;
	srand(time(NULL));

	while(TRUE)
	{
		c = rand()%42;
		if(hangman->litere[c] == FALSE)
			{
				break;
			}
	}

	if( c < 10)
	{
		return (c + 48);
	}

	else if(c < 36)
	{
		return (c + 87);
	}
	
	else
	{
		return hangman->caractere_speciale[c - 36];
	}
}

/*
	Salveaza intr-un fisier stadiul curent din joc, separate prin spatiu.
	Ordine de salvare:
		1.numar de cuvinte
		2.cuvintele
		3.matricea booleana pentru afisarea cuvintelor
		4.vectorul de litere
		5.vietile ramase
		6.numarul de fraze disponibile spre a fi jucate
		7.numarul total de caractere al frazei curente
*/
void adaugare_fisier(propozitie *hangman)
{
	FILE *out;
	int i,j;
	out = fopen("savegame", "w");

	fprintf(out, "%d ", hangman->numar_cuvinte);

	for(i = 0; i < hangman->numar_cuvinte; i++)
	{
		fprintf(out, "%s ", hangman->cuvinte[i]);
	}

	for(i = 0; i < hangman->numar_cuvinte; i++)
	{
		for(j = 0; j < strlen(hangman->cuvinte[i]); j++)
		{
			fprintf(out,"%d ", hangman->ascunse[i][j]);
		}
	}
	
	for(i = 0; i < 42; i++)
	{
		fprintf(out, "%d ", hangman->litere[i]);
	}

	fprintf(out, "%d ", hangman->vieti);
	fprintf(out, "%d ", hangman->numar_fraze_jucate);
	fprintf(out, "%d", hangman->numar_total_caractere);

	fclose(out);
}

/*
	Citeste din fisier pentru a se intoarce la ultimul stadiu de joc.
	Ordinea de citire este ca cea din adaugare_fisier.
	Se introduc datele in <propozitie>->caractere_speciale.
*/
int citire_fisier(propozitie *hangman)
{
	FILE *in;
	int i, j;
	char s[2000];
	in = fopen("savegame", "r");

	if( in == NULL )
	{
		return 0;
	}

	else
	{
		fscanf(in, "%d", &hangman->numar_cuvinte);

		hangman->cuvinte = (char**)malloc
		(hangman->numar_cuvinte * sizeof(char*));

		for(i = 0; i < hangman->numar_cuvinte; i++)
		{
			fscanf(in, "%s", s);
			
			hangman->cuvinte[i] = (char*)malloc
			( (1 + strlen(s) ) * sizeof(char));
			
			strcpy(hangman->cuvinte[i], s);
		}

		hangman->ascunse = (int**)malloc
		(hangman->numar_cuvinte * sizeof(int*));

		for(i = 0; i < hangman->numar_cuvinte; i++)
		{
			hangman->ascunse[i] = (int*)malloc
			(strlen(hangman->cuvinte[i]) * sizeof(int));
			
			for(j = 0; j < strlen(hangman->cuvinte[i]); j++)
			{
				fscanf(in, "%d", &hangman->ascunse[i][j]);
			}
		}

		for(i = 0; i < 42; i++)
		{
			fscanf(in, "%d", &hangman->litere[i]);
		}
	
		fscanf(in, "%d", &hangman->vieti);
		fscanf(in, "%d", &hangman->numar_fraze_jucate);
		fscanf(in, "%d", &hangman->numar_total_caractere);

	}

	hangman->caractere_speciale[0] = ',';
	hangman->caractere_speciale[1] = ';';
	hangman->caractere_speciale[2] = '{';
	hangman->caractere_speciale[3] = '}';
	hangman->caractere_speciale[4] = '\\';
	hangman->caractere_speciale[5] = '\'';

	fclose(in);
	return 1;

}

/*
	Daca s-a selectat 'Settings' se deschide meniul de setari.
	Aici se afla scheletul lui iar in 'grafica.c' partea afisata.
	
	NOTA: La apelarea alege_culoare, doar cand primul argument este
	fundalul e important sa se tina cond de text si spanzuratoare. In
	rest se tine cont doar de fundal.
*/
void meniu_setari()
{
	FILE *out, *in;
	int pas;
	int contor = 0;
	int verif_inchidere_while = 1;

	in = fopen("settings", "rb");

	culori set;
	fread(&set, sizeof(culori), 1, in);
	fclose(in);
	
	out = fopen("settings", "wb");

	afisare_setari(contor);

	while( verif_inchidere_while )
	{
		pas = getch();
		
		switch( tolower(pas) )
		{
			case KEY_UP:
					if(contor != 0)
					{
						afisare_setari(--contor);
					}
					
					else
					{
						contor = 3;
						afisare_setari(contor);
					}
					break;
			
			case KEY_DOWN:
					if(contor != 3)
					{
						afisare_setari(++contor);
					}

					else
					{
						contor = 0;
						afisare_setari(contor);
					}
					break;
			
			case 10:
					if(contor == 0)
					{
						alege_culoare(&set.fundal,
						 set.scris, set.spanzuratoare);
						
						afisare_setari(contor);
					}
					
					else if(contor == 1)
					{
						alege_culoare(&set.scris,
						 set.fundal, 10);
						
						afisare_setari(contor);
					}
					
					else if(contor == 2)
					{
						alege_culoare(&set.spanzuratoare,
						 set.fundal, 10);
						
						afisare_setari(contor);
					}
					
					else if(contor == 3)
					{
						fwrite(&set, sizeof(culori), 1, out);
						verif_inchidere_while = 0;
					}			
		}
	}

	fclose(out);
}

/*
	Dupa ce s-a selectat unde se face modificarea se alege
	si noua culoare.
	Meniul afisat se afla in 'grafica.c'
*/
void alege_culoare(int *x, int y, int z)
{
	int pas;
	int contor=0;

	afisare_culoare(contor);

	while( TRUE )
	{
		pas = getch();
		switch( tolower(pas) )
		{
			case KEY_UP:
					if(contor != 0)
					{
						afisare_culoare(--contor);
					}
					
					else
					{
						contor = 8;
						afisare_culoare(contor);
					}
					break;
			
			case KEY_DOWN:
					if(contor != 8)
					{
						afisare_culoare(++contor);
					}

					else
					{
						contor = 0;
						afisare_culoare(contor);
					}
					break;
			
			case 10:
					if(contor == 8)
					{
						return;
					}

					else if(contor != y && contor != z)
					{
						*x = contor;
						return;
					}
					return;
		}
	}

}

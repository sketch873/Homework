#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
char nume[21];
int timp;
int prioritate;
int scadereP;
int adunareP;
} proces;

typedef struct nod{
proces valori;
struct nod *urm;
} Nod;

/*
 	Realizeaza un nod nou pentru coada.
*/
Nod *initializareNod(proces asta)
{
	Nod *x = (Nod*)malloc(sizeof(Nod));
	
	x->valori = asta;
	x->urm = NULL;

	return x;
}

/*
	Elimina primul element al unei cozi.
*/
Nod *eliminareInceput(Nod *coada)
{
	Nod *x = coada;
	
	coada = coada->urm;
	free(x);
	
	return coada;
}

/*
	Elibereaza memoria din toata coada.
*/
Nod *eliminareCoada(Nod *coada)
{
	Nod *x;
	while(coada)
	{
		x = coada;
		coada = coada->urm;
		free(x);
	}
	return coada;
}

/*
	Face un proces nou din sirul citit pentru functia add.
*/
proces parsare(char *val)
{
	proces nou;

	strcpy(nou.nume, strtok(val, " "));
	nou.timp = atoi(strtok(NULL, " "));
	nou.prioritate = atoi(strtok(NULL, " "));

	return nou;

}

/*
	Functia add pentru comanda 'a'.
	Intoarce coada cu noul proces adaugat.
*/
Nod *add(Nod *coada, char *val)
{
	Nod *parcurs;
	proces nou = parsare(val);
	
	nou.scadereP = 0;
	nou.adunareP = 0;
	
	if(coada == NULL)
		coada = initializareNod(nou);
	else
	{
		parcurs = coada;
		while(parcurs->urm)
			parcurs = parcurs->urm;
		parcurs->urm = initializareNod(nou); 
	}
	
	return coada;
}

/*
	Functia show pentru comanda 's'.
*/
void show(proces curent, FILE *out)
{
	if(curent.timp == 0)
		fprintf(out,"\n");
	else
		fprintf(out,"%s %d\n", curent.nume, curent.timp);
}

/*
	Functia tick pentru comanda 't'.
*/
proces tick(proces curent, Nod **coada, int *timpScurs)
{
	if(curent.timp > 0)
	{
		curent.timp--;
		(*timpScurs)++;
	}
	if(curent.timp == 0 && *coada != NULL)
	{
		curent = (*coada)->valori;
		*coada = eliminareInceput(*coada);
		*timpScurs = 0;
	}

	return curent;
}
/*
	Functia wait pentru comanda 'w'.
*/
Nod *wait(Nod *waiting, proces *curent)
{
	if(waiting == NULL)
		waiting = initializareNod(*curent);
	else
	{
		Nod *parcurs = waiting;
		while(parcurs->urm)
			parcurs = parcurs->urm;
		parcurs->urm = initializareNod(*curent);
	}
	curent->timp = 0;
	return waiting;
}

/*
	Functia event pentru comanda 'e'.
*/
Nod *event(Nod *coada, Nod **waiting, char *nume)
{
	int aux = 0;
	Nod *parcurs;
	Nod *deSters;
	Nod *w = *waiting;
	
	if(coada == NULL)
		aux = 1;
	else
	{
		parcurs = coada;
		while(parcurs->urm)
			parcurs = parcurs->urm;
	}

	if(strcmp(w->valori.nume, nume) == 0 && aux == 1)
	{
		coada = initializareNod(w->valori);
		*waiting = eliminareInceput(*waiting);
		return coada;
	}
	else if(strcmp(w->valori.nume, nume) == 0 && aux == 0)
	{
		parcurs->urm = initializareNod(w->valori);
		*waiting = eliminareInceput(*waiting);
		return coada;
	}
		
	while(w->urm)
	{
		if(strcmp(w->urm->valori.nume, nume) == 0)
		{
			if(aux == 1)
			{
				coada = initializareNod(w->urm->valori);
				deSters = w->urm;
				w->urm = w->urm->urm;
				free(deSters);
				return coada;
			}
			else if(aux == 0)
			{
				parcurs->urm = initializareNod(w->urm->valori);
				deSters = w->urm;
				w->urm = w->urm->urm;
				free(deSters);
				return coada;
			}
		}
		
		w = w->urm;
	}
	return coada; 
}

/*
	Functia multiple add pentru comanda 'ma'
*/
Nod *mAdd(Nod *coada, char *val)
{
	proces nou;
	Nod *parcurs;
	int i = 0;
	
	nou.scadereP = 0;
	nou.adunareP = 0;
	
	strcpy(nou.nume, strtok(val, " ") );
	nou.timp = atoi(strtok(NULL, " ") );
	nou.prioritate = atoi(strtok(NULL, " ") );

	if(coada == NULL)
	{
		coada = initializareNod(nou);
		parcurs = coada;
	}
	else
	{
		parcurs = coada;
		while(parcurs->urm)
			parcurs = parcurs->urm;
		parcurs->urm = initializareNod(nou);
		parcurs = parcurs->urm;
	}
	
	while( (val = strtok(NULL, " ") ) != NULL)
	{
		if(i%3 == 0)
			strcpy(nou.nume, val);
		else if(i%3 == 1)
			nou.timp = atoi(val);
		else if(i%3 == 2)
		{
			nou.prioritate = atoi(val);
			parcurs->urm = initializareNod(nou);
			parcurs = parcurs->urm;
		}
		i++;
	}
	return coada;
	
}

/*
	Functia de sortare dupa timpul de finalizare al unei cozi (SJF).
*/
Nod *sortareTimp(Nod *coada)
{
	Nod *cap = coada;
	Nod *parcurs;
	proces aux;
	
	if(coada == NULL || coada->urm == NULL)
		return coada;
	else
	{
		while(cap->urm)
		{
			parcurs = cap->urm;
			while(parcurs)
			{
				if(parcurs->valori.timp < cap->valori.timp)
				{
					aux = parcurs->valori;
					parcurs->valori = cap->valori;
					cap->valori = aux;
				}
				parcurs = parcurs->urm;
			}
			cap = cap->urm;
		}
	}
	return coada;
}

/*
	Functia de sortare dupa prioritate al unei cozi (PP si BONUS).
*/
Nod *sortarePrioritate(Nod *coada, int niveluri)
{
	Nod *cap = NULL;
	Nod *pas;
	Nod *ptCoada;
	Nod *nou;
	int aux = 0;

	if(coada == NULL)
		return coada;

	while(niveluri)
	{
		ptCoada = coada;
		while(ptCoada)
		{
			if(ptCoada->valori.prioritate == niveluri && aux == 0)
			{
				cap = (Nod*)malloc(sizeof(Nod));
				cap->valori = ptCoada->valori;
				cap->urm = NULL;
				aux = 1;
				pas = cap;
			}
			else if(ptCoada->valori.prioritate == niveluri && aux == 1)
			{
				nou = (Nod*)malloc(sizeof(Nod));
				nou->valori = ptCoada->valori;
				nou->urm = NULL;
				pas->urm = nou;
				pas = pas->urm;
			}
			ptCoada = ptCoada->urm;
		}
		niveluri--;
	}
	coada = eliminareCoada(coada);
	return cap;
}

/*
	Schimbarea procesului curent (adaugat la sfarsitul cozii)
cu un nou proces(de la inceputul cozii).
	Pentru RR unde se adauga in READY procesele.
*/ 
Nod *schimbare(Nod *coada, proces *curent)
{
	
	if(coada == NULL)
		return coada;

	Nod *parcurs = coada;
	
	while(parcurs->urm)
		parcurs = parcurs->urm;
	
	parcurs->urm = initializareNod(*curent);
	*curent = coada->valori;
	coada = eliminareInceput(coada);

	return coada;		
}

/*
	Adaugarea unui proces in coada READY.
	Pentru PP si BONUS.
	Este necesara o sortare dupa prioritate pentru
un nou proces in starea RUNNING.
*/
Nod *adaugareCoada(Nod *coada, proces curent)
{
	Nod *parcurs = coada; 
	
	if(coada == NULL)
		return initializareNod(curent);
	while(parcurs->urm)
		parcurs = parcurs->urm;

	parcurs->urm = initializareNod(curent);

	return coada;
}

/*
	Adaugarea unui nou proces in starea RUNNING din READY
in urma sortarii (pentru PP si BONUS).
*/
proces adaugareCurent(Nod **coada)
{
	proces nou = (*coada)->valori;
	*coada = eliminareInceput(*coada);
	return nou;
}

/*
	Cautarea primului proces cu cea mai mare prioritate.
	Pentru PP si BONUS.
*/
proces cautare(Nod **coada, int niveluri, proces actual)
{
	proces curent;
	Nod *parcurs;

	curent.timp = 0;
	if((*coada) == NULL)
		return actual;
	while(niveluri)
	{
		parcurs = *coada;
		if( (*coada)->valori.prioritate == niveluri && actual.prioritate != niveluri)
		{
			curent = (*coada)->valori;
			(*coada) = eliminareInceput(*coada);
			return curent;
		}
		else if(actual.prioritate == niveluri)
			return actual;
		while(parcurs->urm)
		{
			if(parcurs->urm->valori.prioritate == niveluri && actual.prioritate != niveluri)
			{
				curent = parcurs->urm->valori;
				parcurs->urm = parcurs->urm->urm;
				return curent;
			}
			parcurs = parcurs->urm;
		}
		niveluri--;
	}
	
	return actual;
}
/*
	TASK I
*/
void FCFS(FILE *in, FILE *out)
{
	Nod *coada=NULL;
	Nod *waiting=NULL;
	proces curent;
	char citit[100];
	int gunoi = 0;
	
	curent.timp = 0;
	
	while(fgets(citit, 100, in) )
	{
		if(citit[0] == 'a')
		{
			coada = add(coada, citit+2);
			curent = tick(curent, &coada, &gunoi);
		}
		else if(citit[0] == 's')
			show(curent, out);
		else if(citit[0] == 't')
		{
			curent = tick(curent, &coada, &gunoi);
		}
		else if(citit[0] == 'w')
		{
			waiting = wait(waiting, &curent);
			curent = tick(curent, &coada, &gunoi);
		}
		else if(citit[0] == 'e')
		{
			citit[strlen(citit)-1] = '\0';
			coada = event(coada, &waiting, citit + 2);
			curent = tick(curent, &coada, &gunoi);
		}
		else if(citit[0] == 'm' && citit[1] == 'a')
		{
			coada = mAdd(coada, citit+3);
			curent = tick(curent, &coada, &gunoi);
		}
	}
	coada = eliminareCoada(coada);
	waiting = eliminareCoada(waiting);
}

/*
	TASK II
*/
void SJF(FILE *in, FILE *out)
{
	Nod *coada = NULL;
	Nod *waiting = NULL;
	proces curent;
	char citit[100];
	int gunoi = 0;

	curent.timp = 0;

	while(fgets(citit, 100, in) )
	{
		if(citit[0] == 'a')
		{
			coada = add(coada, citit+2);
			coada = sortareTimp(coada);
			curent = tick(curent, &coada, &gunoi);
		}
		else if(citit[0] == 's')
			show(curent, out);
		else if(citit[0] == 't')
		{
			coada = sortareTimp(coada);
			curent = tick(curent, &coada, &gunoi);
		}
		else if(citit[0] == 'w')
		{
			waiting = wait(waiting, &curent);
			coada = sortareTimp(coada);
			curent = tick(curent, &coada, &gunoi);
		}
		else if(citit[0] == 'e')
		{
			citit[strlen(citit)-1] = '\0';
			coada = event(coada, &waiting, citit+2);
			coada = sortareTimp(coada);
			curent = tick(curent, &coada, &gunoi);
		}
		else if(citit[0] == 'm' && citit[1] == 'a')
		{
			coada = mAdd(coada, citit+3);
			coada = sortareTimp(coada);
			curent = tick(curent, &coada, &gunoi);
		}
	}
	coada = eliminareCoada(coada);
	waiting = eliminareCoada(waiting);
	

}

/*
	TASK III
*/
void RR(FILE *in, FILE *out, int timpTotal)
{
	Nod *coada = NULL;
	Nod *waiting = NULL;
	proces curent;
	char citit[100];
	int timpScurs = 0;

	curent.timp = 0;

	while(fgets(citit, 100, in) )
	{
		if(citit[0] == 'a')
		{
			coada = add(coada, citit+2);
			curent = tick(curent, &coada, &timpScurs);
		}
		else if(citit[0] == 's')
			show(curent, out);
		else if(citit[0] == 't')
			curent = tick(curent, &coada, &timpScurs);
		else if (citit[0] == 'w')
		{
			waiting = wait(waiting, &curent);
			curent = tick(curent, &coada, &timpScurs);
		}
		else if(citit[0] == 'e')
		{
			citit[strlen(citit) - 1] = '\0';
			coada = event(coada, &waiting, citit+2);
			curent = tick(curent, &coada, &timpScurs);
		}
		else if(citit[0] == 'm' && citit[1] == 'a')
		{
			coada = mAdd(coada, citit+3);
			curent = tick(curent, &coada, &timpScurs);
		}

		if(timpScurs == timpTotal)
		{
			coada = schimbare(coada, &curent);
			timpScurs = 0;
		}

	}
	coada = eliminareCoada(coada);
	waiting = eliminareCoada(waiting);
}

/*
	TASK IV
*/
void PP(FILE *in, FILE *out, int timpTotal, int niveluri)
{
	Nod *coada = NULL;
	Nod *waiting = NULL;
	proces curent;
	char citit[100];
	int timpScurs = 0;

	curent.timp = 0;

	while(fgets(citit, 100, in) )
	{
		if(citit[0] == 'a')
		{
			coada = add(coada, citit+2);
			coada = sortarePrioritate(coada, niveluri);
			curent = tick(curent, &coada, &timpScurs);
		}
		else if(citit[0] == 's')
			show(curent, out);
		else if(citit[0] == 't')
		{
			coada = sortarePrioritate(coada, niveluri);
			curent = tick(curent, &coada, &timpScurs);
		}
		else if (citit[0] == 'w')
		{
			waiting = wait(waiting, &curent);
			coada = sortarePrioritate(coada, niveluri);
			curent = tick(curent, &coada, &timpScurs);
		}
		else if(citit[0] == 'e')
		{
			citit[strlen(citit) - 1] = '\0';
			coada = event(coada, &waiting, citit+2);
			coada = sortarePrioritate(coada, niveluri);
			curent = tick(curent, &coada, &timpScurs);
			coada = adaugareCoada(coada, curent);
			curent = cautare(&coada, niveluri, curent);
		}
		else if(citit[0] == 'm' && citit[1] == 'a')
		{
			coada = mAdd(coada, citit+3);
			coada = sortarePrioritate(coada, niveluri);
			curent = tick(curent, &coada, &timpScurs);
		}
		if(timpScurs == timpTotal)
		{
			coada = adaugareCoada(coada, curent);
			timpScurs = 0;
			coada = sortarePrioritate(coada, niveluri);
			curent = adaugareCurent(&coada);

		}


	}
	coada = eliminareCoada(coada);
	waiting = eliminareCoada(waiting);
}

/*
	TASK BONUS
*/
void BONUS(FILE *in, FILE *out, int timpTotal, int niveluri)
{
	Nod *coada = NULL;
	Nod *waiting = NULL;
	proces curent;
	char citit[100];
	int timpScurs=0;

	curent.timp = 0;	//gol

	while(fgets(citit, 100, in) )
	{
		if(citit[0] == 'a')
		{
			coada = add(coada, citit+2);
			coada = sortarePrioritate(coada, niveluri);
			curent = tick(curent, &coada, &timpScurs);
		}
		else if(citit[0] == 's')
			show(curent, out);
		else if(citit[0] == 't')
		{
			coada = sortarePrioritate(coada, niveluri);
			curent = tick(curent, &coada, &timpScurs);
		}
		else if (citit[0] == 'w')
		{
			curent.adunareP++;
			if(curent.adunareP == 2)
			{
				curent.adunareP = 0;
				curent.prioritate++;
			}
			waiting = wait(waiting, &curent);
			coada = sortarePrioritate(coada, niveluri);
			curent = tick(curent, &coada, &timpScurs);
		}
		else if(citit[0] == 'e')
		{
			citit[strlen(citit) - 1] = '\0';
			coada = event(coada, &waiting, citit+2);
			coada = sortarePrioritate(coada, niveluri);
			curent = tick(curent, &coada, &timpScurs);
			curent = cautare(&coada, niveluri, curent);
		}
		else if(citit[0] == 'm' && citit[1] == 'a')
		{
			coada = mAdd(coada, citit+3);
			coada = sortarePrioritate(coada, niveluri);
			curent = tick(curent, &coada, &timpScurs);
		}
		if(timpScurs == timpTotal)
		{
			curent.scadereP++;
			if(curent.scadereP == 2)
			{

				curent.scadereP = 0;
				curent.prioritate--;
			}
			coada = adaugareCoada(coada, curent);
			timpScurs = 0;
			coada = sortarePrioritate(coada, niveluri);
			curent = adaugareCurent(&coada);

		}
	}
	coada = eliminareCoada(coada);
	waiting = eliminareCoada(waiting);

}

/*
	Functia de citire a primei linii din fisier si decidere
	a functiei de prelucrare apelate.
*/
void citire(char *input, char *output)
{
	FILE *in, *out;
	int numar;
	int timpTotal;
	int niveluri;
	in = fopen(input, "r");
	out = fopen(output, "w");

	if(in == NULL)
	{
		fclose(out);
		return;
	}
	else
	{
		fscanf(in, "%d", &numar);
		if(numar == 3)
			fscanf(in, "%d", &timpTotal);
		else if(numar == 4 || numar == 5)
			fscanf(in, "%d%d", &timpTotal, &niveluri);
			
		switch(numar){
			case 1: FCFS(in, out); break;
			case 2: SJF(in, out); break;
			case 3: RR(in, out, timpTotal); break;
			case 4: PP(in, out, timpTotal, niveluri); break;
			case 5: BONUS(in, out, timpTotal, niveluri); break;

		}
	}
	fclose(in);
	fclose(out);
}

int main(int argc, char **argv)
{
	citire(argv[1], argv[2]);
	return 0;

}

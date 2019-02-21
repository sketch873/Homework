#include "arbore.h"

/*
	Daca se citeste numarul intre paranteze patrate e frunza
	(se initializeaza valoarea si are 0 succesori).
*/

mMax *initVal(char *v)
{
	mMax *x = (mMax*)malloc(sizeof(mMax));
	
	x->val = atoi(v);
	x->fii = 0;
	x->copil = NULL;
	
	return x;	
}

/*
	Daca se citeste numarul intre paranteze rotunde atunci
	nodul are succesori iar valoarea se afla in urma algoritmului.
*/

mMax *initChi(char *v)
{
	int i;
	mMax *x = (mMax*)malloc(sizeof(mMax));
	
	x->val = 0;
	x->fii = atoi(v);
	x->copil = (struct mmax**)malloc(x->fii * sizeof(struct mmax*));

	for(i = 0; i < x->fii; i++)
		x->copil[i] = NULL;
	
	return x;
}

/*
		Se citesc toate valorile din fisierul de input si se realizeaza arborele.
	Nodurile frunza primesc valoarea iar celelalte noduri se initializeaza cu
	valoarea 0 si numarul specificat de succesori.
		Parcurgerea este de tip BFS (se parcurge recursiv fiecare nod copil pana cand
	se ajunge la nodul in care trebuie facuta initializarea.
*/

mMax *citireFacere(FILE *in, mMax *arbore, int niveluri)
{
	char v[20];
	int i;
	
	if(niveluri == 0)
	{
		fscanf(in, "%s", v);
		if(v[0] == '[')
			arbore = initVal(v+1);
		else
			arbore = initChi(v+1);
	}
	else
		for(i = 0; i < arbore->fii; i++)
			arbore->copil[i] = citireFacere(in, arbore->copil[i], niveluri - 1);
	
	return arbore;
}

/*
	Determinarea valorii maxime dintre toate valorile succesorilor directi.
*/

int detMaxim(mMax *arbore)
{
	int i;
	int aux = arbore->copil[0]->val;

	for(i = 0; i < arbore->fii; i++)
		if(aux < arbore->copil[i]->val)
			aux = arbore->copil[i]->val;

	return aux;
}

/*
	Determinarea valorii maxime dintre toate valorile succesorilor directi.
*/

int detMinim(mMax *arbore)
{
	int i;
	int aux = arbore->copil[0]->val;

	for(i = 0; i < arbore->fii; i++)
		if(aux > arbore->copil[i]->val)
			aux = arbore->copil[i]->val;

	return aux;
}

/*
		Parcurgere recusiva pana la frunze.
		Nodurile arborelui, in functie de cine e la rand(min sau max),
	primesc valoare determinata de minimul sau maximul valorilor nodurilor
	succesoare.
*/

mMax *minimax(mMax *arbore, int mutare) // 0-min 1-max
{
	int i;

	if(arbore == NULL || arbore->fii == 0)
		return arbore;
	
	for(i = 0; i < arbore->fii; i++)
		arbore->copil[i] = minimax(arbore->copil[i], 1-mutare);

	if(mutare == 1)
		arbore->val = detMaxim(arbore);
	else
		arbore->val = detMinim(arbore);
		
	return arbore;
}

/*
	Afisarea arborelui pentru taskul III.
*/

void afisareMinimax(FILE *out, mMax *arbore, int taburi)
{
	int i;

	for(i = 0; i < taburi; i++)	
		fprintf(out, "\t");

	fprintf(out, "%d\n", arbore->val);

	for(i = 0; i < arbore->fii; i++)
			afisareMinimax(out, arbore->copil[i], taburi + 1);
}

/*
	Eliberarea memoriei pentru task III si bonus.
*/

mMax *eliberareMMAX(mMax *arbore)
{
	int i;

	if(arbore == NULL)
		return arbore;
	
	for(i = 0; i < arbore->fii; i++)
		if(arbore->copil[i])
				arbore->copil[i] = eliberareMMAX(arbore->copil[i]);

	free(arbore->copil);
	free(arbore);

	return arbore;
}

/*
		Algoritmul Alpha-Beta prorpiu-zis.
		Valorile alfa si beta sunt extremele, INT_MIN si INT_MAX.
		In functie de mutare, valoare initiala este cea minima sau cea
	maxima urmand parcurgea recursiva si schimbarea valorilor pentru alfa
	si beta.
*/
	
int alfaBeta(mMax *arbore, int mutare, int alfa, int beta)
{
	int i, j;
	int aux;
	
	if(arbore->fii == 0)
		return arbore->val;


	if(mutare == 1)
	{
		arbore->val = INT_MIN;
		for(i = 0; i < arbore->fii; i++)
		{
			aux = alfaBeta(arbore->copil[i], 1-mutare, alfa, beta);

			if(aux > arbore->val)
				arbore->val = aux;
			if(arbore->val > alfa)
				alfa = arbore->val;
			if(alfa >= beta)
			{
				for(j = i+1; j < arbore->fii; j++)
					arbore->copil[j] = eliberareMMAX(arbore->copil[j]);
				arbore->fii = i + 1;
			}

		}		
	}
	else
	{
		arbore->val = INT_MAX;
		for(i = 0; i < arbore->fii; i++)
		{
			aux = alfaBeta(arbore->copil[i], 1-mutare, alfa, beta);
			if(aux < arbore->val)
				arbore->val = aux;
			if(arbore->val < beta)
				beta = arbore->val;
			if(alfa >= beta)
			{
				for(j = i+1; j < arbore->fii; j++)
					arbore->copil[j] = eliberareMMAX(arbore->copil[j]);
				arbore->fii = i+1;
			}
		}		
	}

	return arbore->val;
}

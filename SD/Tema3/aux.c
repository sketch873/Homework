#include "graf.h"

/*
 * Initializarea grafului.
*/
Graf initGraf(int varfuri)
{
	int i;
	Graf x;

	x.noduri = varfuri;

	x.matCost = (int**)calloc(varfuri, sizeof(int*));
	x.matDrum = (int**)calloc(varfuri, sizeof(int*));
	x.tipuri = (int*)malloc(varfuri * sizeof(int));

	for(i = 0; i < varfuri; i++)
	{
		x.matCost[i] = (int*)calloc(varfuri, sizeof(int));
		x.matDrum[i] = (int*)calloc(varfuri, sizeof(int));
	}
		
	return x;
}

/*
 * Adauga o latura noua in graf.
*/
Graf adaugareLatura(Graf x, int a, int b, int cost)
{
	x.matCost[a][b] = cost;
	return x;
}

/*
 * Citeste din fisier toate tipurile nodurilor, legaturile si
 * lista cu toate traseele pe care drona trebuie sa le faca.
*/
Graf citire(FILE *in, int task)
{
	int noduri, legaturi;
	int x, y, cost;
	int i;
	int tip;
	Graf final;

	fscanf(in, "%d", &noduri);
	final = initGraf(noduri);

	for(i = 0; i < noduri; i++)
	{
		fscanf(in, "%d", &tip);
		final.tipuri[i] = tip;

		if(task == 1 && tip == 1)
			final.start = i;
	}

	fscanf(in, "%d", &legaturi);
	for(i = 0; i < legaturi; i++)
	{
		fscanf(in, "%d%d%d", &x, &y, &cost);
		final = adaugareLatura(final, x, y, cost);
	}

	fscanf(in, "%d", &legaturi);
	final.transporturi = legaturi;
	final.traseu[0] = (int*)malloc(legaturi * sizeof(int));
	final.traseu[1] = (int*)malloc(legaturi * sizeof(int));

	for(i = 0; i < legaturi; i++)
	{
		fscanf(in, "%d%d", &x, &y);
		final.traseu[0][i] = x;
		final.traseu[1][i] = y;
	}

	if(task == 2 || task == 3)
	{
		fscanf(in, "%d", &tip);
		final.start = tip;
	}

	return final;

}

/*
 * Algoritmul FLoydWarshall care realizeaza
 * si matricea drumurilor minime.
*/
Graf FloydWarshall(Graf g)
{
	int d[g.noduri][g.noduri];
	int i, j, k;

	for(i = 0; i < g.noduri; i++)
		for(j = 0; j < g.noduri; j++)
		{
			if(g.matCost[i][j])
				d[i][j] = g.matCost[i][j];
			else
				d[i][j] = MAX;
			g.matDrum[i][j] = j;
		}

	for(k = 0; k < g.noduri; k++)
		for(i = 0; i < g.noduri; i++)
			for(j = 0; j < g.noduri; j++)
				if(d[i][j] > d[i][k] + d[k][j])
				{
					d[i][j] = d[i][k] + d[k][j];
					g.matDrum[i][j] = g.matDrum[i][k];
				}
	for(i = 0; i < g.noduri; i++)
		for(j = 0; j < g.noduri; j++)
			g.matCost[i][j] = d[i][j];

return g;
}

/*
 * Folosind matricea drumurilor minime afiseaza
 * drumul dintre doua noduri.
*/
void afisareDrum(Graf g, int x, int y, FILE *out)
{
	while(x != y)
	{
		x = g.matDrum[x][y];
		fprintf(out, "%d ", x);
	}
}

/*
 * Elibereaza memoria alocata grafului.
*/
Graf eliberare(Graf final)
{
	int i;
	for(i = 0; i < final.noduri; i++)
	{
		free(final.matCost[i]);
		free(final.matDrum[i]);
	}

	free(final.matCost);
	free(final.matDrum);
	free(final.tipuri);
	free(final.traseu[0]);
	free(final.traseu[1]);

	return final;
}

/*
 * Verificare pentru backtracking daca
 * este ales un traseu de mai multe ori.
*/
int verificare(int *v, int k)
{
	int i, j;
	for(i = 0; i <= k; i++)
		for(j = 0; j < i; j++)
			if(v[i] == v[j])
				return 0;
	return 1;
}

/*
 * Calculeaza lungimea unui drum complet in functie de vectorul
 * care dicteaza ordinea selectarii comenzilor.
*/
int calLungime(Graf final, int *v)
{
	int lungime = 0;
	int i, j;
	int auxTraseu;
	int verificareTraseu;
	int start = final.start;

	if(final.tipuri[start] != final.traseu[0][v[0]])
	{
		verificareTraseu = 0;
		for(j = 0; j < final.noduri; j++)
		{
			if(final.tipuri[j] == final.traseu[0][v[0]] && verificareTraseu == 0)
			{
				auxTraseu = j;
				verificareTraseu = 1;
			}
			else if(final.tipuri[j] == final.traseu[0][v[0]] &&
			final.matCost[start][j] + final.matCost[j][final.traseu[1][v[0]]]
			< final.matCost[start][auxTraseu] + final.matCost[auxTraseu][final.traseu[1][v[0]]] )
				auxTraseu = j;
		}
	
		lungime += final.matCost[start][auxTraseu];
		start = auxTraseu;
	}

	for( i = 0; i < final.transporturi; i++)
	{
		if(final.tipuri[start] == final.traseu[0][v[i]])
		{
			lungime += final.matCost[start][final.traseu[1][v[i]]];
			start = final.traseu[1][v[i]];
		}
		else
		{
			verificareTraseu = 0;
			for(j = 0; j < final.noduri; j++)
			{
				if(final.tipuri[j] == final.traseu[0][v[i]] && verificareTraseu == 0)
				{
					auxTraseu = j;
					verificareTraseu = 1;
				}
				else if(final.tipuri[j] == final.traseu[0][v[i]] &&
				final.matCost[final.traseu[1][v[i-1]]][j] + final.matCost[j][final.traseu[1][v[i]]]
				< final.matCost[final.traseu[1][v[i-1]]][auxTraseu] + final.matCost[auxTraseu][final.traseu[1][v[i]]]
				)
					auxTraseu = j;
			}

			lungime += final.matCost[final.traseu[1][v[i-1]]][auxTraseu];
			lungime += final.matCost[auxTraseu][final.traseu[1][v[i]]];
			start = final.traseu[1][v[i]];
		}
	}

	verificareTraseu = 0;
	for(j = 0; j < final.noduri; j++)
		if(final.tipuri[j] != 0 && verificareTraseu == 0)
		{
			auxTraseu = j;
			verificareTraseu = 1;
		}
		else if(final.tipuri[j] != 0 && final.matCost[final.traseu[1][v[final.transporturi-1]]][j]
		< final.matCost[final.traseu[1][v[final.transporturi-1]]][auxTraseu] )
			auxTraseu = j;
				
		lungime += final.matCost[final.traseu[1][v[final.transporturi-1]]][auxTraseu];
		return lungime;
}

/*
 * Verifica daca nou vector realizat prin backtracking are
 * un drumul mai scurt. Daca da, acel traseu este mai bun.
*/
void modificare(Graf final, int *v, int *ordine)
{
	int i;
	if(calLungime(final, v) < calLungime(final, ordine) )
	{
		for(i = 0 ; i < final.transporturi; i++)
			ordine[i] = v[i];
	}
}

/*
 * Backtracking care genereaza toate modurile de a realiza comenzile.
*/
void bk(int k, Graf final, int *v, int *ordine)
{
	int i;
	for(i = 0; i < final.transporturi; i++)
	{
		v[k] = i;
		if(k == final.transporturi-1)
		{
			if(verificare(v, k))
				modificare(final, v, ordine);
		}
		else
			bk(k+1, final, v, ordine);
		}
}

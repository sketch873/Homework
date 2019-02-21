#include "graf.h"

/*
 * Functii generale.
*/
Graf initGraf(int varfuri);
Graf insertEdge(Graf x, int a, int b, int cost);
Graf citire(FILE *in, int task);
Graf FloydWarshall(Graf g);
void afisareDrum(Graf g, int x, int y, FILE *out);
Graf eliberare(Graf final);

/*
 * Functii specifice taskului 3.
*/
void bk(int k, Graf final, int *v, int *ordine);
int verificare(int *v, int k);
void modificare(Graf final, int *v, int *ordine);
int calLungime(Graf final, int *v);

/*
 * Taskurile.
 * Explicarea fiecarui task este in Readme.
*/
void task1(FILE *out, Graf final);
void task2(FILE *out, Graf final);
void task3(FILE *out, Graf final);

int main(int argc, char **argv)
{
		FILE *in, *out;
		int task;
		Graf final;

		in = fopen(argv[1], "r");
		out = fopen(argv[2], "w");

		fscanf(in, "%d", &task);
		final = citire(in, task);
		final = FloydWarshall(final);
		
		if(task == 1)
			task1(out, final);
		else if(task == 2)
			task2(out, final);
		else if(task == 3)
			task3(out, final);

		fclose(in);
		fclose(out);

		return 0;
}

/*
 * Taskul 1.
*/
void task1(FILE *out, Graf final)
{
		
	int lungime = 0;
	int i, j;

	fprintf(out, "%d ", final.start);
	
	for(i = 0; i < final.transporturi; i++)
	{
		if(final.tipuri[final.start] == 1)
		{
			lungime += final.matCost[final.start][final.traseu[1][i]];
			afisareDrum(final, final.start, final.traseu[1][i], out);
			final.start = final.traseu[1][i];
		}
		else
		{
			for(j = 0; j < final.noduri; j++)
				if(final.tipuri[j] == 1)
					break;

			afisareDrum(final, final.traseu[1][i-1], j, out);
			afisareDrum(final, j, final.traseu[1][i], out);
			lungime += final.matCost[final.traseu[1][i-1]][j];
			lungime += final.matCost[j][final.traseu[1][i]];
			final.start = final.traseu[1][i];
		}
	}

	for(j = 0; j < final.noduri; j++)
		if(final.tipuri[j] == 1)
			break;
	
	afisareDrum(final, final.traseu[1][i-1], j, out);
	lungime += final.matCost[final.traseu[1][i-1]][j];
	
	fseek(out, -sizeof(char), SEEK_CUR);
	fprintf(out, "\n%d\n", lungime);
	final = eliberare(final);
}

/*
 * Taskul 2
*/
void task2(FILE *out, Graf final)
{
		
	int lungime = 0;
	int auxTraseu;
	int verificareTraseu;
	int i, j;

	fprintf(out, "%d ", final.start);
	
	for(i = 0; i < final.transporturi; i++)
	{
		if(final.tipuri[final.start] == 1)
		{
			lungime += final.matCost[final.start][final.traseu[1][i]];
			afisareDrum(final, final.start, final.traseu[1][i], out);
			final.start = final.traseu[1][i];
		}
		else
		{
			verificareTraseu = 0;
			for(j = 0; j < final.noduri; j++)
			{
				if(final.tipuri[j] == 1 && verificareTraseu == 0)
				{
					auxTraseu = j;
					verificareTraseu = 1;
				}

				else if(final.tipuri[j] == 1 && final.matCost[final.traseu[1][i-1]][j]
				+ final.matCost[j][final.traseu[1][i]] < final.matCost[final.traseu[1][i-1]][auxTraseu]
				+ final.matCost[auxTraseu][final.traseu[1][i]] )
				{
					auxTraseu = j;
				}
			}

				afisareDrum(final, final.traseu[1][i-1], auxTraseu, out);
				afisareDrum(final, auxTraseu, final.traseu[1][i], out);
				lungime += final.matCost[final.traseu[1][i-1]][auxTraseu];
				lungime += final.matCost[auxTraseu][final.traseu[1][i]];
				final.start = final.traseu[1][i];
		}
	}
	
	verificareTraseu = 0;
	
	for(j = 0; j < final.noduri; j++)
	{
		if(final.tipuri[j] == 1 && verificareTraseu == 0)
		{
			auxTraseu = j;
			verificareTraseu = 1;
		}
		else if(final.tipuri[j] == 1 && final.matCost[final.traseu[1][i-1]][j]
		< final.matCost[final.traseu[1][i-1]][auxTraseu])
			auxTraseu = j;
	}
	
	afisareDrum(final, final.traseu[1][i-1], auxTraseu, out);
	lungime += final.matCost[final.traseu[1][i-1]][auxTraseu];
	
	fseek(out, -sizeof(char), SEEK_CUR);
	fprintf(out, "\n%d\n", lungime);

	final = eliberare(final);
}

/*
 * Taskul 3.
*/
void task3(FILE *out, Graf final)
{		
	int lungime = 0;
	int auxTraseu;
	int verificareTraseu;
	int i, j;
	
	int *ordine;
	int *v;

	ordine = (int*)calloc(final.transporturi, sizeof(int));
	v = (int*)calloc(final.transporturi, sizeof(int));
	
	for(i = 0; i < final.transporturi; i++)
			ordine[i] = i;
	
	bk(0, final, v, ordine);
	fprintf(out, "%d ", final.start);
	
	if( final.tipuri[final.start] != final.traseu[0][ordine[0]] )
	{
		verificareTraseu = 0;
		for(j = 0; j < final.noduri; j++)
		{
			if(final.tipuri[j] == final.traseu[0][ordine[0]] && verificareTraseu == 0)
			{
				auxTraseu = j;
				verificareTraseu = 1;
			}
		
			else if(final.tipuri[j] == final.traseu[0][ordine[0]] && final.matCost[final.start][j]
			+ final.matCost[j][final.traseu[1][ordine[0]]] < final.matCost[final.start][auxTraseu]
			+ final.matCost[auxTraseu][final.traseu[1][ordine[0]]] )
			{
				auxTraseu = j;
			}
		}
			
			lungime += final.matCost[final.start][auxTraseu];
			afisareDrum(final, final.start, auxTraseu, out);
			final.start = auxTraseu;
	}

	for(i = 0; i < final.transporturi; i++)
	{
		if(final.tipuri[final.start] == final.traseu[0][ordine[i]])
		{
			lungime += final.matCost[final.start][final.traseu[1][ordine[i]]];
			afisareDrum(final, final.start, final.traseu[1][ordine[i]], out);
			final.start = final.traseu[1][ordine[i]];
		}
		else
		{
			verificareTraseu = 0;
	
			for(j = 0; j < final.noduri; j++)
			{
				if(final.tipuri[j] == final.traseu[0][ordine[i]] && verificareTraseu == 0)
				{
					auxTraseu = j;
					verificareTraseu = 1;
				}

				else if(final.tipuri[j] == final.traseu[0][ordine[i]] && final.matCost[final.traseu[1][ordine[i-1]]][j]
				+ final.matCost[j][final.traseu[1][ordine[i]]] < final.matCost[final.traseu[1][ordine[i-1]]][auxTraseu]
				+ final.matCost[auxTraseu][final.traseu[1][ordine[i]]])
				{
					auxTraseu = j;
				}
			}

				afisareDrum(final, final.traseu[1][ordine[i-1]], auxTraseu, out);
				afisareDrum(final, auxTraseu, final.traseu[1][ordine[i]], out);
				
				lungime += final.matCost[final.traseu[1][ordine[i-1]]][auxTraseu];
				lungime += final.matCost[auxTraseu][final.traseu[1][ordine[i]]];				
				final.start = final.traseu[1][ordine[i]];
		}
	}

	verificareTraseu = 0;
	
	for(j = 0; j < final.noduri; j++)
	{
		if(final.tipuri[j] != 0 && verificareTraseu == 0)
		{
			auxTraseu = j;
			verificareTraseu = 1;
		}

		else if(final.tipuri[j] != 0 && final.matCost[final.traseu[1][ordine[final.transporturi-1]]][j]
		< final.matCost[final.traseu[1][ordine[final.transporturi-1]]][auxTraseu])
		{
			auxTraseu = j;
		}
	}

	afisareDrum(final, final.traseu[1][ordine[final.transporturi-1]], auxTraseu, out);
	lungime += final.matCost[final.traseu[1][ordine[final.transporturi-1]]][auxTraseu];

	fseek(out, -sizeof(char), SEEK_CUR);
	fprintf(out, "\n%d\n", lungime);

	free(v);
	free(ordine);
	final = eliberare(final);
}

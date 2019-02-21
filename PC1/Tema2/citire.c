#include "info.h"

bool este_bun(char *v);
void aranjare(char *sir);
char *det_random(char **sir, int nr);

/*
	Ia toate frazele din fiesierele date ca argument si o alege pe cea
	cu care urmeaza sa ne jucam (in mod aleator dintre cele valide).
*/
char *fraza_jucata(int nr_argumente, char **argumente, int *nr,int *nr_caractere)
{
        int numar = 0;
        int max = 10;
        char aux[2000];
        char **v;
        int i;
	FILE *in;
	char *final;
	int numar_caractere = 0;
	
	v = (char**)malloc(max * sizeof(char*) );

        for(i = 1; i < nr_argumente; i++)
        {
		in = fopen(argumente[i], "r");
		
		while( fgets(aux, 2000, in) )
		{
			aux[strlen(aux) - 1] = '\0';

			if( este_bun(aux) == TRUE )
			{
				aranjare(aux);

				if( numar == max )
				{
					max *= 2;
					v = realloc(v, max * sizeof(char*) );
				}

				v[numar] = (char*)malloc
				( (strlen(aux) + 1) * sizeof(char) );

				strcpy( v[numar], aux );
				numar++;
			}
		}

		fclose(in);

        }
	
	for(i = 0; i < numar; i++)
	{
		numar_caractere += strlen(v[i]);
	}

	*nr_caractere = numar_caractere;

	strcpy(aux, det_random(v, numar) );

	final = (char*)malloc( (strlen(aux) + 1) * sizeof(char) );
	strcpy(final, aux);

	*nr = numar;

	for(i = 0; i < numar; i++)
	{
		free(v[i]);
	}

	free(v);

	return final;
}

/*
	Cum frazele sun salvate in ordine, se determina un numar aleator si
	acea fraza este trimisa inapoi, ca cea jucata.
*/
char *det_random(char **sir, int nr)
{
	int n = rand() % nr;

	srand( time(NULL) );

	return sir[n];
}

/*
	Verifica existenta caracterelor jucabile. In cazul in care nu exista
	acea fraza nu este introdusa in joc.
*/
bool este_bun(char *v)
{
	int i;
	
	for(i = 0; i < strlen(v); i++)
	{
		if(isdigit(v[i]) || isalpha(v[i]) || strchr(",{}';\\", v[i]))
		{
			return TRUE;
		}
	}

	return FALSE;
}
/*
	Elimina caracterele care nu pot fi jucate.
*/
void aranjare(char *sir)
{
	char aux[500];
	int i = 0;

	while(i < strlen(sir) )	
	{
		if( !isalpha(sir[i]) && !isdigit(sir[i]) && sir[i] != ' ' && !strchr(",;{}\\'", sir[i]))
		{
			strcpy(aux, sir + i + 1);
			strcpy(sir + i, aux);
		}

		else
		{
			sir[i] = tolower(sir[i]);
			i++;
		}
	}

}


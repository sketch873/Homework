#include "info.h"

/*
	Ia fiecare fisier dar ca argument si il verifica
*/
bool ver_argum(int argc, char **argv)
{
        int i;
        FILE *in;

        for(i = 1; i < argc; i++)
        {
                in = fopen(argv[i], "r");
                
		if( in == NULL)
                {
                        printf("Fisierul %s nu poate fi deschis\n", argv[i]);
                        return FALSE;
                }
		
                fclose(in);
        }

        return TRUE;
}

/*
	Ia textul, aflat intr-un (char*), si il separa in mai multe cuvinte.
*/
char **impartire_pe_cuvinte(char *v, int *nr)
{
        char **cuvinte;
        int max = 10;
        int numar = 0;
        char *p;

        cuvinte = (char**)malloc( max * sizeof(char*) );

        p = strtok(v, " ");

        while( p )
        {

                if(numar == max)
                {
                        max *= 2;
                        cuvinte = realloc( cuvinte, max * sizeof(char*) );
                }

                cuvinte[numar] = (char*)malloc( (strlen(p) + 1) * sizeof(char));
                strcpy(cuvinte[numar++], p);

                p = strtok(NULL, " ");

        }
        
	*nr = numar;
        
	free(p);

        return cuvinte;
}

/*
	Pentru fiecare cuvant, in functie de lungime, specifica daca litera
	este de afisat sau nu (inainte de a se putea introduce litere de la
	tastatura).
*/
int **initializare_joaca(char **cuvinte, int nr_cuvinte)
{
	int **cuvinte_bool;
	int i, j, k, l;
	char c;
	
	cuvinte_bool = (int**)calloc(nr_cuvinte, sizeof(int*) );
        
	for(i = 0; i < nr_cuvinte; i++)
	{
		cuvinte_bool[i] = (int*)calloc(strlen(cuvinte[i]) ,sizeof(int));
	}
	
	for(i = 0; i < nr_cuvinte; i++)
	{
		if(strlen(cuvinte[i]) == 2)
                {
		        cuvinte_bool[i][0] = 1;
		}

		else if( strlen(cuvinte[i]) > 2)
		{
			cuvinte_bool[i][0] = 1;
			cuvinte_bool[i][strlen(cuvinte[i]) - 1] = 1;
		}
	}

	for(i = 0; i < nr_cuvinte; i++)
	{
		for(j = 0; j < strlen(cuvinte[i]); j++)
		{
			if( cuvinte_bool[i][j] == 1)
                        {
			        c = cuvinte[i][j];
			}
			
			for(k = 0; k < nr_cuvinte; k++)
			{
				for(l = 0; l < strlen(cuvinte[k]); l++)
				{
					if(cuvinte[k][l] == c)
					{
						cuvinte_bool[k][l] = 1;
					}
				}
			}

		}
	}

	return cuvinte_bool;
}

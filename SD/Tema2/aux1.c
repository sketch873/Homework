#include "arbore.h"

/*
	Din fisier se citeste playerul care realizeaza primul mutarea
	precum si tabla de joc.
*/

char **citireFisier(FILE *in, char *inceput)
{
    char **tabla;
    char rand[10];
    int i;

    tabla = (char**)malloc(3*sizeof(char*));
    for(i = 0; i < 3; i++)
        tabla[i] = (char*)malloc(3*sizeof(char));

    fgets(rand, 3, in);
    *inceput = rand[0];
        
    for(i = 0; i < 3; i++)
    {
        fgets(rand, 7, in);
        tabla[i][0] = rand[0];
        tabla[i][1] = rand[2];
        tabla[i][2] = rand[4];
    }
    return tabla;
}

/*
	Numara cate spatii disponibile mai sunt pe tabla de joc.
*/

int pozitiiTabla(char **tabla)
{
    int i, j;
    int x = 0; 
    
	for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            if(tabla[i][j] == '-')
                x++;
    return x;
}

/*
	Realizeaza un nou nod, in care adauga tabla de joc specifica pasului curent
	si aloca memorie pentru "spatii" fii posibili.
*/

Nod *initTree(char **tabla, int spatii)
{
    int i, j;
    Nod *x = (Nod*)malloc(sizeof(Nod));
    
	for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            x->tabla[i][j] = tabla[i][j];

    x->fii = spatii;
    x->copil = (struct nod**)malloc(spatii*sizeof(struct nod*));

    for(i = 0; i < spatii; i++)
        x->copil[i] = NULL;

    return x;
}

/*
	Daca tabla de joc are 3 elemente identice pe linie, coloana sau diagonala
	(neaparat diferite de '-') atunci jocul se termina.
*/

int final(char **tabla)
{

    if(tabla[0][0] == tabla[0][1] && tabla[0][0] == tabla[0][2] && tabla[0][0] != '-')
        return 1;
    if(tabla[1][0] == tabla[1][1] && tabla[1][0] == tabla[1][2] && tabla[1][0] != '-')
        return 1;
    if(tabla[2][0] == tabla[2][1] && tabla[2][0] == tabla[2][2] && tabla[2][0] != '-')
        return 1;
    if(tabla[0][0] == tabla[1][0] && tabla[0][0] == tabla[2][0] && tabla[0][0] != '-')
        return 1;
    if(tabla[0][1] == tabla[1][1] && tabla[0][1] == tabla[2][1] && tabla[0][1] != '-')
        return 1;
    if(tabla[0][2] == tabla[1][2] && tabla[0][2] == tabla[2][2] && tabla[0][2] != '-')
        return 1;
    if(tabla[0][0] == tabla[1][1] && tabla[0][0] == tabla[2][2] && tabla[0][0] != '-')
        return 1;
    if(tabla[2][0] == tabla[1][1] && tabla[2][0] == tabla[0][2] && tabla[2][0] != '-')
        return 1;
    return 0;
}

/*
	Se parcurge tabla de joc si pentru fiecare spatiu liber se realizeaza un nou succesor
	pentru nodul curent.
*/

Nod *realizareArbore(Nod *arbore, char **tabla, int mutare, int spatii)
{
    char cazuri[] = "OX";
    int i, j;
    int aux = 0;

	for(i = 0; i < 3; i++)
        for(j = 0; j < 3; j++)
            if(tabla[i][j] == '-' && !final(tabla))
            {
                tabla[i][j] = cazuri[mutare];
                arbore->copil[aux] = initTree(tabla, spatii-1);
                
				if(!final(tabla) )
                {
                    arbore->copil[aux] = realizareArbore(arbore->copil[aux], tabla, 1-mutare, spatii-1);
                }
                
				aux++;
                tabla[i][j] = '-';
            }

    if(final(tabla))
        arbore->fii = 0;
 
	return arbore;
}

/*
	Se afiseaza numarul de taburi specific distantei dintre radacina si nodul de afisat
	precum si tabla de joc.
*/

void afisare(FILE *out, Nod *arbore, int taburi)
{
    int i;

    for(i = 0; i < taburi; i++)
        fprintf(out, "\t");
    fprintf(out, "%c %c %c\n", arbore->tabla[0][0], arbore->tabla[0][1], arbore->tabla[0][2]);

    for(i = 0; i < taburi; i++)
        fprintf(out, "\t");
    fprintf(out, "%c %c %c\n", arbore->tabla[1][0], arbore->tabla[1][1], arbore->tabla[1][2]);

    for(i = 0; i < taburi; i++)
        fprintf(out, "\t");
    fprintf(out, "%c %c %c\n", arbore->tabla[2][0], arbore->tabla[2][1], arbore->tabla[2][2]);
    
	fprintf(out, "\n");
    
	for(i = 0; i < arbore->fii; i++)
        if(arbore->copil[i])
            afisare(out, arbore->copil[i], taburi + 1);
}

/*
	Se elimina memoria incepand cu frunzele pana la radacina.
*/

Nod *eliberareMemorie(Nod *arbore)
{
	int i;
    if(arbore == NULL)
		return NULL;

	for(i = 0; i < arbore->fii; i++)
		arbore->copil[i] = eliberareMemorie(arbore->copil[i]);

	free(arbore->copil);
	free(arbore);

    return NULL;
}

/*
	Verifica daca jucatorul curent a castigat jocul (intoarce T),
	altfel intoarce F.
*/

char verificareAdevar(char tabla[3][3], int jucator)
{
    char cazuri[] = "OX";
    int i;
    
	for(i = 0; i < 3; i++)
    {
        if(tabla[i][0] == tabla[i][1] && tabla[i][0] == tabla[i][2] && tabla[i][0] == cazuri[jucator])
            return 'T';
        else if(tabla[0][i] == tabla[1][i] && tabla[0][i] == tabla[2][i] && tabla[0][i] == cazuri[jucator])
            return 'T';
    }
    
	if(tabla[0][0] == tabla[1][1] && tabla[1][1] == tabla[2][2] && tabla[0][0] == cazuri[jucator])
        return 'T';
    else if(tabla[2][0] == tabla[1][1] && tabla[1][1] == tabla[0][2] && tabla[1][1] == cazuri[jucator])
        return 'T';
    
	return 'F';
}

/*
	Se realizeaza arborele SI/SAU de la frunze catre radacina.
	Dupa terminarea celui mai de jos nivel se decide daca nodul parinte
	este de tip SI ori SAU si verifica toti urmasii.
*/

Nod *siSau(Nod *arbore, int jucator, int mutare)
{
    int i;
    int auxSau = 0;
    if(arbore == NULL)
        return NULL;
    if(arbore->fii == 0)
        arbore->sisau = verificareAdevar(arbore->tabla, jucator);
    else
    {
        for(i = 0; i < arbore->fii; i++)
        {
            if(arbore->copil[i])
                arbore->copil[i] = siSau(arbore->copil[i], jucator, 1 - mutare);
        }
        
		if(jucator == mutare)
        {
            arbore->sisau = 'F';
            for(i = 0; i < arbore->fii; i++)
                if(arbore->copil[i] && arbore->copil[i]->sisau == 'T')
                {
                    arbore->sisau = 'T';
                    break;
                }

        }
        else
        {
            for(i = 0; i < arbore->fii; i++)
                if(arbore->copil[i] && arbore->copil[i]->sisau == 'T')
                    auxSau++;
            if(auxSau == arbore->fii || verificareAdevar(arbore->tabla, jucator) == 'T')
                arbore->sisau = 'T';
            else
                arbore->sisau = 'F';
        }
    }

    return arbore;
}

/*
	Se afiseaza numarul de taburi determinat de distanta de la radacina la
	nodul curent si unul din caracterele T sau F, in functie de tipul nodului.
*/

void afisareSiSau(FILE *out, Nod *arbore, int taburi)
{
    int i;

    for(i = 0; i < taburi; i++)
        fprintf(out, "\t");

    fprintf(out, "%c\n", arbore->sisau);

    for(i = 0; i < arbore->fii; i++)
        if(arbore->copil[i])
            afisareSiSau(out, arbore->copil[i], taburi+1);
}


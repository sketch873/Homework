#include "arbore.h"


void taskI(char *input, char *output);
void taskII(char *input, char *output);
void taskIII(char *input, char *output);
void bonus(char *input, char *output);

/*
	Pentru taskurile I si II
	Din fisierul aux1.c
	Ultimele 3 functii ajuta pentru taskul II
*/
char **citireFisier(FILE *in, char *inceput);
int pozitiiTabla(char **tabla);
Nod *initTree(char **tabla, int spatii);
int final(char **tabla);
Nod *realizareArbore(Nod *arbore, char **tabla, int mutare, int spatii);
void afisare(FILE *out, Nod *arbore, int taburi);
Nod *eliberareMemorie(Nod *arbore);

char verificareAdevar(char tabla[3][3], int jucator);
Nod *siSau(Nod *arbore, int jucator, int mutare);
void afisareSiSau(FILE *out, Nod *arbore, int taburi);

/*
	Pentru taskul III si bonus
	Din fisierul aux2.c
*/
mMax *citireFacere(FILE *in, mMax *arbore, int niveluri);
mMax *minimax(mMax *arbore,int mutare);
void afisareMinimax(FILE *out, mMax *arbore, int taburi);
mMax *eliberareMMAX(mMax *arbore);
int alfaBeta(mMax *arbore, int mutare, int alfa, int beta);


/*
	Pentru fiecare posibilitate a primului argument
	am trimis la functia specifica a taskului numele
	fisierelor de input si output.
*/

int main(int argc, char **argv)
{
	if(strcmp(argv[1], "-c1") == 0)
		taskI(argv[2], argv[3]);
	
	else if(strcmp(argv[1], "-c2") == 0)
		taskII(argv[2], argv[3]);
	
	else if(strcmp(argv[1], "-c3") == 0)
		taskIII(argv[2], argv[3]);
	
	else if(strcmp(argv[1], "-b") == 0)
		bonus(argv[2], argv[3]);
	
	return 0;
}

/*
	Arborele de joc:
		->se citesc jucatorul care incepe si tabla initiala de joc
		->se realizeaza, afiseaza si elibereaza arborele
	OBS: initializarea radacinii se face prin initTree in taskI

*/

void taskI(char *input, char *output)
{
	Nod *arbore = NULL;
	FILE *in, *out;
	char inceput;
	int numarSpatii;
	char **tabla;

	in = fopen(input, "r");
	
	if(in == NULL)
		return;
	
	out = fopen(output, "w");

	tabla = citireFisier(in, &inceput);
	numarSpatii = pozitiiTabla(tabla);
	arbore = initTree(tabla, numarSpatii);
	
	if(inceput == 'X')
		arbore = realizareArbore(arbore, tabla, 1, numarSpatii);
	else
		arbore = realizareArbore(arbore, tabla, 0, numarSpatii);

	afisare(out, arbore, 0);

	arbore = eliberareMemorie(arbore);

	free(tabla[0]);
	free(tabla[1]);
	free(tabla[2]);
	free(tabla);
	fclose(in);
	fclose(out);
}

/*
	Arborele SI/SAU:
		->se citesc jucatorul care incepe si tabla initiala de joc
		->se realizeaza arborele de joc
		->pe baza arborelui de joc se realizeaza nodurile T si F
		->se afiseaza si elibereaza memoria
	OBS: initializarea radacinii se face prin initTree in taskII
*/

void taskII(char *input, char *output)
{
	Nod *arbore = NULL;
	FILE *in, *out;
	char inceput;
	int numarSpatii;
	char **tabla;

	in = fopen(input, "r");

	if(in == NULL)
		return;

	out = fopen(output, "w");

	tabla = citireFisier(in, &inceput);
	numarSpatii = pozitiiTabla(tabla);
	arbore = initTree(tabla, numarSpatii);

	if(inceput == 'X')
	{
		arbore = realizareArbore(arbore, tabla, 1, numarSpatii);
		arbore = siSau(arbore, 1, 1);
	}
	else
	{
		arbore = realizareArbore(arbore, tabla, 0, numarSpatii);
		arbore = siSau(arbore, 0, 0);
	}

	afisareSiSau(out, arbore, 0);
	arbore = eliberareMemorie(arbore);

	free(tabla[0]);
	free(tabla[1]);
	free(tabla[2]);
	free(tabla);
	fclose(in);
	fclose(out);
}

/*
	Arborele minimax:
		->se citesc nivelurile
		->se realizeaza arborele pe nivel
		(apelarea functiei citireFacere de "niveluri" ori)
		->se afiseaza si elibereaza memoria
*/

void taskIII(char *input, char *output)
{
	mMax *arbore = NULL;
	FILE *in, *out;
	int niveluri, i;

	in = fopen(input, "r");

	if(in == NULL)
		return;

	out = fopen(output, "w");

	fscanf(in, "%d", &niveluri);
	for(i = 0; i < niveluri; i++)
		arbore = citireFacere(in, arbore, i);

	arbore = minimax(arbore, 1);
	afisareMinimax(out, arbore, 0);
	arbore = eliberareMMAX(arbore);	

	fclose(in);
	fclose(out);
}

/*
	Alpha-Beta Pruning:
		->se citesc nivelurile
		->se realizeaza arborele pe nivel
		(apelarea functiei citireFacere de "niveluri" ori)
		->se afiseaza si elibereaza memoria
*/

void bonus(char *input, char *output)
{
	mMax *arbore = NULL;
	FILE *in, *out;
	int niveluri, i;

	in = fopen(input, "r");

	if(in == NULL)
		return;

	out = fopen(output, "w");

	fscanf(in, "%d", &niveluri);
	for(i = 0; i < niveluri; i++)
		arbore = citireFacere(in, arbore, i);

	arbore->val = alfaBeta(arbore, 1, INT_MIN, INT_MAX);
	afisareMinimax(out, arbore, 0);
	arbore = eliberareMMAX(arbore);	

	fclose(in);
	fclose(out);
}

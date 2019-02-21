#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
	'strrev' inverseaza un sir de caractere.
*/
void strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
}
/*
	Verifica daca sirul e cuvant.
	1 - afirmativ, 2 - negativ
*/
int verificare_cuvant( char *sir)
{
	int i;
	for(i = 0; i < strlen(sir); i++)
		if( sir[i] < '0' || sir[i] > '9')
			return 1;
	return 0;

}
/*
	Cauta cel mai mare divizor al lungimii cuvantului.
*/
int divizor(int n)
{
	int i;
	for(i = n - 1; i >= 1; i--)
		if(! ( n % i) )
			return i;
	return 1;
}
/*
	Codifica in cazul in care e cuvant.
	Algoritmul din interiorul blocurilor if-else:
	-> Sirul 'a' primeste a doua parte a sirului si
		o adauga la inceputul noului cuvant.
	-> Sirul 'b' se inverseaza cu 'strrev' si se adauga
		ultimele 'd' caractere (primele 'd'
		caractere din sirul neinversat) .
	->Sirul 'b' se inverseaza, pentru a avea forma initiala, si se lipeste la 'a'.
*/
void codificare_cuvant(char *c)
{
	int i;
	int ver_cifr = 0;
	//contor care memoreaza daca are cifre
	for(i = 0; i < strlen(c); i++)
	//cautare cifre
		if( c[i] >= '0' && c[i] <= '9')
		{
			ver_cifr = 1;
			break;
		}
	int d = divizor( strlen(c) );

	char a[51];
	//stocheaza a doua parte a cuvantului
	char b[51];
	//stocheaza prima parte a cuvantului
	if(ver_cifr == 0)
	//daca are cifre
	{
		strcpy(a, c + d);
		strrev(c);
		strcpy(b, c + strlen(c) - d);
		strrev(b);
		strcpy(c, a);
		strcat(c, b);
	}
	else
	//daca nu are cifre
	{
		strcpy(a, c + d);
		strrev(a);
		strrev(c);
		strcpy(b, c + strlen(c) - d);
		strrev(b);
		strcpy(c, a);
		strcat(c, b);
	}
}
/*
	Codifica in cazul in care e caracter.
	Algoritm:
	-> 'v' este un vector de frecventa.
	-> Cum codurile ASCII sunt din intervalul [32,127), adica 95 de caractere,
		'v' are 95 de spatii.
	-> 'min' si 'max' tin minte numarul minim/maxim
		al aparitiei unui caracter din sirul format pana atunci.
	-> 'c_min' si 'c_max' sunt caracterele cu cea
		mai mica/mare frecventa a aparitiei din sir.
	-> Noul sir 'c' este format din 'c_max', vechiul 'c' si 'c_min'.
*/
void codificare_caracter(char c[], char *sir)
{
	int v[95] = {0};
	int i;
	int min = strlen(sir);
	int max = 0;
	char c_min;
	char c_max;
	int contor_max = 0;
	int contor_min = 0;
	for(i = 0; i < strlen(sir); i++)
		v[ sir[i] - 32 ] ++;
	for(i = 0; i < 95; i++)
	{
		if(v[i] > max) max=v[i];
		if(v[i] < min && v[i] != 0) min=v[i];
	}
	for(i = 0; i < strlen(sir); i++)
	{
		if(v[ sir[i] - 32 ] == max && ! contor_max)
		{
			c_max = sir[i];
			contor_max ++;
		}
		if(v[ sir[i] - 32] == min && ! contor_min)
		{
			c_min = sir[i];
			contor_min ++;
		}
	}
	c[1] = c[0];
	c[0] = c_max;
	c[2] = c_min;
	c[3] = '\0';

}

/*
	Functie care codifica sirul de tip numar, in cazul in care e pozitiv.
	Algoritm:
		-> Se executa n rotatii ciclice, cat numarul cifrelor.
		-> 'v' stocheaza pasul curent din rotatia ciclica.
		-> 'aux' stocheaza numarul cel mai mare.
		-> 'k' este un sir auxiliar, folosit la rotatia ciclica.
	Observatie: se obtine cel mai mare numar.
*/
void codificare_numar_pozitiv(char c[])
{
        int i;
        char v[51];
        char aux[51];
        strcpy(v, c);
        strcpy(aux, v);
        for(i = 0; i < strlen(v); i++)
        {
                char k[51];
                int lungime;
                strcpy(k, v+1);
                lungime = strlen(k);
                k[lungime] = v[0];
                k[lungime+1] = '\0';
                strcpy(v, k);
                if( strcmp(aux, v) < 0)
                        strcpy(aux, v);
        }
        strcpy(c, aux);
}
/*
	Functie care codifica sirul de tip numar, in cazul in care e negativ.
	Algoritmul este acelasi ca la 'codificare_numar_pozitiv'
		dar se obtine cel mai mic numar.
*/
void codificare_numar_negativ(char c[])
{
	int i;
	char v[51];
	char aux[51];
	strcpy(v, c+1);
	strcpy(aux, v);
	for(i = 1; i < strlen(v); i++)
	{
		char k[51];
		int lungime;
		strcpy(k, v+1);
		lungime = strlen(k);
		k[lungime] = v[0];
		k[lungime+1] = '\0';
		strcpy(v, k);
		if( strcmp(aux, v) > 0)
			strcpy(aux, v);
	}
	strcpy(c, aux);
}
/*
	Verifica si numara cate aparitii sunt din fiecare tip.
*/
void verificare(int *cuvant, int *caracter, int *numar, char c[], char *sir)
{
	if( strlen(c) == 1  && (c[0] < '0' || c[0] > '9') )
		//daca are lungimea 1 si nu e cifra e caracter
	{	
		(*caracter) ++;
		if( sir == NULL )
			return;
		//in cazul in care sirul codificat pana atunci
		//este gol atunci nu adauga nimic
		else
			codificare_caracter(c, sir);
		//daca sirul codificat nu e gol adauga caracterele
		//cu minimul si maximul de aparitii

	}

	else if( c[0] == '-' && !verificare_cuvant(c+1) )
		//daca primul caracter este '-'
		//si nu contine litere atunci e numar
	{
		(*numar) ++;
		codificare_numar_negativ(c);
	}

	else if( verificare_cuvant(c) )
		//verifica daca noul text citit contine litere
	{
		(*cuvant) ++;
		codificare_cuvant(c);
	}

	else
		//daca nu e cuvant si nici caracter => e numar
	{
		(*numar) ++;
		codificare_numar_pozitiv(c);
	}
}
/*
	Citeste	si verifica de ce tip este informatia introdusa.
	Algoritm:
		*1* Introducerea sirurilor de caractere 'c' si 'aux'.
		->  Citirea initiala se face in 'c' dar se continua in 'aux'.
		->  Scopul este acela de a gasi numarul 'n' (task III) mult mai rapid
			(daca 'aux' contine "END" atunci continutul lui 'c' este 'n').
		*2* Compararea initiala a sirului 'c' cu "END", ca sa termine programul.
		*3* Prima citire in 'aux' si verificarea separata daca are "END".
		*4* Initializarea sirului.
		*5*  In urma fiecarui pas, daca 'aux' este diferit de "END" atunci
			'c' este diferit de 'n' si are loc urmatoarea secventa:
				sir <- c ; c <- aux;
		*6* Daca 'aux' este "END" atunci 'c' este 'n' si se converteste
			'c' la tipul intreg.
		
*/
void citire(int *cuvant, int *caracter, int *numar, char **sir, int *lungime, int *n)
{
	char c[51];
/*1*/	char aux[51];
	
	scanf("%s", c);
	
/*2*/	if( !strcmp(c, "END") )
		return;
	scanf("%s", aux);
/*3*/	if( strcmp(aux, "END") )
	{
		verificare(cuvant, caracter, numar, c, *sir);
/*4*/		*sir = (char *)malloc( (strlen(c) + 1 ) * sizeof(char) );
		*lungime += strlen(c);
		strcpy(*sir, c);
		strcpy(c, aux);
	}

	while( 1 )
	{
		scanf("%s", aux);
/*5*/		if( strcmp(aux, "END") )
		{
			verificare(cuvant, caracter, numar, c, *sir);
			*sir = (char *)realloc(*sir, (*lungime + strlen(c) + 1) * sizeof(char) );
			*lungime += strlen(c);
			strcat(*sir, c);
			strcpy(c, aux);
		}
		else
		{
/*6*/			*n = atoi(c);
			break;
		}
	}

}

typedef struct{
char *bucata;					//fragment din sirul mare
double complexitate;				//complexitatea lui
} fragmente;
/*
	Afisarea fragmentelor sortate.
	Algoritm:
		-> Afisam fragmentul de pe pozitia 'i' (cel mai mare) si
		fragmentul de pe pozitia 'n-1-i' (cel mai mic) la fiecare pas.
		-> Daca sirul are un numar impar de termeni, pe cel din mijloc 
		il afisez ultimul, pozitia 'n%2".
*/
void afisare(fragmente *fragment, int n)
{
	int i;
	for(i = 0; i < n/2; i++)
		printf("%s%s", fragment[i].bucata, fragment[n - 1 - i].bucata);
	if( n % 2 )
		printf("%s", fragment[n / 2].bucata);
	else
		printf("\n");
}
/*
	Ordoneaza fragmentele dupa complexitate si lexicografic.
	Algoritm:
		*1* Introducerea unei structuri auxiliare.
		*2* Sortare dupa complexitate.
		*3* Interschimbarea complexitatilor.
		*4* Interschimbarea sirurilor 'bucata' se face prin si
		cu alocari de memorie, pentru a nu copia nu text mai lung
		in spatiul unde se afla unul mai scurt.
		*5* Aceeasi interschimbare aplicata pe elementelor consecutive
		(dupa sortare dupa complexitate, elementele consecutive pot avea
		aceeasi complexitate, deci sorteaza lexicografic).
*/
void ordonare_complexitate(fragmente *fragment, int n)
{
	int i, j;
/*1*/	fragmente auxiliar;
	for(i = 0; i < n - 1; i++)
		for(j = i + 1; j < n; j++)
/*2*/			if( fragment[i].complexitate < fragment[j].complexitate )
		{	//blocul apartine de if
			auxiliar.complexitate = fragment[i].complexitate;						
/*3*/			fragment[i].complexitate = fragment[j].complexitate;
			fragment[j].complexitate = auxiliar.complexitate;
			auxiliar.bucata = (char *)malloc( (strlen( fragment[i].bucata ) + 1) * sizeof(char) );
			strcpy( auxiliar.bucata, fragment[i].bucata );
			free(fragment[i].bucata);
/*4*/			fragment[i].bucata = (char *)malloc( (strlen( fragment[j].bucata ) + 1 ) * sizeof(char) );
			strcpy( fragment[i].bucata, fragment[j].bucata );
			free(fragment[j].bucata);
			fragment[j].bucata = (char *)malloc( (strlen( auxiliar.bucata) + 1 ) * sizeof(char) );
			strcpy(fragment[j].bucata, auxiliar.bucata);
		
			free(auxiliar.bucata);
		}

/*5*/	for(i = 0; i < n - 1; i++)
		if( strcmp( fragment[i].bucata, fragment[i+1].bucata ) > 0  &&  
		fragment[i].complexitate == fragment[i+1].complexitate)
	{	//blocul apartine de if
		auxiliar.complexitate = fragment[i].complexitate;
		fragment[i].complexitate = fragment[i + 1].complexitate;
		fragment[i + 1].complexitate = auxiliar.complexitate;
		auxiliar.bucata = (char *)malloc( (strlen( fragment[i].bucata ) + 1) * sizeof(char) );
		strcpy( auxiliar.bucata, fragment[i].bucata );
		fragment[i].bucata = (char *)malloc( (strlen( fragment[i + 1].bucata ) + 1 ) * sizeof(char) );
		strcpy( fragment[i].bucata, fragment[i + 1].bucata );
		fragment[i + 1].bucata = (char *)malloc( (strlen( auxiliar.bucata) + 1 ) * sizeof(char) );
		strcpy(fragment[i + 1].bucata, auxiliar.bucata);
	
		free(auxiliar.bucata);
	}

}
/*
	Face complexitatea pentru fiecare fragment.
	Algoritm:
		-> Face suma codurilor ASCII pe care o imparte la
		numarul de elemente din sir.
*/
void adaugare_complexitate(fragmente *fragment, int n)
{
	int i,j;
	double suma;
	for(i = 0; i < n; i++)
	{
		suma = 0;
		for(j = 0; j < strlen( fragment[i].bucata ); j++)
			suma += fragment[i].bucata[j];
		fragment[i].complexitate = suma / strlen( fragment[i].bucata );
	}

}
/*
	'task3' cuprinde intregul task, adaugarea, ordonarea si afisarea.
	Algoritm:
		*1* Copiez ultimele 'lungime' elemente din sirul inversat
		(adica primele 'lungime' elemente din sirul normal'.				
		*2* Aloc memoria necesara pentru sirul de caractere din fragmentul
		din structura si copiez.							
		*3* Elimin primele 'lungime' elemente din sirul initial.  			
		*4* Adaug separat ultima parte din 'sir'
		(in urma divizarii poate avea un numar diferit de elemente)			
		*5* Adaug pentru fiecare fragment complexitatea ('adaugare_complexitate').	
		*6* Sortez dupa complexitate si lexicografic.					
		*7* Afisez.									
*/
void task3(char *sir, int n)
{
	int lungime = strlen(sir) / n;
	char c[200];
	int i;
	fragmente *fragment=(fragmente *)malloc(n * sizeof(fragmente) );
	for(i = 0; i < n - 1; i++)
	{
		strrev(sir);
		strcpy(c, sir + strlen(sir) - lungime );
		strrev(c);
/*1*/		strrev(sir);
/*2*/		fragment[i].bucata = (char *)malloc( (strlen(c) + 1 ) * sizeof(char) );
		strcpy( fragment[i].bucata, c);
		char *copie = (char *)malloc( (strlen(sir) - lungime + 1 ) * sizeof(char) );
/*3*/		strcpy(copie, sir + lungime);
		strcpy(sir, copie);
		free(copie);
	}
/*4*/	fragment[n - 1].bucata = (char *)malloc( (strlen(sir) + 1 ) * sizeof(char) );
	strcpy( fragment[n - 1].bucata, sir);

/*5*/	adaugare_complexitate(fragment, n);
	
/*6*/	ordonare_complexitate(fragment, n);
	
/*7*/	afisare(fragment, n);
	for(i = 0; i < n; i++)
		free(fragment[i].bucata);	
	free(fragment);
}

void afisare_task1(int a, int b, int c)
{
	printf("%d %d %d\n", a, b, c);
	//afisare task I
}
void afisare_task2(char *sir)
{
	printf("%s\n", sir);
	//afisare task II
}

int main()
{
	int caracter = 0;		//contor caractere
	int cuvant = 0;			//contor cuvinte
	int numar = 0;			//contor numare
	char *sir = '\0';		//initializare sir codificat (task I) cu NULL
	int lungime = 0;		//lungimea sirului codificat
	int n;				//numarul de diviziuni (task III)
	/*
	Functia 'citire' are rolul de a primi inputul, numararea cuvintelor, caracterelor
	si a numerelor si codificarea lor pe rand, urmand lipirea la 'sir'.
	*/
	citire(&cuvant, &caracter, &numar, &sir, &lungime, &n);	
	/*
	Afisarea raspunsurilor de la primele doua taskuri.
	*/
	afisare_task1(cuvant, caracter, numar);
	afisare_task2(sir);
	/*
	Functia 'task3' cuprinde divizarea, conform enuntului, cat si
	sortarea dupa complexitate si afisarea.
	*/
	task3(sir, n);
	free(sir);

	return 0;
}


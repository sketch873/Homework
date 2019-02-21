#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <ctype.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <math.h>


#define TRUE 1
#define FALSE 0

#define SEC 7
#define USEC 0

typedef struct{
char **cuvinte;
int **ascunse;
int vieti;
int litere[42];
char caractere_speciale[6];
int numar_cuvinte;
int numar_fraze_jucate;
int numar_total_caractere;
}propozitie;

typedef struct{
int spanzuratoare;
int scris;
int fundal;
}culori;

#define rad(unghi) (unghi * (180.0f/M_PI))

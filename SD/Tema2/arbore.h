#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct nod{
	char tabla[3][3];
	int fii;
	char sisau;
	struct nod **copil;
} Nod;

typedef struct mmax{
	int val;
	int fii;
	int alfa, beta;
	struct mmax **copil;
} mMax;

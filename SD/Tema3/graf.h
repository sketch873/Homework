#include <stdio.h>
#include <stdlib.h>

#define MAX 1000000

typedef struct{
		int **matCost;
		int **matDrum;
		int *tipuri;
		int noduri;
		int start;

		int transporturi;
		int *traseu[2];
}Graf;

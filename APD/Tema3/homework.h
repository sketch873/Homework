#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


typedef struct {
    int maxWidth;
    int maxHeight;
    int maxVal;
    char type[3];
    unsigned char **rP, **gP, **bP;
} image;

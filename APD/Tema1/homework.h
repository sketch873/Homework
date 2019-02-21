#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

typedef struct {
  int maxWidth;
  int maxHeight;
  int maxVal;
  char type[3];
  unsigned char **rP, **gP, **bP;
}image;

void* resizeColor(void *var);

void* resizeGrayscale(void *var);

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */

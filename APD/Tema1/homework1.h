#ifndef HOMEWORK_H1
#define HOMEWORK_H1

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
  int resolution;
  int maxVal;
  char type[3];
  unsigned char **gP;
}image;

void initialize(image *im);
void render(image *im);
void writeData(const char * fileName, image *img);

#endif /* HOMEWORK_H1 */

#include "homework1.h"

int num_threads;
int resolution;
image *img;

/*  Generare imagine neagra  */
void initialize(image *im) {
  int i, j;

  im->resolution = resolution;
  strcpy(im->type, "P5");
  im->maxVal = 255;
  im->gP = (unsigned char**) malloc (resolution * sizeof(unsigned char*));

  for(i = 0; i < resolution; i++) {
    im->gP[i] = (unsigned char*) malloc (resolution * sizeof(unsigned char));
    for(j = 0; j < resolution; j++) {
      im->gP[i][j] = 255;
    }
  }

}

/*  Distanta dintr un punct si o dreapta  */
double getDistance(double x, double y, int a, int b, int c) {
  double coordX = x / (resolution-1) * 100;
  double coordY = y / (resolution-1) * 100;
  int dist = coordX * a + coordY * b + c;
  return abs(dist) / sqrt(a*a + b*b);
}

/*  Functia pentru thread-uri  */
void* func(void *var) {
  int start, stop, i, j;
  int Tid = *(int*) var;

  start = Tid * ceil(resolution/(double)num_threads);
  stop = (Tid+1) * ceil(resolution/(double)num_threads) < resolution ?
            (Tid+1) * ceil(resolution/(double)num_threads) :
            resolution;

  /*  Paralelizare pe inaltime
   *  se face o transpunere din sistemul de coordonate al matricei
   *  in cel al imaginii (i -> resolution - i - 1)
   */
  for(i = start; i < stop; i++) {
    for(j = 0; j < resolution; j++) {
      if(getDistance((double)j + 0.5, (double)i + 0.5, -1, 2, 0) <= 3) {
        img->gP[resolution - i - 1][j] = 0;
      }
    }
  }

  return var;
}

/*  Functia de randare  */
void render(image *im) {
  int i;
  pthread_t tid[num_threads];
  int thread_id[num_threads];

  img = im;

  for(i = 0; i < num_threads; i++) {
    thread_id[i] = i;
  }

  for(i = 0; i < num_threads; i++) {
    pthread_create(&(tid[i]), NULL, func, &(thread_id[i]));
  }

  for(i = 0; i < num_threads; i++) {
    pthread_join(tid[i], NULL);
  }



}

/*  Functia de scriere  */
void writeData(const char * fileName, image *img) {
  FILE *output = fopen(fileName, "wb");
  int i;

  fprintf(output, "%s\n%d %d\n%d\n",
  img->type, img->resolution, img->resolution, img->maxVal);

  for(i = 0; i < resolution; i++) {
    fwrite(img->gP[i], sizeof(unsigned char), resolution, output);
  }
}

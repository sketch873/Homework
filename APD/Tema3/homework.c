#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*  Structura de poza  */
typedef struct {
    int maxWidth;
    int maxHeight;
    int maxVal;
    char type[3];
    unsigned char **rP, **gP, **bP;
} image;

/*  Realizeaza matricea filtru pentru smooth  */
void makeMatrixSmooth(float matrix[3][3]) {
  int i, j;
  float aux = (float) 1 / 9;
  for(i = 0; i < 3; i++)
    for(j = 0; j < 3; j++)
      matrix[i][j] = aux;
}

/*  Realizeaza matricea filtru pentru sharpen  */
void makeMatrixSharpen(float matrix[3][3]) {
  matrix[0][0] = 0;
  matrix[0][1] = (float) -2 / 3;
  matrix[0][2] = 0;
  matrix[1][0] = (float) -2 / 3;
  matrix[1][1] = (float) 11 / 3;
  matrix[1][2] = (float) -2 / 3;
  matrix[2][0] = 0;
  matrix[2][1] = (float) -2 / 3;
  matrix[2][2] = 0;
}

void sharpen(image *img, int rank, int nProc,
  unsigned char **bufR, unsigned char **bufG, unsigned char **bufB,
  unsigned char *aux_botR, unsigned char *aux_botG, unsigned char *aux_botB,
  unsigned char *aux_upR, unsigned char *aux_upG, unsigned char *aux_upB)
  {
    float matrix[3][3];
    int start, stop;
    int i, j;
    float sum;

    start = rank * ceil(img->maxHeight/(double)nProc);
    stop = (rank+1) * ceil(img->maxHeight/(double)nProc) < img->maxHeight ?
      (rank+1) * ceil(img->maxHeight/(double)nProc) :
      img->maxHeight;

      for(i = 0; i < stop - start; i++) {
        int a, b;
        if(rank == 0 && i == 0)
          continue;
        if(rank == nProc - 1 && i == stop - start - 1)
          continue;
        for(j = 1; j < img->maxWidth-1; j++) {
          makeMatrixSharpen(matrix);
          if (i == 0) {
            matrix[1][1] *= img->gP[i][j];
            matrix[0][0] *= aux_upG[j-1];
            matrix[0][1] *= aux_upG[j];
            matrix[0][2] *= aux_upG[j+1];
            matrix[1][0] *= img->gP[i][j-1];
            matrix[1][2] *= img->gP[i][j+1];
            matrix[2][0] *= img->gP[i+1][j-1];
            matrix[2][1] *= img->gP[i+1][j];
            matrix[2][2] *= img->gP[i+1][j+1];
            sum = 0;
            for(a = 0; a < 3; a++)
              for(b = 0; b < 3; b++)
                sum += matrix[a][b];

            bufG[i][j] = sum;

            if(img->type[1] == '6') {
              makeMatrixSharpen(matrix);
              matrix[1][1] *= img->rP[i][j];
              matrix[0][0] *= aux_upR[j-1];
              matrix[0][1] *= aux_upR[j];
              matrix[0][2] *= aux_upR[j+1];
              matrix[1][0] *= img->rP[i][j-1];
              matrix[1][2] *= img->rP[i][j+1];
              matrix[2][0] *= img->rP[i+1][j-1];
              matrix[2][1] *= img->rP[i+1][j];
              matrix[2][2] *= img->rP[i+1][j+1];
              sum = 0;
              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufR[i][j] = sum;

              makeMatrixSharpen(matrix);
              matrix[1][1] *= img->bP[i][j];
              matrix[0][0] *= aux_upB[j-1];
              matrix[0][1] *= aux_upB[j];
              matrix[0][2] *= aux_upB[j+1];
              matrix[1][0] *= img->bP[i][j-1];
              matrix[1][2] *= img->bP[i][j+1];
              matrix[2][0] *= img->bP[i+1][j-1];
              matrix[2][1] *= img->bP[i+1][j];
              matrix[2][2] *= img->bP[i+1][j+1];
              sum = 0;
              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufB[i][j] = sum;
              }
          } else if(i == stop - start - 1) {
            makeMatrixSharpen(matrix);
            matrix[1][1] *= img->gP[i][j];
            matrix[0][0] *= img->gP[i-1][j-1];
            matrix[0][1] *= img->gP[i-1][j];
            matrix[0][2] *= img->gP[i-1][j+1];
            matrix[1][0] *= img->gP[i][j-1];
            matrix[1][2] *= img->gP[i][j+1];
            matrix[2][0] *= aux_botG[j-1];
            matrix[2][1] *= aux_botG[j];
            matrix[2][2] *= aux_botG[j+1];

            sum = 0;
            for(a = 0; a < 3; a++)
              for(b = 0; b < 3; b++)
                sum += matrix[a][b];

            bufG[i][j] = sum;

            if(img->type[1] == '6') {
              makeMatrixSharpen(matrix);
              matrix[1][1] *= img->rP[i][j];
              matrix[0][0] *= img->rP[i-1][j-1];
              matrix[0][1] *= img->rP[i-1][j];
              matrix[0][2] *= img->rP[i-1][j+1];
              matrix[1][0] *= img->rP[i][j-1];
              matrix[1][2] *= img->rP[i][j+1];
              matrix[2][0] *= aux_botR[j-1];
              matrix[2][1] *= aux_botR[j];
              matrix[2][2] *= aux_botR[j+1];

              sum = 0;
              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufR[i][j] = sum;

              makeMatrixSharpen(matrix);
              matrix[1][1] *= img->bP[i][j];
              matrix[0][0] *= img->bP[i-1][j-1];
              matrix[0][1] *= img->bP[i-1][j];
              matrix[0][2] *= img->bP[i-1][j+1];
              matrix[1][0] *= img->bP[i][j-1];
              matrix[1][2] *= img->bP[i][j+1];
              matrix[2][0] *= aux_botB[j-1];
              matrix[2][1] *= aux_botB[j];
              matrix[2][2] *= aux_botB[j+1];

              sum = 0;
              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufB[i][j] = sum;
            }
          } else {
              makeMatrixSharpen(matrix);
              matrix[1][1] *= img->gP[i][j];
              matrix[0][0] *= img->gP[i-1][j-1];
              matrix[0][1] *= img->gP[i-1][j];
              matrix[0][2] *= img->gP[i-1][j+1];
              matrix[1][0] *= img->gP[i][j-1];
              matrix[1][2] *= img->gP[i][j+1];
              matrix[2][0] *= img->gP[i+1][j-1];
              matrix[2][1] *= img->gP[i+1][j];
              matrix[2][2] *= img->gP[i+1][j+1];
              sum = 0;

              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufG[i][j] = sum;

            if(img->type[1] == '6') {
              makeMatrixSharpen(matrix);
              matrix[1][1] *= img->rP[i][j];
              matrix[0][0] *= img->rP[i-1][j-1];
              matrix[0][1] *= img->rP[i-1][j];
              matrix[0][2] *= img->rP[i-1][j+1];
              matrix[1][0] *= img->rP[i][j-1];
              matrix[1][2] *= img->rP[i][j+1];
              matrix[2][0] *= img->rP[i+1][j-1];
              matrix[2][1] *= img->rP[i+1][j];
              matrix[2][2] *= img->rP[i+1][j+1];
              sum = 0;

              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufR[i][j] = sum;

              makeMatrixSharpen(matrix);
              matrix[1][1] *= img->bP[i][j];
              matrix[0][0] *= img->bP[i-1][j-1];
              matrix[0][1] *= img->bP[i-1][j];
              matrix[0][2] *= img->bP[i-1][j+1];
              matrix[1][0] *= img->bP[i][j-1];
              matrix[1][2] *= img->bP[i][j+1];
              matrix[2][0] *= img->bP[i+1][j-1];
              matrix[2][1] *= img->bP[i+1][j];
              matrix[2][2] *= img->bP[i+1][j+1];
              sum = 0;

              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufB[i][j] = sum;
            }
          }
        }
    }
}

void smooth(image *img, int rank, int nProc,
  unsigned char **bufR, unsigned char **bufG, unsigned char **bufB,
  unsigned char *aux_botR, unsigned char *aux_botG, unsigned char *aux_botB,
  unsigned char *aux_upR, unsigned char *aux_upG, unsigned char *aux_upB)
  {
    float matrix[3][3];
    int start, stop;
    int i, j;
    float sum;

    start = rank * ceil(img->maxHeight/(double)nProc);
    stop = (rank+1) * ceil(img->maxHeight/(double)nProc) < img->maxHeight ?
      (rank+1) * ceil(img->maxHeight/(double)nProc) :
      img->maxHeight;

      for(i = 0; i < stop - start; i++) {
        int a, b;
        if(rank == 0 && i == 0)
          continue;
        if(rank == nProc - 1 && i == stop - start - 1)
          continue;
        for(j = 1; j < img->maxWidth-1; j++) {
          makeMatrixSmooth(matrix);
          if (i == 0) {
            matrix[1][1] *= img->gP[i][j];
            matrix[0][0] *= aux_upG[j-1];
            matrix[0][1] *= aux_upG[j];
            matrix[0][2] *= aux_upG[j+1];
            matrix[1][0] *= img->gP[i][j-1];
            matrix[1][2] *= img->gP[i][j+1];
            matrix[2][0] *= img->gP[i+1][j-1];
            matrix[2][1] *= img->gP[i+1][j];
            matrix[2][2] *= img->gP[i+1][j+1];
            sum = 0;
            for(a = 0; a < 3; a++)
              for(b = 0; b < 3; b++)
                sum += matrix[a][b];

            bufG[i][j] = sum;

            if(img->type[1] == '6') {
              makeMatrixSmooth(matrix);
              matrix[1][1] *= img->rP[i][j];
              matrix[0][0] *= aux_upR[j-1];
              matrix[0][1] *= aux_upR[j];
              matrix[0][2] *= aux_upR[j+1];
              matrix[1][0] *= img->rP[i][j-1];
              matrix[1][2] *= img->rP[i][j+1];
              matrix[2][0] *= img->rP[i+1][j-1];
              matrix[2][1] *= img->rP[i+1][j];
              matrix[2][2] *= img->rP[i+1][j+1];
              sum = 0;
              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufR[i][j] = sum;

              makeMatrixSmooth(matrix);
              matrix[1][1] *= img->bP[i][j];
              matrix[0][0] *= aux_upB[j-1];
              matrix[0][1] *= aux_upB[j];
              matrix[0][2] *= aux_upB[j+1];
              matrix[1][0] *= img->bP[i][j-1];
              matrix[1][2] *= img->bP[i][j+1];
              matrix[2][0] *= img->bP[i+1][j-1];
              matrix[2][1] *= img->bP[i+1][j];
              matrix[2][2] *= img->bP[i+1][j+1];
              sum = 0;
              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufB[i][j] = sum;
              }
          } else if(i == stop - start - 1) {
            makeMatrixSmooth(matrix);
            matrix[1][1] *= img->gP[i][j];
            matrix[0][0] *= img->gP[i-1][j-1];
            matrix[0][1] *= img->gP[i-1][j];
            matrix[0][2] *= img->gP[i-1][j+1];
            matrix[1][0] *= img->gP[i][j-1];
            matrix[1][2] *= img->gP[i][j+1];
            matrix[2][0] *= aux_botG[j-1];
            matrix[2][1] *= aux_botG[j];
            matrix[2][2] *= aux_botG[j+1];

            sum = 0;
            for(a = 0; a < 3; a++)
              for(b = 0; b < 3; b++)
                sum += matrix[a][b];

            bufG[i][j] = sum;

            if(img->type[1] == '6') {
              makeMatrixSmooth(matrix);
              matrix[1][1] *= img->rP[i][j];
              matrix[0][0] *= img->rP[i-1][j-1];
              matrix[0][1] *= img->rP[i-1][j];
              matrix[0][2] *= img->rP[i-1][j+1];
              matrix[1][0] *= img->rP[i][j-1];
              matrix[1][2] *= img->rP[i][j+1];
              matrix[2][0] *= aux_botR[j-1];
              matrix[2][1] *= aux_botR[j];
              matrix[2][2] *= aux_botR[j+1];

              sum = 0;
              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufR[i][j] = sum;

              makeMatrixSmooth(matrix);
              matrix[1][1] *= img->bP[i][j];
              matrix[0][0] *= img->bP[i-1][j-1];
              matrix[0][1] *= img->bP[i-1][j];
              matrix[0][2] *= img->bP[i-1][j+1];
              matrix[1][0] *= img->bP[i][j-1];
              matrix[1][2] *= img->bP[i][j+1];
              matrix[2][0] *= aux_botB[j-1];
              matrix[2][1] *= aux_botB[j];
              matrix[2][2] *= aux_botB[j+1];

              sum = 0;
              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufB[i][j] = sum;
            }
          } else {
              makeMatrixSmooth(matrix);
              matrix[1][1] *= img->gP[i][j];
              matrix[0][0] *= img->gP[i-1][j-1];
              matrix[0][1] *= img->gP[i-1][j];
              matrix[0][2] *= img->gP[i-1][j+1];
              matrix[1][0] *= img->gP[i][j-1];
              matrix[1][2] *= img->gP[i][j+1];
              matrix[2][0] *= img->gP[i+1][j-1];
              matrix[2][1] *= img->gP[i+1][j];
              matrix[2][2] *= img->gP[i+1][j+1];
              sum = 0;

              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufG[i][j] = sum;

            if(img->type[1] == '6') {
              makeMatrixSmooth(matrix);
              matrix[1][1] *= img->rP[i][j];
              matrix[0][0] *= img->rP[i-1][j-1];
              matrix[0][1] *= img->rP[i-1][j];
              matrix[0][2] *= img->rP[i-1][j+1];
              matrix[1][0] *= img->rP[i][j-1];
              matrix[1][2] *= img->rP[i][j+1];
              matrix[2][0] *= img->rP[i+1][j-1];
              matrix[2][1] *= img->rP[i+1][j];
              matrix[2][2] *= img->rP[i+1][j+1];
              sum = 0;

              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufR[i][j] = sum;

              makeMatrixSmooth(matrix);
              matrix[1][1] *= img->bP[i][j];
              matrix[0][0] *= img->bP[i-1][j-1];
              matrix[0][1] *= img->bP[i-1][j];
              matrix[0][2] *= img->bP[i-1][j+1];
              matrix[1][0] *= img->bP[i][j-1];
              matrix[1][2] *= img->bP[i][j+1];
              matrix[2][0] *= img->bP[i+1][j-1];
              matrix[2][1] *= img->bP[i+1][j];
              matrix[2][2] *= img->bP[i+1][j+1];
              sum = 0;

              for(a = 0; a < 3; a++)
                for(b = 0; b < 3; b++)
                  sum += matrix[a][b];

              bufB[i][j] = sum;
            }
          }
        }
    }
}

void blur(image *img, int rank, int nProc,
  unsigned char **bufR, unsigned char **bufG, unsigned char **bufB,
  unsigned char *aux_botR, unsigned char *aux_botG, unsigned char *aux_botB,
  unsigned char *aux_upR, unsigned char *aux_upG, unsigned char *aux_upB)
  {
    int start, stop;
    int i, j;
    float aux1 = (float) 1 / 16;
    float aux2 = (float) 2 / 16;
    float aux4 = (float) 4 / 16;
    start = rank * ceil(img->maxHeight/(double)nProc);
    stop = (rank+1) * ceil(img->maxHeight/(double)nProc) < img->maxHeight ?
      (rank+1) * ceil(img->maxHeight/(double)nProc) :
      img->maxHeight;

      for(i = 0; i < stop - start; i++) {
        if(rank == 0 && i == 0)
          continue;
        if(rank == nProc - 1 && i == stop - start - 1)
          continue;
        for(j = 1; j < img->maxWidth-1; j++) {
          if (i == 0) {

            bufG[i][j] = img->gP[i][j] * aux4 + aux_upG[j-1] * aux1 + aux_upG[j] * aux2 + aux_upG[j+1] * aux1 + img->gP[i][j-1] * aux2 + img->gP[i][j+1] * aux2 + img->gP[i+1][j-1] * aux1 + img->gP[i+1][j] * aux2 + img->gP[i+1][j+1] * aux1;

            if(img->type[1] == '6') {
              bufR[i][j] = img->rP[i][j] * aux4 + aux_upR[j-1] * aux1 + aux_upR[j] * aux2 + aux_upR[j+1] * aux1 + img->rP[i][j-1] * aux2 + img->rP[i][j+1] * aux2 + img->rP[i+1][j-1] * aux1 + img->rP[i+1][j] * aux2 + img->rP[i+1][j+1] * aux1;

              bufB[i][j] = img->bP[i][j] * aux4 + aux_upB[j-1] * aux1 + aux_upB[j] * aux2 + aux_upB[j+1] * aux1 + img->bP[i][j-1] * aux2 + img->bP[i][j+1] * aux2 + img->bP[i+1][j-1] * aux1 + img->bP[i+1][j] * aux2 + img->bP[i+1][j+1] * aux1;
            }
          } else if(i == stop - start - 1) {

            bufG[i][j] = img->gP[i][j] * aux4 + img->gP[i-1][j-1] * aux1 + img->gP[i-1][j] * aux2 + img->gP[i-1][j+1] * aux1 + img->gP[i][j-1] * aux2 + img->gP[i][j+1] * aux2 +  aux_botG[j-1] * aux1 + aux_botG[j] * aux2 + aux_botG[j+1] * aux1;

            if(img->type[1] == '6') {
              bufR[i][j] = img->rP[i][j] * aux4 + img->rP[i-1][j-1] * aux1 + img->rP[i-1][j] * aux2 + img->rP[i-1][j+1] * aux1 + img->rP[i][j-1] * aux2 + img->rP[i][j+1] * aux2 +  aux_botR[j-1] * aux1 + aux_botR[j] * aux2 + aux_botR[j+1] * aux1;

              bufB[i][j] = img->bP[i][j] * aux4 + img->bP[i-1][j-1] * aux1 + img->bP[i-1][j] * aux2 + img->bP[i-1][j+1] * aux1 + img->bP[i][j-1] * aux2 + img->bP[i][j+1] * aux2 +  aux_botB[j-1] * aux1 + aux_botB[j] * aux2 + aux_botB[j+1] * aux1;
            }
          } else {

            bufG[i][j] = img->gP[i][j] * aux4 + img->gP[i-1][j-1] * aux1 + img->gP[i-1][j] * aux2 + img->gP[i-1][j+1] * aux1 + img->gP[i][j-1] * aux2 + img->gP[i][j+1] * aux2 + img->gP[i+1][j-1] * aux1 + img->gP[i+1][j] * aux2 + img->gP[i+1][j+1] * aux1;

            if(img->type[1] == '6') {
              bufR[i][j] = img->rP[i][j] * aux4 + img->rP[i-1][j-1] * aux1 + img->rP[i-1][j] * aux2 + img->rP[i-1][j+1] * aux1 + img->rP[i][j-1] * aux2 + img->rP[i][j+1] * aux2 + img->rP[i+1][j-1] * aux1 + img->rP[i+1][j] * aux2 + img->rP[i+1][j+1] * aux1;

              bufB[i][j] = img->bP[i][j] * aux4 + img->bP[i-1][j-1] * aux1 + img->bP[i-1][j] * aux2 + img->bP[i-1][j+1] * aux1 + img->bP[i][j-1] * aux2 + img->bP[i][j+1] * aux2 + img->bP[i+1][j-1] * aux1 + img->bP[i+1][j] * aux2 + img->bP[i+1][j+1] * aux1;

            }
          }
        }
    }
}

void mean(image *img, int rank, int nProc,
  unsigned char **bufR, unsigned char **bufG, unsigned char **bufB,
  unsigned char *aux_botR, unsigned char *aux_botG, unsigned char *aux_botB,
  unsigned char *aux_upR, unsigned char *aux_upG, unsigned char *aux_upB)
  {
    int start, stop;
    int i, j;

    start = rank * ceil(img->maxHeight/(double)nProc);
    stop = (rank+1) * ceil(img->maxHeight/(double)nProc) < img->maxHeight ?
      (rank+1) * ceil(img->maxHeight/(double)nProc) :
      img->maxHeight;

      for(i = 0; i < stop - start; i++) {
        if(rank == 0 && i == 0)
          continue;
        if(rank == nProc - 1 && i == stop - start - 1)
          continue;
        for(j = 1; j < img->maxWidth-1; j++) {
          if (i == 0) {
            bufG[i][j] = 9 * img->gP[i][j] - aux_upG[j-1] - aux_upG[j] - aux_upG[j+1] - img->gP[i][j-1] - img->gP[i][j+1] - img->gP[i+1][j-1] - img->gP[i+1][j] - img->gP[i+1][j+1];
            if(img->type[1] == '6') {
              bufR[i][j] = 9 * img->rP[i][j] - aux_upR[j-1] - aux_upR[j] - aux_upR[j+1] - img->rP[i][j-1] - img->rP[i][j+1] - img->rP[i+1][j-1] - img->rP[i+1][j] - img->rP[i+1][j+1];
              bufB[i][j] = 9 * img->bP[i][j] - aux_upB[j-1] - aux_upB[j] - aux_upB[j+1] - img->bP[i][j-1] - img->bP[i][j+1] - img->bP[i+1][j-1] - img->bP[i+1][j] - img->bP[i+1][j+1];
            }
          } else if(i == stop - start - 1) {
            bufG[i][j] = 9 * img->gP[i][j] - img->gP[i-1][j-1] - img->gP[i-1][j] - img->gP[i-1][j+1] - img->gP[i][j-1] - img->gP[i][j+1] - aux_botG[j-1] - aux_botG[j] - aux_botG[j+1];
            if(img->type[1] == '6') {
              bufR[i][j] = 9 * img->rP[i][j] - img->rP[i-1][j-1] - img->rP[i-1][j] - img->rP[i-1][j+1] - img->rP[i][j-1] - img->rP[i][j+1] - aux_botR[j-1] - aux_botR[j] - aux_botR[j+1];
              bufB[i][j] = 9 * img->bP[i][j] - img->bP[i-1][j-1] - img->bP[i-1][j] - img->bP[i-1][j+1] - img->bP[i][j-1] - img->bP[i][j+1] - aux_botB[j-1] - aux_botB[j] - aux_botB[j+1];
            }
          } else {
              bufG[i][j] = 9 * img->gP[i][j] - img->gP[i-1][j-1] - img->gP[i-1][j] - img->gP[i-1][j+1] - img->gP[i][j-1] - img->gP[i][j+1] - img->gP[i+1][j-1] - img->gP[i+1][j] - img->gP[i+1][j+1];
              if(img->type[1] == '6') {
                bufR[i][j] = 9 * img->rP[i][j] - img->rP[i-1][j-1] - img->rP[i-1][j] - img->rP[i-1][j+1] - img->rP[i][j-1] - img->rP[i][j+1] - img->rP[i+1][j-1] - img->rP[i+1][j] - img->rP[i+1][j+1];
                bufB[i][j] = 9 * img->bP[i][j] - img->bP[i-1][j-1] - img->bP[i-1][j] - img->bP[i-1][j+1] - img->bP[i][j-1] - img->bP[i][j+1] - img->bP[i+1][j-1] - img->bP[i+1][j] - img->bP[i+1][j+1];
              }
          }
        }
    }
  }

void emboss(image *img, int rank, int nProc,
  unsigned char **bufR, unsigned char **bufG, unsigned char **bufB,
  unsigned char *aux_botR, unsigned char *aux_botG, unsigned char *aux_botB,
  unsigned char *aux_upR, unsigned char *aux_upG, unsigned char *aux_upB)
  {
    int start, stop;
    int i, j;

    start = rank * ceil(img->maxHeight/(double)nProc);
    stop = (rank+1) * ceil(img->maxHeight/(double)nProc) < img->maxHeight ?
      (rank+1) * ceil(img->maxHeight/(double)nProc) :
      img->maxHeight;

      for(i = 0; i < stop - start; i++) {
        if(rank == 0 && i == 0)
          continue;
        if(rank == nProc - 1 && i == stop - start - 1)
          continue;
        for(j = 1; j < img->maxWidth-1; j++) {
          if (i == 0) {
              bufG[i][j] = aux_upG[j] - img->gP[i+1][j];
              if(img->type[1] == '6') {
                bufR[i][j] = aux_upR[j] - img->rP[i+1][j];
                bufB[i][j] = aux_upB[j] - img->bP[i+1][j];
              }
          } else if(i == stop - start - 1) {
              bufG[i][j] = img->gP[i-1][j] - aux_botG[j];
              if(img->type[1] == '6') {
                bufR[i][j] = img->rP[i-1][j] - aux_botR[j];
                bufB[i][j] = img->bP[i-1][j] - aux_botB[j];
              }
          } else {
              bufG[i][j] = img->gP[i-1][j] - img->gP[i+1][j];
              if(img->type[1] == '6') {
                bufR[i][j] = img->rP[i-1][j] - img->rP[i+1][j];
                bufB[i][j] = img->bP[i-1][j] - img->bP[i+1][j];
              }
          }
        }
    }
  }

void applyFilter(image *img, int rank, int nProc, char *op) {
  unsigned char** bufR;
  unsigned char** bufG;
  unsigned char** bufB;
  unsigned char *aux_botR;
  unsigned char *aux_botG;
  unsigned char *aux_botB;
  unsigned char *aux_upR;
  unsigned char *aux_upG;
  unsigned char *aux_upB;
  int start, stop;
  int i, j;

  /*  Fiecare proces isi gaseste intervalul pe care il modifica  */
  start = rank * ceil(img->maxHeight/(double)nProc);
  stop = (rank+1) * ceil(img->maxHeight/(double)nProc) < img->maxHeight ?
    (rank+1) * ceil(img->maxHeight/(double)nProc) :
    img->maxHeight;

  /*  Aloca dimensiuni pentru buffere suplimentare  */
  if(img->type[1] == '5') {
    bufG = (unsigned char**) malloc (img->maxHeight * sizeof(unsigned char*));
    for(i = 0; i < stop - start; i++) {
      bufG[i] = (unsigned char*) malloc (img->maxWidth * sizeof(unsigned char));
    }
  } else {
    bufR = (unsigned char**) malloc (img->maxHeight * sizeof(unsigned char*));
    bufG = (unsigned char**) malloc (img->maxHeight * sizeof(unsigned char*));
    bufB = (unsigned char**) malloc (img->maxHeight * sizeof(unsigned char*));
    for(i = 0; i < stop - start; i++) {
      bufR[i] = (unsigned char*) malloc (img->maxWidth * sizeof(unsigned char));
      bufG[i] = (unsigned char*) malloc (img->maxWidth * sizeof(unsigned char));
      bufB[i] = (unsigned char*) malloc (img->maxWidth * sizeof(unsigned char));
    }
  }

  /*
   *  Procesele primesc si trimit prima/ultima/ambele linii pentru cele
   *  superioare/inferioare pe cazul grayscale
   */
  if(rank == 0 && img->type[1] == '5' && nProc != 1) {
    aux_botG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Send(img->gP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botG, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  } else if(rank == nProc - 1 && img->type[1] == '5' && nProc != 1) {
    aux_upG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upG, img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->gP[0], img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD);
  } else if(img->type[1] == '5' && nProc != 1) {
    aux_botG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    aux_upG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upG, img->maxWidth, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->gP[0], img->maxWidth, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD);
    MPI_Send(img->gP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botG, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  /*
   *  Procesele primesc si trimit prima/ultima/ambele linii pentru cele
   *  superioare/inferioare pe cazul color
   */
  if(rank == 0 && img->type[1] == '6' && nProc != 1) {
    aux_botR = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Send(img->rP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botR, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    aux_botG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Send(img->gP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botG, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    aux_botB = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Send(img->bP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botB, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  } else if(rank == nProc - 1 && img->type[1] == '6' && nProc != 1) {
    aux_upR = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upR, img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->rP[0], img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD);
    aux_upG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upG, img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->gP[0], img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD);
    aux_upB = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upB, img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->bP[0], img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD);
  } else if(img->type[1] == '6' && nProc != 1) {
    aux_botR = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    aux_upR = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upR, img->maxWidth, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->rP[0], img->maxWidth, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD);
    MPI_Send(img->rP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botR, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    aux_botG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    aux_upG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upG, img->maxWidth, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->gP[0], img->maxWidth, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD);
    MPI_Send(img->gP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botG, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    aux_botB = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    aux_upB = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upB, img->maxWidth, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->bP[0], img->maxWidth, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD);
    MPI_Send(img->bP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botB, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  /*  pentru fiecare mesaj aplica filtrul  */
  if(strcmp(op, "sharpen") == 0)
    sharpen(img, rank, nProc, bufR, bufG, bufB, aux_botR, aux_botG, aux_botB,
    aux_upR, aux_upG, aux_upB);
  if(strcmp(op, "emboss") == 0)
    emboss(img, rank, nProc, bufR, bufG, bufB, aux_botR, aux_botG, aux_botB,
    aux_upR, aux_upG, aux_upB);
  if(strcmp(op, "mean") == 0)
    mean(img, rank, nProc, bufR, bufG, bufB, aux_botR, aux_botG, aux_botB,
    aux_upR, aux_upG, aux_upB);
  if(strcmp(op, "blur") == 0)
    blur(img, rank, nProc, bufR, bufG, bufB, aux_botR, aux_botG, aux_botB,
    aux_upR, aux_upG, aux_upB);
  if(strcmp(op, "smooth") == 0)
    smooth(img, rank, nProc, bufR, bufG, bufB, aux_botR, aux_botG, aux_botB,
    aux_upR, aux_upG, aux_upB);

  /* `` Salveaza modificarile in imagine  */
  for(i = 0; i < stop-start; i++) {
    if(rank == 0 && i == 0)
      continue;
    if(rank == nProc - 1 && i == stop - start - 1)
      continue;
    for(j = 1; j < img->maxWidth - 1; j++) {
      img->gP[i][j] = bufG[i][j];
      if(img->type[1] == '6') {
        img->rP[i][j] = bufR[i][j];
        img->bP[i][j] = bufB[i][j];
      }
    }
  }

}

image readInput(char *name) {
    FILE* in = fopen(name, "rb");
    int i, j;
    image img;
    unsigned char *buf;

    fscanf(in, "%s%d%d%d", img.type, &img.maxWidth, &img.maxHeight, &img.maxVal);
    fseek(in, 1, SEEK_CUR);

    /*  rPayscale  */
    if(img.type[1] == '5') {
        img.gP = (unsigned char**) malloc (img.maxHeight  * sizeof(unsigned char*));
        for(i = 0; i < img.maxHeight; i++) {
            img.gP[i] = (unsigned char*) malloc (img.maxWidth * sizeof(unsigned char));
            fread(img.gP[i], sizeof(unsigned char), img.maxWidth, in);
    }
    /*  Color  */
    } else {
        buf = (unsigned char*) malloc (img.maxWidth * 3 * sizeof(unsigned char));

        img.rP = (unsigned char**) malloc (img.maxHeight  * sizeof(unsigned char*));
        img.gP = (unsigned char**) malloc (img.maxHeight  * sizeof(unsigned char*));
        img.bP = (unsigned char**) malloc (img.maxHeight  * sizeof(unsigned char*));
        for(i = 0; i < img.maxHeight; i++) {
            img.rP[i] = (unsigned char*) malloc (img.maxWidth * sizeof(unsigned char));
            img.gP[i] = (unsigned char*) malloc (img.maxWidth * sizeof(unsigned char));
            img.bP[i] = (unsigned char*) malloc (img.maxWidth * sizeof(unsigned char));
            fread(buf, sizeof(unsigned char), img.maxWidth * 3, in);
            for(j = 0; j < img.maxWidth; j++) {
                img.rP[i][j] = buf[j*3];
                img.gP[i][j] = buf[j*3 + 1];
                img.bP[i][j] = buf[j*3 + 2];
            }
        }
    }
    fclose(in);
    return img;
}

void writeOutput(const char * fileName, image img) {
    FILE *out = fopen(fileName, "wb");
    int i, j;
    unsigned char *buf;

    fprintf(out, "%s\n%d %d\n%d\n", img.type, img.maxWidth, img.maxHeight, img.maxVal);
    /*  Cazul in care este rPayscale  */
    if(img.type[1] == '5') {
        for(i = 0; i < img.maxHeight; i++) {
            fwrite(img.gP[i], sizeof(unsigned char), img.maxWidth, out);
        }
    /*  Cazul in care este color  */
    } else {
        buf = (unsigned char*) malloc (img.maxWidth  * 3 * sizeof(unsigned char));
        for(i = 0; i < img.maxHeight; i++) {
            for(j = 0; j < img.maxWidth; j++) {
                buf[j*3] = img.rP[i][j];
                buf[j*3 + 1] = img.gP[i][j];
                buf[j*3 + 2] = img.bP[i][j];
        }
        fwrite(buf, sizeof(unsigned char), img.maxWidth * 3, out);
        }
    }
    fclose(out);
}

int main(int argc, char **argv) {
    int rank;
    int nProc;
    image img;
    int buf[4];
    int start, stop;
    int i, j;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    /*  Citeste poza si realizeaza un mesaj pentru broadcast  */
    if(rank == 0) {
        img = readInput(argv[1]);
        buf[0] = img.maxWidth;
        buf[1] = img.maxHeight;
        buf[2] = img.maxVal;
        buf[3] = img.type[1] - '0';
    }

    /*  Face broadcast pentru dimensiunile matricei  */
    MPI_Bcast(&buf, 4, MPI_INT, 0, MPI_COMM_WORLD);
    img.maxWidth = buf[0];
    img.maxHeight = buf[1];
    img.maxVal = buf[2];
    sprintf(img.type, "P%d", buf[3]);

    /*  Daca imaginea e grayscale trimite elementele pentru fiecare proces  */
    if(rank == 0 && img.type[1] == '5') {
        for(i = 1; i < nProc; i++) {
          start = i * ceil(img.maxHeight/(double)nProc);
          stop = (i+1) * ceil(img.maxHeight/(double)nProc) < img.maxHeight ?
            (i+1) * ceil(img.maxHeight/(double)nProc) :
            img.maxHeight;
          for(j = start; j < stop; j++) {
            MPI_Send(img.gP[j], img.maxWidth, MPI_CHAR, i, 0, MPI_COMM_WORLD);
          }
        }
    }

    /*  Daca imaginea e color trimite 3*elemente pentru fiecare proces  */
    if (rank == 0 && img.type[1] == '6') {
      for(i = 1; i < nProc; i++) {
        start = i * ceil(img.maxHeight/(double)nProc);
        stop = (i+1) * ceil(img.maxHeight/(double)nProc) < img.maxHeight ?
          (i+1) * ceil(img.maxHeight/(double)nProc) :
          img.maxHeight;
        for(j = start; j < stop; j++) {
          MPI_Send(img.rP[j], img.maxWidth, MPI_CHAR, i, 0, MPI_COMM_WORLD);
          MPI_Send(img.gP[j], img.maxWidth, MPI_CHAR, i, 0, MPI_COMM_WORLD);
          MPI_Send(img.bP[j], img.maxWidth, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
      }
    }

    /*  Daca imaginea e grayscale fiecare proces primeste elementele  */
    if(rank != 0 && img.type[1] == '5') {
      start = rank * ceil(img.maxHeight/(double)nProc);
      stop = (rank+1) * ceil(img.maxHeight/(double)nProc) < img.maxHeight ?
        (rank+1) * ceil(img.maxHeight/(double)nProc) :
        img.maxHeight;
        img.gP = (unsigned char**)malloc((stop-start) * sizeof(unsigned char*));
        for(i = 0; i < stop-start; i++) {
          img.gP[i] = (unsigned char*)malloc(img.maxWidth * sizeof(unsigned char));
          MPI_Recv(img.gP[i], img.maxWidth, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    /*  Daca imaginea e color fiecare proces primeste 3*elemente  */
    if (rank != 0 && img.type[1] == '6') {
      start = rank * ceil(img.maxHeight/(double)nProc);
      stop = (rank+1) * ceil(img.maxHeight/(double)nProc) < img.maxHeight ?
        (rank+1) * ceil(img.maxHeight/(double)nProc) :
        img.maxHeight;
        img.rP = (unsigned char**)malloc((stop-start+1) * sizeof(unsigned char*));
        img.gP = (unsigned char**)malloc((stop-start+1) * sizeof(unsigned char*));
        img.bP = (unsigned char**)malloc((stop-start+1) * sizeof(unsigned char*));
        for(i = 0; i < stop-start; i++) {
          img.rP[i] = (unsigned char*)malloc(img.maxWidth * sizeof(unsigned char));
          img.gP[i] = (unsigned char*)malloc(img.maxWidth * sizeof(unsigned char));
          img.bP[i] = (unsigned char*)malloc(img.maxWidth * sizeof(unsigned char));
          MPI_Recv(img.rP[i], img.maxWidth, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          MPI_Recv(img.gP[i], img.maxWidth, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          MPI_Recv(img.bP[i], img.maxWidth, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    /*  Pentru fiecare argument suplimentar se face filtrul  */
    for(i = 3; i < argc; i++)
      applyFilter(&img, rank, nProc, argv[i]);

    /*  Rank-ul 0 primeste pixelii modificati dupa toate filtrele  */
    if(rank == 0) {
        for(i = 1; i < nProc; i++) {
          start = i * ceil(img.maxHeight/(double)nProc);
          stop = (i+1) * ceil(img.maxHeight/(double)nProc) < img.maxHeight ?
            (i+1) * ceil(img.maxHeight/(double)nProc) :
            img.maxHeight;
          for(j = start; j < stop; j++) {
            MPI_Recv(img.gP[j], img.maxWidth, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(img.type[1] == '6') {
              MPI_Recv(img.rP[j], img.maxWidth, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
              MPI_Recv(img.bP[j], img.maxWidth, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
          }
        }
        writeOutput(argv[2], img);
    }

    /*  Procesele trimit catre cel cu rank-ul 0 pixelii modificati  */
    if(rank != 0) {
      start = rank * ceil(img.maxHeight/(double)nProc);
      stop = (rank+1) * ceil(img.maxHeight/(double)nProc) < img.maxHeight ?
        (rank+1) * ceil(img.maxHeight/(double)nProc) :
        img.maxHeight;
      for(i = 0; i < stop - start; i++) {
        MPI_Send(img.gP[i], img.maxWidth, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        if(img.type[1] == '6') {
          MPI_Send(img.rP[i], img.maxWidth, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
          MPI_Send(img.bP[i], img.maxWidth, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
      }
    }

    MPI_Finalize();

    return 0;
}

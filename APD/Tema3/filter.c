

void makeMatrixSmooth(float matrix[3][3]) {
  int i, j;
  float aux = (float) 1 / 9;
  for(i = 0; i < 3; i++)
    for(j = 0; j < 3; j++)
      matrix[i][j] = aux;
}

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

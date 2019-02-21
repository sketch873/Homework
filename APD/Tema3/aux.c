
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

  start = rank * ceil(img->maxHeight/(double)nProc);
  stop = (rank+1) * ceil(img->maxHeight/(double)nProc) < img->maxHeight ?
    (rank+1) * ceil(img->maxHeight/(double)nProc) :
    img->maxHeight;

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

  /*  Gestionare pentru rPayscale  */
  if(rank == 0 && img->type[1] == '5') {
    aux_botG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Send(img->gP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botG, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  } else if(rank == nProc - 1 && img->type[1] == '5') {
    aux_upG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upG, img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->gP[0], img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD);
  } else if(img->type[1] == '5') {
    aux_botG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    aux_upG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upG, img->maxWidth, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->gP[0], img->maxWidth, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD);
    MPI_Send(img->gP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botG, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  /*  Gestionare pentru Color  */
  if(rank == 0 && img->type[1] == '6') {
    aux_botR = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Send(img->rP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botR, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    aux_botG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Send(img->gP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botG, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    aux_botB = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Send(img->bP[stop - start - 1], img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    MPI_Recv(aux_botB, img->maxWidth, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  } else if(rank == nProc - 1 && img->type[1] == '6') {
    aux_upR = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upR, img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->rP[0], img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD);
    aux_upG = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upG, img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->gP[0], img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD);
    aux_upB = (unsigned char*)malloc(img->maxWidth * sizeof(unsigned char));
    MPI_Recv(aux_upB, img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(img->bP[0], img->maxWidth, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD);
  } else if(img->type[1] == '6') {
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

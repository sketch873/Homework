#include "homework.h"

int num_threads;
int resize_factor;
int nr_elems;
int gauss_kernel[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
image *output, *input;

/*  Citire  */
void readInput(const char * fileName, image *img) {
  int i, j;
  FILE *input = fopen(fileName, "rb");
  unsigned char *buf;

  /* Citire header */
  fscanf(input, "%s%d%d%d", img->type, &img->maxWidth, &img->maxHeight, &img->maxVal);
  fseek(input, 1, SEEK_CUR);


  /*  Citire pixeli  */
  /*  Cazul in care este grayscale  */
  if(img->type[1] == '5') {
    img->gP = (unsigned char**) malloc (img->maxHeight  * sizeof(unsigned char*));
    for(i = 0; i < img->maxHeight; i++) {
      img->gP[i] = (unsigned char*) malloc (img->maxWidth * sizeof(unsigned char));
      fread(img->gP[i], sizeof(unsigned char), img->maxWidth, input);
    }
  /*  Cazul in care este color  */
  } else {
    buf = (unsigned char*) malloc (img->maxWidth * 3 * sizeof(unsigned char));

    img->rP = (unsigned char**) malloc (img->maxHeight  * sizeof(unsigned char*));
    img->gP = (unsigned char**) malloc (img->maxHeight  * sizeof(unsigned char*));
    img->bP = (unsigned char**) malloc (img->maxHeight  * sizeof(unsigned char*));
    for(i = 0; i < img->maxHeight; i++) {
      img->rP[i] = (unsigned char*) malloc (img->maxWidth * sizeof(unsigned char));
      img->gP[i] = (unsigned char*) malloc (img->maxWidth * sizeof(unsigned char));
      img->bP[i] = (unsigned char*) malloc (img->maxWidth * sizeof(unsigned char));
      fread(buf, sizeof(unsigned char), img->maxWidth * 3, input);
      for(j = 0; j < img->maxWidth; j++) {
        img->rP[i][j] = buf[j*3];
        img->gP[i][j] = buf[j*3 + 1];
        img->bP[i][j] = buf[j*3 + 2];
      }
    }
  }

  fclose(input);
}

/*  Scriere date  */
void writeData(const char * fileName, image *img) {
  FILE *output = fopen(fileName, "wb");
  int i, j;
  unsigned char *buf;

  fprintf(output, "%s\n%d %d\n%d\n", img->type, img->maxWidth, img->maxHeight, img->maxVal);
  /*  Cazul in care este grayscale  */
  if(img->type[1] == '5') {
    for(i = 0; i < img->maxHeight; i++) {
      fwrite(img->gP[i], sizeof(unsigned char), img->maxWidth, output);
    }
  /*  Cazul in care este color  */
  } else {
    buf = (unsigned char*) malloc (img->maxWidth  * 3 * sizeof(unsigned char));
    for(i = 0; i < img->maxHeight; i++) {
      for(j = 0; j < img->maxWidth; j++) {
        buf[j*3] = img->rP[i][j];
        buf[j*3 + 1] = img->gP[i][j];
        buf[j*3 + 2] = img->bP[i][j];
      }
      fwrite(buf, sizeof(unsigned char), img->maxWidth * 3, output);
    }
  }

  fclose(output);
}

void resize(image *in, image *out) {
  int i;
  pthread_t tid[num_threads];
  int thread_id[num_threads];

  for(i = 0; i < num_threads; i++) {
    thread_id[i] = i;
  }
  /*  Salvez headerul  */
  out->maxWidth = in->maxWidth / resize_factor;
  out->maxHeight = in->maxHeight / resize_factor;
  out->maxVal = in->maxVal;
  strcpy(out->type, in->type);

  nr_elems = resize_factor * resize_factor;

  /* Grayscale */
  if(out->type[1] == '5') {
    out->gP = (unsigned char**) malloc (out->maxHeight * sizeof(unsigned char*));
    for(i = 0; i < out->maxHeight; i++) {
      out->gP[i] = (unsigned char*) malloc (out->maxWidth * sizeof(unsigned char));
    }
    output = out;
    input = in;
    for(i = 0; i < num_threads; i++) {
  		pthread_create(&(tid[i]), NULL, resizeGrayscale, &(thread_id[i]));
  	}
  /* Color */
  } else {
    out->rP = (unsigned char**) malloc (out->maxHeight * sizeof(unsigned char*));
    out->gP = (unsigned char**) malloc (out->maxHeight * sizeof(unsigned char*));
    out->bP = (unsigned char**) malloc (out->maxHeight * sizeof(unsigned char*));
    for(i = 0; i < out->maxHeight; i++) {
      out->rP[i] = (unsigned char*) malloc (out->maxWidth * sizeof(unsigned char));
      out->gP[i] = (unsigned char*) malloc (out->maxWidth * sizeof(unsigned char));
      out->bP[i] = (unsigned char*) malloc (out->maxWidth * sizeof(unsigned char));
    }
    output = out;
    input = in;
    for(i = 0; i < num_threads; i++) {
  		pthread_create(&(tid[i]), NULL, resizeColor, &(thread_id[i]));
  	}
  }

  for(i = 0; i < num_threads; i++) {
    pthread_join(tid[i], NULL);
  }

}
/*  Functie de thread pentru color  */
void* resizeColor(void *var) {
  int i, j, k, l;
  int sum1, sum2, sum3;
  int start, stop;

  int Tid = *(int*) var;
  start = Tid * ceil(output->maxHeight/(double)num_threads);
	stop = (Tid+1) * ceil(output->maxHeight/(double)num_threads) < output->maxHeight ?
            (Tid+1) * ceil(output->maxHeight/(double)num_threads) :
            output->maxHeight;

  /*  resize_factor par  */
  if(resize_factor != 3) {
    for(i = start; i < stop; i++) {
      for(j = 0 ; j < output->maxWidth; j++) {
        sum1 = 0;
        sum2 = 0;
        sum3 = 0;

        for(k = i * resize_factor; k < (i+1) * resize_factor; k++) {
          for(l = j * resize_factor; l < (j+1) * resize_factor; l++) {
            sum1 += input->rP[k][l];
            sum2 += input->gP[k][l];
            sum3 += input->bP[k][l];
          }
        }

        output->rP[i][j] = sum1 / nr_elems;
        output->gP[i][j] = sum2 / nr_elems;
        output->bP[i][j] = sum3 / nr_elems;

      }
    }
  /*  resize_factor impar  */
  } else {
    for(i = start; i < stop; i++) {
      for(j = 0 ; j < output->maxWidth; j++) {
        sum1 = 0;
        sum2 = 0;
        sum3 = 0;

        for(k = i * resize_factor; k < (i+1) * resize_factor; k++) {
          for(l = j * resize_factor; l < (j+1) * resize_factor; l++) {
            sum1 += input->rP[k][l] * gauss_kernel[k%3][l%3];
            sum2 += input->gP[k][l] * gauss_kernel[k%3][l%3];
            sum3 += input->bP[k][l] * gauss_kernel[k%3][l%3];
          }
        }

        output->rP[i][j] = sum1 / 16;
        output->gP[i][j] = sum2 / 16;
        output->bP[i][j] = sum3 / 16;

      }
    }
  }

  return var;
}
/*  Functie de thread pentru grayscale  */
void* resizeGrayscale(void *var) {
  int i, j, k, l;
  int sum, start, stop;

  int Tid = *(int*) var;
  start = Tid * ceil(output->maxHeight/(double)num_threads);
	stop = (Tid+1) * ceil(output->maxHeight/(double)num_threads) < output->maxHeight ?
            (Tid+1) * ceil(output->maxHeight/(double)num_threads) :
            output->maxHeight;

  /*  resize_factor par  */
  if(resize_factor != 3) {
    for(i = start; i < stop; i++) {
      for(j = 0 ; j < output->maxWidth; j++) {
        sum = 0;

        for(k = i * resize_factor; k < (i+1) * resize_factor; k++) {
          for(l = j * resize_factor; l < (j+1) * resize_factor; l++) {
            sum += input->gP[k][l];
          }
        }

        output->gP[i][j] = sum / nr_elems;

      }
    }
  /*  resize_factor impar  */
  } else {
    for(i = start; i < stop; i++) {
      for(j = 0 ; j < output->maxWidth; j++) {
        sum = 0;

        for(k = i * resize_factor; k < (i+1) * resize_factor; k++) {
          for(l = j * resize_factor; l < (j+1) * resize_factor; l++) {
            sum += input->gP[k][l] * gauss_kernel[k%3][l%3];
          }
        }

        output->gP[i][j] = sum / 16;

      }
    }
  }

  return var;
}

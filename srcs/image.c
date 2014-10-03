#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "image.h"



image_t *creer_image() {
  image_t *img = calloc(1, sizeof(image_t));
  if(img == NULL) {
    perror("créer_image");
  }
  return img;
}

image_t *copier_image(image_t *src) {
  image_t *dst = creer_image();
  dst->w = src->w;
  dst->h = src->h;
  if(src->path) {
    dst->path = strdup(src->path);
  }
  if(dst->path == NULL) {
    perror("copier_image");
  }
  if(src->buff) {
    dst->buff = malloc(sizeof(char) * src->w * src->h);
  }
  if(dst->buff) {
    memcpy(dst->buff, src->buff, sizeof(char) * src->w * src->h);
  } else {
    perror("copier-image");
  }
  return dst;
}

void detruire_image(image_t *p) {
  if(p) {
    if(p->path){
      free(p->path);
    }
    if(p->buff){
      free(p->buff);
    }
    free(p);
  }
}


// Ici commence le test <-
int main() {
  image_t *test = creer_image();
  image_t *test2 = copier_image();
  printf("%d %d \n%d %d", test->w, test->h, test2->w, test2->h);
  detruire_image(test2);
  detruire_image(test);
}


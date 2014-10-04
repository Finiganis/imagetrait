#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

// #define VAL(img,i,j) (img)->buff[(i)*(img)->w+(j)]


typedef struct image_ {
  uint32_t w; // largeur en pixels
  uint32_t h; // hauteur en pixels
  char *path; // le chemin absolu du fichier correspondant
  uint8_t *buff; // w * x * h octets correspondant aux pixels
} image_t;

image_t *creer_image(char *path);
image_t *copier_image(image_t *src);
image_t *creer_image_wh(char *path, uint32_t w, uint32_t h);
uint8_t val_image(image_t *image, uint32_t i, uint32_t j);
void detruire_image(image_t *p);

#endif /* IMAGE_H */

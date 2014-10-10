#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

// #define VAL(img,i,j) (img)->buff[(i)*(img)->w+(j)]

typedef struct image_ {
  size_t w; // width in pixels.
  size_t h; // height en pixels.
  uint8_t maxval; // grey nuances
  uint8_t bbp;
  char *path; // path to the file.
  uint8_t *buff; // w * x * h byte per pixels.
} image_t;

image_t *creer_image(const char *path);
image_t *copier_image(image_t *src);
image_t *creer_image_wh(const char *path, uint32_t w, uint32_t h);
uint8_t val_image(image_t *image, size_t i, size_t j);
void detruire_image(image_t *p);

#endif /* IMAGE_H */

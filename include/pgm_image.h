#ifndef PGM_IMAGE_H
#define PGM_IMAGE_H

#include "image.h"

// according to left to the clockwise rule:
// this a pointer to a function who take (char*, size_t, size_t)
// And return a char pointer.
typedef uint8_t *(*read_funct_t)(FILE*, image_t*);

typedef enum format {
  BIN = 5,
  ASCII = 2
} pgm_form;


image_t *charger_image_pgm(const char *nom_fichier);
int sauver_image_pgm(const char *nom_fichier, image_t *img);

read_funct_t read_strategy(char *buffer);
uint8_t *read_bin(FILE *file, image_t *img);
uint8_t *read_ascii(FILE *file, image_t *img);

#endif /* PGM_IMAGE */

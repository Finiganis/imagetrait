#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "image.h"
#include "pgm_image.h"
#include "my_string.h"

#define TMP_STR_SIZE 256

read_funct_t read_strategy(char *buffer) {
  if (my_strnequ(buffer, "P5", 2)) {
    return (read_bin);
  } else if (my_strnequ(buffer, "P2", 2)) {
    return (read_ascii);
  } else {
    return (NULL);
  }
}

uint8_t *read_bin(FILE *file, image_t *img) {
  //uint8_t *buff = malloc(sizeof(char) * img->w * img->h);
  fprintf(stderr, "Not yet implemented.\n");
  (void) file;
  (void) img;
  return NULL;
}


// Think about making a custom reader for this purpose
// with fread
//fgets(buffer, TMP_STR_SIZE, file);
//printf("|%s|\n", buffer);
//sscanf(buffer, "%hhu", &row[cur]);
uint8_t *read_ascii(FILE *file, image_t *img) {
  uint8_t *buffer = malloc(sizeof(char) * img->w * img->h);
  size_t max = 0;

  for (size_t y = 0, x = 0; y < img->h; y++, x = 0) {
    for (; x < img->w; x++) {
      const size_t cur = x + y * img->w;
      fscanf(file, "%hhu", &buffer[cur]);
      if (buffer[cur] > img->maxval) {
        fprintf(stderr,
            "read_ascii: Value out of bound (%hhu > %hhu).\n",
            buffer[cur], img->maxval);
        free(buffer);
        return NULL;
      }
    }
    max += x;
  }
  if (max != img->h * img->w) {
    fprintf(stderr,
        "read_ascii: Incoerrent image size "
        "(observed: %zu, expected: %zu).\n", max, img->h * img->w);
    free(buffer);
    return NULL;
  }
  return buffer;
}

image_t *charger_image_pgm(const char *nom_fichier) {
  FILE *file = fopen(nom_fichier, "r");
  image_t *img = NULL;

  if (file) {
    char buffer[TMP_STR_SIZE];

    fgets(buffer, TMP_STR_SIZE, file);
    read_funct_t fun_read = read_strategy(buffer);
    if (fun_read) {
      img = creer_image_path(nom_fichier);
      while (fgets(buffer, TMP_STR_SIZE, file) &&
          (sscanf(buffer, "%zu %zu", &img->w, &img->h) == 0)) {}
      while (fgets(buffer, TMP_STR_SIZE, file) &&
          (sscanf(buffer, "%hhu", &img->maxval) == 0)) {}
      img->buff = fun_read(file, img);
    } else {
      perror("charge_image_pgm: Error invalid pgm file.");
    }
    fclose(file);
  } else {
    perror("charge_image_pgm: Error invalid filename or file.");
  }
  return img;
}

// 0 -> Error | 1 -> OK
int sauver_image_pgm(const char *nom_fichier, image_t *img) {
  FILE* file = fopen(nom_fichier, "w");

  if (file) {
    fprintf(file, "%c%d\n%zu %zu\n%d\n",
        'P', ASCII, img->w, img->h, img->maxval);
    for (size_t y = 0, x = 0; y < img->h; y++, x = 0) {
      for (; x < img->w; x++) {
        const size_t cur = x + y * img->w;
        fprintf(file, "%hhu\n", img->buff[cur]);
      }
    }
    fclose(file);
  } else {
    perror("sauver_image_pgm: Error impossible to create file.");
    return 0;
  }
  return 1;
}

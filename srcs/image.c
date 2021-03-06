#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include "image.h"
#include "my_string.h"

inline
uint8_t val_pixel(const image_t *image, const size_t x, const size_t y) {
  return (image->buff[x + image->w * y]);
}

image_t *copier_image_sup(image_t *src) {
  image_t *dst = NULL;
  if (src) {
    dst = creer_image_wh(src->path, src->w, src->h);
    dst->buff = malloc(sizeof(uint8_t) * dst->w * dst->h);
    dst->maxval = src->maxval;
    if (!dst->buff) {
      perror("copier_image: ");
    }
  } else {
    fprintf(stderr, "copier_image_sup: Invalid source image is NULL.\n");
  }
  return dst;
}

image_t *creer_image(void) {
  image_t *img = NULL;

  img = malloc(sizeof(image_t));
  if (img == NULL) {
    perror("creer_image: Fail on allocating an image.");
  }
  return (img);
}

image_t *creer_image_path(const char *path) {
  image_t *img = NULL;
  if (path) {
    img = creer_image();
    if (img) {
      img->path = my_strdup(path);
    } else {
      perror("creer_image_path: ");
    }
  } else {
    perror("creer_image: Error fail on malloc or incorrect path.");
  }
  return (img);
}

image_t *creer_image_wh(const char *path, uint32_t w, uint32_t h) {
  image_t *image = creer_image_path(path);
  image->w = w;
  image->h = h;

  return (image);
}

image_t *copier_image(image_t *src) {
  image_t *dst = NULL;
  if(src->path) {
    dst = creer_image_wh(src->path, src->w, src->h);
    if(src->buff) {
      dst->buff = malloc(sizeof(char) * src->w * src->h);
      if (dst->buff) {
        memcpy(dst->buff,
            src->buff,
            sizeof(char) * src->w * src->h);
      } else {
        perror("copier-image: Error malloc failled "
            "to allocate the destination buffer.");
      }
    } else {
      perror("copier_image: Error there is no source buffer");
    }
  } else {
    perror("copier_image: Error path to image source is invalid.");
  }
  return (dst);
}

void detruire_image(image_t *p) {
  if(p) {
    if(p->path) {
      free(p->path);
    }
    if(p->buff) {
      free(p->buff);
    }
    free(p);
  }
}


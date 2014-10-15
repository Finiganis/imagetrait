#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "noyau.h"



image_t *copier_image2(image_t *src) {
  image_t *dst = creer_image();
  dst->w = src->w;
  dst->h = src->h;
  if(src->path) {
    dst->path = my_strdup(src->path);
  }
  if(dst->path == NULL) {
    perror("copier_image2");
  }
  dst->buffer = malloc(sizeof(char) * src->w * src->h);
}

image_t *negatif(image_t *src) {
  image_t *dst = copier_image2(src);
  for (int i = 0; i < dst->h * dst->w; i++) {
    dst->buff[i] = 255 - src->buff[i];
  }
  return dst;
}

static
void rotation90_aux(image_t *src, image_t *dst,int angle) {
  if (angle == 0) {
    return dst;
  }
  for (int y = 0; y < src->h; y++) {
    for (int x = 0; x < src->w; x++) {
      dst->buff[x + src->w * y] = src->buff[(src->h - y) + (src->w * x)];
    }
  }
  rotation(dst, angle - 90);
  detruire_image(dst);
}

image_t *rotation90(image_t *src, int angle) {
  image_t *dst = copier_image2(src);
  rotation90_aux(src, dst, angle);
  return (dst);
}

void swap_pixel(uint8_t *src_pix, uint8_t *dst_pix) {
  const uint8_t tmp_pix = *src_pix;
  *src_pix = dst_pix;
  *dst_pix = tmp_pix;
}

image_t *rotation(image_t *src, int angle) {
  if (angle % 90 != 0) {
    image_t *dst = copier_image2(src);
    return dst;
  }
  image_t *dst = NULL;
  dst = rotation90(src, angle);
  return dst;
}

image_t *modifier_lumin(image_t *src, int pourcent) {
  image_t *dst = copier_image2(src);
  for (int i = 0; i < dst->h * dst->w; i++) {
    dst->buff[i] = (src->buff[i] > 255) ? 255 : (src->buff[i] * pourcent) / 100;
  }
  return dst;
}

image_t *bruiter_image(image_t *src, int pourcent) {
  image_t *dst = copier_image2(src);
  for (int x = 0; x < src->w; x++) {
    for (int y = 0; y < src->h; y++) {
      if ((rand() % 100) < pourcent){
        dst->buff[x + src->w * y] = rand() % 256;
      } else {
        dst->buff[x + src->w * y] = src->buff[x + src->w * y];
      }
    }
  }
  return dst;
}

image_t *filtrer_median(image_t *src) {
  image_t *dst = copier_image2(src);
  for (int x = 0; x < src->w; x++) {
    for (int y = 0; y < src->h; y++) {
      filtrage_median(src, dest, x, y);
    }
  }
  return dst;
}

int somme(noyau *pn) {
  int res = 0;

  for (int x = 0, x < pn->dim * dim, i++) {
    res += pn->coeff[x];
  }
  return res;
}

int sortie(int a, int max) {
  if (a < 0) {
    return 0;
  } else if (a > max) {
    return max;
  } else {
    return a;
  }
}

void convolution(image_t *src, image_t *dst, noyau *pn, int x, int y) {
  int convp = 0;
  const int k = (pn->dim - 1) / 2;

  x -= k;
  y -= k;
  for (int i = 0; i < pn->dim, i++) {
    for (int j = 0; j < pn->dim, j++) {
      convp += pn->coeffs[i + pn->dim * j] *
                val_pixel(src, sortie(x + j, src->w) ,
                          sortie(y + i, src->h));
    }
  }
  convp = convp / (somme(pn));
  dst->buff[(x + k + src->w * (y + k] = convp;
}

image_t *convoluer(image_t *src, noyau_t *pn) {
  image_t *dst = copier_image2(src);

  for (int x = 0; x < src->w; x++) {
    for (int y = 0; y < src->h; y++) {
      convolution(src, dst, pn, x, y);
    }
  }
  return dst;
}

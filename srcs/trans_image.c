#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"
#include "noyau.h"
#include "trans_image.h"

void swap_pixel(uint8_t *src_pix, uint8_t *dst_pix) {
  const uint8_t tmp_pix = *src_pix;

  *src_pix = *dst_pix;
  *dst_pix = tmp_pix;
}

image_t *negatif(image_t *src) {
  image_t *dst = copier_image_sup(src);
  for (size_t i = 0; i < dst->h * dst->w; i++) {
    dst->buff[i] -= 255;
  }
  return dst;
}

static
void rotation90_aux(image_t *src, image_t *dst, int angle) {
  for (size_t y = 0; y < src->h; y++) {
    for (size_t x = 0; x < src->w; x++) {
      dst->buff[x + src->w * y] = (src->buff[(src->h - y) + (src->w * x)]);
    }
  }
  rotation(dst, angle - 90);
  src->h = dst->w;
  src->w = dst->h;
}

static
image_t *rotation90(image_t *src, int angle) {
  image_t *dst = copier_image_sup(src);
  rotation90_aux(src, dst, angle);
  return (dst);
}

image_t *rotation(image_t *src, int angle) {
  image_t *dst = NULL;
  if (angle % 90 == 0) {
    dst = rotation90(src, angle);
    return dst;
  }
  return dst;
}

image_t *modifier_lumin(image_t *src, int pourcent) {
  image_t *dst = copier_image_sup(src);
  for (size_t i = 0; i < dst->h * dst->w; i++) {
    // Dirty Hack FIX THAT
    // dst->buff[i] = (src->buff[i] > 255) ? 255 : (src->buff[i] * pourcent) / 100;
    dst->buff[i] = (src->buff[i] * pourcent) / 100;
  }
  return dst;
}

image_t *bruiter_image(image_t *src, int pourcent) {
  image_t *dst = copier_image_sup(src);
  for (size_t x = 0; x < src->w; x++) {
    for (size_t y = 0; y < src->h; y++) {
      const int random_number = rand();
      if ((random_number % 100) < pourcent){
        dst->buff[x + src->w * y] = random_number % 256;
      } else {
        dst->buff[x + src->w * y] = src->buff[x + src->w * y];
      }
    }
  }
  return dst;
}

image_t *filtrer_median(image_t *src) {
  image_t *dst = copier_image_sup(src);
  for (size_t x = 0; x < src->w; x++) {
    for (size_t y = 0; y < src->h; y++) {
      //filtrage_median(src, dst, x, y);
    }
  }
  return dst;
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

static
void convolution(image_t *src, image_t *dst, noyau_t *pn, int x, int y) {
  int convp = 0;
  const int k = (pn->dim - 1) / 2;

  x -= k;
  y -= k;
  for (size_t i = 0; i < pn->dim; i++) {
    for (size_t j = 0; j < pn->dim; j++) {
      convp += (pn->coeffs[i + pn->dim * j] *
          val_pixel(src, sortie(x + j, src->w),
            sortie(y + i, src->h)));
    }
  }
  convp = convp / core_sum(pn);
  dst->buff[x + k + src->w * (y + k)] = convp;
}

image_t *convoluer(image_t *src, noyau_t *pn) {
  image_t *dst = copier_image_sup(src);

  for (size_t x = 0; x < src->w; x++) {
    for (size_t y = 0; y < src->h; y++) {
      convolution(src, dst, pn, x, y);
    }
  }
  return dst;
}

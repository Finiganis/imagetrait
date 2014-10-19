#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"
#include "noyau.h"
#include "trans_image.h"

inline
static
int sortie(int32_t a, int32_t max) {
  max = max - 1;
  if (a < 0) {
    return 0;
  } else if (a > max) {
    return max;
  } else {
    return a;
  }
}

inline
static
int divide_(int32_t a, int32_t b) {
  if (b == 0) {
    return a;
  } else {
    return (a / b);
  }
}

image_t *negatif(image_t *src) {
  image_t *dst = copier_image_sup(src);
  for (size_t i = 0; i < dst->h * dst->w; i += 1) {
    dst->buff[i] = ~src->buff[i];
  }
  return dst;
}

inline
static
size_t index_(const size_t x, const size_t y, const image_t *img) {
  return (x + img->w * y);
}

static
void rotation90_aux(image_t *src, image_t *dst) {
  dst->h = src->w;
  dst->w = src->h;
  for (size_t y = 0; y < src->h; y += 1) {
    for (size_t x = 0; x < src->w; x += 1) {
      const size_t src_cur = index_(x, y, src),
                   dst_cur = index_(src->h - y, x, dst) - 1;
      dst->buff[dst_cur] = src->buff[src_cur];
    }
  }
}

static
void rotation180_aux(image_t *src, image_t *dst) {
  dst->w = src->w;
  dst->h = src->h;
  for (size_t src_cur = src->w * src->h, dst_cur = 0;
      (src_cur > 0) && (dst_cur < dst->w * dst->h);
      src_cur -= 1, dst_cur += 1) {
    dst->buff[dst_cur] = src->buff[src_cur];
  }
}

static
void rotation270_aux(image_t *src, image_t *dst) {
  dst->h = src->w;
  dst->w = src->h;
  for (size_t y = 0; y < src->h; y += 1) {
    for (size_t x = 0; x < src->w; x += 1) {
      const size_t src_cur = index_(x, y, src),
                   dst_cur = index_(y, src->w - x, dst);
      dst->buff[dst_cur] = src->buff[src_cur];
    }
  }
}

static
func_rot_t *strategy_rotation(int angle) {
  if (angle == 90) {
    return rotation90_aux;
  } else if (angle == 180) {
    return rotation180_aux;
  } else if (angle == 270) {
    return rotation270_aux;
  } else {
    return NULL;
  }
}

image_t *rotation(image_t *src, int angle) {
  image_t *dst = NULL;
  func_rot_t *fun = strategy_rotation(angle);
  if (fun) {
    dst = copier_image_sup(src);
    if (dst) {
      fun(src, dst);
    } else {
      fprintf(stderr, "rotation: image is NULL\n");
    }
  } else {
    fprintf(stderr, "rotation: Invalid angle %d.\n", angle);
  }
  return dst;
}

static
uint8_t fix_pixel_lumin(const int new_pixel) {
  if (new_pixel >= 255) {
    return (255);
  } else if (new_pixel <= 0) {
    return (0);
  } else {
    return ((uint8_t) new_pixel);
  }
}

image_t *modifier_lumin(image_t *src, int pourcent) {
  image_t *dst = copier_image_sup(src);
  for (size_t i = 0; i < dst->h * dst->w; i += 1) {
    const int new_pixel = (src->buff[i] * pourcent) / 100;
    dst->buff[i] = fix_pixel_lumin(new_pixel);
  }
  return dst;
}

image_t *bruiter_image(image_t *src, int pourcent) {
  image_t *dst = copier_image_sup(src);
  for (size_t i = 0; i < src->w * src->h; i++) {
    if ((rand() % 100) < pourcent) {
      dst->buff[i] = rand() % 255;
    } else {
      dst->buff[i] = src->buff[i];
    }
  }
  return dst;
}

static
void remplissage(uint8_t *medians, uint8_t pixel, size_t count) {
  for (; (count > 0) && (pixel < medians[count - 1]); count -= 1) {
    medians[count] = medians[count - 1];
  }
  medians[count] = pixel;
}

static
void filtrage_median(uint8_t *medians, image_t *dst,
    const image_t *src,
    const int32_t x,
    const int32_t y) {
  size_t count = 0;

  for (int32_t mod_y = -2; mod_y < 3; mod_y += 1) {
    for (int32_t mod_x = -2; mod_x < 3; mod_x += 1) {
      if ((mod_y != -2 && mod_y != 2) ||
          (mod_x != -2  && mod_x != 2)) {
        const size_t tmp_x = sortie(x + mod_x, src->w),
                     tmp_y = sortie(y + mod_y, src->h),
                     cursor = tmp_x + src->w * tmp_y;

        remplissage(medians, src->buff[cursor], count);
        count += 1;
      }
    }
  }
  dst->buff[x + dst->w * y] = medians[10];
}

image_t *filtrer_median(image_t *src) {
  image_t *dst = copier_image_sup(src);
  uint8_t medians[21] = { 0 };
  for (size_t x = 0; x < src->w; x++) {
    for (size_t y = 0; y < src->h; y++) {
      filtrage_median(medians, dst, src, x, y);
    }
  }
  return dst;
}

static
void convolution(image_t *src, image_t *dst, noyau_t *pn, size_t x, size_t y) {
  int convp = 0;
  const int k = (pn->dim - 1) / 2;

  x -= k;
  y -= k;
  for (size_t i = 0; i < pn->dim; i++) {
    for (size_t j = 0; j < pn->dim; j++) {
      const size_t src_x = sortie(x + j, src->w),
                   src_y = sortie(y + i, src->h),
                   cur_core = i + pn->dim * j;
      const uint8_t pixel = val_pixel(src, src_x, src_y);
      convp += pn->coeffs[cur_core] * pixel;
    }
  }
  convp = divide_(convp, core_sum(pn));
  const size_t cursor = x + k + src->w * (y + k);
  dst->buff[cursor] = convp;
}

image_t *convoluer(image_t *src, noyau_t *pn) {
  image_t *dst = copier_image_sup(src);

  for (size_t y = 0; y < src->h; y += 1) {
    for (size_t x = 0; x < src->w; x += 1) {
      convolution(src, dst, pn, x, y);
    }
  }
  return dst;
}

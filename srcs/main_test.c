#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pgm_image.h"
#include "my_string.h"
#include "image.h"
#include "noyau.h"
#include "trans_image.h"

int main(int argc, char *argv[]) {
  if (argc == 2) {
    char *path = my_strdup(argv[1]);
    image_t *img = charger_image_pgm(path);
    if (!img) { fprintf(stderr, "main: Invalid image abort\n"); return 1;}
    image_t *new_img = rotation(img, 90);
    image_t *tmp = rotation(new_img, 90);
    detruire_image(new_img);
    new_img = rotation(tmp, 90);
    detruire_image(tmp);
    tmp = rotation(new_img, 90);
    detruire_image(new_img);
    new_img = tmp;
    if (memcmp(new_img->buff, img->buff, img->h * img->w)) {
      fprintf(stderr, "Error image are differents after 4 * 90 rot.\n");
    } else {
      fprintf(stderr, "Images are equals rotation is OK!\n");
    }
    sauver_image_pgm("replique2.pgm", new_img);
    char const *noyaux[11] = {
      "noyaux/noyau_deriv.txt",
      "noyaux/noyau_deriv2.txt",
      "noyaux/noyau_derivxy2.txt",
      "noyaux/noyau_derivy2.txt",
      "noyaux/noyau_exp.txt",
      "noyaux/noyau_gauss.txt",
      "noyaux/noyau_laplace4.txt",
      "noyaux/noyau_laplace8.txt",
      "noyaux/noyau_moy.txt",
      "noyaux/noyau_sobelx.txt",
      "noyaux/noyau_sobely.txt"};
    for (size_t x = 0; x < 11; x += 1) {
      noyau_t *core = charger_noyau(noyaux[x]);
      fprintf(stderr, "%zu: core.path: %s"
          " | core.dim: %d\n",x, noyaux[x], core->dim);
      image_t *img2 = convoluer(new_img, core);
      detruire_image(img2);
      detruire_noyau(core);
    }
    detruire_image(img);
    detruire_image(new_img);
    free(path);
  }
  return (0);
}

#include <stdio.h>
#include <stdlib.h>

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
    putchar('\n');
    image_t *new_img = rotation(img, 90);
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
    for (size_t x = 0; x < 10; x += 1) {
      noyau_t *core = charger_noyau(noyaux[x]);
      fprintf(stderr, "core.path: %s"
          " | core.dim: %d\n", noyaux[x], core->dim);
      convoluer(new_img, core);
      detruire_noyau(core);
    }
    detruire_image(img);
    detruire_image(new_img);
    free(path);
  }
  return (0);
}

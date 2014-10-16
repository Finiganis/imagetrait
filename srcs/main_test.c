#include <stdio.h>
#include <stdlib.h>

#include "pgm_image.h"
#include "my_string.h"
#include "image.h"
#include "noyau.h"
#include "trans_image.h"

int main(int argc, char *argv[]) {
  if (argc == 3) {
    char *path = my_strdup(argv[1]);
    char *filename_core = my_strdup(argv[2]);
    printf("filename_img: %s | filename_core: %s\n",
        path, filename_core);
    image_t *img = charger_image_pgm(path);
    noyau_t *core = charger_noyau(filename_core);
    printf("core.dim: %d\n", core->dim);
    for (size_t x = 0; x < (core->dim * core->dim); x++) {
      printf("| %du |", core->coeffs[x]);
    }
    putchar('\n');
    detruire_noyau(core);
    image_t *new_img = rotation(img, 90);
    sauver_image_pgm("replique2.pgm", new_img);
    detruire_image(img);
    detruire_image(new_img);
    free(path);
    free(filename_core);
  }
  return (0);
}

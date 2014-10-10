#include <stdio.h>
#include <stdlib.h>

#include "pgm_image.h"
#include "my_string.h"
#include "image.h"

int main(int argc, char *argv[]) {
  if (argc == 3) {
    const char *path = my_strdup(argv[1]);
    const char *filename_core = my_strdup(argv[2]);
    printf("filename_img: %s\n | filename_core: %s",
        path, filename_core);
    image_t *img = charger_image_pgm(path);
    noyau_t *core = charger_noyau(
        sauver_image_pgm("replique.pgm", img);
        detruire_image(img);
        free(path);
        }
        return (0);
        }

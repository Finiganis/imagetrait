#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "noyau.h"

#define TMP_STR_SIZE 128

noyau_t *creer_noyau(uint32_t dim) {
  noyau_t *core = NULL;
  if (dim) {
    core = malloc(sizeof(noyau_t));
    if (core) {
      core->coeffs = malloc(sizeof(int32_t) * dim * dim);
      core->dim = dim;
      if (!core->coeffs) {
        free(core->coeffs);
        perror("creer_noyau: ");
      }
    } else {
      perror("creer_noyau: ");
    }
  } else {
    fprintf(stderr,
        "creer_noyau: core dimension must be superior "
        "than zero.\n");
  }
  return (core);
}

void detruire_noyau(noyau_t *pn) {
  if (pn) {
    if (pn->coeffs) {
      free(pn->coeffs);
    }
    free(pn);
  }
}

static
void read_core(FILE *file, noyau_t *pn) {
  for (size_t y = 0, x = 0; y < pn->dim; y++, x = 0) {
    for (; x < pn->dim; x++) {
      const size_t cur = x + y * pn->dim;
      fscanf(file, "%d", &pn->coeffs[cur]);
    }
  }
}

noyau_t *charger_noyau(const char *nom_fichier) {
  noyau_t *core = NULL;

  if (nom_fichier) {
    FILE *file = fopen(nom_fichier, "r");

    if (file) {
      uint32_t dim = 0;
      (void)fscanf(file, "%u", &dim);
      core = creer_noyau(dim);
      if (core) {
        read_core(file, core);
      }
    } else {
      perror("charger_noyau: ");
    }
    fclose(file);
  } else {
    fprintf(stderr,
        "charger_noyau: filename provided is NULL.\n");
  }
  return core;
}

int core_sum(noyau_t *pn) {
  int res = 0;

  for (int x = 0; x < (pn->dim * pn->dim); x++) {
    res += pn->coeffs[x];
  }
  return res;
}

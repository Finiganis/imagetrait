#ifndef NOYAU_H
#define NOYAU_H

#include <stdint.h>

typedef struct {
  uint32_t dim;
  int32_t *coeffs;
} noyau_t;

noyau_t *creer_noyau(uint32_t dim);
void detruire_noyau(noyau_t *pn);
noyau_t *charger_noyau(const char *nom_fichier);

#endif

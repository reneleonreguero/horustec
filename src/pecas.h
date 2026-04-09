#ifndef PECAS_H
#define PECAS_H

#include "csv.h"

#define PECAS_FILE "data/pecas.csv"

void pecas_add(char *nome, char *qtd, char *custo, char *venda);
void pecas_list(void);
void pecas_get(int id);
void pecas_update(int id, int campo, const char *valor);
void pecas_delete(int id);
void pecas_venda(int id, int qtd);
void pecas_low_stock(int min);

#endif

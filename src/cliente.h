#ifndef CLIENTE_H
#define CLIENTE_H

#include "csv.h"

#define CLIENTE_FILE "data/clientes.csv"

void cliente_add(char *nome, char *tel, char *email, char *endereco);
void cliente_list(void);
void cliente_get(int id);
void cliente_search(const char *termo);
void cliente_update(int id, int campo, const char *valor);
void cliente_delete(int id);

#endif

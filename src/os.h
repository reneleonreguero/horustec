#ifndef OS_H
#define OS_H

#include "csv.h"

#define OS_FILE "data/os.csv"

#define STATUS_ORCAMENTO "orcamento"
#define STATUS_APROVADO  "aprovado"
#define STATUS_EM_BANCO  "em_banco"
#define STATUS_AGUARDANDO "aguardando"
#define STATUS_FINALIZADO "finalizado"
#define STATUS_ENTREGUE  "entregue"

void os_add(int cliente_id, char *equip, char *problema);
void os_list(void);
void os_get(int id);
void os_update(int id, int campo, const char *valor);
void os_delete(int id);
void os_list_by_cliente(int cliente_id);
void os_list_by_status(const char *status);

#endif

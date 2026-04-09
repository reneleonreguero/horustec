#ifndef AGENDA_H
#define AGENDA_H

#include "csv.h"

#define AGENDA_FILE "data/agenda.csv"

void agenda_add(char *data, char *hora, char *desc, char *cli_id, char *os_id);
void agenda_list(void);
void agenda_list_by_date(const char *data);
void agenda_get(int id);
void agenda_delete(int id);

#endif

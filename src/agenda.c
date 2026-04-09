#include "agenda.h"
#include <stdio.h>

void agenda_add(char *data, char *hora, char *desc, char *cli_id, char *os_id) {
    int id = csv_get_next_id(AGENDA_FILE);
    
    const char *fields[6] = { NULL, data, hora, desc, cli_id ? cli_id : "0", os_id ? os_id : "0" };
    char id_str[16];
    sprintf(id_str, "%d", id);
    fields[0] = id_str;
    
    if(csv_add_row(AGENDA_FILE, fields, 6)) {
        printf("Compromisso #%d agendado para %s as %s!\n", id, data, hora);
    }
}

void agenda_list(void) {
    CSVData *data = csv_load(AGENDA_FILE);
    printf("\n=== AGENDA (%d eventos) ===\n", data->count);
    printf("%-3s | %-12s | %-8s | %-25s | Cliente | OS\n", "ID", "DATA", "HORA", "DESCRICAO");
    printf("----|--------------|----------|---------------------------|---------|-----\n");
    for(int i = 0; i < data->count; i++) {
        printf("%-3s | %-12s | %-8s | %-25s | %-7s | %s\n",
            data->rows[i].fields[0],
            data->rows[i].fields[1],
            data->rows[i].fields[2],
            data->rows[i].fields[3],
            data->rows[i].fields[4],
            data->rows[i].fields[5]);
    }
    printf("\n");
    csv_free(data);
}

void agenda_list_by_date(const char *data) {
    CSVData *data_csv = csv_load(AGENDA_FILE);
    printf("\n=== AGENDA: %s ===\n", data);
    for(int i = 0; i < data_csv->count; i++) {
        if(strcmp(data_csv->rows[i].fields[1], data) == 0) {
            printf("#%s | %s | %s | %s\n",
                data_csv->rows[i].fields[0],
                data_csv->rows[i].fields[2],
                data_csv->rows[i].fields[3],
                data_csv->rows[i].fields[4]);
        }
    }
    csv_free(data_csv);
}

void agenda_get(int id) {
    CSVData *data = csv_load(AGENDA_FILE);
    CSVRow *row = csv_find_by_id(data, id);
    if(row) {
        printf("\n=== COMPROMISSO #%s ===\n", row->fields[0]);
        printf("ID:         %s\n", row->fields[0]);
        printf("Data:       %s\n", row->fields[1]);
        printf("Hora:       %s\n", row->fields[2]);
        printf("Descricao:  %s\n", row->fields[3]);
        printf("Cliente:    #%s\n", row->fields[4]);
        printf("OS:         #%s\n", row->fields[5]);
    } else {
        printf("Compromisso #%d nao encontrado.\n", id);
    }
    csv_free(data);
}

void agenda_delete(int id) {
    if(csv_delete_row(AGENDA_FILE, id)) {
        printf("Compromisso #%d removido.\n", id);
    } else {
        printf("Erro ao remover ou compromisso nao encontrado.\n");
    }
}

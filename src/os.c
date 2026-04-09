#include "os.h"
#include <time.h>

void os_add(int cliente_id, char *equip, char *problema) {
    int id = csv_get_next_id(OS_FILE);
    
    time_t now = time(NULL);
    char data[32];
    strftime(data, sizeof(data), "%Y-%m-%d", localtime(&now));
    
    char cliente_str[16];
    sprintf(cliente_str, "%d", cliente_id);
    
    const char *fields[] = { NULL, cliente_str, equip, problema, STATUS_ORCAMENTO, "0.00", data };
    char id_str[16];
    sprintf(id_str, "%d", id);
    fields[0] = id_str;
    
    if(csv_add_row(OS_FILE, fields, 7)) {
        printf("OS #%d criada para cliente #%s!\n", id, cliente_str);
    }
}

void os_list(void) {
    CSVData *data = csv_load(OS_FILE);
    printf("\n=== ORDENS DE SERVICO (%d) ===\n", data->count);
    printf("%-3s | %-6s | %-15s | %-20s | %-12s | %s\n", "ID", "CLIENTE", "EQUIPAMENTO", "PROBLEMA", "STATUS", "VALOR");
    printf("----|--------|-----------------|---------------------|--------------|--------\n");
    for(int i = 0; i < data->count; i++) {
        printf("%-3s | %-6s | %-15s | %-20s | %-12s | R$ %s\n",
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

void os_get(int id) {
    CSVData *data = csv_load(OS_FILE);
    CSVRow *row = csv_find_by_id(data, id);
    if(row) {
        printf("\n=== OS #%s ===\n", row->fields[0]);
        printf("ID:         %s\n", row->fields[0]);
        printf("Cliente:    #%s\n", row->fields[1]);
        printf("Equipamento: %s\n", row->fields[2]);
        printf("Problema:   %s\n", row->fields[3]);
        printf("Status:     %s\n", row->fields[4]);
        printf("Valor:      R$ %s\n", row->fields[5]);
        printf("Data:       %s\n", row->fields[6]);
    } else {
        printf("OS #%d nao encontrada.\n", id);
    }
    csv_free(data);
}

void os_update(int id, int campo, const char *valor) {
    char campo_str[8];
    sprintf(campo_str, "%d", campo);
    if(csv_update_row(OS_FILE, id, campo_str, valor)) {
        printf("OS #%d atualizada.\n", id);
    } else {
        printf("Erro ao atualizar ou OS nao encontrada.\n");
    }
}

void os_delete(int id) {
    if(csv_delete_row(OS_FILE, id)) {
        printf("OS #%d removida.\n", id);
    } else {
        printf("Erro ao remover ou OS nao encontrada.\n");
    }
}

void os_list_by_cliente(int cliente_id) {
    CSVData *data = csv_load(OS_FILE);
    printf("\n=== OS DO CLIENTE #%d ===\n", cliente_id);
    for(int i = 0; i < data->count; i++) {
        if(atoi(data->rows[i].fields[1]) == cliente_id) {
            printf("#%s | %s | %s | %s\n",
                data->rows[i].fields[0],
                data->rows[i].fields[2],
                data->rows[i].fields[4],
                data->rows[i].fields[5]);
        }
    }
    csv_free(data);
}

void os_list_by_status(const char *status) {
    CSVData *data = csv_load(OS_FILE);
    printf("\n=== OS COM STATUS: %s ===\n", status);
    for(int i = 0; i < data->count; i++) {
        if(strcmp(data->rows[i].fields[4], status) == 0) {
            printf("#%s | Cliente #%s | %s | R$ %s\n",
                data->rows[i].fields[0],
                data->rows[i].fields[1],
                data->rows[i].fields[2],
                data->rows[i].fields[5]);
        }
    }
    csv_free(data);
}

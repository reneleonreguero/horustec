#include "cliente.h"
#include <stdio.h>

void cliente_add(char *nome, char *tel, char *email, char *endereco) {
    int id = csv_get_next_id(CLIENTE_FILE);
    const char *fields[] = { NULL, nome, tel, email, endereco };
    char id_str[16];
    sprintf(id_str, "%d", id);
    fields[0] = id_str;
    
    if(csv_add_row(CLIENTE_FILE, fields, 5)) {
        printf("Cliente #%d '%s' adicionado!\n", id, nome);
    }
}

void cliente_list(void) {
    CSVData *data = csv_load(CLIENTE_FILE);
    printf("\n=== CLIENTES (%d) ===\n", data->count);
    printf("%-3s | %-20s | %-15s | %-25s | %s\n", "ID", "NOME", "TELEFONE", "EMAIL", "ENDERECO");
    printf("----|----------------------|-----------------|---------------------------|---------------------------\n");
    for(int i = 0; i < data->count; i++) {
        printf("%-3s | %-20s | %-15s | %-25s | %s\n",
            data->rows[i].fields[0],
            data->rows[i].fields[1],
            data->rows[i].fields[2],
            data->rows[i].fields[3],
            data->rows[i].fields[4]);
    }
    printf("\n");
    csv_free(data);
}

void cliente_get(int id) {
    CSVData *data = csv_load(CLIENTE_FILE);
    CSVRow *row = csv_find_by_id(data, id);
    if(row) {
        printf("\n=== CLIENTE #%s ===\n", row->fields[0]);
        printf("ID:       %s\n", row->fields[0]);
        printf("Nome:     %s\n", row->fields[1]);
        printf("Telefone: %s\n", row->fields[2]);
        printf("Email:    %s\n", row->fields[3]);
        printf("Endereco: %s\n", row->fields[4]);
    } else {
        printf("Cliente #%d nao encontrado.\n", id);
    }
    csv_free(data);
}

void cliente_search(const char *termo) {
    CSVData *data = csv_load(CLIENTE_FILE);
    CSVData *result = csv_search(data, 1, termo);
    if(result->count > 0) {
        printf("\n=== RESULTADOS (%d) ===\n", result->count);
        for(int i = 0; i < result->count; i++) {
            printf("%s | %s | %s\n", result->rows[i].fields[0], result->rows[i].fields[1], result->rows[i].fields[2]);
        }
    } else {
        printf("Nenhum cliente encontrado.\n");
    }
    csv_free(result);
    csv_free(data);
}

void cliente_update(int id, int campo, const char *valor) {
    char campo_str[8];
    sprintf(campo_str, "%d", campo);
    if(csv_update_row(CLIENTE_FILE, id, campo_str, valor)) {
        printf("Cliente #%d atualizado.\n", id);
    } else {
        printf("Erro ao atualizar ou cliente nao encontrado.\n");
    }
}

void cliente_delete(int id) {
    if(csv_delete_row(CLIENTE_FILE, id)) {
        printf("Cliente #%d removido.\n", id);
    } else {
        printf("Erro ao remover ou cliente nao encontrado.\n");
    }
}

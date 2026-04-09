#include "pecas.h"
#include <stdio.h>

void pecas_add(char *nome, char *qtd, char *custo, char *venda) {
    int id = csv_get_next_id(PECAS_FILE);
    const char *fields[] = { NULL, nome, qtd, custo, venda };
    char id_str[16];
    sprintf(id_str, "%d", id);
    fields[0] = id_str;
    
    if(csv_add_row(PECAS_FILE, fields, 5)) {
        printf("Peca #%d '%s' adicionada! (estoque: %s)\n", id, nome, qtd);
    }
}

void pecas_list(void) {
    CSVData *data = csv_load(PECAS_FILE);
    printf("\n=== ESTOQUE DE PECAS (%d) ===\n", data->count);
    printf("%-3s | %-20s | %-6s | %-10s | %-10s\n", "ID", "NOME", "QTD", "CUSTO", "VENDA");
    printf("----|----------------------|--------|------------|------------\n");
    for(int i = 0; i < data->count; i++) {
        printf("%-3s | %-20s | %-6s | R$ %-7s | R$ %-7s\n",
            data->rows[i].fields[0],
            data->rows[i].fields[1],
            data->rows[i].fields[2],
            data->rows[i].fields[3],
            data->rows[i].fields[4]);
    }
    printf("\n");
    csv_free(data);
}

void pecas_get(int id) {
    CSVData *data = csv_load(PECAS_FILE);
    CSVRow *row = csv_find_by_id(data, id);
    if(row) {
        printf("\n=== PECA #%s ===\n", row->fields[0]);
        printf("ID:          %s\n", row->fields[0]);
        printf("Nome:        %s\n", row->fields[1]);
        printf("Quantidade:  %s\n", row->fields[2]);
        printf("Preco Custo: R$ %s\n", row->fields[3]);
        printf("Preco Venda: R$ %s\n", row->fields[4]);
    } else {
        printf("Peca #%d nao encontrada.\n", id);
    }
    csv_free(data);
}

void pecas_update(int id, int campo, const char *valor) {
    char campo_str[8];
    sprintf(campo_str, "%d", campo);
    if(csv_update_row(PECAS_FILE, id, campo_str, valor)) {
        printf("Peca #%d atualizada.\n", id);
    } else {
        printf("Erro ao atualizar ou peca nao encontrada.\n");
    }
}

void pecas_delete(int id) {
    if(csv_delete_row(PECAS_FILE, id)) {
        printf("Peca #%d removida.\n", id);
    } else {
        printf("Erro ao remover ou peca nao encontrada.\n");
    }
}

void pecas_venda(int id, int qtd) {
    CSVData *data = csv_load(PECAS_FILE);
    int idx = -1;
    for(int i = 0; i < data->count; i++) {
        if(atoi(data->rows[i].fields[0]) == id) {
            idx = i;
            break;
        }
    }
    
    if(idx >= 0) {
        int estoque = atoi(data->rows[idx].fields[2]);
        if(estoque >= qtd) {
            char novo_estoque[16];
            sprintf(novo_estoque, "%d", estoque - qtd);
            csv_update_row(PECAS_FILE, id, "2", novo_estoque);
            printf("Venda registrada! Estoque: %s -> %s\n", data->rows[idx].fields[2], novo_estoque);
        } else {
            printf("Estoque insuficiente! Disponivel: %d\n", estoque);
        }
    } else {
        printf("Peca #%d nao encontrada.\n", id);
    }
    csv_free(data);
}

void pecas_low_stock(int min) {
    CSVData *data = csv_load(PECAS_FILE);
    printf("\n=== ESTOQUE BAIXO (< %d) ===\n", min);
    int found = 0;
    for(int i = 0; i < data->count; i++) {
        if(atoi(data->rows[i].fields[2]) < min) {
            printf("#%s | %s | qtd: %s\n", 
                data->rows[i].fields[0],
                data->rows[i].fields[1],
                data->rows[i].fields[2]);
            found = 1;
        }
    }
    if(!found) printf("Nenhuma peca com estoque baixo.\n");
    printf("\n");
    csv_free(data);
}

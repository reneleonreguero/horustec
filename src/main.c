#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "csv.h"
#include "cliente.h"
#include "os.h"
#include "pecas.h"
#include "agenda.h"

static void ensure_data_dir(void) {
    system("mkdir -p data");
}

static void print_help(void) {
    printf("\n");
    printf("  ___  ___  ___  ___  ___  ___  ___  ___\n");
    printf(" |___  horustec - Sistema p/ Tecnicos  ___\n");
    printf("\n");
    printf("Uso: horus <comando> [subcomando] [args]\n");
    printf("\n");
    printf("CLIENTES:\n");
    printf("  horus cliente add <nome> <tel> <email> <endereco>  Adiciona cliente\n");
    printf("  horus cliente list                                 Lista todos\n");
    printf("  horus cliente get <id>                             Busca por ID\n");
    printf("  horus cliente search <termo>                       Busca por nome\n");
    printf("  horus cliente update <id> <campo>=<valor>         Atualiza\n");
    printf("  horus cliente delete <id>                          Remove\n");
    printf("\n");
    printf("ORDENS DE SERVICO:\n");
    printf("  horus os add <cliente_id> <equip> <problema>       Nova OS\n");
    printf("  horus os list                                      Lista todas\n");
    printf("  horus os list --cliente <id>                       OS do cliente\n");
    printf("  horus os list --status <status>                    Por status\n");
    printf("  horus os get <id>                                  Detalhes\n");
    printf("  horus os update <id> status=<valor>                Atualiza status\n");
    printf("  horus os delete <id>                               Remove\n");
    printf("\n");
    printf("  Status: orcamento|aprovado|em_banco|aguardando|finalizado|entregue\n");
    printf("\n");
    printf("PECAS:\n");
    printf("  horus pecas add <nome> <qtd> <custo> <venda>       Adiciona peca\n");
    printf("  horus pecas list                                   Lista estoque\n");
    printf("  horus pecas get <id>                              Detalhes\n");
    printf("  horus pecas update <id> <campo>=<valor>           Atualiza\n");
    printf("  horus pecas delete <id>                            Remove\n");
    printf("  horus pecas venda <id> <qtd>                       Registra venda\n");
    printf("  horus pecas low <min>                              Estoque baixo\n");
    printf("\n");
    printf("AGENDA:\n");
    printf("  horus agenda add <data> <hora> <desc> [cli] [os]  Novo compromisso\n");
    printf("  horus agenda list                                  Lista todos\n");
    printf("  horus agenda list <data>                          Por data\n");
    printf("  horus agenda get <id>                              Detalhes\n");
    printf("  horus agenda delete <id>                           Remove\n");
    printf("\n");
    printf("Ex: horus pecas add \"Memoria 8GB\" 10 180.00 280.00\n");
    printf("Ex: horus agenda add 2026-04-15 14:30 \"Troca tela\" 3 15\n");
    printf("\n");
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        print_help();
        return 1;
    }
    
    ensure_data_dir();
    
    if(strcmp(argv[1], "help") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_help();
    }
    else if(strcmp(argv[1], "cliente") == 0) {
        if(argc < 3) { printf("Use: horus cliente <add|list|get|search|update|delete>\n"); return 1; }
        if(strcmp(argv[2], "add") == 0 && argc >= 7) cliente_add(argv[3], argv[4], argv[5], argv[6]);
        else if(strcmp(argv[2], "list") == 0) cliente_list();
        else if(strcmp(argv[2], "get") == 0 && argc > 3) cliente_get(atoi(argv[3]));
        else if(strcmp(argv[2], "search") == 0 && argc > 3) cliente_search(argv[3]);
        else if(strcmp(argv[2], "update") == 0 && argc > 4) {
            int id; char campo[8], valor[256];
            sscanf(argv[3], "%d", &id);
            sscanf(argv[4], "%[^=]=%s", campo, valor);
            cliente_update(id, atoi(campo), valor);
        }
        else if(strcmp(argv[2], "delete") == 0 && argc > 3) cliente_delete(atoi(argv[3]));
        else printf("Use: horus cliente <add|list|get|search|update|delete>\n");
    }
    else if(strcmp(argv[1], "os") == 0) {
        if(argc < 3) { printf("Use: horus os <add|list|get|update|delete>\n"); return 1; }
        if(strcmp(argv[2], "add") == 0 && argc >= 6) os_add(atoi(argv[3]), argv[4], argv[5]);
        else if(strcmp(argv[2], "list") == 0) {
            if(argc > 3 && strcmp(argv[3], "--cliente") == 0 && argc > 4) os_list_by_cliente(atoi(argv[4]));
            else if(argc > 3 && strcmp(argv[3], "--status") == 0 && argc > 4) os_list_by_status(argv[4]);
            else os_list();
        }
        else if(strcmp(argv[2], "get") == 0 && argc > 3) os_get(atoi(argv[3]));
        else if(strcmp(argv[2], "update") == 0 && argc > 4) {
            int id; char campo[8], valor[256];
            sscanf(argv[3], "%d", &id);
            sscanf(argv[4], "%[^=]=%s", campo, valor);
            os_update(id, atoi(campo), valor);
        }
        else if(strcmp(argv[2], "delete") == 0 && argc > 3) os_delete(atoi(argv[3]));
        else printf("Use: horus os <add|list|get|update|delete>\n");
    }
    else if(strcmp(argv[1], "pecas") == 0) {
        if(argc < 3) { printf("Use: horus pecas <add|list|get|update|delete|venda|low>\n"); return 1; }
        if(strcmp(argv[2], "add") == 0 && argc >= 7) pecas_add(argv[3], argv[4], argv[5], argv[6]);
        else if(strcmp(argv[2], "list") == 0) pecas_list();
        else if(strcmp(argv[2], "get") == 0 && argc > 3) pecas_get(atoi(argv[3]));
        else if(strcmp(argv[2], "update") == 0 && argc > 4) {
            int id; char campo[8], valor[256];
            sscanf(argv[3], "%d", &id);
            sscanf(argv[4], "%[^=]=%s", campo, valor);
            pecas_update(id, atoi(campo), valor);
        }
        else if(strcmp(argv[2], "delete") == 0 && argc > 3) pecas_delete(atoi(argv[3]));
        else if(strcmp(argv[2], "venda") == 0 && argc > 4) pecas_venda(atoi(argv[3]), atoi(argv[4]));
        else if(strcmp(argv[2], "low") == 0 && argc > 3) pecas_low_stock(atoi(argv[3]));
        else printf("Use: horus pecas <add|list|get|update|delete|venda|low>\n");
    }
    else if(strcmp(argv[1], "agenda") == 0) {
        if(argc < 3) { printf("Use: horus agenda <add|list|get|delete>\n"); return 1; }
        if(strcmp(argv[2], "add") == 0 && argc >= 6) agenda_add(argv[3], argv[4], argv[5], argc > 6 ? argv[6] : NULL, argc > 7 ? argv[7] : NULL);
        else if(strcmp(argv[2], "list") == 0) {
            if(argc > 3) agenda_list_by_date(argv[3]);
            else agenda_list();
        }
        else if(strcmp(argv[2], "get") == 0 && argc > 3) agenda_get(atoi(argv[3]));
        else if(strcmp(argv[2], "delete") == 0 && argc > 3) agenda_delete(atoi(argv[3]));
        else printf("Use: horus agenda <add|list|get|delete>\n");
    }
    else {
        printf("Comando desconhecido: %s\n", argv[1]);
        print_help();
    }
    
    return 0;
}

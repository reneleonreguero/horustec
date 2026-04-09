#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
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
    printf("Uso: horus <comando> [opcoes]\n");
    printf("\n");
    printf("CLIENTES:\n");
    printf("  horus cliente add [-n nome] [-t tel] [-e email] [-d end]  Adiciona cliente\n");
    printf("  horus cliente list                                           Lista todos\n");
    printf("  horus cliente get <id>                                       Busca por ID\n");
    printf("  horus cliente search <termo>                                Busca por nome\n");
    printf("  horus cliente update <id> -c<num>=<valor>                    Atualiza campo\n");
    printf("  horus cliente delete <id>                                   Remove\n");
    printf("\n");
    printf("ORDENS DE SERVICO:\n");
    printf("  horus os add -c <cliente_id> -e <equip> -p <problema>       Nova OS\n");
    printf("  horus os list [--cliente <id>] [--status <status>]          Lista\n");
    printf("  horus os get <id>                                           Detalhes\n");
    printf("  horus os update <id> -c<num>=<valor>                        Atualiza\n");
    printf("  horus os delete <id>                                        Remove\n");
    printf("\n");
    printf("  Status: orcamento|aprovado|em_banco|aguardando|finalizado|entregue\n");
    printf("\n");
    printf("PECAS:\n");
    printf("  horus pecas add -n <nome> [-q qtd] [-u custo] [-v venda]  Adiciona\n");
    printf("  horus pecas list                                            Lista estoque\n");
    printf("  horus pecas get <id>                                       Detalhes\n");
    printf("  horus pecas update <id> -c<num>=<valor>                    Atualiza\n");
    printf("  horus pecas delete <id>                                     Remove\n");
    printf("  horus pecas venda <id> <qtd>                               Registra venda\n");
    printf("  horus pecas low <min>                                      Estoque baixo\n");
    printf("\n");
    printf("AGENDA:\n");
    printf("  horus agenda add -d <data> -h <hora> [-m desc] [-c cli] [-o os]\n");
    printf("  horus agenda list [data]                                    Lista\n");
    printf("  horus agenda get <id>                                       Detalhes\n");
    printf("  horus agenda delete <id>                                   Remove\n");
    printf("\n");
    printf("Exemplos:\n");
    printf("  horus cliente add -n \"Rene\" -t \"(11)99999-8888\"\n");
    printf("  horus cliente add -n \"Maria\"\n");
    printf("  horus pecas add -n \"Memoria 8GB\" -q 10 -u 180 -v 280\n");
    printf("  horus os add -c 1 -e \"Notebook\" -p \"Tela quebrada\"\n");
    printf("  horus agenda add -d 2026-04-15 -h 14:30 -m \"Trocar tela\"\n");
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
        
        if(strcmp(argv[2], "add") == 0) {
            int opt;
            char *nome = "", *tel = "", *email = "", *end = "";
            
            static struct option long_options[] = {
                {"nome",     required_argument, 0, 'n'},
                {"telefone", required_argument, 0, 't'},
                {"email",    required_argument, 0, 'e'},
                {"endereco", required_argument, 0, 'd'},
                {0, 0, 0, 0}
            };
            
            optind = 3;
            while((opt = getopt_long(argc, argv, "n:t:e:d:", long_options, NULL)) != -1) {
                switch(opt) {
                    case 'n': nome = optarg; break;
                    case 't': tel = optarg; break;
                    case 'e': email = optarg; break;
                    case 'd': end = optarg; break;
                }
            }
            cliente_add(nome, tel, email, end);
        }
        else if(strcmp(argv[2], "list") == 0) cliente_list();
        else if(strcmp(argv[2], "get") == 0 && argc > 3) cliente_get(atoi(argv[3]));
        else if(strcmp(argv[2], "search") == 0 && argc > 3) cliente_search(argv[3]);
        else if(strcmp(argv[2], "update") == 0 && argc > 3) {
            int id; char campo[8], valor[256];
            sscanf(argv[3], "%d", &id);
            if(argc > 4) {
                sscanf(argv[4], "%[^=]=%s", campo, valor);
                cliente_update(id, atoi(campo), valor);
            }
        }
        else if(strcmp(argv[2], "delete") == 0 && argc > 3) cliente_delete(atoi(argv[3]));
        else printf("Use: horus cliente <add|list|get|search|update|delete>\n");
    }
    else if(strcmp(argv[1], "os") == 0) {
        if(argc < 3) { printf("Use: horus os <add|list|get|update|delete>\n"); return 1; }
        
        if(strcmp(argv[2], "add") == 0) {
            int opt;
            int cliente_id = 0;
            char *equip = "", *problema = "";
            
            static struct option long_options[] = {
                {"cliente",  required_argument, 0, 'c'},
                {"equip",    required_argument, 0, 'e'},
                {"problema", required_argument, 0, 'p'},
                {0, 0, 0, 0}
            };
            
            optind = 3;
            while((opt = getopt_long(argc, argv, "c:e:p:", long_options, NULL)) != -1) {
                switch(opt) {
                    case 'c': cliente_id = atoi(optarg); break;
                    case 'e': equip = optarg; break;
                    case 'p': problema = optarg; break;
                }
            }
            if(cliente_id > 0) os_add(cliente_id, equip, problema);
            else printf("Use: horus os add -c <cliente_id> [-e <equip>] [-p <problema>]\n");
        }
        else if(strcmp(argv[2], "list") == 0) {
            if(argc > 3 && strcmp(argv[3], "--cliente") == 0 && argc > 4) os_list_by_cliente(atoi(argv[4]));
            else if(argc > 3 && strcmp(argv[3], "--status") == 0 && argc > 4) os_list_by_status(argv[4]);
            else os_list();
        }
        else if(strcmp(argv[2], "get") == 0 && argc > 3) os_get(atoi(argv[3]));
        else if(strcmp(argv[2], "update") == 0 && argc > 3) {
            int id; char campo[8], valor[256];
            sscanf(argv[3], "%d", &id);
            if(argc > 4) {
                sscanf(argv[4], "%[^=]=%s", campo, valor);
                os_update(id, atoi(campo), valor);
            }
        }
        else if(strcmp(argv[2], "delete") == 0 && argc > 3) os_delete(atoi(argv[3]));
        else printf("Use: horus os <add|list|get|update|delete>\n");
    }
    else if(strcmp(argv[1], "pecas") == 0) {
        if(argc < 3) { printf("Use: horus pecas <add|list|get|update|delete|venda|low>\n"); return 1; }
        
        if(strcmp(argv[2], "add") == 0) {
            int opt;
            char *nome = "", *qtd = "0", *custo = "0", *venda = "0";
            
            static struct option long_options[] = {
                {"nome", required_argument, 0, 'n'},
                {"qtd",  required_argument, 0, 'q'},
                {"custo",required_argument, 0, 'u'},
                {"venda",required_argument, 0, 'v'},
                {0, 0, 0, 0}
            };
            
            optind = 3;
            while((opt = getopt_long(argc, argv, "n:q:u:v:", long_options, NULL)) != -1) {
                switch(opt) {
                    case 'n': nome = optarg; break;
                    case 'q': qtd = optarg; break;
                    case 'u': custo = optarg; break;
                    case 'v': venda = optarg; break;
                }
            }
            pecas_add(nome, qtd, custo, venda);
        }
        else if(strcmp(argv[2], "list") == 0) pecas_list();
        else if(strcmp(argv[2], "get") == 0 && argc > 3) pecas_get(atoi(argv[3]));
        else if(strcmp(argv[2], "update") == 0 && argc > 3) {
            int id; char campo[8], valor[256];
            sscanf(argv[3], "%d", &id);
            if(argc > 4) {
                sscanf(argv[4], "%[^=]=%s", campo, valor);
                pecas_update(id, atoi(campo), valor);
            }
        }
        else if(strcmp(argv[2], "delete") == 0 && argc > 3) pecas_delete(atoi(argv[3]));
        else if(strcmp(argv[2], "venda") == 0 && argc > 4) pecas_venda(atoi(argv[3]), atoi(argv[4]));
        else if(strcmp(argv[2], "low") == 0 && argc > 3) pecas_low_stock(atoi(argv[3]));
        else printf("Use: horus pecas <add|list|get|update|delete|venda|low>\n");
    }
    else if(strcmp(argv[1], "agenda") == 0) {
        if(argc < 3) { printf("Use: horus agenda <add|list|get|delete>\n"); return 1; }
        
        if(strcmp(argv[2], "add") == 0) {
            int opt;
            char *data = "", *hora = "", *desc = "", *cli_id = "0", *os_id = "0";
            
            static struct option long_options[] = {
                {"data", required_argument, 0, 'd'},
                {"hora", required_argument, 0, 'h'},
                {"msg",  required_argument, 0, 'm'},
                {"cli",  required_argument, 0, 'c'},
                {"os",   required_argument, 0, 'o'},
                {0, 0, 0, 0}
            };
            
            optind = 3;
            while((opt = getopt_long(argc, argv, "d:h:m:c:o:", long_options, NULL)) != -1) {
                switch(opt) {
                    case 'd': data = optarg; break;
                    case 'h': hora = optarg; break;
                    case 'm': desc = optarg; break;
                    case 'c': cli_id = optarg; break;
                    case 'o': os_id = optarg; break;
                }
            }
            if(strlen(data) > 0 && strlen(hora) > 0) {
                agenda_add(data, hora, desc, cli_id, os_id);
            } else {
                printf("Use: horus agenda add -d <data> -h <hora> [-m desc] [-c cli] [-o os]\n");
            }
        }
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

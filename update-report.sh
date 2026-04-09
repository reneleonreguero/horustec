#!/bin/bash
# Script para atualizar o RELATORIO.md automaticamente

DATE=$(date '+%Y-%m-%d %H:%M')
GIT_LOG=$(git log --oneline -5 2>/dev/null || echo "Sem commits ainda")

cat > RELATORIO.md << 'HEADER'
# ═══════════════════════════════════════════════════════════════
#                    HORUSTEC - RELATORIO
#         Sistema para Tecnicos de Computadores
# ═══════════════════════════════════════════════════════════════

HEADER

cat >> RELATORIO.md << EOF
**Versao:** 1.0.0
**Ultima Atualizacao:** $DATE
**Repositorio:** github.com/reneleonreguero/horustec

---

## RESUMO DO PROJETO

| Modulo | Status | Descricao |
|--------|--------|-----------|
| Clientes | ATIVO | Cadastro completo de clientes |
| OS | ATIVO | Ordens de servico com status |
| Pec~as | ATIVO | Controle de estoque |
| Agenda | ATIVO | Agendamento de servicos |

---

## FUNCIONALIDADES

### Clientes
- horus cliente add -n <nome> [-t tel] [-e email] [-d end]
- horus cliente list
- horus cliente get <id>
- horus cliente search <termo>
- horus cliente update <id> -c<num>=<valor>
- horus cliente delete <id>

### Ordens de Servico
- horus os add -c <cliente_id> -e <equip> -p <problema>
- horus os list [--cliente <id>] [--status <status>]
- horus os get <id>
- horus os update <id> -c<num>=<valor>
- horus os delete <id>

### Pec~as/Estoque
- horus pecas add -n <nome> [-q qtd] [-u custo] [-v venda]
- horus pecas list
- horus pecas get <id>
- horus pecas update <id> -c<num>=<valor>
- horus pecas delete <id>
- horus pecas venda <id> <qtd>
- horus pecas low <min>

### Agenda
- horus agenda add -d <data> -h <hora> [-m desc] [-c cli] [-o os]
- horus agenda list [data]
- horus agenda get <id>
- horus agenda delete <id>

---

## ESTRUTURA

horustec/
  src/
    main.c, csv.c/h, cliente.c/h, os.c/h, pecas.c/h, agenda.c/h
  data/       (CSVs)
  Makefile, README.md, RELATORIO.md

---

## INSTALACAO

  make install && source ~/.bashrc

---

## CHANGELOG

| Data | Mudanca |
|------|---------|
| $DATE | Atualizacao geral |

---

## ULTIMOS COMMITS

$GIT_LOG

---

**Criado com opencode + skill c-cli-tool**
EOF

echo "RELATORIO.md atualizado!"

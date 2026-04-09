# Horustec - Sistema para Tecnicos de PCs

Ferramenta CLI em C puro para gerenciar o trabalho de tecnicos de reparo de computadores.

## Funcionalidades

- **Clientes**: Cadastro, busca, atualizacao e remocao
- **Ordens de Servico**: Controle completo com status
- **Estoque de Pecas**: Controle de inventario com alertas
- **Agenda**: agendamento de servicos

## Instalacao

```bash
make install
source ~/.bashrc  # ou source ~/.zshrc
```

## Comandos

### Clientes
```bash
horus cliente add "Joao Silva" "(11)99999-9999" "joao@email.com" "Rua X, 123"
horus cliente list
horus cliente get 1
horus cliente search "Joao"
horus cliente update 1 3=novo@email.com
horus cliente delete 1
```

### Ordens de Servico
```bash
horus os add 1 "Notebook Dell" "Tela quebrada"
horus os list
horus os list --status orcamento
horus os update 1 status=aprovado
horus os update 1 5=350.00  # Define valor
```

**Status**: orcamento, aprovado, em_banco, aguardando, finalizado, entregue

### Pecas
```bash
horus pecas add "Memoria 8GB DDR4" 10 180.00 280.00
horus pecas list
horus pecas venda 1 2
horus pecas low 5
```

### Agenda
```bash
horus agenda add 2026-04-15 14:30 "Troca de tela" 1 1
horus agenda list
horus agenda list 2026-04-15
```

## Git

```bash
make git-init        # Inicializa repositorio
make github-create   # Cria repo no GitHub
make git-push        # Envia para remote
```

## Estrutura

```
horustec/
├── src/
│   ├── main.c      # Entry point
│   ├── csv.c/h     # Biblioteca CSV
│   ├── cliente.c/h # Modulo clientes
│   ├── os.c/h      # Modulo OS
│   ├── pecas.c/h   # Modulo pecas
│   ├── agenda.c/h  # Modulo agenda
│   └── Makefile
├── data/           # CSVs gerados automaticamente
└── README.md
```

## Autor

Criado com opencode + skill c-cli-tool

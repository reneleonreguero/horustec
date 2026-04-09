CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = src
TARGET = horus
ALIAS = horus
INSTALL_DIR = $(HOME)/bin/$(TARGET)
PROJECT_DIR = $(shell pwd)
DATE = $(shell date '+%Y-%m-%d %H:%M')
GIT_LOG = $(shell git log --oneline -5 2>/dev/null || echo "Sem commits ainda")

.PHONY: all clean install uninstall git-init git-push report

all: $(TARGET)

$(TARGET): $(SRC)/*.c
	@mkdir -p build data
	$(CC) $(CFLAGS) -o $@ $(SRC)/*.c

clean:
	rm -f $(TARGET) build/*

install: all
	mkdir -p $(HOME)/bin
	cp $(TARGET) $(INSTALL_DIR)
	@if ! grep -q "alias $(ALIAS)=" $(HOME)/.bashrc 2>/dev/null; then \
		echo "alias $(ALIAS)='$(INSTALL_DIR)'" >> $(HOME)/.bashrc; \
		echo "Alias '$(ALIAS)' adicionado ao ~/.bashrc"; \
	fi
	@if ! grep -q "alias $(ALIAS)=" $(HOME)/.zshrc 2>/dev/null; then \
		echo "alias $(ALIAS)='$(INSTALL_DIR)'" >> $(HOME)/.zshrc 2>/dev/null || true; \
		echo "Alias '$(ALIAS)' adicionado ao ~/.zshrc"; \
	fi
	@echo ""
	@echo "Execute: source ~/.bashrc  (ou source ~/.zshrc)"
	@echo "Depois use: horus help"

uninstall:
	rm -f $(INSTALL_DIR)
	@sed -i '/alias $(ALIAS)=/d' $(HOME)/.bashrc 2>/dev/null || true
	@sed -i '/alias $(ALIAS)=/d' $(HOME)/.zshrc 2>/dev/null || true
	@echo "Alias '$(ALIAS)' removido."

git-init:
	@if [ ! -d .git ]; then \
		git init; \
		git add .; \
		git commit -m "Initial commit: horustec - sistema para tecnicos de PCs"; \
		echo "Git inicializado!"; \
	else \
		echo "Ja e um repositorio git."; \
	fi

git-push:
	@if [ ! -d .git ]; then \
		echo "Execute 'make git-init' primeiro!"; \
		exit 1; \
	fi
	@git remote get-url origin >/dev/null 2>&1 || { \
		echo "Remote nao configurado!"; \
		echo "Configure com: git remote add origin <url-do-repo>"; \
		exit 1; \
	}
	git add .
	git commit -m "Update: $$(date '+%Y-%m-%d %H:%M')" || true
	git push -u origin $$(git branch --show-current)

github-create:
	@gh repo create horustec --public --source=. --push 2>/dev/null || \
		{ echo "Repo pode ja existir ou gh nao esta configurado."; echo "Crie manualmente em github.com e execute: git remote add origin <url>"; }

report:
	@echo "# ============================================" > RELATORIO.md
	@echo "#          HORUSTEC - RELATORIO              " >> RELATORIO.md
	@echo "# ============================================" >> RELATORIO.md
	@echo "" >> RELATORIO.md
	@echo "Versao: 1.0.0" >> RELATORIO.md
	@echo "Atualizado: $(DATE)" >> RELATORIO.md
	@echo "Repositorio: github.com/reneleonreguero/horustec" >> RELATORIO.md
	@echo "" >> RELATORIO.md
	@echo "--- MODULOS ---" >> RELATORIO.md
	@echo "  [OK] Clientes  - CRUD completo" >> RELATORIO.md
	@echo "  [OK] OS        - Ordens de servico" >> RELATORIO.md
	@echo "  [OK] Pec~as    - Controle de estoque" >> RELATORIO.md
	@echo "  [OK] Agenda    - Agendamentos" >> RELATORIO.md
	@echo "" >> RELATORIO.md
	@echo "--- COMANDOS PRINCIPAIS ---" >> RELATORIO.md
	@echo "  horus cliente add -n <nome> [-t tel] [-e email]" >> RELATORIO.md
	@echo "  horus os add -c <id> -e <equip> -p <problema>" >> RELATORIO.md
	@echo "  horus pecas add -n <nome> [-q qtd] [-u custo]" >> RELATORIO.md
	@echo "  horus agenda add -d <data> -h <hora>" >> RELATORIO.md
	@echo "" >> RELATORIO.md
	@echo "--- ULTIMOS COMMITS ---" >> RELATORIO.md
	@$(shell echo '$(GIT_LOG)' | sed 's/^/  /') >> RELATORIO.md 2>/dev/null || true
	@echo "" >> RELATORIO.md
	@echo "Atualizado: $(DATE)" >> RELATORIO.md
	@echo "Relatorio atualizado com sucesso!"

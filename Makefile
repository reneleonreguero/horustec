CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = src
TARGET = horus
ALIAS = horus
INSTALL_DIR = $(HOME)/bin/$(TARGET)
PROJECT_DIR = $(shell pwd)

.PHONY: all clean install uninstall git-init git-push

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

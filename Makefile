CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -I./include
LDFLAGS = -lcurl

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

TARGET = $(BIN_DIR)/weather
VERSION_FILE = VERSION

THIRD_PARTY = $(INCLUDE_DIR)/nlohmann/json.hpp

.DEFAULT_GOAL := all

.PHONY: all clean run install-deps help tag install-hooks version

tag:
	@bash script_tools/version.sh patch

install-hooks:
	@mkdir -p .git/hooks
	@cp script_tools/git-hooks/pre-push .git/hooks/pre-push
	@chmod +x .git/hooks/pre-push
	@echo "Hooks instalados en .git/hooks/"

version:
	@if [ -f $(VERSION_FILE) ]; then \
		echo "Version actual: $$(cat $(VERSION_FILE))"; \
	else \
		echo "Generando $(VERSION_FILE)..."; \
		bash -c ' \
			LAST_TAG=$$(git describe --tags --abbrev=0 2>/dev/null || echo "v1.0.0"); \
			if [[ "$$LAST_TAG" =~ ^v([0-9]+)\.([0-9]+)\.([0-9]+)$$ ]]; then \
				MAJOR=$${BASH_REMATCH[1]}; MINOR=$${BASH_REMATCH[2]}; PATCH=$${BASH_REMATCH[3]}; \
				PATCH=$$((PATCH + 1)); \
				echo "v$$MAJOR.$$MINOR.$$PATCH" > $(VERSION_FILE); \
			else \
				echo "v1.0.1" > $(VERSION_FILE); \
			fi \
		'; \
	fi

all: version $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(THIRD_PARTY) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

install-deps:
	@echo "Instalando dependencias..."
	sudo apt-get update
	sudo apt-get install -y libcurl4-openssl-dev

help:
	@echo "Targets disponibles:"
	@echo "  all         - Compila el proyecto"
	@echo "  run         - Compila y ejecuta"
	@echo "  clean       - Limpia archivos compilados"
	@echo "  install-deps- Instala dependencias del sistema"
	@echo "  tag         - Crea tag semantico y pushea (patch)"
	@echo "  install-hooks- Instala git hooks (pre-push)"
	@echo "  version     - Muestra/actualiza VERSION desde ultimo tag"
	@echo "  help        - Muestra esta ayuda"

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

THIRD_PARTY = $(INCLUDE_DIR)/nlohmann/json.hpp

.PHONY: all clean run install-deps help

all: $(TARGET)

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
	@echo "  help        - Muestra esta ayuda"

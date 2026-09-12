# Compilador e opções de compilação
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude

# Diretórios do projeto
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Arquivos fonte e objetos
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Nome do executável
TARGET := $(BIN_DIR)/8-puzzle

# Regra padrão: compila o projeto
all: $(TARGET)


# Gera o executável a partir dos arquivos objeto
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compila cada arquivo .cpp em um arquivo .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cria o diretório de objetos
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Cria o diretório do executável
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compila e executa o jogo
run: all
	./$(TARGET)

# Remove os arquivos gerados pela compilação
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Define as regras que não são arquivos
.PHONY: all run clean

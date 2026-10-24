# Makefile - Atividade de Implementacao 2 (Polinomios)
# Compilacao no Kubuntu/Linux com G++ e C++11
#
# Estrutura:
#   Makefile
#   src/main.cpp
#   src/polinomio.cpp
#   src/polinomio.h
#
# Arquivos gerados:
#   obj/main.o
#   obj/polinomio.o
#   bin/programa

CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++11 -g

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/programa

SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/polinomio.cpp

OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/polinomio.o

HEADERS = $(SRC_DIR)/polinomio.h

.PHONY: all run clean

# Compila o programa por padrao
all: $(TARGET)

# Gera o executavel
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# Compila main.cpp
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

# Compila polinomio.cpp
$(OBJ_DIR)/polinomio.o: $(SRC_DIR)/polinomio.cpp $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/polinomio.cpp -o $(OBJ_DIR)/polinomio.o

# Cria as pastas necessarias
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Compila e executa o programa
run: $(TARGET)
	./$(TARGET)

# Remove os arquivos gerados
clean:
	rm -f $(OBJECTS) $(TARGET)

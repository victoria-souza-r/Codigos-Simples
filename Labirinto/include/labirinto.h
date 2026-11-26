#ifndef LABIRINTO_H
#define LABIRINTO_H

#include <string>
#include <vector>

// Define os símbolos utilizados na representação do labirinto.
constexpr char PAREDE = 'X';
constexpr char LIVRE = '_';
constexpr char CAMINHO = '*';
constexpr char VISITADO = '.';

// Define as cores utilizadas na exibição do labirinto no terminal.
inline constexpr char COR_CAMINHO[] = "\033[1;33m";
inline constexpr char COR_PAREDE[] = "\033[0;37m";
inline constexpr char COR_RESET[] = "\033[0m";

// Armazena as coordenadas de uma posição.
struct Posicao {
    int linha;
    int coluna;
};

// Armazena as dimensões e a matriz do labirinto.
struct Labirinto {
    int linhas;
    int colunas;
    std::vector<std::string> grade;
};

// Cria um labirinto com as dimensões informadas.
Labirinto criarLabirinto(int linhas, int colunas);

// Verifica se uma posição está dentro dos limites do labirinto.
bool posicaoDentro(const Labirinto& lab, int linha, int coluna);

// Verifica se uma posição contém uma parede.
bool ehParede(const Labirinto& lab, int linha, int coluna);

// Verifica se uma posição está livre.
bool ehLivre(const Labirinto& lab, int linha, int coluna);

// Verifica se a estrutura do labirinto é válida.
bool validarLabirinto(const Labirinto& lab);

// Padroniza os caracteres utilizados na matriz do labirinto.
void normalizarLabirinto(Labirinto& lab);

// Conta a quantidade de posições livres vizinhas.
int contarVizinhosLivres(const Labirinto& lab, int linha, int coluna);

// Verifica se uma posição possui mais de dois vizinhos livres.
bool posicaoComCaminhoDuplo(const Labirinto& lab, int linha, int coluna);

// Verifica se o labirinto possui posições com caminhos duplos.
bool labirintoTemCaminhoDuplo(const Labirinto& lab, Posicao& onde);

// Exibe o labirinto no terminal.
void exibirLabirinto(const Labirinto& lab);

// Remove as marcações temporárias do labirinto.
void limparMarcas(Labirinto& lab);

#endif

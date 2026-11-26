#include "labirinto.h"
#include <iostream>
using namespace std;

// Deslocamentos utilizados para percorrer as posições vizinhas.
// A ordem representa: cima, direita, baixo e esquerda.
static const int VIZ_LINHA[4] = {-1, 0, 1, 0};
static const int VIZ_COLUNA[4] = {0, 1, 0, -1};

// Cria a matriz do labirinto com as dimensões informadas.
// Todas as posições são inicializadas como paredes.
Labirinto criarLabirinto(int linhas, int colunas)
{
    Labirinto lab;

    // Verifica se as dimensões são válidas.
    if (linhas <= 0 || colunas <= 0) {
        lab.linhas = 0;
        lab.colunas = 0;
        return lab;
    }

    lab.linhas = linhas;
    lab.colunas = colunas;
    lab.grade.assign(linhas, string(colunas, PAREDE));

    return lab;
}

// Verifica se as coordenadas estão dentro dos limites da matriz.
bool posicaoDentro(const Labirinto& lab, int linha, int coluna)
{
    return linha >= 0 && linha < lab.linhas &&
           coluna >= 0 && coluna < lab.colunas;
}

// Verifica se a posição informada contém uma parede.
bool ehParede(const Labirinto& lab, int linha, int coluna)
{
    return posicaoDentro(lab, linha, coluna) &&
           lab.grade[linha][coluna] == PAREDE;
}

// Verifica se a posição pode ser percorrida.
// Posições livres, visitadas ou pertencentes ao caminho são consideradas acessíveis.
bool ehLivre(const Labirinto& lab, int linha, int coluna)
{
    if (!posicaoDentro(lab, linha, coluna))
        return false;

    char caractere = lab.grade[linha][coluna];

    return caractere == LIVRE ||
           caractere == CAMINHO ||
           caractere == VISITADO;
}

// Valida as dimensões da matriz e os caracteres utilizados no labirinto.
bool validarLabirinto(const Labirinto& lab)
{
    if (lab.linhas <= 0 || lab.colunas <= 0)
        return false;

    // Confere se a quantidade de linhas corresponde à dimensão declarada.
    if (static_cast<int>(lab.grade.size()) != lab.linhas)
        return false;

    for (const string& linha : lab.grade) {
        // Confere se todas as linhas possuem a mesma quantidade de colunas.
        if (static_cast<int>(linha.size()) != lab.colunas)
            return false;

        // Verifica se cada caractere pertence ao conjunto permitido.
        for (char caractere : linha) {
            if (caractere != PAREDE &&
                caractere != LIVRE &&
                caractere != ' ' &&
                caractere != CAMINHO &&
                caractere != VISITADO) {
                return false;
            }
        }
    }

    return true;
}

// Padroniza a representação do labirinto, substituindo espaços por posições livres.
void normalizarLabirinto(Labirinto& lab)
{
    if (!validarLabirinto(lab))
        return;

    for (int l = 0; l < lab.linhas; l++) {
        for (int c = 0; c < lab.colunas; c++) {
            if (lab.grade[l][c] == ' ')
                lab.grade[l][c] = LIVRE;
        }
    }
}

// Conta quantas das quatro posições vizinhas estão livres.
int contarVizinhosLivres(const Labirinto& lab, int linha, int coluna)
{
    int total = 0;

    for (int d = 0; d < 4; d++) {
        int novaLinha = linha + VIZ_LINHA[d];
        int novaColuna = coluna + VIZ_COLUNA[d];

        if (ehLivre(lab, novaLinha, novaColuna))
            total++;
    }

    return total;
}

// Identifica posições livres que possuem três ou mais vizinhos acessíveis.
bool posicaoComCaminhoDuplo(const Labirinto& lab, int linha, int coluna)
{
    return ehLivre(lab, linha, coluna) &&
           contarVizinhosLivres(lab, linha, coluna) >= 3;
}

// Percorre a matriz procurando posições com três ou mais vizinhos livres.
// Armazena as coordenadas da primeira ocorrência encontrada.
bool labirintoTemCaminhoDuplo(const Labirinto& lab, Posicao& onde)
{
    for (int l = 0; l < lab.linhas; l++) {
        for (int c = 0; c < lab.colunas; c++) {
            if (posicaoComCaminhoDuplo(lab, l, c)) {
                onde.linha = l;
                onde.coluna = c;
                return true;
            }
        }
    }

    return false;
}

// Exibe a matriz do labirinto no terminal.
// Utiliza cores para destacar paredes e posições pertencentes ao caminho.
void exibirLabirinto(const Labirinto& lab)
{
    cout << '\n';

    for (int l = 0; l < lab.linhas; l++) {
        for (int c = 0; c < lab.colunas; c++) {
            char caractere = lab.grade[l][c];

            if (caractere == CAMINHO) {
                cout << COR_CAMINHO << CAMINHO << COR_RESET;
            }
            else if (caractere == PAREDE) {
                cout << COR_PAREDE << PAREDE << COR_RESET;
            }
            else {
                cout << ' ';
            }
        }

        cout << '\n';
    }

    cout << '\n';
}

// Remove as marcações temporárias da resolução.
// As posições do caminho e as visitadas voltam a ser posições livres.
void limparMarcas(Labirinto& lab)
{
    for (int l = 0; l < lab.linhas; l++) {
        for (int c = 0; c < lab.colunas; c++) {
            if (lab.grade[l][c] == CAMINHO ||
                lab.grade[l][c] == VISITADO) {
                lab.grade[l][c] = LIVRE;
            }
        }
    }
}

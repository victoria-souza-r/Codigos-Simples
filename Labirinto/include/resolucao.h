#ifndef RESOLUCAO_H
#define RESOLUCAO_H

#include "labirinto.h"

// Define a quantidade de direções possíveis de movimento.
constexpr int NUM_DIRECOES = 4;

// Vetores que armazenam os deslocamentos nas linhas e colunas.
extern const int DESLOCA_LINHA[NUM_DIRECOES];
extern const int DESLOCA_COLUNA[NUM_DIRECOES];

// Verifica se as posições inicial e final são válidas.
bool posicaoInicialFinalValida(const Labirinto& lab, const Posicao& p);

// Busca recursivamente um caminho até a posição final.
bool buscarCaminho(Labirinto& lab, int l, int c, const Posicao& fim);

// Executa a resolução do labirinto.
bool resolverLabirinto(Labirinto& lab, const Posicao& inicio, const Posicao& fim);

// Conta as posições que fazem parte do caminho encontrado.
int contarPassosCaminho(const Labirinto& lab);

#endif

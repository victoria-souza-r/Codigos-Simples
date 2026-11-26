#include "resolucao.h"

// Vetores que definem os deslocamentos nas quatro direções:
// cima, direita, baixo e esquerda.
const int DESLOCA_LINHA[NUM_DIRECOES] = {-1, 0, 1, 0};
const int DESLOCA_COLUNA[NUM_DIRECOES] = {0, 1, 0, -1};

// Verifica se a posição inicial ou final está dentro do labirinto e é livre.
bool posicaoInicialFinalValida(const Labirinto& lab, const Posicao& p)
{
    return posicaoDentro(lab, p.linha, p.coluna) &&
           lab.grade[p.linha][p.coluna] == LIVRE;
}

// Busca recursivamente um caminho entre a posição atual e o destino.
// Utiliza DFS com Backtracking para explorar as posições disponíveis.
bool buscarCaminho(Labirinto& lab, int l, int c, const Posicao& fim)
{
    // Interrompe a busca se a posição estiver fora dos limites ou não estiver livre.
    if (!posicaoDentro(lab, l, c) ||
        lab.grade[l][c] != LIVRE) {
        return false;
    }

    // Marca a posição atual como visitada para evitar ciclos.
    lab.grade[l][c] = VISITADO;

    // Verifica se o destino foi alcançado.
    if (l == fim.linha && c == fim.coluna) {
        lab.grade[l][c] = CAMINHO;
        return true;
    }

    // Explora recursivamente as quatro direções possíveis.
    for (int d = 0; d < NUM_DIRECOES; d++) {
        int novaLinha = l + DESLOCA_LINHA[d];
        int novaColuna = c + DESLOCA_COLUNA[d];

        // Se encontrar o destino, marca a posição atual como parte do caminho.
        if (buscarCaminho(lab, novaLinha, novaColuna, fim)) {
            lab.grade[l][c] = CAMINHO;
            return true;
        }
    }

    // Retorna falso quando nenhum caminho válido é encontrado a partir da posição.
    return false;
}

// Valida as posições e inicia o processo de resolução do labirinto.
bool resolverLabirinto(Labirinto& lab, const Posicao& inicio, const Posicao& fim)
{
    // Impede a resolução de estruturas inválidas.
    if (!validarLabirinto(lab))
        return false;

    // Remove marcações de tentativas anteriores.
    limparMarcas(lab);

    // Verifica se as posições inicial e final são válidas.
    if (!posicaoInicialFinalValida(lab, inicio) ||
        !posicaoInicialFinalValida(lab, fim)) {
        return false;
    }

    // Inicia a busca recursiva a partir da posição inicial.
    bool achou = buscarCaminho(
        lab,
        inicio.linha,
        inicio.coluna,
        fim
    );

    // Remove as marcações das posições visitadas que não fazem parte do caminho.
    for (int l = 0; l < lab.linhas; l++) {
        for (int c = 0; c < lab.colunas; c++) {
            if (lab.grade[l][c] == VISITADO)
                lab.grade[l][c] = LIVRE;
        }
    }

    return achou;
}

// Conta quantas posições foram marcadas como parte do caminho encontrado.
int contarPassosCaminho(const Labirinto& lab)
{
    int total = 0;

    for (int l = 0; l < lab.linhas; l++) {
        for (int c = 0; c < lab.colunas; c++) {
            if (lab.grade[l][c] == CAMINHO)
                total++;
        }
    }

    return total;
}

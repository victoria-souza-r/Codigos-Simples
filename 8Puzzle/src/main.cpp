#include <iostream>
#include "puzzle.h"

using namespace std;

// Função principal que controla o fluxo do jogo
int main() {
    bool jogarNovamente = true;

    // Laço principal que permite reiniciar o jogo caso o usuário deseje jogar outra partida
    while (jogarNovamente) {

        Tabuleiro atual;
        Tabuleiro objetivo;

        int passo = 0;
        int ultimaRegra = 0;

        bool resolvido = false;
        bool gameOver = false;

        // Posição inicial do cursor no tabuleiro
        int linhaCursor = 0;
        int colunaCursor = 0;

        cout << "============================================\n";
        cout << "           JOGO DO 8-PUZZLE\n";
        cout << "============================================\n";

        // Define a configuração final correta do puzzle
        definirObjetivo(objetivo);

        // Obtém a quantidade de movimentos de acordo com o nível escolhido
        int qtdMovimentos = escolherNivel();
        int maxPassos = qtdMovimentos;

        // Embaralha o tabuleiro a partir do objetivo com base na dificuldade
        embaralharTabuleiro(
            atual,
            objetivo,
            qtdMovimentos
        );

        // Valida se a configuração embaralhada possui solução matemática
        if (ehResolvivel(atual, objetivo)) {
            cout << "\n[Verificacao] Puzzle gerado e soluvel.\n";
        } else {
            cout << "\n[Verificacao] Puzzle gerado NAO e soluvel!\n";
        }

        // Verifica se o tabuleiro já nasceu resolvido
        resolvido = verificarObjetivo(
            atual,
            objetivo
        );

        // Ativa o modo raw do terminal para capturar teclas instantaneamente sem precisar de Enter
        habilitarModoRaw();

        // Desenha a interface inicial do jogo na tela
        desenharJogo(
            atual,
            passo,
            maxPassos,
            linhaCursor,
            colunaCursor,
            ultimaRegra
        );

        // Laço de execução da partida (enquanto não vencer e não estourar o limite de passos)
        while (!resolvido && !gameOver) {

            // Lê a próxima tecla digitada pelo usuário
            int tecla = lerTecla();

            switch (tecla) {

                // Move o cursor para a esquerda, respeitando os limites da matriz (0 a 2)
                case TECLA_ESQUERDA:

                    if (colunaCursor > 0)
                        colunaCursor--;

                    break;

                // Move o cursor para cima
                case TECLA_CIMA:

                    if (linhaCursor > 0)
                        linhaCursor--;

                    break;

                // Move o cursor para a direita
                case TECLA_DIREITA:

                    if (colunaCursor < TAM - 1)
                        colunaCursor++;

                    break;

                // Move o cursor para baixo
                case TECLA_BAIXO:

                    if (linhaCursor < TAM - 1)
                        linhaCursor++;

                    break;

                // Tenta mover a peça selecionada para o espaço vazio 
                case TECLA_SELECIONAR: {

                    int regra = moverPecaSelecionada(
                        atual,
                        linhaCursor,
                        colunaCursor
                    );

                    // Se o movimento foi válido (retornou uma regra de 1 a 4)
                    if (regra != 0) {

                        ultimaRegra = regra;

                        passo++; // Incrementa a contagem de jogadas efetuadas

                        // Checa se o tabuleiro atual atingiu o objetivo após a jogada
                        resolvido = verificarObjetivo(
                            atual,
                            objetivo
                        );

                        // Se esgotou os passos permitidos sem resolver, ativa o Game Over
                        if (
                            !resolvido &&
                            passo >= maxPassos
                        ) {
                            gameOver = true;
                        }
                    }

                    break;
                }

                // Encerra a execução do programa imediatamente
                case TECLA_SAIR:

                    desabilitarModoRaw();

                    limparTela();

                    cout << "Jogo encerrado pelo usuario.\n";

                    return 0;

                default:
                    continue;
            }

            // Atualiza e redesenha o tabuleiro e o status na tela após cada ação
            desenharJogo(
                atual,
                passo,
                maxPassos,
                linhaCursor,
                colunaCursor,
                ultimaRegra
            );
        }

        // Desativa o modo raw antes de exibir mensagens finais para restaurar o comportamento normal do terminal
        desabilitarModoRaw();

        cout << "\n============================================\n";

        // Exibe mensagem de vitória ou derrota de acordo com o estado final
        if (resolvido) {

            cout << "PARABENS! Voce resolveu o 8-Puzzle em "
                 << passo
                 << " passo(s)!\n";

        } else {

            cout << "GAME OVER! Voce atingiu o limite de "
                 << maxPassos
                 << " jogadas sem resolver o puzzle.\n";
        }

        cout << "============================================\n";

        // Pergunta se o jogador quer iniciar uma nova partida
        jogarNovamente = perguntarJogarNovamente();
    }

    cout << "\nAte a proxima!\n";

    return 0;
}

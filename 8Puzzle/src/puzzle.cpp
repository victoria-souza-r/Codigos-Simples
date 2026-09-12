#include <iostream>
#include <utility>      // std::swap
#include <cstdlib>      // rand e srand
#include <ctime>        // time

// Bibliotecas para controle do terminal e teclado no Linux
#include <termios.h>    // Configuração do terminal
#include <unistd.h>     // read e STDIN_FILENO
#include <sys/select.h> // Leitura das teclas com timeout
#include "puzzle.h"     // Estruturas, constantes e funções do jogo

using namespace std;

// Configura o tabuleiro com a disposição final 
void definirObjetivo(Tabuleiro tab) {
    char valores[TAM][TAM] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', VAZIO}
    };

    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            tab[i][j] = valores[i][j];
}

// Embaralha o tabuleiro a partir da matriz objetivo aplicando movimentos aleatórios válidos
void embaralharTabuleiro(
    Tabuleiro tab,
    const Tabuleiro objetivo,
    int qtdMovimentos
) {
    copiarTabuleiro(objetivo, tab);

    srand((unsigned int)time(nullptr));

    int ultimoMovimento = -1;

    for (int i = 0; i < qtdMovimentos; i++) {
        int direcao;
        bool aplicado = false;

        do {
            direcao = rand() % 4;

            switch (direcao) {

                case 0:
                    // Evita desfazer imediatamente o movimento anterior (esquerda/direita)
                    if (ultimoMovimento != 2)
                        aplicado = moverEsquerda(tab);
                    break;

                case 1:
                    // Evita desfazer o movimento anterior (cima/baixo)
                    if (ultimoMovimento != 3)
                        aplicado = moverCima(tab);
                    break;

                case 2:
                    if (ultimoMovimento != 0)
                        aplicado = moverDireita(tab);
                    break;

                case 3:
                    if (ultimoMovimento != 1)
                        aplicado = moverBaixo(tab);
                    break;
            }

        } while (!aplicado);

        ultimoMovimento = direcao;
    }
}

// Lê, valida e retorna o número de movimentos iniciais com base na dificuldade escolhida
int escolherNivel() {
    int opcao = 0;

    do {
        cout << "\nEscolha o nivel de dificuldade:\n";
        cout << "1 - Facil   (10 movimentos)\n";
        cout << "2 - Medio   (30 movimentos)\n";
        cout << "3 - Dificil (60 movimentos)\n";
        cout << "Opcao: ";

        cin >> opcao;

        // Trata entradas inválidas (letras ou símbolos) no fluxo do cin
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            opcao = 0;
        }

        if (opcao < 1 || opcao > 3) {
            cout << "Opcao invalida! Digite 1, 2 ou 3.\n";
        }

    } while (opcao < 1 || opcao > 3);

    switch (opcao) {
        case 1:
            return 10;

        case 2:
            return 30;

        default:
            return 60;
    }
}

// Desenha o tabuleiro no terminal
void imprimirTabuleiro(
    const Tabuleiro tab,
    int linhaCursor,
    int colunaCursor
) {
    cout << "\n+---+---+---+\n";

    for (int i = 0; i < TAM; i++) {
        cout << "|";

        for (int j = 0; j < TAM; j++) {

            if (i == linhaCursor && j == colunaCursor) {
                cout << "[" << tab[i][j] << "]|";
            } else {
                cout << " " << tab[i][j] << " |";
            }
        }

        cout << "\n+---+---+---+\n";
    }
}

// Atualiza a tela limpando o console e imprimindo o cabeçalho, contadores de jogadas e instruções
void desenharJogo(
    const Tabuleiro tab,
    int passo,
    int maxPassos,
    int linhaCursor,
    int colunaCursor,
    int ultimaRegra
) {
    limparTela();

    cout << "============================================\n";
    cout << "           JOGO DO 8-PUZZLE\n";
    cout << "============================================\n";

    cout << "Jogadas: " << passo << "/" << maxPassos << "\n";

    if (ultimaRegra != 0) {
        cout << "Ultima regra aplicada: ";

        switch (ultimaRegra) {

            case 1:
                cout << "R1 - mover a lacuna para a esquerda\n";
                break;

            case 2:
                cout << "R2 - mover a lacuna para cima\n";
                break;

            case 3:
                cout << "R3 - mover a lacuna para a direita\n";
                break;

            case 4:
                cout << "R4 - mover a lacuna para baixo\n";
                break;
        }

    } else {
        cout << "Ultima regra aplicada: nenhuma\n";
    }

    imprimirTabuleiro(tab, linhaCursor, colunaCursor);

    cout << "\nSetas/WASD: mover o cursor [ ]\n";
    cout << "ESPACO ou ENTER: mover a peca selecionada para a lacuna\n";
    cout << "ESC ou Q: sair\n";
}

// Pergunta se o usuário deseja reiniciar a partida e retorna true para sim
bool perguntarJogarNovamente() {
    char resposta;

    cout << "\nDeseja jogar novamente? (S/N): ";
    cin >> resposta;

    return (resposta == 's' || resposta == 'S');
}


// Controle de terminal / teclado
static struct termios configOriginal;

// Ativa o modo de leitura direta do teclado
void habilitarModoRaw() {
    struct termios raw;

    tcgetattr(STDIN_FILENO, &configOriginal);

    raw = configOriginal;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Restaura as configurações originais do terminal
void desabilitarModoRaw() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &configOriginal);
}

// Limpa a tela
void limparTela() {
    cout << "\033[3J\033[2J\033[H";
    cout.flush();
}

// Lê entradas do teclado de forma assíncrona, tratando setas direcionais, teclas WASD e atalhos
int lerTecla() {
    char c;

    read(STDIN_FILENO, &c, 1);

    // Verifica se foi pressionada uma tecla de controle
    if (c == 27) {
        fd_set fds;
        struct timeval tv;

        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        tv.tv_sec = 0;
        tv.tv_usec = 50000; // Timeout curto para verificar se é uma seta ou a tecla ESC pura

        if (select(
            STDIN_FILENO + 1,
            &fds,
            NULL,
            NULL,
            &tv
        ) > 0) {

            char seq[2];

            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);

            if (seq[0] == '[') {
                switch (seq[1]) {

                    case 'A':
                        return TECLA_CIMA;

                    case 'B':
                        return TECLA_BAIXO;

                    case 'C':
                        return TECLA_DIREITA;

                    case 'D':
                        return TECLA_ESQUERDA;
                }
            }

            return TECLA_INVALIDA;

        } else {
            return TECLA_SAIR; // Pressionou apenas ESC
        }
    }

    switch (c) {

        case 'a':
        case 'A':
            return TECLA_ESQUERDA;

        case 'w':
        case 'W':
            return TECLA_CIMA;

        case 'd':
        case 'D':
            return TECLA_DIREITA;

        case 's':
        case 'S':
            return TECLA_BAIXO;

        case ' ':
        case '\n':
        case '\r':
            return TECLA_SELECIONAR;

        case 'q':
        case 'Q':
            return TECLA_SAIR;

        default:
            return TECLA_INVALIDA;
    }
}

// Varre a matriz em busca do caractere vazio e retorna sua posição por referência
void encontrarLacuna(
    const Tabuleiro tab,
    int &linha,
    int &coluna
) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {

            if (tab[i][j] == VAZIO) {
                linha = i;
                coluna = j;
                return;
            }
        }
    }

    linha = -1;
    coluna = -1;
}

// Move a lacuna para a esquerda trocando-a de posição com a peça adjacente
bool moverEsquerda(Tabuleiro tab) {
    int linha, coluna;

    encontrarLacuna(tab, linha, coluna);

    if (coluna == 0)
        return false;

    swap(
        tab[linha][coluna],
        tab[linha][coluna - 1]
    );

    return true;
}

// Move a lacuna para cima trocando-a com a peça superior
bool moverCima(Tabuleiro tab) {
    int linha, coluna;

    encontrarLacuna(tab, linha, coluna);

    if (linha == 0)
        return false;

    swap(
        tab[linha][coluna],
        tab[linha - 1][coluna]
    );

    return true;
}

// Move a lacuna para a direita trocando-a com a peça adjacente
bool moverDireita(Tabuleiro tab) {
    int linha, coluna;

    encontrarLacuna(tab, linha, coluna);

    if (coluna == TAM - 1)
        return false;

    swap(
        tab[linha][coluna],
        tab[linha][coluna + 1]
    );

    return true;
}

// Move a lacuna para baixo trocando-a com a peça inferior
bool moverBaixo(Tabuleiro tab) {
    int linha, coluna;

    encontrarLacuna(tab, linha, coluna);

    if (linha == TAM - 1)
        return false;

    swap(
        tab[linha][coluna],
        tab[linha + 1][coluna]
    );

    return true;
}

// Verifica se a peça selecionada é adjacente à lacuna e executa o movimento correspondente
int moverPecaSelecionada(
    Tabuleiro tab,
    int linhaCursor,
    int colunaCursor
) {
    int linhaLacuna, colunaLacuna;

    encontrarLacuna(
        tab,
        linhaLacuna,
        colunaLacuna
    );

    // Peça acima da lacuna
    if (
        linhaCursor == linhaLacuna - 1 &&
        colunaCursor == colunaLacuna
    ) {
        moverCima(tab);
        return 2;
    }

    // Peça abaixo da lacuna
    if (
        linhaCursor == linhaLacuna + 1 &&
        colunaCursor == colunaLacuna
    ) {
        moverBaixo(tab);
        return 4;
    }

    // Peça à esquerda da lacuna
    if (
        colunaCursor == colunaLacuna - 1 &&
        linhaCursor == linhaLacuna
    ) {
        moverEsquerda(tab);
        return 1;
    }

    // Peça à direita da lacuna
    if (
        colunaCursor == colunaLacuna + 1 &&
        linhaCursor == linhaLacuna
    ) {
        moverDireita(tab);
        return 3;
    }

    return 0;
}

// Compara todas as posições do tabuleiro atual com o objetivo para verificar vitória
bool verificarObjetivo(
    const Tabuleiro atual,
    const Tabuleiro objetivo
) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {

            if (atual[i][j] != objetivo[i][j])
                return false;
        }
    }

    return true;
}

// Analisa se o puzzle embaralhado possui solução matemática com base na contagem de inversões
bool ehResolvivel(
    const Tabuleiro atual,
    const Tabuleiro objetivo
) {
    const int QTD_PECAS = TAM * TAM - 1;

    char sequenciaAtual[QTD_PECAS];
    char sequenciaObjetivo[QTD_PECAS];

    int qtd = 0;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {

            if (objetivo[i][j] != VAZIO)
                sequenciaObjetivo[qtd++] = objetivo[i][j];
        }
    }

    qtd = 0;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {

            if (atual[i][j] != VAZIO)
                sequenciaAtual[qtd++] = atual[i][j];
        }
    }

    int rank[QTD_PECAS];

    for (int i = 0; i < QTD_PECAS; i++) {

        for (int k = 0; k < QTD_PECAS; k++) {

            if (sequenciaAtual[i] == sequenciaObjetivo[k]) {
                rank[i] = k;
                break;
            }
        }
    }

    int inversoes = 0;

    for (int i = 0; i < QTD_PECAS; i++) {

        for (int j = i + 1; j < QTD_PECAS; j++) {

            if (rank[i] > rank[j])
                inversoes++;
        }
    }

    return (inversoes % 2 == 0);
}

// Copia o conteúdo de uma matriz tabuleiro para outra posição de memória
void copiarTabuleiro(
    const Tabuleiro origem,
    Tabuleiro destino
) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {

            destino[i][j] = origem[i][j];
        }
    }
}

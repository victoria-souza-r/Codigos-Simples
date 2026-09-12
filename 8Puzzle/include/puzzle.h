#ifndef PUZZLE_H
#define PUZZLE_H

const int TAM = 3;             // Tamanho do tabuleiro
const char VAZIO = ' ';        // Representa o espaço vazio 

// Tipo que representa o tabuleiro
typedef char Tabuleiro[TAM][TAM];

// Códigos de controle para as teclas de entrada do usuário
const int TECLA_ESQUERDA   = 1;
const int TECLA_CIMA       = 2;
const int TECLA_DIREITA    = 3;
const int TECLA_BAIXO      = 4;
const int TECLA_SELECIONAR = 5;
const int TECLA_SAIR       = 0;
const int TECLA_INVALIDA   = -1;

// Define o tabuleiro objetivo
void definirObjetivo(Tabuleiro tab);

// Embaralha o tabuleiro a partir do objetivo com base em uma quantidade de movimentos
void embaralharTabuleiro(Tabuleiro tab, const Tabuleiro objetivo, int qtdMovimentos);

//  Permite escolher o nível
int escolherNivel();

// Exibe o tabuleiro na tela
void imprimirTabuleiro(const Tabuleiro tab, int linhaCursor, int colunaCursor);

// Desenha a interface completa do jogo (incluindo status, passos e mensagens)
void desenharJogo(
    const Tabuleiro tab,
    int passo,
    int maxPassos,
    int linhaCursor,
    int colunaCursor,
    int ultimaRegra
);

// Pergunta se o jogador quer jogar novamente
bool perguntarJogarNovamente();

// Ativa a leitura direta do teclado
void habilitarModoRaw();

// Desativa a leitura direta do teclado
void desabilitarModoRaw();

//  Lê uma tecla
int lerTecla();

void limparTela();

// Encontra e retorna por referência as coordenadas (linha e coluna) do espaço vazio
void encontrarLacuna(const Tabuleiro tab, int &linha, int &coluna);

// Move a peça para a esquerda em direção à lacuna (retorna verdadeiro se o movimento for válido)
bool moverEsquerda(Tabuleiro tab);

// Move a peça para cima em direção à lacuna
bool moverCima(Tabuleiro tab);

// Move a peça para direita em direção à lacuna
bool moverDireita(Tabuleiro tab);

// Move a peça para baixo em direção à lacuna
bool moverBaixo(Tabuleiro tab);

// Executa o movimento da peça selecionada com base na posição atual do cursor
int moverPecaSelecionada(Tabuleiro tab, int linhaCursor, int colunaCursor);

// Verifica se o tabuleiro atual é idêntico ao tabuleiro objetivo 
bool verificarObjetivo(const Tabuleiro atual, const Tabuleiro objetivo);

// Verifica se a configuração embaralhada do tabuleiro possui solução matemática
bool ehResolvivel(const Tabuleiro atual, const Tabuleiro objetivo);

// Copia o conteúdo de um tabuleiro de origem para um tabuleiro de destino
void copiarTabuleiro(const Tabuleiro origem, Tabuleiro destino);

#endif

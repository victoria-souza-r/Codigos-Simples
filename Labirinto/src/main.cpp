#include <iostream>
#include <sstream>
#include <string>

#include "labirinto.h"
#include "arquivos.h"
#include "resolucao.h"
using namespace std;

// Lê uma linha de entrada, ignorando entradas vazias.
static bool lerLinha(const string& mensagem, string& entrada)
{
    cout << mensagem;

    do {
        if (!getline(cin, entrada))
            return false;

        // Remove o caractere de retorno de carro, quando presente.
        if (!entrada.empty() && entrada.back() == '\r')
            entrada.pop_back();

    } while (entrada.empty());

    return true;
}

// Lê um número inteiro e verifica se está dentro do intervalo permitido.
static bool lerInteiroEntre(const string& mensagem, int minimo,
                            int maximo, int& valor)
{
    string entrada;

    while (true) {
        if (!lerLinha(mensagem, entrada))
            return false;

        istringstream iss(entrada);
        char extra;

        // Valida o número e impede caracteres adicionais na entrada.
        if ((iss >> valor) && !(iss >> extra) &&
            valor >= minimo && valor <= maximo) {
            return true;
        }

        cout << "Valor invalido. Digite um numero entre "
             << minimo << " e " << maximo << ".\n";
    }
}

// Verifica se o usuário confirmou a operação com 's' ou 'S'.
static bool confirmar(const string& mensagem)
{
    string resposta;

    if (!lerLinha(mensagem, resposta))
        return false;

    return resposta == "s" || resposta == "S";
}

// Solicita as coordenadas de uma posição e verifica se ela é válida.
static bool pedirPosicao(const string& titulo, const Labirinto& lab,
                         Posicao& p)
{
    cout << "\n" << titulo << "\n";

    while (true) {
        int linha, coluna;

        // Solicita a linha dentro dos limites do labirinto.
        if (!lerInteiroEntre(
                "  Linha (1 a " + to_string(lab.linhas) + "): ",
                1, lab.linhas, linha)) {
            return false;
        }

        // Solicita a coluna dentro dos limites do labirinto.
        if (!lerInteiroEntre(
                "  Coluna (1 a " + to_string(lab.colunas) + "): ",
                1, lab.colunas, coluna)) {
            return false;
        }

        // Converte as coordenadas para índices iniciados em zero.
        p.linha = linha - 1;
        p.coluna = coluna - 1;

        if (posicaoInicialFinalValida(lab, p))
            return true;

        cout << "A posicao informada e uma parede ou nao esta livre. "
             << "Escolha outra posicao.\n";
    }
}

// Realiza a leitura de um labirinto armazenado em arquivo e sua resolução.
static void resolverDeArquivo()
{
    // Solicita o nome do arquivo que contém o labirinto.
    string nome = pedirNomeArquivo(
        "Nome do arquivo do labirinto: "
    );

    if (nome.empty())
        return;

    Labirinto lab;

    // Lê o arquivo e identifica possíveis erros.
    StatusArquivo status = lerLabirinto(nome, lab);

    if (status == ARQ_NAO_ABRIU) {
        cout << "Erro: nao foi possivel abrir o arquivo '"
             << nome << "'.\n";
        return;
    }

    if (status == ARQ_FORMATO_INVALIDO) {
        cout << "Erro: o arquivo '" << nome
             << "' nao esta no formato esperado.\n";
        return;
    }

    // Exibe o labirinto carregado e suas dimensões.
    cout << "\nLabirinto lido ("
         << lab.linhas << " x " << lab.colunas << "):\n";

    exibirLabirinto(lab);

    // Permite realizar novas tentativas com diferentes posições.
    do {
        Posicao inicio, fim;

        if (!pedirPosicao("Posicao de INICIO:", lab, inicio))
            return;

        if (!pedirPosicao("Posicao de FIM:", lab, fim))
            return;

        // Executa o algoritmo de busca do caminho.
        if (resolverLabirinto(lab, inicio, fim)) {
            cout << "\nCaminho encontrado! ("
                 << contarPassosCaminho(lab)
                 << " posicoes, incluindo inicio e fim)\n";
        }
        else {
            cout << "\nNao existe caminho entre as posicoes informadas.\n";
        }

        // Exibe o resultado e remove as marcações da tentativa.
        exibirLabirinto(lab);
        limparMarcas(lab);

    } while (confirmar("Resolver com outras posicoes? (s/n): "));
}

// Ponto de entrada do programa e responsável pelo menu principal.
int main()
{
    int opcao;

    cout << "=== LABIRINTO ===\n";

    do {
        cout << "\n1 - Resolver labirinto de um arquivo\n"
             << "2 - Criar novo labirinto\n"
             << "0 - Sair\n";

        // Valida a opção selecionada pelo usuário.
        if (!lerInteiroEntre("Opcao: ", 0, 2, opcao))
            break;

        // Executa a operação correspondente à opção escolhida.
        switch (opcao) {
            case 1:
                resolverDeArquivo();
                break;

            case 2:
                criarESalvarLabirinto();
                break;

            case 0:
                break;
        }

    } while (opcao != 0);

    cout << "\nAte logo!\n";

    return 0;
}

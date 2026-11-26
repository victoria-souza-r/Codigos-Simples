#include "arquivos.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Remove o caractere CR do final da linha.
static void removerCR(string& texto)
{
    if (!texto.empty() && texto.back() == '\r')
        texto.pop_back();
}

// Remove espaços e tabulações do início e do final do texto.
static string aparar(const string& texto)
{
    size_t inicio = texto.find_first_not_of(" \t");

    if (inicio == string::npos)
        return "";

    size_t fim = texto.find_last_not_of(" \t");

    return texto.substr(inicio, fim - inicio + 1);
}

// Solicita e valida um número inteiro positivo.
static int lerInteiroPositivo(const string& mensagem)
{
    string entrada;

    while (true) {
        cout << mensagem;

        do {
            if (!getline(cin, entrada))
                return 0;

            removerCR(entrada);

        } while (entrada.empty());

        istringstream iss(entrada);

        int valor;
        char extra;

        if ((iss >> valor) && !(iss >> extra) && valor > 0)
            return valor;

        cout << "Valor invalido. Digite um numero inteiro maior que zero.\n";
    }
}

// Verifica o tamanho e os caracteres permitidos em uma linha digitada.
static bool linhaValida(const string& linha, int colunas, string& motivo)
{
    if (static_cast<int>(linha.size()) != colunas) {
        ostringstream mensagem;

        mensagem << "A linha deve ter exatamente " << colunas
                 << " caracteres. Voce digitou " << linha.size() << ".";

        motivo = mensagem.str();
        return false;
    }

    for (char caractere : linha) {
        if (caractere != PAREDE &&
            caractere != LIVRE &&
            caractere != ' ') {

            motivo = "Use somente X (parede), _ ou espaco (livre).";
            return false;
        }
    }

    return true;
}

// Verifica se a linha lida do arquivo contém apenas caracteres permitidos.
static bool linhaArquivoValida(const string& linha)
{
    for (char caractere : linha) {
        if (caractere != PAREDE &&
            caractere != LIVRE &&
            caractere != ' ') {
            return false;
        }
    }

    return true;
}

// Abre, lê e valida um labirinto armazenado em arquivo.
StatusArquivo lerLabirinto(const string& nomeArquivo, Labirinto& lab)
{
    ifstream arq(nomeArquivo);

    if (!arq.is_open())
        return ARQ_NAO_ABRIU;

    int linhas, colunas;

    if (!(arq >> linhas >> colunas) ||
        linhas <= 0 || colunas <= 0) {
        return ARQ_FORMATO_INVALIDO;
    }

    string resto;

    getline(arq, resto);
    removerCR(resto);

    if (resto.find_first_not_of(" \t") != string::npos)
        return ARQ_FORMATO_INVALIDO;

    Labirinto lido = criarLabirinto(linhas, colunas);

    // Lê e verifica cada linha da matriz.
    for (int l = 0; l < linhas; l++) {
        string linha;

        if (!getline(arq, linha))
            return ARQ_FORMATO_INVALIDO;

        removerCR(linha);

        if (static_cast<int>(linha.size()) > colunas) {
            if (linha.find_first_not_of(' ', colunas) != string::npos)
                return ARQ_FORMATO_INVALIDO;

            linha.erase(colunas);
        }

        if (static_cast<int>(linha.size()) != colunas)
            return ARQ_FORMATO_INVALIDO;

        if (!linhaArquivoValida(linha))
            return ARQ_FORMATO_INVALIDO;

        lido.grade[l] = linha;
    }

    // Padroniza e valida a matriz antes de armazená-la.
    normalizarLabirinto(lido);

    if (!validarLabirinto(lido))
        return ARQ_FORMATO_INVALIDO;

    lab = lido;

    return ARQ_OK;
}

// Valida e grava o labirinto em um arquivo de texto.
StatusArquivo salvarLabirinto(const string& nomeArquivo, const Labirinto& lab)
{
    if (!validarLabirinto(lab))
        return ARQ_FORMATO_INVALIDO;

    ofstream arq(nomeArquivo);

    if (!arq.is_open())
        return ARQ_NAO_ABRIU;

    // Grava as dimensões do labirinto.
    arq << lab.linhas << '\n';
    arq << lab.colunas << '\n';

    // Grava as linhas, removendo marcações temporárias.
    for (int l = 0; l < lab.linhas; l++) {
        string linha = lab.grade[l];

        for (char& caractere : linha) {
            if (caractere == CAMINHO ||
                caractere == VISITADO) {
                caractere = LIVRE;
            }
        }

        arq << linha << '\n';
    }

    arq.flush();

    if (!arq)
        return ARQ_NAO_ABRIU;

    return ARQ_OK;
}

// Solicita o nome do arquivo e adiciona a pasta padrão quando necessário.
string pedirNomeArquivo(const string& mensagem)
{
    string nome;

    cout << mensagem;

    do {
        if (!getline(cin, nome))
            return "";

        nome = aparar(nome);

    } while (nome.empty());

    if (nome.find('/') == string::npos &&
        nome.find('\\') == string::npos) {
        nome = PASTA_LABIRINTOS + nome;
    }

    return nome;
}

// Recebe as dimensões e os caracteres do labirinto pelo teclado.
bool lerLabirintoDoTeclado(Labirinto& lab)
{
    while (true) {
        int linhas = lerInteiroPositivo("Numero de linhas: ");

        if (linhas == 0)
            return false;

        int colunas = lerInteiroPositivo("Numero de colunas: ");

        if (colunas == 0)
            return false;

        cout << "\nDigite cada linha do labirinto.\n";
        cout << "Use X para parede e _ ou espaco para caminho livre.\n\n";

        Labirinto novo = criarLabirinto(linhas, colunas);

        for (int l = 0; l < linhas; l++) {
            string entrada;
            string motivo;

            // Solicita novamente a linha caso ela seja inválida.
            while (true) {
                cout << "Linha " << (l + 1)
                     << "/" << linhas << ": ";

                if (!getline(cin, entrada))
                    return false;

                removerCR(entrada);

                if (linhaValida(entrada, colunas, motivo))
                    break;

                cout << "Linha invalida: " << motivo << "\n";
            }

            novo.grade[l] = entrada;
        }

        normalizarLabirinto(novo);

        if (!validarLabirinto(novo))
            return false;

        // Verifica se existem posições com caminhos duplos.
        Posicao onde;

        if (labirintoTemCaminhoDuplo(novo, onde)) {
            cout << "\nCaminho duplo encontrado na linha "
                 << (onde.linha + 1)
                 << ", coluna " << (onde.coluna + 1) << ".\n";

            exibirLabirinto(novo);

            string resposta;

            cout << "Deseja digitar o labirinto novamente? (s/n): ";

            if (!getline(cin, resposta))
                return false;

            resposta = aparar(resposta);

            if (resposta == "s" || resposta == "S")
                continue;

            return false;
        }

        lab = novo;

        return true;
    }
}

// Cria um labirinto pelo teclado e salva no arquivo informado.
bool criarESalvarLabirinto()
{
    Labirinto lab;

    if (!lerLabirintoDoTeclado(lab)) {
        cout << "Criacao de labirinto cancelada.\n";
        return false;
    }

    string nome = pedirNomeArquivo(
        "Nome do arquivo para salvar o labirinto: "
    );

    if (nome.empty()) {
        cout << "Criacao de labirinto cancelada.\n";
        return false;
    }

    if (salvarLabirinto(nome, lab) != ARQ_OK) {
        cout << "Erro: nao foi possivel gravar o arquivo '"
             << nome << "'.\n";

        return false;
    }

    cout << "Labirinto salvo em '" << nome << "'.\n";

    return true;
}

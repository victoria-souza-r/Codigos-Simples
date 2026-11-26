#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <string>
#include "labirinto.h"

// Define os possíveis resultados das operações com arquivos.
enum StatusArquivo {
    ARQ_OK,
    ARQ_NAO_ABRIU,
    ARQ_FORMATO_INVALIDO
};

// Diretório padrão onde os labirintos são armazenados.
inline const std::string PASTA_LABIRINTOS = "labirintos/testes/";

// Lê um labirinto a partir de um arquivo.
StatusArquivo lerLabirinto(const std::string& nomeArquivo, Labirinto& lab);

// Salva um labirinto em um arquivo.
StatusArquivo salvarLabirinto(const std::string& nomeArquivo, const Labirinto& lab);

// Solicita ao usuário o nome do arquivo.
std::string pedirNomeArquivo(const std::string& mensagem);

// Lê e valida um labirinto informado pelo usuário.
bool lerLabirintoDoTeclado(Labirinto& lab);

// Cria um novo labirinto e realiza seu salvamento.
bool criarESalvarLabirinto();

#endif

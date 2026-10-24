#include <iostream>
#include <limits>
#include <string>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include "polinomio.h"
using namespace std;


// FUNCOES AUXILIARES DE ENTRADA                   
// Descarta o restante da linha digitada.
static void limparEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Limpa a tela do terminal no Linux.
static void limparTela() {
    cout << "\033[2J\033[H" << flush;
}

// Aguarda o usuário antes de voltar ao menu.
static void pausarTela() {
    cout << "\nPressione ENTER para voltar ao menu...";
    cin.get();
}

// Lê um número inteiro.
static int lerInteiro(const string& mensagem) {
    int valor;

    while (true) {
        cout << mensagem;

        if (cin >> valor) {
            limparEntrada();
            return valor;
        }

        if (cin.eof()) {
            throw runtime_error("Entrada encerrada.");
        }

        limparEntrada();
        cout << "Valor invalido. Tente novamente." << endl;
    }
}

// Lê um número real finito.
static double lerReal(const string& mensagem) {
    double valor;

    while (true) {
        cout << mensagem;

        if (cin >> valor) {
            limparEntrada();

            if (isfinite(valor)) {
                return valor;
            }

            cout << "Digite um numero real finito." << endl;
            continue;
        }

        if (cin.eof()) {
            throw runtime_error("Entrada encerrada.");
        }

        limparEntrada();
        cout << "Valor invalido. Tente novamente." << endl;
    }
}

// Lê um caractere.
static char lerCaractere(const string& mensagem) {
    char valor;

    while (true) {
        cout << mensagem;

        if (cin >> valor) {
            limparEntrada();
            return valor;
        }

        if (cin.eof()) {
            throw runtime_error("Entrada encerrada.");
        }

        limparEntrada();
        cout << "Caractere invalido. Tente novamente." << endl;
    }
}

// FUNCOES DE MANIPULACAO DOS POLINOMIOS                              
// Lê os monômios e substitui o conteúdo anterior do polinômio.
static void lerPolinomio(Polinomio& p) {
    int quantidade;

    do {
        quantidade = lerInteiro("Quantidade de monomios: ");

        if (quantidade < 0) {
            cout << "A quantidade nao pode ser negativa." << endl;
        }
    } while (quantidade < 0);

    // Cria um novo polinômio antes de substituir o anterior.
    Polinomio novo = criarPolinomio(p.incognita);

    try {
        for (int i = 1; i <= quantidade; i++) {
            cout << "\n--- Monomio " << i << " ---" << endl;

            double coeficiente = lerReal("Coeficiente: ");

            int expoente;

            do {
                expoente = lerInteiro(
                    "Expoente (inteiro >= 0): "
                );

                if (expoente < 0) {
                    cout << "O expoente nao pode ser negativo."
                         << endl;
                }
            } while (expoente < 0);

            Monomio m = criarMonomio(
                coeficiente,
                p.incognita,
                expoente
            );

            adicionarMonomio(novo, m);
        }

        normalizarPolinomio(novo);

    } catch (...) {
        destruirPolinomio(novo);
        throw;
    }

    // Substitui o polinômio anterior após concluir a leitura.
    destruirPolinomio(p);
    p = novo;
}

// Exibe um polinômio com um rótulo.
static void exibirComRotulo(
    const string& rotulo,
    const Polinomio& p
) {
    cout << rotulo << " = ";
    mostrarPolinomio(p);
    cout << endl;
}

// Escolhe entre os polinômios A e B.
static Polinomio& escolherPolinomio(
    Polinomio& a,
    Polinomio& b
) {
    char opcao;

    do {
        opcao = lerCaractere("Escolha o polinomio (A/B): ");

        if (opcao != 'A' && opcao != 'a' &&
            opcao != 'B' && opcao != 'b') {
            cout << "Opcao invalida. Digite A ou B." << endl;
        }
    } while (opcao != 'A' && opcao != 'a' &&
             opcao != 'B' && opcao != 'b');

    if (opcao == 'A' || opcao == 'a') {
        return a;
    }

    return b;
}

// Exibe o resultado e libera a memória utilizada.
static void exibirResultado(Polinomio& resultado) {
    exibirComRotulo("Resultado", resultado);
    destruirPolinomio(resultado);
}

// Exibe o menu principal.
static void mostrarMenu() {
    cout << "========== POLINOMIOS ==========" << endl;
    cout << "1 - Ler polinomio A" << endl;
    cout << "2 - Ler polinomio B" << endl;
    cout << "3 - Mostrar A e B" << endl;
    cout << "4 - Calcular valor numerico" << endl;
    cout << "5 - Somar A + B" << endl;
    cout << "6 - Subtrair A - B" << endl;
    cout << "7 - Multiplicar por constante" << endl;
    cout << "8 - Multiplicar por monomio" << endl;
    cout << "9 - Multiplicar A * B" << endl;
    cout << "0 - Sair" << endl;
    cout << "================================" << endl;
}

// Executa uma operação e apresenta possíveis erros.
template <typename Funcao>
static void executarOperacao(Funcao operacao) {
    try {
        operacao();
    } catch (const exception& erro) {
        cout << "\nNao foi possivel executar a operacao."
             << endl;
        cout << "Motivo: " << erro.what() << endl;
    }
}


// PROGRAMA PRINCIPAL                                               
int main() {
    Polinomio a{};
    Polinomio b{};

    try {
        cout << "Manipulacao de polinomios de uma incognita"
             << endl;

        char incognita;

        do {
            incognita = lerCaractere(
                "Informe a incognita (ex.: x): "
            );

            if (!isalpha(static_cast<unsigned char>(incognita))) {
                cout << "Utilize uma letra como incognita."
                     << endl;
            }
        } while (!isalpha(static_cast<unsigned char>(incognita)));

        a = criarPolinomio(incognita);
        b = criarPolinomio(incognita);

        limparTela();

        int opcao;

        do {
            mostrarMenu();
            opcao = lerInteiro("Opcao: ");

            // Limpa o menu antes de executar a operação escolhida.
            limparTela();

            switch (opcao) {
                case 1:
                    executarOperacao([&]() {
                        cout << "========== LER POLINOMIO A =========="
                             << endl << endl;

                        lerPolinomio(a);
                        exibirComRotulo("A", a);
                    });
                    break;

                case 2:
                    executarOperacao([&]() {
                        cout << "========== LER POLINOMIO B =========="
                             << endl << endl;

                        lerPolinomio(b);
                        exibirComRotulo("B", b);
                    });
                    break;

                case 3:
                    cout << "========== POLINOMIOS CADASTRADOS =========="
                         << endl << endl;

                    exibirComRotulo("A", a);
                    exibirComRotulo("B", b);
                    break;

                case 4:
                    executarOperacao([&]() {
                        cout << "========== VALOR NUMERICO =========="
                             << endl << endl;

                        Polinomio& p = escolherPolinomio(a, b);

                        double valor = lerReal(
                            string("Valor de ") + incognita + ": "
                        );

                        double resultado = calcularValor(p, valor);

                        cout << "\nP(" << valor << ") = "
                             << resultado << endl;
                    });
                    break;

                case 5:
                    executarOperacao([&]() {
                        cout << "========== SOMA DE POLINOMIOS =========="
                             << endl << endl;

                        exibirComRotulo("A", a);
                        exibirComRotulo("B", b);

                        Polinomio resultado =
                            somarPolinomios(a, b);

                        exibirResultado(resultado);
                    });
                    break;

                case 6:
                    executarOperacao([&]() {
                        cout << "========== SUBTRACAO DE POLINOMIOS =========="
                             << endl << endl;

                        exibirComRotulo("A", a);
                        exibirComRotulo("B", b);

                        Polinomio resultado =
                            subtrairPolinomios(a, b);

                        exibirResultado(resultado);
                    });
                    break;

                case 7:
                    executarOperacao([&]() {
                        cout << "========== MULTIPLICACAO POR CONSTANTE =========="
                             << endl << endl;

                        Polinomio& p = escolherPolinomio(a, b);

                        exibirComRotulo(
                            "Polinomio escolhido",
                            p
                        );

                        double constante = lerReal("Constante: ");

                        Polinomio resultado =
                            multiplicarPorConstante(p, constante);

                        exibirResultado(resultado);
                    });
                    break;

                case 8:
                    executarOperacao([&]() {
                        cout << "========== MULTIPLICACAO POR MONOMIO =========="
                             << endl << endl;

                        Polinomio& p = escolherPolinomio(a, b);

                        exibirComRotulo(
                            "Polinomio escolhido",
                            p
                        );

                        double coeficiente =
                            lerReal("Coeficiente do monomio: ");

                        int expoente;

                        do {
                            expoente = lerInteiro(
                                "Expoente do monomio (inteiro >= 0): "
                            );

                            if (expoente < 0) {
                                cout << "O expoente nao pode ser negativo."
                                     << endl;
                            }
                        } while (expoente < 0);

                        Monomio m = criarMonomio(
                            coeficiente,
                            incognita,
                            expoente
                        );

                        Polinomio resultado =
                            multiplicarPorMonomio(p, m);

                        exibirResultado(resultado);
                    });
                    break;

                case 9:
                    executarOperacao([&]() {
                        cout << "========== MULTIPLICACAO DE POLINOMIOS =========="
                             << endl << endl;

                        exibirComRotulo("A", a);
                        exibirComRotulo("B", b);

                        Polinomio resultado =
                            multiplicarPolinomios(a, b);

                        exibirResultado(resultado);
                    });
                    break;

                case 0:
                    cout << "Encerrando o programa..."
                         << endl;
                    break;

                default:
                    cout << "Opcao invalida!" << endl;
                    break;
            }

            // Aguarda o usuário antes de voltar ao menu.
            if (opcao != 0) {
                cout << endl;
                pausarTela();
                limparTela();
            }

        } while (opcao != 0);

    } catch (const exception& erro) {
        cout << "\nO programa foi encerrado." << endl;
        cout << "Motivo: " << erro.what() << endl;
    }

    // Libera a memória dos polinômios principais.
    destruirPolinomio(a);
    destruirPolinomio(b);

    return 0;
}

#include "polinomio.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace std;

// Tolerância para considerar um coeficiente igual a zero.
static const double EPSILON = 1e-12;

// Verifica se um valor pode ser considerado zero.
static bool ehZero(double valor) {
    return fabs(valor) < EPSILON;
}

// Verifica se um coeficiente é válido.
static void validarCoeficiente(double coeficiente) {
    if (!isfinite(coeficiente)) {
        throw invalid_argument("O coeficiente deve ser um numero finito.");
    }
}

// Verifica se o expoente é válido.
static void validarExpoente(int expoente) {
    if (expoente < 0) {
        throw invalid_argument("O expoente nao pode ser negativo.");
    }
}

// Verifica se dois polinômios possuem a mesma incógnita.
static void validarIncognitas(const Polinomio& a, const Polinomio& b) {
    if (a.incognita != b.incognita) {
        throw invalid_argument(
            "Os polinomios devem utilizar a mesma incognita."
        );
    }
}

// Verifica se o monômio é compatível com o polinômio.
// Quando o expoente é zero, a incógnita do monômio não altera o resultado.
static void validarMonomio(const Polinomio& p, const Monomio& m) {
    validarCoeficiente(m.coeficiente);
    validarExpoente(m.expoente);

    if (m.expoente > 0 && m.incognita != p.incognita) {
        throw invalid_argument(
            "O monomio deve utilizar a mesma incognita do polinomio."
        );
    }
}

// Verifica a consistência interna do polinômio.
static void validarPolinomio(const Polinomio& p) {
    if (p.quantidade < 0 || p.capacidade < 0 ||
        p.quantidade > p.capacidade) {
        throw invalid_argument("Estrutura do polinomio inconsistente.");
    }

    if (p.quantidade > 0 && p.termos == nullptr) {
        throw invalid_argument("O vetor de termos nao foi alocado.");
    }
}

// CRIACAO, DESTRUICAO e MANUTENCAO                                   
Polinomio criarPolinomio(char incognita, int capacidadeInicial) {
    if (incognita == '\0') {
        throw invalid_argument("A incognita nao pode ser vazia.");
    }

    if (capacidadeInicial < 1) {
        capacidadeInicial = 1;
    }

    Polinomio p;

    p.incognita = incognita;
    p.quantidade = 0;
    p.capacidade = capacidadeInicial;
    p.termos = new Monomio[capacidadeInicial];

    return p;
}

void destruirPolinomio(Polinomio& p) {
    delete[] p.termos;

    p.termos = nullptr;
    p.quantidade = 0;
    p.capacidade = 0;
}

Polinomio copiarPolinomio(const Polinomio& p) {
    validarPolinomio(p);

    int capacidadeInicial = (p.quantidade > 0) ? p.quantidade : 1;

    Polinomio copia = criarPolinomio(
        p.incognita,
        capacidadeInicial
    );

    for (int i = 0; i < p.quantidade; i++) {
        copia.termos[i] = p.termos[i];
    }

    copia.quantidade = p.quantidade;

    return copia;
}

Monomio criarMonomio(double coeficiente, char incognita, int expoente) {
    validarCoeficiente(coeficiente);
    validarExpoente(expoente);

    if (incognita == '\0') {
        throw invalid_argument("A incognita nao pode ser vazia.");
    }

    Monomio m;

    m.coeficiente = coeficiente;
    m.incognita = incognita;
    m.expoente = expoente;

    return m;
}

// Aumenta a capacidade do vetor quando necessário.
static void aumentarCapacidade(Polinomio& p) {
    int novaCapacidade;

    if (p.capacidade == 0) {
        novaCapacidade = 1;
    } else {
        if (p.capacidade > 2147483647 / 2) {
            throw length_error("Capacidade maxima do polinomio atingida.");
        }

        novaCapacidade = p.capacidade * 2;
    }

    Monomio* novo = new Monomio[novaCapacidade];

    for (int i = 0; i < p.quantidade; i++) {
        novo[i] = p.termos[i];
    }

    delete[] p.termos;

    p.termos = novo;
    p.capacidade = novaCapacidade;
}

void adicionarMonomio(Polinomio& p, const Monomio& m) {
    validarPolinomio(p);
    validarMonomio(p, m);

    // Coeficiente zero não precisa ser armazenado.
    if (ehZero(m.coeficiente)) {
        return;
    }

    // Agrupa termos que possuem o mesmo expoente.
    for (int i = 0; i < p.quantidade; i++) {
        if (p.termos[i].expoente == m.expoente) {
            double novoCoeficiente =
                p.termos[i].coeficiente + m.coeficiente;

            validarCoeficiente(novoCoeficiente);

            p.termos[i].coeficiente = novoCoeficiente;

            return;
        }
    }

    // Aumenta o vetor caso não exista espaço disponível.
    if (p.quantidade == p.capacidade) {
        aumentarCapacidade(p);
    }

    p.termos[p.quantidade] = m;
    p.termos[p.quantidade].incognita = p.incognita;

    p.quantidade++;
}

void normalizarPolinomio(Polinomio& p) {
    validarPolinomio(p);

    // Agrupa termos semelhantes.
    int i = 0;

    while (i < p.quantidade) {
        int j = i + 1;

        while (j < p.quantidade) {
            if (p.termos[i].expoente == p.termos[j].expoente) {
                double novoCoeficiente =
                    p.termos[i].coeficiente +
                    p.termos[j].coeficiente;

                validarCoeficiente(novoCoeficiente);

                p.termos[i].coeficiente = novoCoeficiente;

                // Remove o termo j deslocando os seguintes.
                for (int k = j; k < p.quantidade - 1; k++) {
                    p.termos[k] = p.termos[k + 1];
                }

                p.quantidade--;
            } else {
                j++;
            }
        }

        i++;
    }

    // Remove termos com coeficiente zero.
    int novaQuantidade = 0;

    for (int k = 0; k < p.quantidade; k++) {
        if (!ehZero(p.termos[k].coeficiente)) {
            p.termos[novaQuantidade] = p.termos[k];
            p.termos[novaQuantidade].incognita = p.incognita;

            novaQuantidade++;
        }
    }

    p.quantidade = novaQuantidade;

    // Ordena os termos por expoente decrescente.
    for (int k = 1; k < p.quantidade; k++) {
        Monomio chave = p.termos[k];
        int j = k - 1;

        while (j >= 0 &&
               p.termos[j].expoente < chave.expoente) {
            p.termos[j + 1] = p.termos[j];
            j--;
        }

        p.termos[j + 1] = chave;
    }
}

// EXIBICAO 
void mostrarPolinomio(const Polinomio& p) {
    validarPolinomio(p);

    bool primeiroTermo = true;

    for (int i = 0; i < p.quantidade; i++) {
        const Monomio& m = p.termos[i];

        // Ignora termos com coeficiente zero.
        if (ehZero(m.coeficiente)) {
            continue;
        }

        double coeficiente = m.coeficiente;
        double absoluto = fabs(coeficiente);

        // Exibe o sinal do termo.
        if (primeiroTermo) {
            if (coeficiente < 0) {
                cout << "-";
            }
        } else {
            cout << (coeficiente < 0 ? " - " : " + ");
        }

        // O coeficiente 1 pode ser omitido quando há parte literal.
        if (m.expoente == 0 ||
            !ehZero(absoluto - 1.0)) {
            cout << absoluto;
        }

        // Exibe a parte literal.
        if (m.expoente > 0) {
            cout << p.incognita;

            if (m.expoente != 1) {
                cout << "^" << m.expoente;
            }
        }

        primeiroTermo = false;
    }

    // Se não houver termos válidos, o polinômio é zero.
    if (primeiroTermo) {
        cout << "0";
    }
}

// VALOR NUMERICO
double calcularValor(const Polinomio& p, double valor) {
    validarPolinomio(p);
    validarCoeficiente(valor);

    double resultado = 0.0;

    // Calcula a soma dos termos do polinômio.
    for (int i = 0; i < p.quantidade; i++) {
        double termo =
            p.termos[i].coeficiente *
            pow(valor, p.termos[i].expoente);

        resultado += termo;

        validarCoeficiente(resultado);
    }

    return resultado;
}

// SOMA E SUBTRACAO
Polinomio somarPolinomios(const Polinomio& a, const Polinomio& b) {
    validarPolinomio(a);
    validarPolinomio(b);
    validarIncognitas(a, b);

    Polinomio resultado = copiarPolinomio(a);

    try {
        for (int i = 0; i < b.quantidade; i++) {
            adicionarMonomio(resultado, b.termos[i]);
        }

        normalizarPolinomio(resultado);
    } catch (...) {
        destruirPolinomio(resultado);
        throw;
    }

    return resultado;
}

Polinomio subtrairPolinomios(const Polinomio& a, const Polinomio& b) {
    validarPolinomio(a);
    validarPolinomio(b);
    validarIncognitas(a, b);

    Polinomio resultado = copiarPolinomio(a);

    try {
        // Subtrair equivale a somar o oposto de cada termo.
        for (int i = 0; i < b.quantidade; i++) {
            Monomio oposto = b.termos[i];

            oposto.coeficiente = -oposto.coeficiente;

            adicionarMonomio(resultado, oposto);
        }

        normalizarPolinomio(resultado);
    } catch (...) {
        destruirPolinomio(resultado);
        throw;
    }

    return resultado;
}

// MUÇTIPLICACAO POR CONSTANTE

Polinomio multiplicarPorConstante(
    const Polinomio& p,
    double constante
) {
    validarPolinomio(p);
    validarCoeficiente(constante);

    Polinomio resultado = criarPolinomio(
        p.incognita,
        p.quantidade > 0 ? p.quantidade : 1
    );

    try {
        for (int i = 0; i < p.quantidade; i++) {
            Monomio m = p.termos[i];

            m.coeficiente *= constante;

            validarCoeficiente(m.coeficiente);

            adicionarMonomio(resultado, m);
        }

        normalizarPolinomio(resultado);
    } catch (...) {
        destruirPolinomio(resultado);
        throw;
    }

    return resultado;
}

// MUÇTIPLICACAO POR MONOMIO

Polinomio multiplicarPorMonomio(
    const Polinomio& p,
    const Monomio& m
) {
    validarPolinomio(p);
    validarMonomio(p, m);

    Polinomio resultado = criarPolinomio(
        p.incognita,
        p.quantidade > 0 ? p.quantidade : 1
    );

    try {
        for (int i = 0; i < p.quantidade; i++) {
            int novoExpoente =
                p.termos[i].expoente + m.expoente;

            // Detecta possível estouro de inteiro.
            if (novoExpoente < p.termos[i].expoente) {
                throw overflow_error("Expoente fora do limite permitido.");
            }

            double novoCoeficiente =
                p.termos[i].coeficiente * m.coeficiente;

            validarCoeficiente(novoCoeficiente);

            Monomio produto = criarMonomio(
                novoCoeficiente,
                p.incognita,
                novoExpoente
            );

            adicionarMonomio(resultado, produto);
        }

        normalizarPolinomio(resultado);
    } catch (...) {
        destruirPolinomio(resultado);
        throw;
    }

    return resultado;
}

// MULTIPLICACAO POR POLINOMIOS

Polinomio multiplicarPolinomios(
    const Polinomio& a,
    const Polinomio& b
) {
    validarPolinomio(a);
    validarPolinomio(b);
    validarIncognitas(a, b);

    Polinomio resultado = criarPolinomio(a.incognita, 1);

    try {
        // Multiplica cada termo de a por cada termo de b.
        for (int i = 0; i < a.quantidade; i++) {
            for (int j = 0; j < b.quantidade; j++) {
                int novoExpoente =
                    a.termos[i].expoente + b.termos[j].expoente;

                // Detecta possível estouro de inteiro.
                if (novoExpoente < a.termos[i].expoente) {
                    throw overflow_error(
                        "Expoente fora do limite permitido."
                    );
                }

                double novoCoeficiente =
                    a.termos[i].coeficiente *
                    b.termos[j].coeficiente;

                validarCoeficiente(novoCoeficiente);

                Monomio produto = criarMonomio(
                    novoCoeficiente,
                    a.incognita,
                    novoExpoente
                );

                adicionarMonomio(resultado, produto);
            }
        }

        normalizarPolinomio(resultado);
    } catch (...) {
        destruirPolinomio(resultado);
        throw;
    }

    return resultado;
}

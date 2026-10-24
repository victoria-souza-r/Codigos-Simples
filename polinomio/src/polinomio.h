#ifndef POLINOMIO_H
#define POLINOMIO_H

// TIPOS DE DADOS                                                     

// Um termo do polinômio: coeficiente * incógnita^expoente
struct Monomio {
    double coeficiente;
    char incognita;
    int expoente;
};

// Polinômio formado por monômios armazenados em um vetor dinâmico
struct Polinomio {
    char incognita;
    Monomio* termos;
    int quantidade;
    int capacidade;
};

// CRIACAO, DESTRUICAO e MANUTENCAO                                   

// Cria um polinômio vazio com a incógnita e a capacidade inicial indicadas
Polinomio criarPolinomio(char incognita, int capacidadeInicial = 4);

// Libera a memória do polinômio e zera seus dados
void destruirPolinomio(Polinomio& p);

// Cria uma cópia independente do polinômio, com nova alocação de memória
Polinomio copiarPolinomio(const Polinomio& p);

// Cria um monômio
Monomio criarMonomio(double coeficiente, char incognita, int expoente);

// Insere um monômio no polinômio, realocando o vetor se necessário.
// Se já existir um termo com o mesmo expoente, os coeficientes são somados.
void adicionarMonomio(Polinomio& p, const Monomio& m);

// Agrupa termos semelhantes, remove termos com coeficiente zero
// e ordena os termos em ordem decrescente de expoente.
void normalizarPolinomio(Polinomio& p);

// OPERACOES                                 
// Mostra o polinômio formatado na tela.
void mostrarPolinomio(const Polinomio& p);

// Calcula o valor numérico do polinômio para um valor da incógnita.
double calcularValor(const Polinomio& p, double valor);

// Soma e subtração de dois polinômios, gerando um terceiro.
Polinomio somarPolinomios(const Polinomio& a, const Polinomio& b);
Polinomio subtrairPolinomios(const Polinomio& a, const Polinomio& b);

// Multiplica um polinômio por uma constante.
Polinomio multiplicarPorConstante(const Polinomio& p, double constante);

// Multiplica um polinômio por um monômio.
Polinomio multiplicarPorMonomio(const Polinomio& p, const Monomio& m);

// Multiplica dois polinômios entre si.
Polinomio multiplicarPolinomios(const Polinomio& a, const Polinomio& b);

#endif 

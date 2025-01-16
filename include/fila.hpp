#ifndef FILA_HPP
#define FILA_HPP

#include "paciente.hpp"

// Estrutura do nó da fila
typedef struct Nodo {
    Paciente *paciente;
    struct Nodo *proximo;
    float tempoEntrada; // Tempo de entrada na fila
} Nodo;

// Estrutura da fila
typedef struct {
    Nodo *inicio;
    Nodo *fim;
    int tamanho;
    float ultimoRegistro; // Último tempo registrado
    float tempoTotalEspera; // Soma de todos os tempos de espera
    int totalPacientes;    // Contagem de pacientes processados
} Fila;

// Comandos da fila
void Inicializa(Fila *fila);
void Enfileira(Fila *fila, Paciente *paciente, float tempoAtual);
Paciente *Desenfileira(Fila *fila, float tempoAtual);
int FilaVazia(Fila *fila);
float CalculaTempoMedioEspera(Fila *fila); // Calcula o tempo médio de espera
void Finaliza(Fila *fila);

#endif // FILA_HPP

#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "evento.hpp"

// Estrutura do escalonador (minheap)
typedef struct {
    Evento *eventos;    // Array de eventos (minheap)
    int tamanho;        // Tamanho atual do heap
    int capacidade;     // Capacidade máxima do heap
    float tempoAtual;   // Tempo atual do escalonador
} Escalonador;

// Inicializa o escalonador com uma capacidade máxima
void inicializaEscalonador(Escalonador *esc, int capacidade);

// Insere um evento no escalonador
void insereEvento(Escalonador *esc, Evento evento);

// Retira e retorna o próximo evento (menor tempo) do escalonador
int retiraProximoEvento(Escalonador *esc, Evento *evento);

// Retorna o tempo atual do escalonador
float obtemTempoAtual(Escalonador *esc);

// Finaliza o escalonador e libera a memória
void finalizaEscalonador(Escalonador *esc);

#endif // ESCALONADOR_HPP


#include <stdio.h>
#include <stdlib.h>
#include "escalonador.hpp"

// Troca dois eventos no heap
static inline void troca(Evento *a, Evento *b) {
    Evento temp = *a;
    *a = *b;
    *b = temp;
}

// Corrige a posição do elemento para cima (up-heap)
static void corrigeParaCima(Escalonador *esc, int indice) {
    while (indice > 0) {
        int pai = (indice - 1) / 2;
        if (esc->eventos[indice].tempo >= esc->eventos[pai].tempo) {
            break;
        }
        troca(&esc->eventos[indice], &esc->eventos[pai]);
        indice = pai;
    }
}

// Corrige a posição do elemento para baixo (down-heap)
static void corrigeParaBaixo(Escalonador *esc, int indice) {
    int menor = indice;
    int esquerda = 2 * indice + 1;
    int direita = 2 * indice + 2;

    if (esquerda < esc->tamanho && esc->eventos[esquerda].tempo < esc->eventos[menor].tempo) {
        menor = esquerda;
    }

    if (direita < esc->tamanho && esc->eventos[direita].tempo < esc->eventos[menor].tempo) {
        menor = direita;
    }

    if (menor != indice) {
        troca(&esc->eventos[indice], &esc->eventos[menor]);
        corrigeParaBaixo(esc, menor);
    }
}

// Expande a capacidade do escalonador dinamicamente
static void expandeCapacidade(Escalonador *esc) {
    esc->capacidade *= 2;
    esc->eventos = (Evento *)realloc(esc->eventos, esc->capacidade * sizeof(Evento));
    if (!esc->eventos) {
        perror("Erro ao expandir capacidade do escalonador");
        exit(EXIT_FAILURE);
    }
}

// Inicializa o escalonador com uma capacidade máxima
void inicializaEscalonador(Escalonador *esc, int capacidade) {
    if (!esc || capacidade <= 0) {
        fprintf(stderr, "Erro: Parâmetros inválidos em inicializaEscalonador.\n");
        exit(EXIT_FAILURE);
    }

    esc->eventos = (Evento *)malloc(capacidade * sizeof(Evento));
    if (!esc->eventos) {
        perror("Erro ao alocar memória para o escalonador");
        exit(EXIT_FAILURE);
    }

    esc->tamanho = 0;
    esc->capacidade = capacidade;
    esc->tempoAtual = 0.0f;
}

// Insere um evento no escalonador
void insereEvento(Escalonador *esc, Evento evento) {
    if (!esc) {
        fprintf(stderr, "Erro: Ponteiro inválido em insereEvento.\n");
        return;
    }

    if (esc->tamanho >= esc->capacidade) {
        expandeCapacidade(esc);
    }

    esc->eventos[esc->tamanho] = evento;
    corrigeParaCima(esc, esc->tamanho);
    esc->tamanho++;
}

// Retira e retorna o próximo evento (menor tempo) do escalonador
int retiraProximoEvento(Escalonador *esc, Evento *evento) {
    if (!esc || esc->tamanho == 0) {
        return 0; // Escalonador vazio
    }

    *evento = esc->eventos[0];
    esc->eventos[0] = esc->eventos[esc->tamanho - 1];
    esc->tamanho--;
    corrigeParaBaixo(esc, 0);

    // Atualiza o tempo atual do escalonador
    esc->tempoAtual = evento->tempo;
    return 1; // Sucesso
}

// Retorna o tempo atual do escalonador
float obtemTempoAtual(Escalonador *esc) {
    return esc ? esc->tempoAtual : -1.0f;
}

// Finaliza o escalonador e libera a memória
void finalizaEscalonador(Escalonador *esc) {
    if (!esc) {
        fprintf(stderr, "Erro: Ponteiro de escalonador inválido em finalizaEscalonador.\n");
        return;
    }

    free(esc->eventos);
    esc->eventos = NULL;
    esc->tamanho = 0;
    esc->capacidade = 0;
    esc->tempoAtual = 0.0f;
}

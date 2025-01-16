#include <stdio.h>
#include <stdlib.h>
#include "fila.hpp"

// Inicializa a fila
void Inicializa(Fila *fila) {
    if (!fila) {
        fprintf(stderr, "Erro: Ponteiro de fila inválido.\n");
        exit(EXIT_FAILURE);
    }
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
    fila->ultimoRegistro = 0.0f;
    fila->tempoTotalEspera = 0.0f;
    fila->totalPacientes = 0;
}

// Enfileira um paciente
void Enfileira(Fila *fila, Paciente *paciente, float tempoAtual) {
    if (!fila || !paciente) {
        fprintf(stderr, "Erro: Ponteiro inválido em Enfileira.\n");
        return;
    }

    Nodo *novoNodo = (Nodo *)malloc(sizeof(Nodo));
    if (!novoNodo) {
        perror("Erro ao alocar memória para o nó da fila");
        exit(EXIT_FAILURE);
    }
    novoNodo->paciente = paciente;
    novoNodo->proximo = NULL;
    novoNodo->tempoEntrada = tempoAtual;

    if (fila->fim != NULL) {
        fila->fim->proximo = novoNodo;
    } else {
        fila->inicio = novoNodo;
    }

    fila->fim = novoNodo;
    fila->tamanho++;
}

// Desenfileira um paciente e calcula o tempo de espera
Paciente *Desenfileira(Fila *fila, float tempoAtual) {
    if (!fila || fila->inicio == NULL) {
        fprintf(stderr, "Erro: Fila vazia ou ponteiro inválido em Desenfileira.\n");
        return NULL;
    }

    Nodo *nodoRemovido = fila->inicio;
    Paciente *paciente = nodoRemovido->paciente;

    // Atualiza o tempo de espera total
    float tempoEspera = tempoAtual - nodoRemovido->tempoEntrada;
    fila->tempoTotalEspera += tempoEspera;
    fila->totalPacientes++;

    fila->inicio = nodoRemovido->proximo;
    if (fila->inicio == NULL) {
        fila->fim = NULL; // Fila ficou vazia
    }

    free(nodoRemovido);
    fila->tamanho--;
    return paciente;
}

// Calcula o tempo médio de espera
float CalculaTempoMedioEspera(Fila *fila) {
    if (!fila || fila->totalPacientes == 0) {
        return 0.0f;
    }
    return fila->tempoTotalEspera / fila->totalPacientes;
}

// Verifica se a fila está vazia
int FilaVazia(Fila *fila) {
    if (!fila) {
        fprintf(stderr, "Erro: Ponteiro de fila inválido em FilaVazia.\n");
        return 1;
    }
    return (fila->inicio == NULL);
}

// Finaliza a fila e libera a memória
void Finaliza(Fila *fila) {
    if (!fila) {
        fprintf(stderr, "Erro: Ponteiro de fila inválido em Finaliza.\n");
        return;
    }
    while (!FilaVazia(fila)) {
        Desenfileira(fila, fila->ultimoRegistro);
    }
}

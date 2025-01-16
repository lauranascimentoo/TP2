#include <stdio.h>
#include <stdlib.h>
#include "evento.hpp"

// Cria um evento com os dados fornecidos
Evento criaEvento(float tempo, TipoEvento tipo, Paciente *paciente) {
    Evento novoEvento;
    novoEvento.tempo = tempo;
    novoEvento.tipo = tipo;
    novoEvento.paciente = paciente;
    novoEvento.prioridade = (tipo == EVENTO_TRIAGEM && paciente) ? paciente->prioridade : -1;
    return novoEvento;
}

// Valida se os dados de um evento são consistentes
int validaEvento(Evento evento) {
    if (evento.tempo < 0) {
        fprintf(stderr, "Erro: Tempo do evento inválido.\n");
        return 0;
    }
    if (evento.tipo < EVENTO_CHEGADA || evento.tipo > EVENTO_GENERICO) {
        fprintf(stderr, "Erro: Tipo de evento inválido.\n");
        return 0;
    }
    if (evento.tipo != EVENTO_GENERICO && evento.paciente == NULL) {
        fprintf(stderr, "Erro: Evento associado a paciente nulo.\n");
        return 0;
    }
    return 1;
}

// Compara dois eventos (baseado no tempo)
int comparaEventos(const Evento *a, const Evento *b) {
    if (a->tempo < b->tempo) {
        return -1;
    }
    if (a->tempo > b->tempo) {
        return 1;
    }
    return 0; // Empate
}

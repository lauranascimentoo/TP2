#ifndef EVENTO_HPP
#define EVENTO_HPP

#include "paciente.hpp"

// Tipos de evento possíveis
typedef enum {
    EVENTO_CHEGADA,        // Paciente chega ao hospital
    EVENTO_TRIAGEM,        // Paciente entra na triagem
    EVENTO_ATENDIMENTO,    // Início de atendimento
    EVENTO_EXAME,          // Início de exame
    EVENTO_MEDICAMENTO,    // Início de aplicação de medicamento
    EVENTO_ALTA,           // Paciente recebe alta
    EVENTO_GENERICO        // Evento genérico para outros usos
} TipoEvento;

// Estrutura de um evento
typedef struct {
    float tempo;        // Tempo do evento (em horas)
    TipoEvento tipo;    // Tipo do evento
    Paciente *paciente; // Paciente associado ao evento (NULL para eventos genéricos)
    int prioridade;     // Prioridade do evento (-1 se indefinida, definido após triagem)
} Evento;

// Função para criar eventos
Evento criaEvento(float tempo, TipoEvento tipo, Paciente *paciente);

// Função para validar um evento
int validaEvento(Evento evento);

// Função para comparar dois eventos (baseado no tempo)
int comparaEventos(const Evento *a, const Evento *b);

#endif // EVENTO_HPP

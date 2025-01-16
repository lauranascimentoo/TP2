#ifndef PACIENTE_HPP
#define PACIENTE_HPP

#include <stdio.h>
#include <stdlib.h>

#define BASE_ANO 2000
#define HORAS_POR_DIA 24

typedef struct {
    char id[11];            // Identificador único do paciente
    int teveAlta;           // 0 - Não teve alta, 1 - Teve alta
    int ano, mes, dia, hora; // Data e hora de entrada
    int ano_s, mes_s, dia_s, hora_s; // Data e hora de saída
    int prioridade;         // 0 - Verde, 1 - Amarelo, 2 - Vermelho
    int medidas;            // Nº de medidas hospitalares
    int testes;             // Nº de testes de laboratório
    int exames;             // Nº de exames de imagem
    int medicamentos;       // Nº de medicamentos/instrumentos
    int estado;             // Estado atual do paciente (1 a 14)
    float tempoEspera;      // Tempo total de espera (em filas)
    float tempoAtendimento; // Tempo total em atendimento
    float tempoOcioso;      // Tempo total ocioso (fora de filas e atendimento)
    float horaEntrada;      // Hora de entrada em uma fila
    float horaSaida;        // Hora de saída de uma fila
    float tempoTotal;       // Tempo total no hospital (opcional)
} Paciente;

// Função para inicializar um paciente com dados do arquivo
void inicializaPaciente(Paciente *paciente, const char *linha);

// Função para atualizar o estado do paciente
void atualizaEstado(Paciente *paciente, int novoEstado);

// Função para avançar para o próximo estado do paciente
void avancaEstado(Paciente *paciente);

// Tempo total oscioso (soma dos tempos de fila - acumula na variável tempoOcioso do paciente)
void calculaTempoOscioso(Paciente *paciente, float horaEntrada, float horaSaida); 

// Tempo total em atendimento (soma dos tempos de procedimentos feitos  - acumula na variável tempoAtendimento do paciente)
void calculaTempoAtendimento(Paciente *paciente, float horaEntrada, float horaSaida); 

// Transforma em horas a (data e hora) com base no  dia 01/01/2000
float transformaHoras(float ano, float mes, float dia, float hora);

// Transforma as horas em (datae hora) com base no  dia 01/01/2000
void transformaData(float horas, int *ano, int *mes, int *dia, int *hora); 

#endif // PACIENTE_HPP

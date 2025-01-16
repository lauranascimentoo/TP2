#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paciente.hpp"

#define BASE_ANO 2000
#define HORAS_POR_DIA 24

// Tabela de dias em cada mês considerando anos não bissextos
const int diasPorMes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Verifica se um ano é bissexto
int ehBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

// Inicializa um paciente a partir de uma linha do arquivo
void inicializaPaciente(Paciente *paciente, const char *linha) {
    sscanf(linha, "%10s %d %d %d %d %d %d %d %d %d %d",
           paciente->id, &paciente->teveAlta, &paciente->ano,
           &paciente->mes, &paciente->dia, &paciente->hora,
           &paciente->prioridade, &paciente->medidas, 
           &paciente->testes, &paciente->exames, 
           &paciente->medicamentos);

    paciente->estado = 1; // Estado inicial (1: Não chegou ainda ao hospital)
    paciente->tempoEspera = 0.0f;       // Tempo de espera inicial
    paciente->tempoAtendimento = 0.0f; // Tempo de atendimento inicial
    paciente->tempoOcioso = 0.0f;      // Tempo ocioso inicial
    paciente->horaEntrada = 0.0f;      // Tempo entrada (variável - para cálculo do tempo fila)
    paciente->horaSaida = 0.0f;        // Tempo saída inicial (variável - para cálculo do tempo fila)
    paciente->tempoTotal = 0.0f;       // Tempo total no hospital inicial
}

// Atualiza o estado do paciente
void atualizaEstado(Paciente *paciente, int novoEstado) {
    paciente->estado = novoEstado;
}

// Avança o estado do paciente para o próximo
void avancaEstado(Paciente *paciente) {
    if (paciente->estado < 14) {
        atualizaEstado(paciente, paciente->estado + 1);
    }
}

// Calcula o tempo em uma fila (horas fracionadas)
void calculaTempoFila(Paciente *paciente, float horaEntrada, float horaSaida) {
    if (horaSaida > horaEntrada) {
        float tempo = horaSaida - horaEntrada;
        paciente->tempoEspera += tempo;
        paciente->tempoTotal += tempo; // Incrementa o tempo total
    }
}

// Calcula o tempo ocioso (fora de filas e atendimentos)
void calculaTempoOcioso(Paciente *paciente, float horaEntrada, float horaSaida) {
    if (horaSaida > horaEntrada) {
        float tempo = horaSaida - horaEntrada;
        paciente->tempoOcioso += tempo;
        paciente->tempoTotal += tempo; // Incrementa o tempo total
    }
}

// Calcula o tempo de um procedimento
void calculaTempoProcedimento(Paciente *paciente, float horaEntrada, float horaSaida) {
    if (horaSaida > horaEntrada) {
        float tempo = horaSaida - horaEntrada;
        paciente->tempoAtendimento += tempo;
        paciente->tempoTotal += tempo; // Incrementa o tempo total
    }
}

// Converte uma data (ano, mês, dia, hora) em horas desde 01/01/2000
float transformaHoras(float ano, float mes, float dia, float hora) {
    int anos = ano - BASE_ANO;
    int dias = 0;

    // Soma os dias dos anos completos
    for (int i = 0; i < anos; i++) {
        dias += ehBissexto(BASE_ANO + i) ? 366 : 365;
    }

    // Soma os dias dos meses completos no ano atual
    for (int i = 0; i < mes - 1; i++) {
        dias += diasPorMes[i];
        if (i == 1 && ehBissexto(ano)) { // Ajusta fevereiro se for bissexto
            dias += 1;
        }
    }

    // Soma os dias do mês atual
    dias += dia - 1;

    // Converte para horas
    return dias * HORAS_POR_DIA + hora;
}

// Converte um número de horas em uma data (ano, mês, dia, hora)
void transformaData(float horas, int *ano, int *mes, int *dia, int *hora) {
    int dias = (int)(horas / HORAS_POR_DIA); // Converte as horas para o número inteiro de dias
    *hora = (int)horas % HORAS_POR_DIA;     // Extrai a parte inteira das horas
    *ano = BASE_ANO;

    // Calcula o ano
    while (dias >= (ehBissexto(*ano) ? 366 : 365)) {
        dias -= ehBissexto(*ano) ? 366 : 365;
        (*ano)++;
    }

    // Calcula o mês
    *mes = 1;
    for (int i = 0; i < 12; i++) {
        int diasNoMes = diasPorMes[i];
        if (i == 1 && ehBissexto(*ano)) { // Ajusta fevereiro se for bissexto
            diasNoMes += 1;
        }

        if (dias < diasNoMes) {
            break;
        }

        dias -= diasNoMes;
        (*mes)++;
    }

    // O restante são os dias
    *dia = dias + 1;
}


// Calcula o tempo total no atendimento (somando tempos de procedimentos)
void calculaTempoAtendimento(Paciente *paciente, float horaEntrada, float horaSaida) {
    if (horaSaida > horaEntrada) {
        float tempo = horaSaida - horaEntrada;
        paciente->tempoAtendimento += tempo;
        paciente->tempoTotal += tempo; // Incrementa o tempo total
    }
}

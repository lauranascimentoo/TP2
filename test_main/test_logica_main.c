#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paciente.hpp"
#include "procedimento.hpp"
#include "evento.hpp"
#include "escalonador.hpp"
#include "fila.hpp"

#define MAX_FILAS 16
#define TOTAL_PROCEDIMENTOS 6

const char *diasSemana[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char *meses[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int calculaDiaDaSemana(int ano, int mes, int dia) {
    if (mes < 3) {
        mes += 12;
        ano--;
    }
    int k = ano % 100;
    int j = ano / 100;
    int h = (dia + 13 * (mes + 1) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;
    return (h + 7) % 7;
}

// Enum para facilitar a identificação dos procedimentos
enum {
    TRIAGEM = 0,
    ATEND,
    MH,
    TL,
    EI,
    IM,
    MAX_PROCEDIMENTOS_ENUM
};

// Enum para as filas
enum {
    FILA_TRIAGEM = 0,
    FILA_ATEND_PRIORIDADE_2,
    FILA_ATEND_PRIORIDADE_1,
    FILA_ATEND_PRIORIDADE_0,
    FILA_MH_PRIORIDADE_2,
    FILA_MH_PRIORIDADE_1,
    FILA_MH_PRIORIDADE_0,
    FILA_TL_PRIORIDADE_2,
    FILA_TL_PRIORIDADE_1,
    FILA_TL_PRIORIDADE_0,
    FILA_EI_PRIORIDADE_2,
    FILA_EI_PRIORIDADE_1,
    FILA_EI_PRIORIDADE_0,
    FILA_IM_PRIORIDADE_2,
    FILA_IM_PRIORIDADE_1,
    FILA_IM_PRIORIDADE_0,
    MAX_FILAS_ENUM
};

// Avança o paciente para o próximo estado
void avancaEstadoPaciente(Escalonador *escalonador, Paciente *paciente, float tempoAtual, TipoEvento proximoEvento, Fila *filas, int filaDestino) {
    Evento novoEvento = criaEvento(tempoAtual, proximoEvento, paciente);
    insereEvento(escalonador, novoEvento);
    Enfileira(&filas[filaDestino], paciente, tempoAtual);
}

// Processa o paciente em um procedimento
void processaProcedimento(Escalonador *escalonador, Paciente *paciente, Procedimento *procedimento, float tempoAtual, TipoEvento proximoEvento, Fila *filas, int filaDestino) {
    int unidade = encontraUnidadeOciosa(procedimento, tempoAtual);
    if (unidade >= 0) {
        ocupaUnidade(procedimento, unidade, tempoAtual, procedimento->tempoMedio);
        calculaTempoAtendimento(paciente, tempoAtual, tempoAtual + procedimento->tempoMedio);
        avancaEstadoPaciente(escalonador, paciente, tempoAtual + procedimento->tempoMedio, proximoEvento, filas, filaDestino);
    } else {
        fprintf(stderr, "Erro: Nenhuma unidade disponível para o procedimento %d\n", procedimento->id);
    }
}

int main() {
    const char *dadosProcedimento[] = {
        "0.2 3", "0.5 5", "0.1 10", "0.05 5", "0.5 20", "0.05 5"
    };
    const char *dadosPacientes[] = {
        "0009600008 0 2017 3 21 2 0 5 43 2 110", "0009600009 0 2017 3 21 2 0 3 1 5 21",
        "0009600010 0 2017 3 21 2 1 3 4 2 8", "0009600011 0 2017 3 21 2 1 15 28 4 68",
        "0009600012 0 2017 3 21 2 0 3 2 7 9", "0009600013 1 2017 3 21 3 0 2 2 2 8",
        "0009600015 1 2017 3 21 3 0 1 3 1 4", "0009600016 0 2017 3 21 3 0 5 14 1 28",
        "0009600017 1 2017 3 21 4 0 1 0 0 0", "0009600018 0 2017 3 21 4 1 2 8 3 11"
    };

    float tempoAtual = 0.0f;
    Escalonador escalonador;
    inicializaEscalonador(&escalonador, 1000);

    Fila filas[MAX_FILAS_ENUM];
    for (int i = 0; i < MAX_FILAS_ENUM; i++) {
        Inicializa(&filas[i]);
    }

    Procedimento procedimentos[MAX_PROCEDIMENTOS_ENUM];
    for (int i = 0; i < MAX_PROCEDIMENTOS_ENUM; i++) {
        inicializaProcedimentoComLinha(&procedimentos[i], dadosProcedimento[i], i);
    }

    int totalPacientes = sizeof(dadosPacientes) / sizeof(dadosPacientes[0]);
    Paciente *pacientes = (Paciente *)malloc(totalPacientes * sizeof(Paciente));
    if (!pacientes) {
        perror("Erro ao alocar memória para pacientes");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < totalPacientes; i++) {
        inicializaPaciente(&pacientes[i], dadosPacientes[i]);
        float tempoEntrada = transformaHoras(
            pacientes[i].ano, pacientes[i].mes, pacientes[i].dia, pacientes[i].hora
        );
        pacientes[i].horaEntrada = tempoEntrada;
        Evento eventoInicial = criaEvento(tempoEntrada, EVENTO_CHEGADA, &pacientes[i]);
        insereEvento(&escalonador, eventoInicial);
    }

while (escalonador.tamanho > 0) {
    Evento evento;
    if (!retiraProximoEvento(&escalonador, &evento)) break;

    tempoAtual = evento.tempo;

    switch (evento.tipo) {
        case EVENTO_CHEGADA:
            avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual, EVENTO_TRIAGEM, filas, FILA_TRIAGEM);
            break;

        case EVENTO_TRIAGEM:
            processaProcedimento(&escalonador, evento.paciente, &procedimentos[TRIAGEM], tempoAtual, EVENTO_ATENDIMENTO,
                                 filas, FILA_ATEND_PRIORIDADE_2 - evento.paciente->prioridade);
            break;

        case EVENTO_ATENDIMENTO:
            processaProcedimento(&escalonador, evento.paciente, &procedimentos[ATEND], tempoAtual, 
                                 EVENTO_MEDIDAS, filas, FILA_MH_PRIORIDADE_2 - evento.paciente->prioridade);
            break;

        case EVENTO_MEDIDAS:
            if (evento.paciente->medidas > 0) {
                evento.paciente->medidas--;
                processaProcedimento(&escalonador, evento.paciente, &procedimentos[MH], tempoAtual, 
                                     EVENTO_MEDIDAS, filas, FILA_MH_PRIORIDADE_2 - evento.paciente->prioridade);
            } else {
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual, EVENTO_TESTES, 
                                     filas, FILA_TL_PRIORIDADE_2 - evento.paciente->prioridade);
            }
            break;

        case EVENTO_TESTES:
            if (evento.paciente->testes > 0) {
                evento.paciente->testes--;
                processaProcedimento(&escalonador, evento.paciente, &procedimentos[TL], tempoAtual, 
                                     EVENTO_TESTES, filas, FILA_TL_PRIORIDADE_2 - evento.paciente->prioridade);
            } else {
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual, EVENTO_EXAME, 
                                     filas, FILA_EI_PRIORIDADE_2 - evento.paciente->prioridade);
            }
            break;

        case EVENTO_EXAME:
            if (evento.paciente->exames > 0) {
                evento.paciente->exames--;
                processaProcedimento(&escalonador, evento.paciente, &procedimentos[EI], tempoAtual, 
                                     EVENTO_EXAME, filas, FILA_EI_PRIORIDADE_2 - evento.paciente->prioridade);
            } else {
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual, EVENTO_MEDICAMENTO, 
                                     filas, FILA_IM_PRIORIDADE_2 - evento.paciente->prioridade);
            }
            break;

        case EVENTO_MEDICAMENTO:
            if (evento.paciente->medicamentos > 0) {
                evento.paciente->medicamentos--;
                processaProcedimento(&escalonador, evento.paciente, &procedimentos[IM], tempoAtual, 
                                     EVENTO_MEDICAMENTO, filas, FILA_IM_PRIORIDADE_2 - evento.paciente->prioridade);
            } else {
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual, EVENTO_ALTA, filas, 0);
            }
            break;

        case EVENTO_ALTA:
            evento.paciente->horaSaida = tempoAtual;
            evento.paciente->tempoTotal = tempoAtual - evento.paciente->horaEntrada;
            break;

        default:
            fprintf(stderr, "Erro: Evento desconhecido\n");
            break;
    }
}


    FILE *saida = fopen("saida.txt", "w");
    if (!saida) {
        perror("Erro ao criar arquivo de saída");
        free(pacientes);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < totalPacientes; i++) {
        int anoEntrada, mesEntrada, diaEntrada, horaEntrada, minutosEntrada, segundosEntrada;
        int anoSaida, mesSaida, diaSaida, horaSaida, minutosSaida, segundosSaida;

        // Transforma o tempo de entrada em data e hora
        transformaData(pacientes[i].horaEntrada, &anoEntrada, &mesEntrada, &diaEntrada, &horaEntrada);
        float minutosEntradaFracao = (pacientes[i].horaEntrada - (int)pacientes[i].horaEntrada) * 60;
        minutosEntrada = (int)minutosEntradaFracao;
        segundosEntrada = (int)((minutosEntradaFracao - minutosEntrada) * 60);

        // Transforma o tempo de saída em data e hora
        transformaData(pacientes[i].horaSaida, &anoSaida, &mesSaida, &diaSaida, &horaSaida);
        float minutosSaidaFracao = (pacientes[i].horaSaida - (int)pacientes[i].horaSaida) * 60;
        minutosSaida = (int)minutosSaidaFracao;
        segundosSaida = (int)((minutosSaidaFracao - minutosSaida) * 60);

        // Calcula o dia da semana
        int diaSemanaEntrada = calculaDiaDaSemana(anoEntrada, mesEntrada, diaEntrada);
        int diaSemanaSaida = calculaDiaDaSemana(anoSaida, mesSaida, diaSaida);

        // Escreve no arquivo de saída
        fprintf(saida, "%s %s %s %02d %02d:%02d:%02d %04d %s %s %02d %02d:%02d:%02d %04d %.2f %.2f %.2f\n",
                pacientes[i].id,
                diasSemana[diaSemanaEntrada],
                meses[mesEntrada - 1], diaEntrada, horaEntrada, minutosEntrada, segundosEntrada, anoEntrada,
                diasSemana[diaSemanaSaida],
                meses[mesSaida - 1], diaSaida, horaSaida, minutosSaida, segundosSaida, anoSaida,
                pacientes[i].tempoTotal,
                pacientes[i].tempoAtendimento,
                pacientes[i].tempoEspera);
    }

    fclose(saida);

    finalizaEscalonador(&escalonador);
    for (int i = 0; i < MAX_FILAS_ENUM; i++) {
        Finaliza(&filas[i]);
    }
    free(pacientes);

    return 0;
}

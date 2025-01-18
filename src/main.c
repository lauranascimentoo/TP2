#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paciente.hpp"
#include "procedimento.hpp"
#include "evento.hpp"
#include "escalonador.hpp"
#include "fila.hpp"

#define MAX_FILAS 16

const char *diasSemana[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char *meses[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

int calculaDiaDaSemana(int ano, int mes, int dia) {
    // Fórmula de Zeller para calcular o dia da semana
    if (mes < 3) {
        mes += 12;
        ano--;
    }
    int k = ano % 100;
    int j = ano / 100;
    int h = (dia + 13 * (mes + 1) / 5 + k + k / 4 + j / 4 - 2 * j) % 7;
    return ((h + 5) % 7) + 1; // Ajusta para 0 = Domingo, 1 = Segunda...
}

int main() {
    // Variáveis principais
    Escalonador escalonador;
    Fila filas[MAX_FILAS];
    Paciente *pacientes = NULL;
    int totalPacientes = 0;
    int capacidadePacientes = 0;
    float tempoAtual = 0.0f;

    // Inicializa o escalonador
    inicializaEscalonador(&escalonador, 1000);

    // Inicializa as filas
    for (int i = 0; i < MAX_FILAS; i++) {
        Inicializa(&filas[i]);
    }

    // Ler arquivo de entrada
    FILE *entrada = fopen("20242TP2.csv", "r");
    if (!entrada) {
        perror("Erro ao abrir o arquivo de entrada");
        return EXIT_FAILURE;
    }

    char linha[256];
    float temposProcedimentos[6];
    int unidadesProcedimentos[6];

    // Lê tempos e unidades para os procedimentos
    for (int i = 0; i < 6; i++) {
        if (fgets(linha, sizeof(linha), entrada)) {
            sscanf(linha, "%f %d", &temposProcedimentos[i], &unidadesProcedimentos[i]);
        } else {
            fprintf(stderr, "Erro ao ler configuração dos procedimentos\n");
            fclose(entrada);
            return EXIT_FAILURE;
        }
    }

    // Lê o número de pacientes
    if (fgets(linha, sizeof(linha), entrada)) {
        capacidadePacientes = atoi(linha);
        if (capacidadePacientes <= 0) {
            fprintf(stderr, "Erro: Número inválido de pacientes\n");
            fclose(entrada);
            return EXIT_FAILURE;
        }
        pacientes = (Paciente *)malloc(capacidadePacientes * sizeof(Paciente));
        if (!pacientes) {
            perror("Erro ao alocar memória para pacientes");
            fclose(entrada);
            return EXIT_FAILURE;
        }
    }

    // Lê pacientes do arquivo
    while (fgets(linha, sizeof(linha), entrada)) {
        if (totalPacientes >= capacidadePacientes) {
            fprintf(stderr, "Erro: Excedido o número de pacientes esperado\n");
            break;
        }
        inicializaPaciente(&pacientes[totalPacientes], linha);
        float tempoEntrada = transformaHoras(
            pacientes[totalPacientes].ano,
            pacientes[totalPacientes].mes,
            pacientes[totalPacientes].dia,
            pacientes[totalPacientes].hora
        );
        Evento eventoInicial = criaEvento(tempoEntrada, EVENTO_CHEGADA, &pacientes[totalPacientes]);
        insereEvento(&escalonador, eventoInicial);
        totalPacientes++;
    }

    fclose(entrada);

    // Loop principal da simulação
    while (escalonador.tamanho > 0) {
        Evento evento;
        if (!retiraProximoEvento(&escalonador, &evento)) {
            fprintf(stderr, "Erro ao retirar evento do escalonador\n");
            break;
        }
        tempoAtual = evento.tempo;
        switch (evento.tipo) {
            case EVENTO_CHEGADA:
                Enfileira(&filas[0], evento.paciente, tempoAtual);
                break;

            case EVENTO_TRIAGEM:
                Desenfileira(&filas[0], tempoAtual);
                evento.paciente->prioridade = rand() % 3;
                Evento proxEvento = criaEvento(tempoAtual + temposProcedimentos[0], EVENTO_ATENDIMENTO, evento.paciente);
                insereEvento(&escalonador, proxEvento);
                break;

            case EVENTO_ATENDIMENTO:
                calculaTempoFila(evento.paciente, evento.paciente->horaEntrada, tempoAtual);
                calculaTempoAtendimento(evento.paciente, tempoAtual, tempoAtual + temposProcedimentos[1]);
                Evento altaEvento = criaEvento(tempoAtual + temposProcedimentos[1], EVENTO_ALTA, evento.paciente);
                insereEvento(&escalonador, altaEvento);
                break;

            case EVENTO_ALTA:
                evento.paciente->horaSaida = tempoAtual;
                break;

            default:
                fprintf(stderr, "Erro: Tipo de evento desconhecido\n");
                break;
        }
    }

    // Gera saída
    FILE *saida = fopen("saida.txt", "w");
    if (!saida) {
        perror("Erro ao criar arquivo de saída");
        free(pacientes);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < totalPacientes; i++) {
        int anoEntrada, mesEntrada, diaEntrada, horaEntrada;
        transformaData(
            transformaHoras(pacientes[i].ano, pacientes[i].mes, pacientes[i].dia, pacientes[i].hora),
            &anoEntrada, &mesEntrada, &diaEntrada, &horaEntrada
        );

        int diaSemanaEntrada = calculaDiaDaSemana(pacientes[i].ano, pacientes[i].mes, pacientes[i].dia);

        fprintf(saida, "%s %s %s %02d %02d:00 %04d\n",
                pacientes[i].id,
                diasSemana[diaSemanaEntrada],
                meses[mesEntrada - 1], diaEntrada, horaEntrada, anoEntrada);
    }

    fclose(saida);

    finalizaEscalonador(&escalonador);
    for (int i = 0; i < MAX_FILAS; i++) {
        Finaliza(&filas[i]);
    }
    free(pacientes);
    return 0;
}

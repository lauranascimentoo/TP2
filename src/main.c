#include <stdio.h>
#include <stdlib.h>
#include "paciente.hpp"
#include "procedimento.hpp"
#include "evento.hpp"
#include "escalonador.hpp"
#include "fila.hpp"

#define MAX_FILAS 16

int main() {
    // Variáveis principais
    Escalonador escalonador;
    Fila filas[MAX_FILAS];
    Paciente *pacientes = NULL;
    int totalPacientes = 0;
    int capacidadePacientes = 0;
    float tempoAtual = 0.0f;

    // Inicializa o escalonador
    inicializaEscalonador(&escalonador, 1000); // Capacidade inicial arbitrária

    // Inicializa as filas
    for (int i = 0; i < MAX_FILAS; i++) {
        Inicializa(&filas[i]);
    }

    // Ler arquivo de entrada e criar pacientes e eventos iniciais
    FILE *entrada = fopen("20242TP2.csv", "r");
    if (!entrada) {
        perror("Erro ao abrir o arquivo de entrada");
        return EXIT_FAILURE;
    }

    char linha[256];
    // Ignora as primeiras 6 linhas de configuração
    for (int i = 0; i < 6; i++) {
        if (!fgets(linha, sizeof(linha), entrada)) {
            fprintf(stderr, "Erro ao ler as linhas de configuração\n");
            fclose(entrada);
            return EXIT_FAILURE;
        }
    }

    // Lê o valor da linha 7 para determinar MAX_EVENTOS
    if (fgets(linha, sizeof(linha), entrada)) {
        capacidadePacientes = atoi(linha);
        if (capacidadePacientes <= 0) {
            fprintf(stderr, "Erro: Valor inválido para o número de pacientes\n");
            fclose(entrada);
            return EXIT_FAILURE;
        }
        pacientes = (Paciente *)malloc(capacidadePacientes * sizeof(Paciente));
        if (!pacientes) {
            perror("Erro ao alocar memória para pacientes");
            fclose(entrada);
            return EXIT_FAILURE;
        }
    } else {
        fprintf(stderr, "Erro ao ler o número de pacientes\n");
        fclose(entrada);
        return EXIT_FAILURE;
    }

    // Lê pacientes do arquivo
    while (fgets(linha, sizeof(linha), entrada)) {
        if (totalPacientes >= capacidadePacientes) {
            fprintf(stderr, "Erro: Excedido o número de pacientes esperado\n");
            break;
        }
        inicializaPaciente(&pacientes[totalPacientes], linha);
        Evento eventoInicial = criaEvento(
            transformaHoras(pacientes[totalPacientes].ano, pacientes[totalPacientes].mes, pacientes[totalPacientes].dia, pacientes[totalPacientes].hora),
            EVENTO_CHEGADA,
            &pacientes[totalPacientes]
        );
        insereEvento(&escalonador, eventoInicial);
        totalPacientes++;
    }

    fclose(entrada);

    // Loop principal da simulação
    while (escalonador.tamanho > 0) {
        // Retira o próximo evento
        Evento evento;
        if (!retiraProximoEvento(&escalonador, &evento)) {
            fprintf(stderr, "Erro ao retirar evento do escalonador\n");
            break;
        }

        // Atualiza o tempo atual
        tempoAtual = evento.tempo;

        // Processa o evento com base no tipo
        switch (evento.tipo) {
            case EVENTO_CHEGADA:
                if (evento.paciente) {
                    Enfileira(&filas[0], evento.paciente, tempoAtual); // Exemplo: Enfileira na fila de triagem
                } else {
                    fprintf(stderr, "Erro: Paciente nulo no evento de chegada\n");
                }
                break;

            case EVENTO_TRIAGEM:
                // Processa evento de triagem e agenda próximos eventos
                break;

            case EVENTO_ALTA:
                // Finaliza atendimento do paciente
                break;

            default:
                fprintf(stderr, "Tipo de evento desconhecido\n");
                break;
        }

        // Atualiza estatísticas (se necessário)
    }

    // Gera saída
    FILE *saida = fopen("saida.txt", "w");
    if (!saida) {
        perror("Erro ao criar arquivo de saída");
        free(pacientes);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < totalPacientes; i++) {
        fprintf(saida, "%s %.2f %.2f %.2f\n",
                pacientes[i].id,
                pacientes[i].tempoEspera + pacientes[i].tempoAtendimento,
                pacientes[i].tempoAtendimento,
                pacientes[i].tempoEspera);
    }

    fclose(saida);

    // Finaliza recursos
    finalizaEscalonador(&escalonador);
    for (int i = 0; i < MAX_FILAS; i++) {
        Finaliza(&filas[i]);
    }

    free(pacientes);
    return 0;
}

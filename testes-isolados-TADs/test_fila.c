#include <stdio.h>
#include "fila.hpp"
#include "paciente.hpp"

int main() {
    // Inicializa uma fila
    Fila fila;
    Inicializa(&fila);

    // Cria dois pacientes para testar
    const char *linha1 = "0009600010 0 2017 3 21 2 1 3 4 2 8";
    const char *linha2 = "0009600011 0 2017 3 21 3 2 5 6 3 10";

    Paciente paciente1, paciente2;
    inicializaPaciente(&paciente1, linha1);
    inicializaPaciente(&paciente2, linha2);

    // Enfileira os pacientes
    printf("Enfileirando pacientes...\n");
    Enfileira(&fila, &paciente1, 0.0f); // Paciente 1 entra na fila no tempo 0.0
    Enfileira(&fila, &paciente2, 1.0f); // Paciente 2 entra na fila no tempo 1.0

    // Verifica o estado da fila
    printf("Tamanho da fila: %d\n", fila.tamanho);

    // Desenfileira o primeiro paciente
    printf("Desenfileirando paciente...\n");
    Paciente *atendido = Desenfileira(&fila, 2.0f); // Paciente 1 sai no tempo 2.0
    if (atendido) {
        printf("Paciente atendido: %s\n", atendido->id);
    }

    // Desenfileira o segundo paciente
    printf("Desenfileirando paciente...\n");
    atendido = Desenfileira(&fila, 3.0f); // Paciente 2 sai no tempo 3.0
    if (atendido) {
        printf("Paciente atendido: %s\n", atendido->id);
    }

    // Verifica se a fila está vazia
    if (FilaVazia(&fila)) {
        printf("A fila está vazia.\n");
    } else {
        printf("A fila ainda tem elementos.\n");
    }

    // Mostra estatísticas da fila
    printf("Estatísticas da Fila:\n");
    printf("Tempo Total de Espera: %.2f\n", fila.tempoTotalEspera);
    printf("Tempo Médio de Espera: %.2f\n", CalculaTempoMedioEspera(&fila));

    // Finaliza a fila
    Finaliza(&fila);

    return 0;
}

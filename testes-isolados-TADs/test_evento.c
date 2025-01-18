#include <stdio.h>
#include "evento.hpp"
#include "paciente.hpp"

int main() {
    // Cria dois pacientes
    const char *linha1 = "0009600010 0 2017 3 21 2 1 3 4 2 8";
    const char *linha2 = "0009600011 0 2017 3 21 3 2 5 6 3 10";

    Paciente paciente1, paciente2;
    inicializaPaciente(&paciente1, linha1);
    inicializaPaciente(&paciente2, linha2);

    // Cria eventos para os pacientes
    Evento evento1 = criaEvento(10.0, EVENTO_CHEGADA, &paciente1);
    Evento evento2 = criaEvento(12.0, EVENTO_TRIAGEM, &paciente2);

    // Mostra os dados dos eventos
    printf("Evento 1:\n");
    printf("  Tempo: %.2f\n", evento1.tempo);
    printf("  Tipo: %d\n", evento1.tipo);
    printf("  Paciente ID: %s\n", evento1.paciente->id);

    printf("Evento 2:\n");
    printf("  Tempo: %.2f\n", evento2.tempo);
    printf("  Tipo: %d\n", evento2.tipo);
    printf("  Paciente ID: %s\n", evento2.paciente->id);

    // Valida os eventos
    if (validaEvento(evento1)) {
        printf("Evento 1 é válido.\n");
    } else {
        printf("Evento 1 é inválido.\n");
    }

    if (validaEvento(evento2)) {
        printf("Evento 2 é válido.\n");
    } else {
        printf("Evento 2 é inválido.\n");
    }

    // Compara os eventos
    int comparacao = comparaEventos(&evento1, &evento2);
    if (comparacao < 0) {
        printf("Evento 1 ocorre antes do Evento 2.\n");
    } else if (comparacao > 0) {
        printf("Evento 2 ocorre antes do Evento 1.\n");
    } else {
        printf("Evento 1 e Evento 2 ocorrem no mesmo tempo.\n");
    }

    return 0;
}

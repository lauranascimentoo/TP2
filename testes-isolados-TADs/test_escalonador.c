#include <stdio.h>
#include "escalonador.hpp"
#include "evento.hpp"
#include "paciente.hpp"

int main() {
    // Inicializa o escalonador
    Escalonador escalonador;
    inicializaEscalonador(&escalonador, 5); // Capacidade inicial de 5 eventos

    printf("Escalonador inicializado com capacidade de 5 eventos.\n");

    // Cria pacientes para teste
    Paciente paciente1, paciente2, paciente3;
    const char *linha1 = "0009600010 0 2017 3 21 2 1 3 4 2 8";
    const char *linha2 = "0009600011 0 2017 3 21 3 2 5 6 3 10";
    const char *linha3 = "0009600012 0 2017 3 21 4 0 2 2 2 5";

    inicializaPaciente(&paciente1, linha1);
    inicializaPaciente(&paciente2, linha2);
    inicializaPaciente(&paciente3, linha3);

    // Cria e insere eventos no escalonador
    insereEvento(&escalonador, criaEvento(10.0, EVENTO_CHEGADA, &paciente1));
    insereEvento(&escalonador, criaEvento(5.0, EVENTO_ATENDIMENTO, &paciente2));
    insereEvento(&escalonador, criaEvento(15.0, EVENTO_EXAME, &paciente3));
    insereEvento(&escalonador, criaEvento(7.0, EVENTO_TRIAGEM, &paciente1));
    insereEvento(&escalonador, criaEvento(20.0, EVENTO_MEDICAMENTO, &paciente2));

    printf("Eventos inseridos no escalonador.\n");

    // Verifica a ordem dos eventos ao serem retirados
    Evento evento;
    printf("Retirando eventos na ordem correta:\n");
    while (retiraProximoEvento(&escalonador, &evento)) {
        printf("Evento retirado:\n");
        printf("  Tempo: %.2f\n", evento.tempo);
        printf("  Tipo: %d\n", evento.tipo);
        if (evento.paciente) {
            printf("  Paciente ID: %s\n", evento.paciente->id);
        } else {
            printf("  Evento sem paciente associado.\n");
        }
    }

    // Verifica o estado do escalonador após remoção
    if (escalonador.tamanho == 0) {
        printf("O escalonador está vazio após remover todos os eventos.\n");
    } else {
        printf("Erro: O escalonador ainda contém eventos.\n");
    }

    // Finaliza o escalonador
    finalizaEscalonador(&escalonador);
    printf("Escalonador finalizado e recursos liberados.\n");

    return 0;
}

#include <stdio.h>
#include "paciente.hpp"

int main() {
    // Simulando uma entrada de paciente do arquivo CSV
    const char *linha = "0009600010 0 2017 3 21 2 1 3 4 2 8";

    // Inicializa um paciente
    Paciente paciente;
    inicializaPaciente(&paciente, linha);

    // Mostra os dados iniciais do paciente
    printf("Dados Iniciais do Paciente:\n");
    printf("ID: %s\n", paciente.id);
    printf("Ano de Entrada: %d\n", paciente.ano);
    printf("Mes de Entrada: %d\n", paciente.mes);
    printf("Dia de Entrada: %d\n", paciente.dia);
    printf("Hora de Entrada: %d\n", paciente.hora);
    printf("Prioridade: %d\n", paciente.prioridade);
    printf("Medidas: %d\n", paciente.medidas);
    printf("Testes: %d\n", paciente.testes);
    printf("Exames: %d\n", paciente.exames);
    printf("Medicamentos: %d\n", paciente.medicamentos);

    // Testando transformação de horas
    float horasEntrada = transformaHoras(paciente.ano, paciente.mes, paciente.dia, paciente.hora);
    printf("Horas desde 01/01/2000: %.2f\n", horasEntrada);

    int ano, mes, dia, hora;
    transformaData(horasEntrada, &ano, &mes, &dia, &hora);
    printf("Data transformada: %02d/%02d/%04d %02d:00\n", dia, mes, ano, hora);

    // Simulando tempos de espera, atendimento e ocioso
    calculaTempoFila(&paciente, horasEntrada, horasEntrada + 2.0); // 2 horas na fila
    calculaTempoAtendimento(&paciente, horasEntrada + 2.0, horasEntrada + 4.0); // 2 horas de atendimento
    calculaTempoOcioso(&paciente, horasEntrada + 4.0, horasEntrada + 6.0); // 2 horas de ociosidade

    // Mostrando estatísticas acumuladas do paciente
    printf("Estatísticas do Paciente:\n");
    printf("Tempo Total de Espera: %.2f horas\n", paciente.tempoEspera);
    printf("Tempo Total de Atendimento: %.2f horas\n", paciente.tempoAtendimento);
    printf("Tempo Total Ocioso: %.2f horas\n", paciente.tempoOcioso);
    printf("Tempo Total no Hospital: %.2f horas\n", paciente.tempoTotal);

    // Testando avanço de estado
    printf("Estado Inicial: %d\n", paciente.estado);
    avancaEstado(&paciente);
    printf("Próximo Estado: %d\n", paciente.estado);

    // Adicionando 6 horas e convertendo novamente para data
    float horasFinal = horasEntrada + paciente.tempoTotal;
    int anoFinal, mesFinal, diaFinal, horaFinal;
    transformaData(horasFinal, &anoFinal, &mesFinal, &diaFinal, &horaFinal);
    printf("Data Final após adicionar %.2f horas: %02d/%02d/%04d %02d:00\n", paciente.tempoTotal, diaFinal, mesFinal, anoFinal, horaFinal);

    return 0;
}

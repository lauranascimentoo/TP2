#include <stdio.h>
#include "procedimento.hpp"

int main() {
    // Inicializa um procedimento
    Procedimento procedimento;
    inicializaProcedimento(&procedimento, 1.5, 3); // Tempo médio de 1.5 horas, 3 unidades

    printf("Procedimento inicializado:\n");
    printf("Tempo Médio: %.2f\n", procedimento.tempoMedio);
    printf("Número de Unidades: %d\n", procedimento.unidades);

    // Simula ocupação de unidades
    printf("Ocupando unidades...\n");
    ocupaUnidade(&procedimento, 0, 0.0, 2.0); // Unidade 0 ocupada de 0.0 a 2.0
    ocupaUnidade(&procedimento, 1, 1.0, 3.0); // Unidade 1 ocupada de 1.0 a 4.0

    // Atualiza o tempo ocioso
    atualizaTempoOcioso(&procedimento, 5.0); // Atualiza o tempo ocioso até o tempo 5.0

    // Calcula estatísticas
    float tempoTotalOcioso, tempoTotalOcupado;
    calculaEstatisticasProcedimento(&procedimento, &tempoTotalOcioso, &tempoTotalOcupado);

    // Exibe estatísticas
    printf("Estatísticas do Procedimento:\n");
    printf("Tempo Total Ocioso: %.2f\n", tempoTotalOcioso);
    printf("Tempo Total Ocupado: %.2f\n", tempoTotalOcupado);

    // Encontra uma unidade ociosa
    int unidadeLivre = encontraUnidadeOciosa(&procedimento, 5.0);
    if (unidadeLivre != -1) {
        printf("Unidade livre encontrada: %d\n", unidadeLivre);
    } else {
        printf("Nenhuma unidade livre encontrada.\n");
    }

    // Libera recursos
    finalizaProcedimento(&procedimento);
    printf("Recursos do procedimento liberados.\n");

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "procedimento.hpp"

// Inicializa um procedimento a partir de uma linha do arquivo CSV
void inicializaProcedimentoComLinha(Procedimento *proc, const char *linha) {
    if (!proc || !linha) {
        fprintf(stderr, "Erro: Parâmetros inválidos em inicializaProcedimentoComLinha.\n");
        exit(EXIT_FAILURE);
    }

    float tempoMedio;
    int unidades;

    if (sscanf(linha, "%f %d", &tempoMedio, &unidades) != 2) {
        fprintf(stderr, "Erro: Linha inválida em inicializaProcedimentoComLinha.\n");
        exit(EXIT_FAILURE);
    }

    inicializaProcedimento(proc, tempoMedio, unidades);
}

// Inicializa um procedimento com o tempo médio e o número de unidades
void inicializaProcedimento(Procedimento *proc, float tempoMedio, int unidades) {
    if (!proc || tempoMedio <= 0 || unidades <= 0) {
        fprintf(stderr, "Erro: Parâmetros inválidos em inicializaProcedimento.\n");
        exit(EXIT_FAILURE);
    }

    proc->tempoMedio = tempoMedio;
    proc->unidades = unidades;
    proc->statusUnidades = (int *)malloc(unidades * sizeof(int));
    proc->tempoOcioso = (float *)malloc(unidades * sizeof(float));
    proc->tempoOcupadoAte = (float *)malloc(unidades * sizeof(float));

    if (!proc->statusUnidades || !proc->tempoOcioso || !proc->tempoOcupadoAte) {
        perror("Erro ao alocar memória para o procedimento");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < unidades; i++) {
        proc->statusUnidades[i] = 0;     // 0: Ocioso
        proc->tempoOcioso[i] = 0.0f;    // Tempo ocioso inicial
        proc->tempoOcupadoAte[i] = 0.0f; // Nenhum tempo ocupado ainda
    }
}

// Retorna o índice de uma unidade ociosa ou -1 se todas estiverem ocupadas
int encontraUnidadeOciosa(Procedimento *proc, float tempoAtual) {
    if (!proc || tempoAtual < 0) {
        fprintf(stderr, "Erro: Parâmetros inválidos em encontraUnidadeOciosa.\n");
        return -1;
    }

    for (int i = 0; i < proc->unidades; i++) {
        if (proc->statusUnidades[i] == 0 || proc->tempoOcupadoAte[i] <= tempoAtual) {
            return i; // Retorna a primeira unidade disponível
        }
    }
    return -1; // Nenhuma unidade ociosa
}

// Atualiza o status de uma unidade para ocupada até um tempo específico
void ocupaUnidade(Procedimento *proc, int unidade, float tempoAtual, float tempoOcupacao) {
    if (!proc || unidade < 0 || unidade >= proc->unidades || tempoAtual < 0 || tempoOcupacao <= 0) {
        fprintf(stderr, "Erro: Parâmetros inválidos em ocupaUnidade.\n");
        return;
    }

    if (proc->tempoOcupadoAte[unidade] < tempoAtual) {
        proc->tempoOcioso[unidade] += tempoAtual - proc->tempoOcupadoAte[unidade];
    }

    proc->statusUnidades[unidade] = 1; // Ocupado
    proc->tempoOcupadoAte[unidade] = tempoAtual + tempoOcupacao;
}

// Atualiza o tempo ocioso das unidades até o momento atual
void atualizaTempoOcioso(Procedimento *proc, float tempoAtual) {
    if (!proc || tempoAtual < 0) {
        fprintf(stderr, "Erro: Parâmetros inválidos em atualizaTempoOcioso.\n");
        return;
    }

    for (int i = 0; i < proc->unidades; i++) {
        if (proc->tempoOcupadoAte[i] < tempoAtual) {
            proc->tempoOcioso[i] += tempoAtual - proc->tempoOcupadoAte[i];
            proc->tempoOcupadoAte[i] = tempoAtual;
            proc->statusUnidades[i] = 0; // Define como ocioso
        }
    }
}

// Retorna estatísticas gerais do procedimento (tempo total ocupado e ocioso)
void calculaEstatisticasProcedimento(Procedimento *proc, float *tempoTotalOcioso, float *tempoTotalOcupado) {
    if (!proc || !tempoTotalOcioso || !tempoTotalOcupado) {
        fprintf(stderr, "Erro: Parâmetros inválidos em calculaEstatisticasProcedimento.\n");
        return;
    }

    *tempoTotalOcioso = 0.0f;
    *tempoTotalOcupado = 0.0f;

    for (int i = 0; i < proc->unidades; i++) {
        *tempoTotalOcioso += proc->tempoOcioso[i];
        *tempoTotalOcupado += proc->tempoOcupadoAte[i] - proc->tempoOcioso[i];
    }
}

// Libera a memória alocada para o procedimento
void finalizaProcedimento(Procedimento *proc) {
    if (!proc) {
        fprintf(stderr, "Erro: Ponteiro de procedimento inválido em finalizaProcedimento.\n");
        return;
    }

    free(proc->statusUnidades);
    free(proc->tempoOcioso);
    free(proc->tempoOcupadoAte);
}

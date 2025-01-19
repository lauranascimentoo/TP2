#ifndef PROCEDIMENTO_HPP
#define PROCEDIMENTO_HPP

// Representa um procedimento com suas unidades, tempo médio e estados
typedef struct {
    int id;                // ID do procedimento
    float tempoMedio;      // Tempo médio de execução do procedimento
    int unidades;          // Número de unidades disponíveis para o procedimento
    int *statusUnidades;   // Array indicando o status de cada unidade (0: ocioso, 1: ocupado)
    float *tempoOcioso;    // Tempo total ocioso de cada unidade
    float *tempoOcupadoAte;// Tempo até o qual cada unidade está ocupada
} Procedimento;

// Inicializa um procedimento com tempo médio, número de unidades e ID
void inicializaProcedimento(Procedimento *proc, int id, float tempoMedio, int unidades);

// Inicializa um procedimento a partir de uma linha de configuração
void inicializaProcedimentoComLinha(Procedimento *proc, const char *linha, int id);

// Encontra uma unidade ociosa com base no tempo atual (ou -1 se todas estiverem ocupadas)
int encontraUnidadeOciosa(Procedimento *proc, float tempoAtual);

// Marca uma unidade como ocupada até um determinado tempo
void ocupaUnidade(Procedimento *proc, int unidade, float tempoAtual, float tempoOcupacao);

// Atualiza os tempos ociosos das unidades com base no tempo atual
void atualizaTempoOcioso(Procedimento *proc, float tempoAtual);

// Calcula estatísticas globais: tempo total ocioso e ocupado
void calculaEstatisticasProcedimento(Procedimento *proc, float *tempoTotalOcioso, float *tempoTotalOcupado);

// Libera os recursos alocados para o procedimento
void finalizaProcedimento(Procedimento *proc);

#endif // PROCEDIMENTO_HPP

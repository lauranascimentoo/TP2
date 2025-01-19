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

// PACIENTE
void imprimeEstadoPaciente(const Paciente *paciente) {
    printf("Paciente ID: %s\n", paciente->id);
    printf("  Teve Alta: %s\n", paciente->teveAlta ? "Sim" : "Não");
    printf("  Data/Hora Entrada: %04d-%02d-%02d %02d:00\n", paciente->ano, paciente->mes, paciente->dia, paciente->hora);
    printf("  Data/Hora Saída: %04d-%02d-%02d %02d:00\n", paciente->ano_s, paciente->mes_s, paciente->dia_s, paciente->hora_s);
    printf("  Prioridade: %d\n", paciente->prioridade);
    printf("  Nº Medidas Hospitalares: %d\n", paciente->medidas);
    printf("  Nº Testes de Laboratório: %d\n", paciente->testes);
    printf("  Nº Exames de Imagem: %d\n", paciente->exames);
    printf("  Nº Medicamentos/Instrumentos: %d\n", paciente->medicamentos);
    printf("  Estado Atual: %d\n", paciente->estado);
    printf("  Tempo Espera: %.2f horas\n", paciente->tempoEspera);
    printf("  Tempo Atendimento: %.2f horas\n", paciente->tempoAtendimento);
    printf("  Tempo Ocioso: %.2f horas\n", paciente->tempoOcioso);
    printf("  Hora Entrada: %.2f\n", paciente->horaEntrada);
    printf("  Hora Saída: %.2f\n", paciente->horaSaida);
    printf("  Tempo Total no Hospital: %.2f horas\n", paciente->tempoTotal);
}

// Avança o paciente para o próximo estado
void avancaEstadoPaciente(Escalonador *escalonador, Paciente *paciente, float tempoAtual, TipoEvento proximoEvento, Fila *filas, int filaDestino) {
    Evento novoEvento = criaEvento(tempoAtual, proximoEvento, paciente);
    insereEvento(escalonador, novoEvento);
    Enfileira(&filas[filaDestino], paciente, tempoAtual);
    paciente->estado++;
}


// FILAS
// Enum para facilitar o acesso às filas
enum {
    FILA_TRIAGEM = 0,              // Fila 0: Triagem
    FILA_ATEND_PRIORIDADE_2,       // Fila 1: Atendimento Prioridade 2 (vermelho)
    FILA_ATEND_PRIORIDADE_1,       // Fila 2: Atendimento Prioridade 1 (amarelo)
    FILA_ATEND_PRIORIDADE_0,       // Fila 3: Atendimento Prioridade 0 (verde)
    FILA_MH_PRIORIDADE_2,          // Fila 4: Medidas Hospitalares Prioridade 2
    FILA_MH_PRIORIDADE_1,          // Fila 5: Medidas Hospitalares Prioridade 1
    FILA_MH_PRIORIDADE_0,          // Fila 6: Medidas Hospitalares Prioridade 0
    FILA_TL_PRIORIDADE_2,          // Fila 7: Testes de Laboratório Prioridade 2
    FILA_TL_PRIORIDADE_1,          // Fila 8: Testes de Laboratório Prioridade 1
    FILA_TL_PRIORIDADE_0,          // Fila 9: Testes de Laboratório Prioridade 0
    FILA_EI_PRIORIDADE_2,          // Fila 10: Exames de Imagem Prioridade 2
    FILA_EI_PRIORIDADE_1,          // Fila 11: Exames de Imagem Prioridade 1
    FILA_EI_PRIORIDADE_0,          // Fila 12: Exames de Imagem Prioridade 0
    FILA_IM_PRIORIDADE_2,          // Fila 13: Medicamentos/Instrumentos Prioridade 2
    FILA_IM_PRIORIDADE_1,          // Fila 14: Medicamentos/Instrumentos Prioridade 1
    FILA_IM_PRIORIDADE_0,          // Fila 15: Medicamentos/Instrumentos Prioridade 0
    MAX_FILAS_ENUM                 // Total de filas
};

void inicializaFilas(Fila *filas) {
    for (int i = 0; i < MAX_FILAS_ENUM; i++) {
        Inicializa(&filas[i]);
        // printf("Fila %d inicializada.\n", i);
    }
}

// PROCEDIMENTO
// Enum para facilitar o acesso àos procedimentos
enum {
    TRIAGEM = 0,              // Triagem
    ATEND,                    // Atendimento
    MH,                       // Medidas Hospitalares
    TL,                       // Testes de Laboratório
    EI,                       // Exames de Imagem
    IM,                       // Medicamentos/Instrumentos
    MAX_PROCEDIMENTOS_ENUM    // Total de procedimentos
};

void inicProcedimentos(Procedimento *procedimentos, const char **dadosProcedimento) {
    for (int i = 0; i < MAX_PROCEDIMENTOS_ENUM; i++) {
        float tempoMedio;
        int unidades;

        // Lê o tempo médio e o número de unidades do procedimento
        if (sscanf(dadosProcedimento[i], "%f %d", &tempoMedio, &unidades) != 2) {
            fprintf(stderr, "Erro ao processar dados do procedimento %d.\n", i);
            exit(EXIT_FAILURE);
        }

        // Inicializa o procedimento
        inicializaProcedimento(&procedimentos[i], i, tempoMedio, unidades);

        // Exibe informações do procedimento inicializado
        // printf("Procedimento %d inicializado:\n", i);
        // printf("  Tempo Médio: %.2f\n", tempoMedio);
        // printf("  Unidades: %d\n", unidades);
    }
}

// Processa o paciente em um procedimento
//void processaProcedimento(Escalonador *escalonador, Paciente *paciente, Procedimento *procedimento, float tempoAtual, TipoEvento proximoEvento, Fila *filas, int filaDestino) {
void processaProcedimento(Escalonador *escalonador, Paciente *paciente, Procedimento *procedimento, float tempoAtual) {
    int unidade = encontraUnidadeOciosa(procedimento, tempoAtual);
    float tempoProcedimento = procedimento->tempoMedio; // Tempo médio para este procedimento
    
    if (unidade >= 0) {
        ocupaUnidade(procedimento, unidade, tempoAtual, procedimento->tempoMedio);
        // calculaTempoAtendimento(paciente, tempoAtual, tempoAtual + procedimento->tempoMedio);
        //printf("Paciente %s completou parte do procedimento ID %d (Tempo Médio: %.2f horas).\n",
        //       paciente->id, procedimento->id, tempoProcedimento);
    } else {
        //fprintf(stderr, "Erro: Nenhuma unidade disponível para o procedimento %d\n", procedimento->id);
    }
}

// TRATAMENTO FONTE
void leArquivo(const char *caminhoArquivo, char ***dadosProcedimento, char **dadosNumeroPacientes, char ***dadosPacientes, int *totalPacientes) {
    FILE *arquivo = fopen(caminhoArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    int linhaAtual = 0, pacientesIndex = 0;

    // Alocar memória para os dados
    *dadosProcedimento = (char **)malloc(6 * sizeof(char *));
    if (!*dadosProcedimento) {
        perror("Erro ao alocar memória para dadosProcedimento");
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    // Lê linha por linha
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove o caractere de nova linha, se presente
        linha[strcspn(linha, "\n")] = '\0';

        if (linhaAtual < 6) {
            // Dados de procedimentos (linhas 1 a 6)
            (*dadosProcedimento)[linhaAtual] = strdup(linha);
        } else if (linhaAtual == 6) {
            // Número de pacientes (linha 7)
            *dadosNumeroPacientes = strdup(linha);
            *totalPacientes = atoi(linha);
            // Alocar memória para os pacientes
            *dadosPacientes = (char **)malloc(*totalPacientes * sizeof(char *));
            if (!*dadosPacientes) {
                perror("Erro ao alocar memória para dadosPacientes");
                fclose(arquivo);
                exit(EXIT_FAILURE);
            }
        } else {
            // Dados de pacientes (linhas 8 em diante)
            (*dadosPacientes)[pacientesIndex++] = strdup(linha);
        }
        linhaAtual++;
    }

    fclose(arquivo);
}

int main() {
    int condicaoTermino = 0;

    //Estrutura do Arquivo
    char **dadosProcedimento = NULL;
    char *dadosNumeroPacientes = NULL;
    char **dadosPacientes = NULL;
    int totalPacientes = 0;

    const char *caminhoArquivo = "20242TP2.csv";

    leArquivo(caminhoArquivo, &dadosProcedimento, &dadosNumeroPacientes, &dadosPacientes, &totalPacientes);

    float tempoAtual = 0.0f;
    
    // Inicializa o escalonador
    Escalonador escalonador;
    inicializaEscalonador(&escalonador, 1000); //100 número arbitrário

    // Inicializa as filas
    Fila filas[MAX_FILAS_ENUM];
    for (int i = 0; i < MAX_FILAS_ENUM; i++) {
        Inicializa(&filas[i]);
    }

    // Inicializa os procedimentos
    Procedimento procedimentos[MAX_PROCEDIMENTOS_ENUM];
    for (int i = 0; i < MAX_PROCEDIMENTOS_ENUM; i++) {
        inicializaProcedimentoComLinha(&procedimentos[i], dadosProcedimento[i], i);
    }

    // Inicializa pacientes
    totalPacientes = atoi(dadosNumeroPacientes); //Colaca em formato de número o número total de pacientes
    Paciente *pacientes = (Paciente *)malloc(totalPacientes * sizeof(Paciente)); //Alocar memória para os pacientes
    if (!pacientes) {
        perror("Erro ao alocar memória para pacientes");
        return EXIT_FAILURE;
    }

    // printf("=== Inicialização dos Pacientes e Evento ===\n\n");

    for (int i = 0; i < totalPacientes; i++) {
        inicializaPaciente(&pacientes[i], dadosPacientes[i]);
        // printf("Paciente %d inicializado:\n", i + 1);
        float tempoEntrada = transformaHoras(
            pacientes[i].ano, pacientes[i].mes, pacientes[i].dia, pacientes[i].hora
        );
        pacientes[i].horaEntrada = tempoEntrada;
        // imprimeEstadoPaciente(&pacientes[i]);

        Evento eventoInicial = criaEvento(tempoEntrada, EVENTO_CHEGADA, &pacientes[i]);
        insereEvento(&escalonador, eventoInicial);
    }

// printf("=== Início da Simulação ===\n\n");
while (!condicaoTermino) {
    if (escalonador.tamanho == 0) {
        condicaoTermino = 1;
        break;
    }

    Evento evento;
    if (!retiraProximoEvento(&escalonador, &evento)) {
        fprintf(stderr, "Erro ao retirar evento do escalonador\n");
        break;
    }

    tempoAtual = evento.tempo;

    switch (evento.tipo) {
        case EVENTO_CHEGADA:
            avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual, EVENTO_TRIAGEM, filas, FILA_TRIAGEM);
            break;

        case EVENTO_TRIAGEM: {
            float tempoProcedimento = procedimentos[TRIAGEM].tempoMedio;
            processaProcedimento(&escalonador, evento.paciente, &procedimentos[TRIAGEM], tempoAtual);
            evento.paciente->tempoAtendimento += tempoProcedimento;
            evento.paciente->tempoTotal += tempoProcedimento;
            avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual + tempoProcedimento, EVENTO_ATENDIMENTO, filas, FILA_ATEND_PRIORIDADE_2 - evento.paciente->prioridade);
            break;
        }

        case EVENTO_ATENDIMENTO: {
            float tempoProcedimento = procedimentos[ATEND].tempoMedio;
            processaProcedimento(&escalonador, evento.paciente, &procedimentos[ATEND], tempoAtual);
            evento.paciente->tempoAtendimento += tempoProcedimento;
            evento.paciente->tempoTotal += tempoProcedimento;
            avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual + tempoProcedimento, EVENTO_MEDIDAS, filas, FILA_MH_PRIORIDADE_2 - evento.paciente->prioridade);
            break;
        }

        case EVENTO_MEDIDAS: {
            if (evento.paciente->medidas > 0) {
                evento.paciente->medidas--;
                float tempoProcedimento = procedimentos[MH].tempoMedio;
                processaProcedimento(&escalonador, evento.paciente, &procedimentos[MH], tempoAtual);
                evento.paciente->tempoAtendimento += tempoProcedimento;
                evento.paciente->tempoTotal += tempoProcedimento;
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual + tempoProcedimento, EVENTO_MEDIDAS, filas, FILA_MH_PRIORIDADE_2 - evento.paciente->prioridade);
            } else {
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual, EVENTO_TESTES, filas, FILA_TL_PRIORIDADE_2 - evento.paciente->prioridade);
            }
            break;
        }

        case EVENTO_TESTES: {
            if (evento.paciente->testes > 0) {
                evento.paciente->testes--;
                float tempoProcedimento = procedimentos[TL].tempoMedio;
                processaProcedimento(&escalonador, evento.paciente, &procedimentos[TL], tempoAtual);
                evento.paciente->tempoAtendimento += tempoProcedimento;
                evento.paciente->tempoTotal += tempoProcedimento;
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual + tempoProcedimento, EVENTO_TESTES, filas, FILA_TL_PRIORIDADE_2 - evento.paciente->prioridade);
            } else {
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual, EVENTO_EXAME, filas, FILA_EI_PRIORIDADE_2 - evento.paciente->prioridade);
            }
            break;
        }

        case EVENTO_EXAME: {
            if (evento.paciente->exames > 0) {
                evento.paciente->exames--;
                float tempoProcedimento = procedimentos[EI].tempoMedio;
                processaProcedimento(&escalonador, evento.paciente, &procedimentos[EI], tempoAtual);
                evento.paciente->tempoAtendimento += tempoProcedimento;
                evento.paciente->tempoTotal += tempoProcedimento;
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual + tempoProcedimento, EVENTO_EXAME, filas, FILA_EI_PRIORIDADE_2 - evento.paciente->prioridade);
            } else {
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual, EVENTO_MEDICAMENTO, filas, FILA_IM_PRIORIDADE_2 - evento.paciente->prioridade);
            }
            break;
        }

        case EVENTO_MEDICAMENTO: {
            if (evento.paciente->medicamentos > 0) {
                evento.paciente->medicamentos--;
                float tempoProcedimento = procedimentos[IM].tempoMedio;
                processaProcedimento(&escalonador, evento.paciente, &procedimentos[IM], tempoAtual);
                evento.paciente->tempoAtendimento += tempoProcedimento;
                evento.paciente->tempoTotal += tempoProcedimento;
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual + tempoProcedimento, EVENTO_MEDICAMENTO, filas, FILA_IM_PRIORIDADE_2 - evento.paciente->prioridade);
            } else {
                avancaEstadoPaciente(&escalonador, evento.paciente, tempoAtual, EVENTO_ALTA, filas, 0);
            }
            break;
        }

        case EVENTO_ALTA:
            evento.paciente->horaSaida = tempoAtual;
            evento.paciente->estado = 14;
            // printf("Paciente %s recebeu alta no tempo %.2f\n", evento.paciente->id, tempoAtual);
            break;

        default:
            fprintf(stderr, "Erro: Evento desconhecido\n");
            break;
    }
}

// printf("\nResumo de Tempos por Paciente:\n");
// for (int i = 0; i < totalPacientes; i++) {
//     printf("Paciente %s:\n", pacientes[i].id);
//     printf("  Tempo Total no Hospital: %.2f horas\n", pacientes[i].tempoTotal);
//     printf("  Tempo Total de Atendimento: %.2f horas\n", pacientes[i].tempoAtendimento);
//     printf("  Tempo Total de Espera: %.2f horas\n", pacientes[i].tempoEspera);
//     printf("  Medidas Restantes: %d\n", pacientes[i].medidas);
//     printf("  Testes Restantes: %d\n", pacientes[i].testes);
//     printf("  Exames Restantes: %d\n", pacientes[i].exames);
//     printf("  Medicamentos Restantes: %d\n\n", pacientes[i].medicamentos);
// }

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

    for (int i = 0; i < 6; i++) {
        free(dadosProcedimento[i]);
    }
    free(dadosProcedimento);

    free(dadosNumeroPacientes);

    for (int i = 0; i < totalPacientes; i++) {
        free(dadosPacientes[i]);
    }
    free(dadosPacientes);

    return 0;
}

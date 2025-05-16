#ifndef MESA_H
#define MESA_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "estrutura_jogador.h"
#include "cartas.h"
#include "pilhaLE.h"
#include "listade.h"

// Constantes do jogo
#define MAX_JOGADORES 8       // Número máximo de jogadores na mesa
#define CARTAS_COMUNITARIAS 5 // Número de cartas comunitárias (Flop+Turn+River)

/**
 * Enumeração das combinações implementadas (6 no total)
 * Ordenadas da mais forte para a mais fraca
 */
typedef enum {
    FULL_HOUSE,    // Trinca + Par (ex: AAAKK)
    FLUSH,         // 5 cartas do mesmo naipe (não em sequência)
    SEQUENCIA,     // 5 cartas em sequência (naipes diferentes)
    TRINCA,        // Três cartas do mesmo valor (ex: AAA)
    DOIS_PARES,    // Dois pares diferentes (ex: AAKKQ)
    PAR,           // Um par de cartas (ex: AAKQJ)
    CARTA_ALTA     // Nenhuma combinação válida
} Combinacao;

/**
 * Estrutura que representa a mesa de poker
 * Contém todas as informações do estado atual do jogo
 */
typedef struct {
    tp_pilha baralho;                   // Baralho de cartas (pilha)
    jogador *jogadores[MAX_JOGADORES];  // Array de ponteiros para jogadores
    tp_listad *cartas_comunitarias;     // Lista de cartas comunitárias
    int num_jogadores;                  // Número atual de jogadores
    int dealer_pos;                     // Posição do dealer na mesa
    int pote;                           // Valor total do pote atual
} Mesa;

// Protótipos das funções públicas
void inicializar_mesa(Mesa *mesa);
void preparar_cartas_mesa(Mesa *mesa);
void adicionar_jogador_a_mesa(Mesa *mesa, jogador *j);
void distribuir_cartas_jogadores(Mesa *mesa);
Combinacao avaliar_mao(jogador *j, tp_listad *comunitarias);
void realizar_rodada(Mesa *mesa);

// Funções auxiliares (visibilidade restrita a este arquivo)
static int converter_valor_para_indice(const char *valor);
static int contar_ocorrencias_valores(tp_listad *cartas, int contagem[13]);
static int contar_ocorrencias_naipes(tp_listad *cartas, int naipes[4]);
static int verificar_par(tp_listad *cartas);
static int verificar_dois_pares(tp_listad *cartas);
static int verificar_trinca(tp_listad *cartas);
static int verificar_sequencia(tp_listad *cartas);
static int verificar_flush(tp_listad *cartas);
static int verificar_full_house(tp_listad *cartas);

/**
 * Inicializa o estado da mesa
 * @param mesa Ponteiro para a estrutura Mesa a ser inicializada
 */
void inicializar_mesa(Mesa *mesa) {
    Baralho b;
    inicializaBaralho(&b);          // Inicializa um novo baralho
    embaralha(&b, &mesa->baralho);  // Embaralha as cartas

    mesa->num_jogadores = 0;        // Sem jogadores inicialmente
    mesa->dealer_pos = 0;           // Dealer começa na posição 0
    mesa->cartas_comunitarias = inicializa_listad(); // Inicializa lista de cartas comunitárias

    // Queima a primeira carta (regra do poker)
    Carta queimada;
    pop(&mesa->baralho, &queimada);
}

/**
 * Prepara as cartas comunitárias (Flop, Turn, River)
 * @param mesa Ponteiro para a estrutura Mesa
 */
void preparar_cartas_mesa(Mesa *mesa) {
    // Limpa cartas comunitárias existentes (se houver)
    if (mesa->cartas_comunitarias->ini != NULL) {
        destroi_listad(mesa->cartas_comunitarias);
        mesa->cartas_comunitarias = inicializa_listad();
    }

    // Flop: distribui 3 cartas (com uma queima antes)
    Carta queimada;
    pop(&mesa->baralho, &queimada); // Queima uma carta antes do flop

    for (int i = 0; i < 3; i++) {
        Carta c;
        pop(&mesa->baralho, &c);
        insere_listad_no_fim(mesa->cartas_comunitarias, c);
    }

    // Turn: distribui 1 carta (com uma queima antes)
    pop(&mesa->baralho, &queimada); // Queima antes do turn
    
    Carta turn;
    pop(&mesa->baralho, &turn);
    insere_listad_no_fim(mesa->cartas_comunitarias, turn);

    // River: distribui 1 carta (com uma queima antes)
    pop(&mesa->baralho, &queimada); // Queima antes do river
    
    Carta river;
    pop(&mesa->baralho, &river);
    insere_listad_no_fim(mesa->cartas_comunitarias, river);
}

/**
 * Adiciona um jogador à mesa
 * @param mesa Ponteiro para a estrutura Mesa
 * @param j Ponteiro para o jogador a ser adicionado
 */
void adicionar_jogador_a_mesa(Mesa *mesa, jogador *j) {
    if (mesa->num_jogadores < MAX_JOGADORES) {
        mesa->jogadores[mesa->num_jogadores] = j;
        mesa->num_jogadores++;
    }
}

/**
 * Distribui cartas para todos os jogadores (2 cartas cada)
 * @param mesa Ponteiro para a estrutura Mesa
 */
void distribuir_cartas_jogadores(Mesa *mesa) {
    // Distribui duas cartas para cada jogador
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < mesa->num_jogadores; j++) {
            Carta c;
            pop(&mesa->baralho, &c);
            insere_listad_no_fim(mesa->jogadores[j]->mao, c);
        }
    }
}

/**
 * Converte o valor da carta para um índice numérico
 * @param valor String representando o valor da carta ("2" a "A")
 * @return Índice numérico (0 a 12) ou -1 se inválido
 */
static int converter_valor_para_indice(const char *valor) {
    const char *valores[] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    for (int i = 0; i < 13; i++) {
        if (strcmp(valor, valores[i]) == 0) {
            return i;
        }
    }
    return -1; // Valor inválido
}

/**
 * Conta a ocorrência de cada valor de carta
 * @param cartas Lista de cartas a serem analisadas
 * @param contagem Array para armazenar a contagem (deve ter tamanho 13)
 * @return 1 se sucesso, 0 se erro
 */
static int contar_ocorrencias_valores(tp_listad *cartas, int contagem[13]) {
    if (!cartas || !contagem) return 0;

    // Inicializa contagem
    for (int i = 0; i < 13; i++) {
        contagem[i] = 0;
    }

    // Conta cada valor de carta
    tp_no_lista *atu = cartas->ini;
    while (atu != NULL) {
        int indice = converter_valor_para_indice(atu->info.valor);
        if (indice != -1) {
            contagem[indice]++;
        }
        atu = atu->prox;
    }
    return 1;
}

/**
 * Conta a ocorrência de cada naipe
 * @param cartas Lista de cartas a serem analisadas
 * @param naipes Array para armazenar a contagem (0=Copas, 1=Ouros, 2=Espadas, 3=Paus)
 * @return 1 se sucesso, 0 se erro
 */
static int contar_ocorrencias_naipes(tp_listad *cartas, int naipes[4]) {
    if (!cartas || !naipes) return 0;

    // Inicializa contagem
    for (int i = 0; i < 4; i++) {
        naipes[i] = 0;
    }

    // Conta cada naipe
    tp_no_lista *atu = cartas->ini;
    while (atu != NULL) {
        if (strcmp(atu->info.naipe, "Copas") == 0) naipes[0]++;
        else if (strcmp(atu->info.naipe, "Ouros") == 0) naipes[1]++;
        else if (strcmp(atu->info.naipe, "Espadas") == 0) naipes[2]++;
        else if (strcmp(atu->info.naipe, "Paus") == 0) naipes[3]++;
        atu = atu->prox;
    }
    return 1;
}

/**
 * Verifica se há pelo menos um par na mão
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver par, 0 caso contrário
 */
static int verificar_par(tp_listad *cartas) {
    int contagem[13];
    if (!contar_ocorrencias_valores(cartas, contagem)) return 0;

    for (int i = 0; i < 13; i++) {
        if (contagem[i] >= 2) {
            return 1;
        }
    }
    return 0;
}

/**
 * Verifica se há dois pares na mão
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver dois pares, 0 caso contrário
 */
static int verificar_dois_pares(tp_listad *cartas) {
    int contagem[13];
    if (!contar_ocorrencias_valores(cartas, contagem)) return 0;

    int pares = 0;
    for (int i = 0; i < 13; i++) {
        if (contagem[i] >= 2) {
            pares++;
        }
    }
    return (pares >= 2);
}

/**
 * Verifica se há uma trinca na mão
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver trinca, 0 caso contrário
 */
static int verificar_trinca(tp_listad *cartas) {
    int contagem[13];
    if (!contar_ocorrencias_valores(cartas, contagem)) return 0;

    for (int i = 0; i < 13; i++) {
        if (contagem[i] >= 3) {
            return 1;
        }
    }
    return 0;
}

/**
 * Verifica se há uma sequência de 5 cartas
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver sequência, 0 caso contrário
 */
static int verificar_sequencia(tp_listad *cartas) {
    int contagem[13] = {0};
    if (!contar_ocorrencias_valores(cartas, contagem)) return 0;

    // Verifica sequência normal (5 cartas consecutivas)
    for (int i = 0; i <= 8; i++) { // Até 8 porque precisamos de 5 cartas (i+4)
        if (contagem[i] && contagem[i+1] && contagem[i+2] && contagem[i+3] && contagem[i+4]) {
            return 1;
        }
    }

    // Verifica sequência especial (A-2-3-4-5)
    if (contagem[12] && contagem[0] && contagem[1] && contagem[2] && contagem[3]) {
        return 1;
    }

    return 0;
}

/**
 * Verifica se há um flush (5 cartas do mesmo naipe)
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver flush, 0 caso contrário
 */
static int verificar_flush(tp_listad *cartas) {
    int naipes[4];
    if (!contar_ocorrencias_naipes(cartas, naipes)) return 0;

    for (int i = 0; i < 4; i++) {
        if (naipes[i] >= 5) {
            return 1;
        }
    }
    return 0;
}

/**
 * Verifica se há um full house (trinca + par)
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver full house, 0 caso contrário
 */
static int verificar_full_house(tp_listad *cartas) {
    int contagem[13];
    if (!contar_ocorrencias_valores(cartas, contagem)) return 0;

    int tem_trinca = 0, tem_par = 0;
    for (int i = 0; i < 13; i++) {
        if (contagem[i] >= 3) tem_trinca = 1;
        if (contagem[i] >= 2) tem_par = 1;
    }
    return (tem_trinca && tem_par);
}

/**
 * Avalia a melhor combinação possível para um jogador
 * @param j Ponteiro para o jogador
 * @param comunitarias Lista de cartas comunitárias
 * @return A melhor combinação encontrada
 */
Combinacao avaliar_mao(jogador *j, tp_listad *comunitarias) {
    // Combina as cartas do jogador com as comunitárias
    tp_listad *todas_cartas = inicializa_listad();

    // Adiciona cartas da mão do jogador
    tp_no_lista *atu = j->mao->ini;
    while (atu != NULL) {
        insere_listad_no_fim(todas_cartas, atu->info);
        atu = atu->prox;
    }

    // Adiciona cartas comunitárias
    atu = comunitarias->ini;
    while (atu != NULL) {
        insere_listad_no_fim(todas_cartas, atu->info);
        atu = atu->prox;
    }

    // Verifica combinações da melhor para a pior
    if (verificar_full_house(todas_cartas)) {
        destroi_listad(todas_cartas);
        return FULL_HOUSE;
    }
    if (verificar_flush(todas_cartas)) {
        destroi_listad(todas_cartas);
        return FLUSH;
    }
    if (verificar_sequencia(todas_cartas)) {
        destroi_listad(todas_cartas);
        return SEQUENCIA;
    }
    if (verificar_trinca(todas_cartas)) {
        destroi_listad(todas_cartas);
        return TRINCA;
    }
    if (verificar_dois_pares(todas_cartas)) {
        destroi_listad(todas_cartas);
        return DOIS_PARES;
    }
    if (verificar_par(todas_cartas)) {
        destroi_listad(todas_cartas);
        return PAR;
    }

    // Se não encontrou nenhuma combinação válida
    destroi_listad(todas_cartas);
    return CARTA_ALTA;
}

/**
 * Realiza uma rodada completa do jogo
 * @param mesa Ponteiro para a estrutura Mesa
 */
void realizar_rodada(Mesa *mesa) {
    // 1. Prepara as cartas comunitárias
    preparar_cartas_mesa(mesa);

    // 2. Mostra as cartas comunitárias
    printf("\nCartas Comunitárias:\n");
    imprime_listad(mesa->cartas_comunitarias, 1);

    // 3. Mostra as mãos de todos os jogadores
    for (int i = 0; i < mesa->num_jogadores; i++) {
        printf("\nMão de %s:\n", mesa->jogadores[i]->nome);
        imprime_listad(mesa->jogadores[i]->mao, 1);
    }

    // 4. Avalia as mãos de todos os jogadores
    Combinacao melhores_maos[MAX_JOGADORES];
    for (int i = 0; i < mesa->num_jogadores; i++) {
        melhores_maos[i] = avaliar_mao(mesa->jogadores[i], mesa->cartas_comunitarias);
    }

    // 5. Determina o vencedor (maior combinação)
    Combinacao melhor_combinacao = CARTA_ALTA;
    int vencedores[MAX_JOGADORES] = {0};
    int num_vencedores = 0;

    for (int i = 0; i < mesa->num_jogadores; i++) {
        if (melhores_maos[i] < melhor_combinacao) {
            melhor_combinacao = melhores_maos[i];
            num_vencedores = 0;
            vencedores[num_vencedores++] = i;
        } else if (melhores_maos[i] == melhor_combinacao) {
            vencedores[num_vencedores++] = i;
        }
    }

    // 6. Distribui o pote
    if (num_vencedores > 0) {
        int valor_por_vencedor = mesa->pote / num_vencedores;
        for (int i = 0; i < num_vencedores; i++) {
            mesa->jogadores[vencedores[i]]->fichas += valor_por_vencedor;
        }
    }

    // 7. Mostra os resultados
    printf("\n=== RESULTADO DA RODADA ===\n");
    printf("Melhor combinação: ");
    switch (melhor_combinacao) {
        case FULL_HOUSE: printf("Full House\n"); break;
        case FLUSH: printf("Flush\n"); break;
        case SEQUENCIA: printf("Sequência\n"); break;
        case TRINCA: printf("Trinca\n"); break;
        case DOIS_PARES: printf("Dois Pares\n"); break;
        case PAR: printf("Par\n"); break;
        default: printf("Carta Alta\n");
    }

    if(strlen(vencedores) > 1){
        printf("Empate entre: ");
    } else{
        printf("Vencedor: ");
    }
    
    for (int i = 0; i < num_vencedores; i++) {
        printf("%s ", mesa->jogadores[vencedores[i]]->nome);
    }
    printf("\nPrêmio por jogador: %d fichas\n", mesa->pote / num_vencedores);
}

#endif

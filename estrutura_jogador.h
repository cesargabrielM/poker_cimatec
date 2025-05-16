#ifndef ESTRUTURA_JOGADOR
#define ESTRUTURA_JOGADOR

#include <stdio.h>
#include <string.h>

#include "listade.h"
#include "pilhaLE.h"

// Constantes
#define MAXIMO_NOME 20  // Tamanho máximo para nome do jogador
#define NUM_CARTAS 2     // Número de cartas na mão de cada jogador

/**
 * Enumeração dos estados possíveis de um jogador
 */
typedef enum {
    fora,    // Jogador não está na partida
    foldou,  // Jogador desistiu da rodada
    ativo    // Jogador está ativo na rodada
} estado;

/**
 * Estrutura que representa um jogador
 */
typedef struct {
    char nome[MAXIMO_NOME];  // Nome do jogador
    int fichas;              // Quantidade de fichas
    tp_listad *mao;          // Lista de cartas na mão (2 cartas)
    estado atual;            // Estado atual do jogador
    int aposta;              // Valor apostado na rodada atual
    int posicaoMesa;         // Posição física na mesa
} jogador;

/**
 * Inicializa um novo jogador
 * @param j Ponteiro para o jogador
 * @param nome Nome do jogador
 * @param fichas Fichas iniciais
 * @param posicaoMesa Posição na mesa
 */
void criarJogador(jogador *j, const char *nome, int fichas, int posicaoMesa) {
    strncpy(j->nome, nome, MAXIMO_NOME); // Copia o nome
    j->fichas = fichas;                  // Define fichas iniciais
    j->atual = ativo;                    // Começa como ativo
    j->aposta = 0;                       // Aposta inicial zero
    j->posicaoMesa = posicaoMesa;        // Define posição na mesa
    j->mao = inicializa_listad();        // Inicializa lista de cartas

    printf("\nJogador %s criado com %d fichas (Posição %d)\n", 
           j->nome, j->fichas, j->posicaoMesa);
}

#endif

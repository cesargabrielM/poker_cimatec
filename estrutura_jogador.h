#ifndef ESTRUTURA_JOGADOR
#define ESTRUTURA_JOGADOR

#include <stdio.h>
#include <string.h>

#include "listade.h"
#include "pilhaLE.h"

#define MAXIMO_NOME 20 
#define NUM_CARTAS 2

typedef enum {fora, foldou, ativo} estado;

typedef struct {
    char nome[MAXIMO_NOME];
    int fichas;
    tp_listad *mao;
    estado atual;
    int aposta; // Aposta total na mão inteira
    int aposta_rodada; // Aposta na rodada atual (Flop, Turn, etc.)
    int posicaoMesa;
} jogador;

// Modifiquei também a função criarJogador para inicializar o novo campo
static void criarJogador(jogador *j, const char *nome, int fichas, int posicaoMesa) {
    strncpy(j->nome, nome, MAXIMO_NOME);
    j->fichas = fichas;
    j->atual = ativo;
    j->aposta = 0;
    j->aposta_rodada = 0; //Inicializa com 0
    j->posicaoMesa = posicaoMesa;
    j->mao = inicializa_listad();
    
    printf("\nJogador %s criado com %d fichas (Posição %d)\n", j->nome, j->fichas, j->posicaoMesa);
}

#endif

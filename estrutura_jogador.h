#ifndef ESTRUTURA_JOGADOR
#define ESTRUTURA_JOGADOR
#include <stdio.h>
#include <string.h>
#include "listade.h"
#include "pilhaLE.h"
#define MAXIMO_NOME 20 
#define NUM_CARTAS 2

//(Cauê): define 20 caracteres como o limite do nome do jogador e duas cartas (por mão)


/* 
(Cauê): criei esse struct simplesmente para simular um tipo abstrato
de carta, que será usado para definir o tipo do vetor "mao" (lê-se "mão"),
que faz parte da estrutura do jogador. (Cauê)
*/

typedef enum {fora, foldou, ativo} estado;

/*
(Cauê): isso é um enum, basicamente ele atribui um valor inteiro a cada elemento,
no caso, out = 0, fold = 1, active = 2, isso vai ser usado para definir
a situação atual do jogador no momento da partida
*/

typedef struct{
    char nome[MAXIMO_NOME];
    int fichas;
    tp_listad *mao; //apenas armazena as cartas
    estado atual;
    int aposta;
    int posicaoMesa;
    
}jogador;

//(Cauê): cria o struct do tipo abstrato jogador, com os atributos

void criarJogador(jogador *j, const char *nome, int fichas, int posicaoMesa) {
    strncpy(j->nome, nome, MAXIMO_NOME); //(Cauê): copia o parâmetro de nome para o jogador
    j->fichas = fichas;
    j->atual= ativo;  //(Cauê): jogador começa ativo
    j->aposta = 0; 
    j->posicaoMesa = posicaoMesa;
    //jogadores irao receber uma mao vazia, para que futuramente seja implementada com a mesa
    j->mao = inicializa_listad();
    
    //(cesar) exibe informacoes do jogador, junto com suas primeiras cartas
    printf("\nJogador criado com sucesso!\n");
    printf("\nNome: %s\n", j->nome);
    printf("Fichas: %d\n", j->fichas);
    printf("Posicao na Mesa: %d\n", j->posicaoMesa);
    printf("A mao do jogador esta vazia!\n\n");
}

#endif
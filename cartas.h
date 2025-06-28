#ifndef CARTAS_H
#define CARTAS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "pilhaLE.h"

#define NUMERO_DE_CARTAS 13
#define NUMERO_DE_NAIPES 4
#define TOTAL_CARTAS 52

// MODIFICAÇÃO: A struct Baralho foi movida para cá.
// Representa um baralho "físico" antes de ser empilhado para o jogo.
typedef struct{
    Carta cartas[TOTAL_CARTAS];
} Baralho;

// Protótipos
static void inicializaBaralho(Baralho *b);
static void embaralha(Baralho *b, tp_pilha *p);
void imprimirBaralho(Baralho b);

// Função melhorada para embaralhar usando Fisher-Yates
static void inicializaBaralho(Baralho *b) {
    const char *valores[NUMERO_DE_CARTAS] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    const char *naipes[NUMERO_DE_NAIPES] = {"Copas", "Ouros", "Espadas", "Paus"};
    int index = 0;
    for (int i = 0; i < NUMERO_DE_NAIPES; i++) {
        for (int j = 0; j < NUMERO_DE_CARTAS; j++) {
            strncpy(b->cartas[index].valor, valores[j], sizeof(b->cartas[index].valor)-1);
            b->cartas[index].valor[sizeof(b->cartas[index].valor)-1] = '\0';
            strncpy(b->cartas[index].naipe, naipes[i], sizeof(b->cartas[index].naipe)-1);
            b->cartas[index].naipe[sizeof(b->cartas[index].naipe)-1] = '\0';
            index++;
        }
    }
}

static void embaralha(Baralho *b, tp_pilha *p) {
    // MODIFICAÇÃO: A semente do gerador aleatório (srand) deve ser chamada apenas uma vez
    // no início do programa (na main), para garantir um embaralhamento diferente a cada execução.
    // Se for chamada aqui, o baralho será sempre o mesmo se o programa rodar no mesmo segundo.
    
    for (int i = TOTAL_CARTAS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Carta temp = b->cartas[i];
        b->cartas[i] = b->cartas[j];
        b->cartas[j] = temp;
    }
    
    // Limpa a pilha antes de empilhar novamente
    Carta lixo;
    while (!pilhaVazia(p)) {
        pop(p, &lixo);
    }
    
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        push(p, b->cartas[i]);
    }
}

void imprimirBaralho(Baralho b) {
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        printf("%s de %s\n", b.cartas[i].valor, b.cartas[i].naipe);
    }
}

#endif

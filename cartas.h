#ifndef CARTAS_H
#define CARTAS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "pilhaLE.h"

// Constantes
#define NUMERO_DE_CARTAS 13  // Número de valores de cartas (2-A)
#define NUMERO_DE_NAIPES 4   // Número de naipes
#define TOTAL_CARTAS 52      // Total de cartas no baralho

/**
 * Embaralha as cartas usando o algoritmo Fisher-Yates
 * @param b Baralho a ser embaralhado
 * @param p Pilha onde as cartas serão colocadas
 */
void embaralha(Baralho *b, tp_pilha *p) {
    srand(time(NULL)); // Inicializa semente aleatória

    // Algoritmo Fisher-Yates para embaralhar
    for (int i = TOTAL_CARTAS - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Escolhe índice aleatório
        // Troca as cartas
        Carta temp = b->cartas[i];
        b->cartas[i] = b->cartas[j];
        b->cartas[j] = temp;
    }

    // Empilha as cartas embaralhadas
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        push(p, b->cartas[i]);
    }
}

/**
 * Inicializa um baralho com todas as cartas
 * @param b Baralho a ser inicializado
 */
void inicializaBaralho(Baralho *b) {
    const char *valores[] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    const char *naipes[] = {"Copas","Ouros","Espadas","Paus"};

    int index = 0;
    // Cria todas as combinações de valores e naipes
    for (int i = 0; i < NUMERO_DE_NAIPES; i++) {
        for (int j = 0; j < NUMERO_DE_CARTAS; j++) {
            b->cartas[index].valor = (char*)valores[j];
            b->cartas[index].naipe = (char*)naipes[i];
            index++;
        }
    }
}

/**
 * Imprime todas as cartas do baralho (para debug)
 * @param b Baralho a ser impresso
 */
void imprimirBaralho(Baralho b) {
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        printf("%s de %s\n", b.cartas[i].valor, b.cartas[i].naipe);
    }
}

#endif

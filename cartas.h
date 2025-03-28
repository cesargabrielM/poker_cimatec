#ifndef CARTAS_H
#define CARTAS_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pilha.h"
#define NUMERO_DE_CARTAS 13
#define NUMERO_DE_NAIPES 4

void embaralha(Baralho *b, tp_pilha *p){
    srand(time(NULL));
    int ran =0;

    for (int i = 0; i < TOTAL_CARTAS; i++){
        ran = rand() % 52;
        Carta temp = b -> cartas[i];
        b -> cartas[i] = b->cartas[ran];
        b->cartas[ran] = temp;
        push(p, b->cartas[i]);
    }

}

void inicializaBaralho(Baralho *b){
    char *valores[NUMERO_DE_CARTAS] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

    char *naipes[NUMERO_DE_NAIPES] = {"Copas", "Ouros", "Espadas", "Paus"};

    b[TOTAL_CARTAS];

    int index = 0;
    
    for (int i = 0; i < NUMERO_DE_NAIPES; i++) {
        for (int j = 0; j < NUMERO_DE_CARTAS; j++) {
            b -> cartas[index].valor = valores[j];
            b -> cartas[index].naipe = naipes[i];
            index++;
        }
    }


}

void imprimirBaralho(Baralho b){
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        printf("%s de %s\n", b.cartas[i].valor, b.cartas[i].naipe);
  }
}

#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pilha.h"

#define TAMANHO_MAX 52

void embaralhar_pilha(tp_pilha *p) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia. Nada para embaralhar.\n");
        return;
    }

    // Passo 1: Desempilhar todos os elementos para um array temporário
    int temp[TAMANHO_MAX];
    int tamanho = 0;
    while (!pilhaVazia(p)) {
        temp[tamanho++] = pop(p);
    }

    // Passo 2: Embaralhar o array (Fisher-Yates Shuffle)
    srand(time(NULL)); // Inicializa a semente aleatória
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Escolhe um índice aleatório entre 0 e i
        // Troca os elementos
        int aux = temp[i];
        temp[i] = temp[j];
        temp[j] = aux;
    }

    // Passo 3: Reempilhar os elementos embaralhados
    for (int i = 0; i < tamanho; i++) {
        push(p, temp[i]);
    }
}

int main(){
    tp_pilha pilha1;
    
    inicializaPilha(&pilha1);
    
    embaralhar_pilha(&pilha1);
    
    return 0;
}
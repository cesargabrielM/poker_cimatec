#ifndef PILHALE_H
#define PILHALE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_CARTAS 52

// Estrutura para representar uma carta
typedef struct {
    char valor[10];
    char naipe[10];
} Carta;

// MODIFICAÇÃO: Movida a struct Baralho para cartas.h, onde faz mais sentido.

typedef Carta tp_item;

typedef struct tp_no_aux {
    tp_item info;
    struct tp_no_aux *prox;
} tp_no;

typedef struct {
    tp_no *topo;
} tp_pilha;

// Funções da pilha (Protótipos)
tp_pilha *inicializaPilha ();
tp_pilha *destroiPilha(tp_pilha *pilha);
int pilhaVazia(tp_pilha *pilha);
int push(tp_pilha *pilha, tp_item e);
int pop(tp_pilha *pilha, tp_item *e);
int top(tp_pilha *pilha, tp_item *e);
void imprimePilha(tp_pilha *pilha);

//Funções da biblioteca
tp_pilha *inicializaPilha (){
    tp_pilha *pilha = (tp_pilha*) malloc(sizeof(tp_pilha));
    
    if (pilha != NULL) {
        pilha->topo = NULL;
    }
    
    return pilha;
}

// MODIFICAÇÃO: Pequena melhoria na lógica para destruir a pilha.
tp_pilha *destroiPilha(tp_pilha *pilha){
    tp_item e;
    while (!pilhaVazia(pilha)){
        pop(pilha, &e);
    }

    free(pilha);
    
    return NULL;
}

static tp_no *aloca(){ // MODIFICAÇÃO: transformada em "static" pois só é usada neste ficheiro.
    tp_no* pt = (tp_no*) malloc(sizeof(tp_no));
    return pt;
}

int pilhaVazia(tp_pilha *pilha){
    if (!pilha) return 1;
    return (pilha->topo == NULL);
}

int push(tp_pilha *pilha, tp_item e){
    tp_no *novo = aloca();
    if (!novo) return 0;
    
    strncpy(novo->info.valor, e.valor, sizeof(novo->info.valor) - 1);
    novo->info.valor[sizeof(novo->info.valor)-1] = '\0';
    strncpy(novo->info.naipe, e.naipe, sizeof(novo->info.naipe) - 1);
    novo->info.naipe[sizeof(novo->info.naipe)-1] = '\0';
    
    novo->prox = pilha->topo;
    pilha->topo = novo;
    return 1;
}

int pop(tp_pilha *pilha, tp_item *e){
    if(pilhaVazia(pilha)) return 0;
    tp_no *aux = pilha->topo;
    *e = aux->info;
    pilha->topo = aux->prox;
    free(aux);
    return 1;
}

int top(tp_pilha *pilha, tp_item *e){
    if(pilhaVazia(pilha)) return 0;
    *e = pilha->topo->info;
    return 1;
}

void imprimePilha(tp_pilha *pilha){
    if (pilhaVazia(pilha)) return;
    tp_no *atu = pilha->topo;
    printf("--- Conteúdo da Pilha ---\n");
    while (atu != NULL) {
        printf("%s de %s\n", atu->info.valor, atu->info.naipe);
        atu = atu->prox;
    }
    printf("-------------------------\n");
}

#endif

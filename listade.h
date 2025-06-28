#ifndef LISTADE_H
#define LISTADE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pilhaLE.h"

typedef Carta tp_item_lista;

typedef struct tp_no_lista {
    struct tp_no_lista *ant;
    tp_item_lista info;
    struct tp_no_lista *prox;
} tp_no_lista;

typedef struct {
    tp_no_lista *ini;
    tp_no_lista *fim;
} tp_listad;

//Funções da biblioteca
static int cartas_iguais(tp_item_lista a, tp_item_lista b) { // MODIFICAÇÃO: "static"
    return strcmp(a.valor, b.valor) == 0 && strcmp(a.naipe, b.naipe) == 0;
}

static tp_listad *inicializa_listad() {
    tp_listad *lista = (tp_listad*) malloc(sizeof(tp_listad));
    if (lista != NULL) {
        lista->ini = NULL;
        lista->fim = NULL;
    }
    return lista;
}

static int listad_vazia(tp_listad *lista) {
    if (!lista) return 1;
    return (lista->ini == NULL);
}

static tp_no_lista *alocaLista() { // MODIFICAÇÃO: "static"
    return (tp_no_lista*) malloc(sizeof(tp_no_lista));
}

static int insere_listad_no_fim(tp_listad *lista, tp_item_lista e) {
    tp_no_lista *novo = alocaLista();
    if (!novo) return 0;
    
    strncpy(novo->info.valor, e.valor, sizeof(novo->info.valor)-1);
    novo->info.valor[sizeof(novo->info.valor)-1] = '\0';
    strncpy(novo->info.naipe, e.naipe, sizeof(novo->info.naipe)-1);
    novo->info.naipe[sizeof(novo->info.naipe)-1] = '\0';
    novo->prox = NULL;
    
    if(listad_vazia(lista)){
        novo->ant = NULL;
        lista->ini = lista->fim = novo;
    } else {
        novo->ant = lista->fim;
        lista->fim->prox = novo;
        lista->fim = novo;
    }
    return 1;
}

static tp_no_lista *busca_listade (tp_listad *lista, tp_item_lista e) {
    if (listad_vazia(lista)) return NULL;
    tp_no_lista *atu = lista->ini;
    while((atu != NULL) && !cartas_iguais(atu->info, e)){
        atu = atu->prox;
    }
    return atu;
}

int remove_listad(tp_listad *lista, tp_item_lista e) {
    tp_no_lista *atu = busca_listade(lista, e);
    if(atu == NULL) return 0;
    
    if(lista->ini == lista->fim){ // Único elemento
        lista->ini = lista->fim = NULL;
    } else if(lista->ini == atu){ // Primeiro elemento
        lista->ini = atu->prox;
        lista->ini->ant = NULL;
    } else if(lista->fim == atu){ // Último elemento
        lista->fim = atu->ant;
        lista->fim->prox = NULL;
    } else { // Elemento no meio
        atu->prox->ant = atu->ant;
        atu->ant->prox = atu->prox;
    }
    free(atu);
    return 1;
}

static tp_listad *destroi_listad(tp_listad *lista) {
    if (lista == NULL) return NULL;
    tp_no_lista *atu = lista->ini;
    while (atu != NULL) {
        tp_no_lista *prox = atu->prox;
        free(atu);
        atu = prox;
    }
    free(lista);
    return NULL;
}

#endif

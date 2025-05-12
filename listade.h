#ifndef LISTADE_H
#define LISTADE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Adicionado para strcmp
#include "pilhaLE.h"

/*
 * Tipo item da lista (renomeado para evitar conflito)
 * Mantem a mesma estrutura da carta, mas com nome único
 */
typedef Carta tp_item_lista;

/*
 * Estrutura do nó da lista (renomeada para tp_no_lista)
 * Originalmente tp_no_aux, alterado para evitar conflito com pilhaLE.h
 */
typedef struct tp_no_lista {
    struct tp_no_lista *ant;  // Ponteiro para o nó anterior
    tp_item_lista info;       // Informação do nó (agora com tipo único)
    struct tp_no_lista *prox; // Ponteiro para o próximo nó
} tp_no_lista;

/*
 * Descritor da lista (mantido original)
 * Contém ponteiros para início e fim da lista
 */
typedef struct {
    tp_no_lista *ini;  // Ponteiro para o primeiro nó
    tp_no_lista *fim;  // Ponteiro para o último nó
    // int tamanho (comentado originalmente)
} tp_listad;

/*
 * Função de comparação de cartas (adaptada para tp_item_lista)
 * Mantém a mesma lógica original mas com tipo renomeado
 */
int cartas_iguais(tp_item_lista a, tp_item_lista b) {
    // Verifica se ambos os valores são NULL ou iguais
    int valor_igual = (a.valor == b.valor) || 
                     (a.valor && b.valor && strcmp(a.valor, b.valor) == 0);
    
    // Verifica se ambos os naipes são NULL ou iguais
    int naipe_igual = (a.naipe == b.naipe) ||
                     (a.naipe && b.naipe && strcmp(a.naipe, b.naipe) == 0);
    
    return valor_igual && naipe_igual;
}

/*
 * Inicializa a lista (comentário original mantido)
 * Aloca memória para o descritor
 */
tp_listad *inicializa_listad() {
    tp_listad *lista = (tp_listad*) malloc(sizeof(tp_listad));
    lista->ini = NULL;
    lista->fim = NULL;
    // lista->tamanho = 0; (comentado originalmente)
    return lista;
}

/*
 * Verifica se lista está vazia (comentário original mantido)
 */
int listad_vazia(tp_listad *lista) {
    if ( (lista->ini == NULL) ) return 1;
    return 0;
}

/*
 * Aloca um nó da lista (comentário original mantido)
 * Agora usando tp_no_lista
 */
tp_no_lista *alocaLista() {
    tp_no_lista* pt;
    pt = (tp_no_lista*) malloc(sizeof(tp_no_lista));
    return pt;
}

/*
 * Insere novo nó na lista (comentário original mantido)
 * Mantém a mesma lógica com tipos renomeados
 */
int insere_listad_no_fim(tp_listad *lista, tp_item_lista e) {
    tp_no_lista *novo;
    
    novo = alocaLista();
    if (!novo) return 0;
    novo->info = e;
    if( listad_vazia(lista) ){
        novo->prox = NULL;
        novo->ant = NULL;
        lista->ini = lista->fim = novo;
    }
    else{
        novo->prox = NULL;
        novo->ant = lista->fim;
        lista->fim->prox = novo;
        lista->fim = novo;
    }
    return 1;
}

/* 
 * Imprime a lista (comentário original mantido)
 * Agora mostra tp_item_lista (que é Carta)
 */
void imprime_listad(tp_listad *lista, int ordem) {
    if(lista == NULL) printf("Lista não inicializada");
    else{
        tp_no_lista *atu;
        switch(ordem){
            case 1: atu = lista->ini;
                    while (atu != NULL){
                        printf("%s de %s\n", atu->info.valor, atu->info.naipe);
                        atu = atu->prox;
                    }
                    break;
            case 2: atu = lista->fim;
                    while (atu != NULL){
                        printf("%s de %s\n", atu->info.valor, atu->info.naipe);
                        atu = atu->ant;
                    }
                    break;
            default: printf("codigo invalido");
        }
    }
    printf("\n");
}

/*
 * Busca na lista (comentário original mantido)
 * Adaptado para tp_no_lista
 */
tp_no_lista * busca_listade (tp_listad *lista, tp_item_lista e) {
    tp_no_lista *atu;
    atu = lista->ini;
    while((atu != NULL) && !cartas_iguais(atu->info, e)){
        atu = atu->prox;
    }
    return atu;
}

/*
 * Remove da lista (comentário original mantido)
 * Adaptado para tp_no_lista
 */
int remove_listad(tp_listad *lista, tp_item_lista e) {
    tp_no_lista *atu = busca_listade(lista, e);
    if(atu == NULL) return 0;
    
    if(lista->ini == lista->fim){
        lista->ini = lista->fim = NULL;
    }
    else{
        if(lista->ini == atu){
            lista->ini = atu->prox;
            atu->prox->ant = NULL;
        }
        else if(lista->fim == atu){
            lista->fim = atu->ant;
            atu->ant->prox = NULL;
        }
        else{
            atu->prox->ant = atu->ant;
            atu->ant->prox = atu->prox;
        }
    }
    free(atu);
    //lista->amanho--; (comentado originalmente)
    return 1;
}

/*
 * Destroi a lista (comentário original mantido)
 * Adaptado para tp_no_lista
 */
tp_listad * destroi_listad (tp_listad *lista) {
    tp_no_lista *atu = lista->ini;
    while (atu != NULL) {
        lista->ini = atu->prox;
        free(atu);
        atu = lista->ini;
    }
    free(lista);
    return NULL;
}

#endif
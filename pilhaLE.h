#ifndef PILHALE_H
#define PILHALE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define TOTAL_CARTAS 52

// Estrutura para representar uma carta
typedef struct {
    char *valor;
    char *naipe;
} Carta;

typedef struct{
    Carta cartas[TOTAL_CARTAS];
} Baralho;

typedef Carta tp_item;

typedef struct tp_no_aux {
	tp_item info;
	struct tp_no_aux *prox;
} tp_no;

typedef struct {
	tp_no *topo;
} tp_pilha;

//declaraC'C#o das funC'C5es
tp_pilha *inicializaPilha();
int pilhaVazia(tp_pilha *pilha);
int pop(tp_pilha *pilha, tp_item *e);


//funcoes da biblioteca
tp_pilha *inicializaPilha (){
	tp_pilha *pilha = (tp_pilha*) malloc(sizeof(tp_pilha));
	
	pilha->topo = NULL;
	
	return pilha;
}
tp_pilha *destroiPilha(tp_pilha *pilha){
	tp_no *atu = pilha->topo, *aux;
	tp_item e;
	
	while (atu != NULL){
		aux = atu->prox;
		pop(pilha, &e);
		atu = aux;
	}
	
	pilha->topo = NULL;
	
	free(pilha);
	
	return NULL;
}

tp_no *aloca(){
	tp_no* pt;

	pt = (tp_no*) malloc(sizeof(tp_no));

	return pt;
}

int pilhaVazia(tp_pilha *pilha){
	if (pilha->topo == NULL) return 1;
	return 0;
}
int push(tp_pilha *pilha, tp_item e){
	tp_no *novo;

	novo = aloca();

	if (!novo) return 0;

	novo->info = e;

	if (pilhaVazia(pilha)){ //Se for o primeiro elemento da lista
		novo->prox = NULL;
	}
	else {
		novo->prox = pilha->topo;
	}

	pilha->topo = novo;

	return 1;
}
int pop(tp_pilha *pilha, tp_item *e){
	tp_no *aux;

	if(pilhaVazia(pilha)) return 0;

	*e = pilha->topo->info;

	aux = pilha->topo;

	pilha->topo = pilha->topo->prox;

	free(aux);

	aux = NULL;

	return 1;
}
int top(tp_pilha *pilha, tp_item *e){
	if(pilhaVazia(pilha)) return 0;
	
	*e = pilha->topo->info;
	
	return 1;
}

/*
//No momento não esta sendo usada e só gera avisos desnecessarios
void imprimePilha(tp_pilha *pilha){
	tp_pilha *pilha_aux;
	tp_item e;
	
	//cria e inicializa uma pilha auxiliar
	pilha_aux = inicializaPilha();

	while (!pilhaVazia(pilha)){
		pop(pilha,&e);
		
		printf("%d ", e);
		
		push(pilha_aux, e);
	}

	while (!pilhaVazia(pilha_aux)){
		pop(pilha_aux,&e);
		push(pilha, e);
	}
	
	pilha_aux = destroiPilha(pilha_aux);

}*/

#endif

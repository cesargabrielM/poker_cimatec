#ifndef PILHA_H
#define PILHA_H
#include <stdio.h>
#define MAX 100

typedef int tp_item;

typedef struct{
    int topo;
    tp_item item[MAX];
} tp_pilha;

//coloca o topo para -1 para definir a pilha como vazia
void inicializaPilha(tp_pilha *p){
    p->topo = -1;
}
//verifica se a pilha esta vazia
int pilhaVazia(tp_pilha *p){
    if(p->topo == -1) return 1;// pilha esta vazia ? true, a pilha esta com nenhum elemento
    return 0;// pilha esta vazia ? false, ja tem pelo menos um elemento na pilha
}
//verifica se a pilha esta cheia
int pilhaCheia(tp_pilha *p){
    if(p->topo == MAX-1) return 1; // caso o topo esteja alocado em 99 (por eh de 0 ate 99), a pilha esta cheia
    return 0; //pilha ainda tem espaco
}
// o ponteiro p ira alocar o topo para cima e o elemento e ira colocar o conteudo no local onde eh o atual topo
int push(tp_pilha *p, tp_item e){
    if(pilhaCheia(p)==1) return 0; // pilha esta cehia, nao pode colocar mais elementos
    p->topo++;
    p->item[p->topo]= e;
    return 1;
}
// em vez de retirar o elemento, eu apenas movo o topo para baixo, para que o antigo topo seja "esquecido" e o push ira acabar modificando o antigo topo
int pop(tp_pilha *p, tp_item *e){
    if(pilhaVazia(p)==1) return 0;
    *e = p->item[p->topo]; // guarda o valor do antigo topo
    p->topo--;//movo o topo para baixo
    return 1;
}
//verifica qual eh o valor que esta no topo da pilha
int top(tp_pilha *p, tp_item *e){
    if(pilhaVazia(p) == 1) return 0; // verifica se a pilha esta vazia
    *e = p->item[p->topo]; // armazena o valor do topo
    return 1;
}
//verifica qual o tamanho da pilha
int tamanhoPilha(tp_pilha *p){
    return p -> topo + 1; //retorna o tamanho da mesma, adiciona 1 pois os valores do topo sao entre 0 e 99, adicionando 1 sera 1 a 100
}
//ira criar um tipo de copia da pilha e ira desempilhar elemento por elemento que ira mostrar qual elemento eh esse
void imprimirPilha(tp_pilha p){
    tp_item e;

    printf("\n");
    //loop ate a pilha estiver vazia
    while(!pilhaVazia(&p)){
        pop(&p, &e); // funcao de desempilhar
        printf("%d ", e); // printa os elementos
    }
}

#endif
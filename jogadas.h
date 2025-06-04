#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_avl.h"

/* 
COMO USAR ESSA BIBLIOTECA?
Primeiro de tudo, ao começo da cada partida, usamos a função
inicializaArquivo(). 
Depois disso, usamos criaAVL() para criar a árvore AVL que 
armazenas as jogadas.
*/

/*
Essa função abre o arquivo onde serão registradas as jogadas.
SÓ PODE SER USADA NO INÍCIO DE CADA PARTIDA, SENÃO APAGA TUDO QUE
FOI SALVO. Logo após criar o arquivo, ela fecha automaticamente.
O arquivo só é aberto novamente na função salvaEmDisco() no modo
apêndice.
A cada jogada, usamos insereJogada(), com os respectivos parâmetros
para salvar na árvore.
Ao usar insereJogada(), ela automaticamente usa salvaEmDisco() para
registrar no arquivo de texto.
*/
void inicializaArquivo(){
    FILE *f = fopen("jogadas.txt", "w");
    fclose(f);
}

/*
Essa função cria a árvore AVL onde serão armazenadas as jogadas, 
retorna o ponteiro dela.
*/
arvAvl * criaAVL(){
    arvAvl *p_raiz = criarAvl();
    return p_raiz;
}

/*
Essa função insere um nó na árvore AVL de jogadas, ela precisa receber o ponteiro da 
raiz da árvore, o nome do jogador que a fez (recebe o ponteiro para o primeiro 
caractere da string), a ordem da jogada e o título. Após inserir, ela automaticamente
chama a função salvaEmDisco()
*/
void insereJogada(arvAvl *p_raiz, char* nome, int ordem, tipos_jogada titulo){
    arvAvl ultima_jogada;
    ultima_jogada = inserir(p_raiz, ordem, nome, titulo);
    if(ultima_jogada != NULL) salvaEmDisco(ultima_jogada);
    //Somente insere se for possível adicionar a jogada na árvore
}

/*
A função que eu vou criar agora funciona da seguinte forma: assim que um jogador
fizer uma jogada, ela deve ser chamada. Ela precisa receber o ponteiro da última
jogada feita, e que o arquivo de registro tenha sido criado e fechado.
*/
void salvaEmDisco(arvAvl ultima_jogada){
    FILE *f = fopen("jogadas.txt", "a"); //Abre o arquivo no modo apêndice para registrar
    
    if(f == NULL){
        printf("Erro ao acessar o arquivo!");
        return;
    }; //Tenta acessar o arquivo

    tipos_jogada t = ultima_jogada->titulo; //Salva o título da última jogada
    
    char nomeJogada[MAX_NOME]; //Cria uma string para salvar o nome da jogada
    char nomeJogador[MAX_NOME]; //Cria uma string para salvar o nome do jogador
    strcpy(nomeJogador, ultima_jogada->nome_jogador);
    
    //Switch Case, para cada tipo de jogada, ele imprime a string correspondente
    switch (t) {
        case CARTA_ALTA:
            strcpy(nomeJogada, "Carta Alta");
            fprintf(f, "\n%s fez a seguinte jogada: %s", nomeJogador, nomeJogada);
            break;
        case PAR:
            strcpy(nomeJogada, "Par");
            fprintf(f, "\n%s fez a seguinte jogada: %s", nomeJogador, nomeJogada);
            break;
        case DOIS_PARES:
            strcpy(nomeJogada, "Dois Pares");
            fprintf(f, "\n%s fez a seguinte jogada: %s", nomeJogador, nomeJogada);
            break;
        case TRINCA:
            strcpy(nomeJogada, "Trinca");
            fprintf(f, "\n%s fez a seguinte jogada: %s", nomeJogador, nomeJogada);
            break;
        case STRAIGHT:
            strcpy(nomeJogada, "Straight");
            fprintf(f, "\n%s fez a seguinte jogada: %s", nomeJogador, nomeJogada);
            break;
        case FLUSH:
            strcpy(nomeJogada, "Flush");
            fprintf(f, "\n%s fez a seguinte jogada: %s", nomeJogador, nomeJogada);
            break;
        case FULL_HOUSE:
            strcpy(nomeJogada, "Full House");
            fprintf(f, "\n%s fez a seguinte jogada: %s", nomeJogador, nomeJogada);
            break;
        case QUADRA:
            strcpy(nomeJogada, "Quadra");
            fprintf(f, "\n%s fez a seguinte jogada: %s", nomeJogador, nomeJogada);
            break;
        case STRAIGHT_FLUSH:
            strcpy(nomeJogada, "Straight Flush");
            fprintf(f, "\n%s fez a seguinte jogada: %s", nomeJogador, nomeJogada);
            break;
        case ROYAL_FLUSH:
            strcpy(nomeJogada, "Royal Flush");
            fprintf(f, "\n%s fez a seguinte jogada: %s", nomeJogador, nomeJogada);
            break;
        default:
            strcpy(nomeJogada, "Desconhecida");
            fprintf(f, "\n%s fez a seguinte jogada: %s", nomeJogador, nomeJogada);
            break;
    }
    fclose(f);
}

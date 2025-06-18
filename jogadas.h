#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvAvl_dict.h"

/* 
COMO USAR ESSA BIBLIOTECA?
Primeiro de tudo, ao começo da cada partida, usamos a função
inicializaArquivo(). 
Depois disso, usamos criaAVL() para criar a árvore AVL que 
armazenas as jogadas.
Depois disso, a cada jogada, usamos insereJogada() passando o
ponteiro da raiz da árvore, o nome do jogador que fez a jogada,
essa função abre o arquivo onde serão registradas as jogadas.
SÓ PODE SER USADA NO INÍCIO DE CADA PARTIDA, SENÃO APAGA TUDO QUE
FOI SALVO. Logo após criar o arquivo, ela fecha automaticamente.
O arquivo só é aberto novamente na função salvaEmDisco() no modo
apêndice.
A cada jogada, usamos insereJogada(), com os respectivos parâmetros
para salvar na árvore.
Depois disso, quando a jogada for feita, usamos a função
salvaEmDisco() passando o ponteiro da raiz da árvore e a ordem da jogada.
*/

int inicializaArquivo(){
    FILE *f = fopen("jogadas.txt", "w");
    if(f == NULL) return 0; //Se não conseguiu abrir o arquivo, retorna 0
    fclose(f);
    return 1; //Se conseguiu abrir e fechar o arquivo, retorna 1
}

arvAvl * criaAVL(){
    arvAvl *p_raiz = criarAvl();
    if(p_raiz == NULL) return NULL;
    return p_raiz;
}

int insereJogada(arvAvl *p_raiz, char* nome, int ordem, tipos_jogada titulo){
    arvAvl ultima_jogada;
    ultima_jogada = inserir(p_raiz, ordem, nome, titulo);
    if(ultima_jogada == NULL) return 0; //Se não conseguiu inserir, retorna 0
    return 1; //Se conseguiu inserir, retorna 1
}

int salvaEmDisco(arvAvl* p_raiz, int ordem){
    FILE *f = fopen("jogadas.txt", "a"); //Abre o arquivo no modo apêndice para registrar
    
    if(f == NULL){
        printf("Erro ao acessar o arquivo!");
        fclose(f);
        return 0;
    } //Tenta acessar o arquivo

    arvAvl jogada_encontrada = consultarValorAvl(p_raiz, ordem); //Tenta achar a jogada pela ordem
    if(jogada_encontrada == NULL){
        fclose(f); //Se não conseguiu achar, fecha o arquivo
        return 0; //E retorna 0
    }

    tipos_jogada t = jogada_encontrada->titulo; //Salva o título da jogada encontrada
    
    char nomeJogada[MAX_NOME]; //Cria uma string para salvar o nome da jogada
    char nomeJogador[MAX_NOME]; //Cria uma string para salvar o nome do jogador
    strcpy(nomeJogador, jogada_encontrada->nome_jogador);
    
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

    return 1; //Se conseguiu salvar, retorna 1
}

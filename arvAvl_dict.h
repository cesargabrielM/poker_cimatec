#ifndef ARVAVL_DICT_H
#define ARVAVL_DICT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_NOME 20

typedef enum {
  CARTA_ALTA,
  PAR,
  DOIS_PARES,
  TRINCA,
  STRAIGHT,
  FLUSH,
  FULL_HOUSE,
  QUADRA,
  STRAIGHT_FLUSH,
  ROYAL_FLUSH
} tipos_jogada;

typedef struct No* arvAvl;

struct No {
    struct No *esq;
    int info;
    int alt;
    tipos_jogada titulo;
    char nome_jogador[MAX_NOME];
    struct No *dir;
};

/* Protótipos das funções */
static arvAvl* criarAvl();
void preOrd(arvAvl* raiz);
void emOrd(arvAvl* raiz);
// static void posOrd(arvAvl* raiz); // Removido para evitar warning de função não usada
static struct No* inserir(arvAvl* raiz, int valor, char *nome, tipos_jogada titulo);
int remover(arvAvl* raiz, int valor);
static struct No* buscarMenor(struct No* atual);
static struct No* consultarValorAvl(arvAvl* raiz, int valor);
static int maior(int x, int y);
static int altNo(struct No* no);
static int fatorBalanceamentoNo(struct No* no);
static void rotacaoLl(arvAvl* raiz);
static void rotacaoRr(arvAvl* raiz);
static void rotacaoRl(arvAvl* raiz);
static void rotacaoLr(arvAvl* raiz);

int contPrint = 0; // Variável global para controle de impressão

/* Cria uma árvore AVL vazia e retorna um ponteiro para ela */
static arvAvl* criarAvl() {
    arvAvl* raiz = (arvAvl*) malloc(sizeof(arvAvl)); //
    if(raiz != NULL) { //
        *raiz = NULL; //
    }
    return raiz; //
}

/* Verifica se a árvore AVL está vazia.
Retorna 1 se vazia ou ponteiro nulo, 0 caso contrário */
static inline int estahVaziaAvl(arvAvl* raiz) {
    if(raiz == NULL) return 1; //
    if(*raiz == NULL) return 1; //
    return 0; //
}

/* Imprime a árvore em pré-ordem (Raiz, Esquerda, Direita) */
void preOrd(arvAvl* raiz) {
    if(raiz == NULL) return; //
    if(*raiz != NULL) { //
        if(contPrint != 0) { //
            printf(" "); //
        }
        printf("%d", (*raiz)->info); //
        contPrint++; //
        preOrd(&((*raiz)->esq)); //
        preOrd(&((*raiz)->dir)); //
    }
}

/* Imprime a árvore em ordem simétrica (Esquerda, Raiz, Direita) */
void emOrd(arvAvl* raiz) {
    if(raiz == NULL) return; //
    if(*raiz != NULL) { //
        emOrd(&((*raiz)->esq)); //
        if(contPrint != 0) { //
            printf(" "); //
        }
        printf("%d", (*raiz)->info); //
        contPrint++; //
        emOrd(&((*raiz)->dir)); //
    }
}

/* Imprime a árvore em pós-ordem (Esquerda, Direita, Raiz) */
// static void posOrd(arvAvl* raiz) {
//     if(raiz == NULL) return; //
//     if(*raiz != NULL) { //
//         posOrd(&((*raiz)->esq)); //
//         posOrd(&((*raiz)->dir)); //
//         if(contPrint != 0) { //
//             printf(" "); //
//         }
//         printf("%d", (*raiz)->info); //
//         contPrint++; //
//     }
// }

/* Retorna a altura da árvore AVL */
static inline int alturaAvl(arvAvl* raiz) {
    if(raiz == NULL) return 0; //
    if(*raiz == NULL) return 0; //

    int altEsq = alturaAvl(&((*raiz)->esq)); //
    int altDir = alturaAvl(&((*raiz)->dir)); //

    if(altEsq > altDir) { //
        return altEsq + 1; //
    } else {
        return altDir + 1; //
    }
}

/* Retorna o total de nós da árvore */
/* static int totalNosAvl(arvAvl* raiz) {
    if(raiz == NULL) return 0; //
    if(*raiz == NULL) return 0; //

    int totEsq = totalNosAvl(&((*raiz)->esq)); //
    int totDir = totalNosAvl(&((*raiz)->dir)); //

    return (totEsq + totDir + 1); //
} */

/* Insere um valor na árvore AVL mantendo o balanceamento */
// Corrigido para retornar struct No* para evitar warnings de tipo incompatível com arvAvl
static struct No* inserir(arvAvl* raiz, int valor, char *nome, tipos_jogada titulo) {
    if(*raiz == NULL) { //
        struct No* novo = (struct No*) malloc(sizeof(struct No)); //
        if(novo == NULL) return NULL; //
        novo->info = valor; //
        novo->alt = 0; //
        novo->titulo = titulo; //
        strcpy(novo->nome_jogador, nome); //
        novo->dir = NULL; //
        novo->esq = NULL; //
        *raiz = novo; //
        return novo; //
    } else {
        struct No* atual = *raiz; //
        // Passar o endereço do ponteiro para a função recursiva
        if(valor < atual->info) { //
            if(inserir(&(atual->esq), valor, nome, titulo) != NULL) { //
                if(fatorBalanceamentoNo(atual) > 1 || fatorBalanceamentoNo(atual) < -1) { //
                    if(valor < (*raiz)->esq->info) { //
                        rotacaoLl(raiz); //
                    } else {
                        rotacaoLr(raiz); //
                    }
                }
            }
        } else if(valor > atual->info) { //
            if(inserir(&(atual->dir), valor, nome, titulo) != NULL) { //
                if(fatorBalanceamentoNo(atual) > 1 || fatorBalanceamentoNo(atual) < -1) { //
                    if(valor > (*raiz)->dir->info) { //
                        rotacaoRr(raiz); //
                    } else {
                        rotacaoRl(raiz); //
                    }
                }
            }
        } else {
            return NULL; /* Valor duplicado, não insere */ //
        }
        atual->alt = maior(altNo(atual->esq), altNo(atual->dir)) + 1; //
        return atual; //
    }
}

/* Libera a memória de todos os nós recursivamente */
static void liberaNo(struct No* no) {
    if(no == NULL) return; //
    liberaNo(no->esq); //
    liberaNo(no->dir); //
    free(no); //
    // no = NULL; // Não é necessário, 'no' é uma cópia do ponteiro.
}

/* Libera toda a árvore AVL */
static void liberaAvl(arvAvl* raiz) {
    if(raiz == NULL) return; //
    liberaNo(*raiz); // Passa o No* para a função
    free(raiz); //
    // *raiz = NULL; // Opcional: define a raiz para NULL após liberar.
}

/* Consulta se um valor existe na árvore AVL */
// Corrigido para retornar struct No* para evitar warnings de tipo incompatível com arvAvl
static struct No* consultarValorAvl(arvAvl* raiz, int valor) {
    if(raiz == NULL) return NULL; //
    if(*raiz == NULL) return NULL; //

    struct No* atual = *raiz; //
    while(atual != NULL) { //
        if(atual->info == valor) { //
            return atual; //
        } else if(atual->info > valor) { //
            atual = atual->esq; //
        } else {
            atual = atual->dir; //
        }
    }
    return NULL; /* Valor não encontrado */ //
}

/* Retorna a altura de um nó ou -1 se nulo */
static int altNo(struct No* no) {
    if(no == NULL) return -1; //
    else return no->alt; //
}

/* Retorna o fator de balanceamento do nó (altura esq - altura dir) */
static int fatorBalanceamentoNo(struct No* no) {
    if(no == NULL) return 0; //
    return altNo(no->esq) - altNo(no->dir); //
}

/* Retorna o maior valor entre dois inteiros */
static int maior(int x, int y) {
    return (x > y) ? x : y; //
}

/* Rotação simples à esquerda (LL) */
static void rotacaoLl(arvAvl* raiz) {
    struct No* no = (*raiz)->esq; //
    (*raiz)->esq = no->dir; //
    no->dir = *raiz; //

    (*raiz)->alt = maior(altNo((*raiz)->esq), altNo((*raiz)->dir)) + 1; //
    no->alt = maior(altNo(no->esq), (*raiz)->alt) + 1; //

    *raiz = no; //
}

/* Rotação simples à direita (RR) */
static void rotacaoRr(arvAvl* raiz) {
    struct No* no = (*raiz)->dir; //
    (*raiz)->dir = no->esq; //
    no->esq = *raiz; //

    (*raiz)->alt = maior(altNo((*raiz)->esq), altNo((*raiz)->dir)) + 1; //
    no->alt = maior(altNo(no->dir), (*raiz)->alt) + 1; //

    *raiz = no; //
}

/* Rotação dupla à esquerda-direita (LR) */
static void rotacaoLr(arvAvl* raiz) {
    rotacaoRr(&(*raiz)->esq); // Passa o endereço do ponteiro do filho esquerdo
    rotacaoLl(raiz); //
}

/* Rotação dupla à direita-esquerda (RL) */
static void rotacaoRl(arvAvl* raiz) {
    rotacaoLl(&(*raiz)->dir); // Passa o endereço do ponteiro do filho direito
    rotacaoRr(raiz); //
}

/* Remove um valor da árvore AVL mantendo o balanceamento */
int remover(arvAvl* raiz, int valor) {
    int res; //
    if(*raiz == NULL) { /* Valor não encontrado */ //
        printf("Valor %d nao encontrado na árvore!\n", valor); //
        return 0; //
    }

    // Usar um ponteiro temporário para o nó atual para facilitar o acesso
    struct No* atual_no = *raiz; //

    if(valor < atual_no->info) { //
        if((res = remover(&(atual_no->esq), valor)) == 1) { //
            if(fatorBalanceamentoNo(atual_no) > 1 || fatorBalanceamentoNo(atual_no) < -1) { //
                if(altNo(atual_no->dir->esq) <= altNo(atual_no->dir->dir)) { //
                    rotacaoRr(raiz); //
                } else {
                    rotacaoRl(raiz); //
                }
            }
        }
    } else if(valor > atual_no->info) { //
        if((res = remover(&(atual_no->dir), valor)) == 1) { //
            if(fatorBalanceamentoNo(atual_no) > 1 || fatorBalanceamentoNo(atual_no) < -1) { //
                if(altNo(atual_no->esq->dir) <= altNo(atual_no->esq->esq)) { //
                    rotacaoLl(raiz); //
                } else {
                    rotacaoLr(raiz); //
                }
            }
        }
    } else { /* Nó encontrado */ //
        if(((atual_no->esq == NULL) || (atual_no->dir == NULL))) { //
            /* Nó com um filho ou nenhum */ //
            struct No* noVelho = atual_no; //
            if(atual_no->esq != NULL) *raiz = atual_no->esq; //
            else *raiz = atual_no->dir; //
            free(noVelho); //
            return 1; //
        } else {
            /* Nó com dois filhos: substituir pelo menor nó da subárvore direita */ //
            struct No* temp = buscarMenor(atual_no->dir); //
            atual_no->info = temp->info; //
            // Recursivamente remove o nó que foi copiado
            remover(&(atual_no->dir), atual_no->info); //
            
            // Rebalancear após a remoção recursiva
            if(fatorBalanceamentoNo(atual_no) > 1 || fatorBalanceamentoNo(atual_no) < -1) { //
                if(altNo(atual_no->esq->dir) <= altNo(atual_no->esq->esq)) { //
                    rotacaoLl(raiz); //
                } else {
                    rotacaoLr(raiz); //
                }
            }
        }
    }
    // Atualiza a altura do nó atual após qualquer alteração
    if (*raiz != NULL) { // Verifica se a raiz não ficou NULL após remoção
        (*raiz)->alt = maior(altNo((*raiz)->esq), altNo((*raiz)->dir)) + 1; //
    }
    return 1; //
}

/* Busca o nó com menor valor da árvore (mais à esquerda) */
static struct No* buscarMenor(struct No* atual) {
    while(atual->esq != NULL) { //
        atual = atual->esq; //
    }
    return atual; //
}

#endif // ARVAVL_DICT_H

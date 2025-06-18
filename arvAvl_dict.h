#include <stdio.h>   /* Biblioteca padrão para entrada e saída (printf, scanf, etc.) */
#include <stdlib.h>  /* Biblioteca para alocação dinâmica, controle de memória e outras funções úteis */
#include <string.h>  /* Biblioteca para manipulação de strings */
#include <locale.h>  /* Biblioteca para configuração regional e suporte a caracteres locais */

#define MAX_NOME 20

typedef struct No* arvAvl; 
/* Tipo arvAvl é um ponteiro para a estrutura No, que representa um nó da árvore AVL */

struct No {
    struct No *esq; /* Ponteiro para o filho esquerdo */
    int info;       /* Valor armazenado no nó, guarda a ordem da jogada*/
    int alt;        /* Altura da subárvore a partir deste nó, usada para cálculo do fator de balanceamento */
    tipos_jogada titulo;
    char nome_jogador[MAX_NOME];
    struct No *dir; /* Ponteiro para o filho direito */
};

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

/* Protótipos das funções */
arvAvl* criarAvl();
void preOrd(arvAvl* raiz);
void emOrd(arvAvl* raiz);
void posOrd(arvAvl* raiz);
struct No* inserir(arvAvl* raiz, int valor, char *nome, tipos_jogada titulo);
int remover(arvAvl* raiz, int valor);
struct No* buscarMenor(struct No* atual);
int consultarValorAvl(arvAvl* raiz, int valor);
int maior(int x, int y);
int altNo(struct No* no);
int fatorBalanceamentoNo(struct No* no);
void rotacaoLl(arvAvl* raiz);
void rotacaoRr(arvAvl* raiz);
void rotacaoRl(arvAvl* raiz);
void rotacaoLr(arvAvl* raiz);

int contPrint = 0;

/* Cria uma árvore AVL vazia e retorna um ponteiro para ela */
arvAvl* criarAvl() {
    arvAvl* raiz = (arvAvl*) malloc(sizeof(arvAvl));
    if(raiz != NULL) {
        *raiz = NULL; /* Inicializa a árvore com ponteiro nulo */
    }
    return raiz;
}

/* Verifica se a árvore AVL está vazia.
Retorna 1 se vazia ou ponteiro nulo, 0 caso contrário */
int estahVaziaAvl(arvAvl* raiz) {
    if(raiz == NULL) return 1;
    if(*raiz == NULL) return 1;
    return 0;
}

/* Imprime a árvore em pré-ordem (Raiz, Esquerda, Direita) */
void preOrd(arvAvl* raiz) {
    if(raiz == NULL) return;
    if(*raiz != NULL) {
        if(contPrint != 0) {
            printf(" ");
        }
        printf("%d", (*raiz)->info);
        contPrint++;
        preOrd(&((*raiz)->esq));
        preOrd(&((*raiz)->dir));
    }
}

/* Imprime a árvore em ordem simétrica (Esquerda, Raiz, Direita) */
void emOrd(arvAvl* raiz) {
    if(raiz == NULL) return;
    if(*raiz != NULL) {
        emOrd(&((*raiz)->esq));
        if(contPrint != 0) {
            printf(" ");
        }
        printf("%d", (*raiz)->info);
        contPrint++;
        emOrd(&((*raiz)->dir));
    }
}

/* Imprime a árvore em pós-ordem (Esquerda, Direita, Raiz) */
void posOrd(arvAvl* raiz) {
    if(raiz == NULL) return;
    if(*raiz != NULL) {
        posOrd(&((*raiz)->esq));
        posOrd(&((*raiz)->dir));
        if(contPrint != 0) {
            printf(" ");
        }
        printf("%d", (*raiz)->info);
        contPrint++;
    }
}

/* Retorna a altura da árvore AVL */
int alturaAvl(arvAvl* raiz) {
    if(raiz == NULL) return 0;
    if(*raiz == NULL) return 0;

    int altEsq = alturaAvl(&((*raiz)->esq));
    int altDir = alturaAvl(&((*raiz)->dir));

    if(altEsq > altDir) {
        return altEsq + 1;
    } else {
        return altDir + 1;
    }
}

/* Retorna o total de nós da árvore */
int totalNosAvl(arvAvl* raiz) {
    if(raiz == NULL) return 0;
    if(*raiz == NULL) return 0;

    int totEsq = totalNosAvl(&((*raiz)->esq));
    int totDir = totalNosAvl(&((*raiz)->dir));

    return (totEsq + totDir + 1);
}

/* Insere um valor na árvore AVL mantendo o balanceamento */
arvAvl inserir(arvAvl* raiz, int valor, char *nome, tipos_jogada titulo) {
    int res;
    if(*raiz == NULL) {
        struct No* novo = (struct No*) malloc(sizeof(struct No));
        if(novo == NULL) return NULL; //Alterei a função para que ela retorne o endereço do novo nó (NULL)
        novo->info = valor;
        novo->alt = 0;
        novo->titulo = titulo; //Armazena o título da jogada
        strcpy(novo->nome_jogador, nome); //Armazena o nome do jogador
        novo->dir = NULL;
        novo->esq = NULL;
        *raiz = novo;
        return novo; //Alterei a função para que ela retorne o endereço do novo nó (NOVO)
    } else {
        struct No* atual = *raiz;
        if(valor < atual->info) {
            if(inserir(&(atual->esq), valor, nome, titulo) != NULL) {
                if(fatorBalanceamentoNo(atual) > 1 || fatorBalanceamentoNo(atual) < -1) {
                    if(valor < (*raiz)->esq->info) {
                        rotacaoLl(raiz);
                    } else {
                        rotacaoLr(raiz);
                    }
                }
            }
        } else if(valor > atual->info) {
            if(inserir(&(atual->dir), valor, nome, titulo) != NULL) {
                if(fatorBalanceamentoNo(atual) > 1 || fatorBalanceamentoNo(atual) < -1) {
                    if(valor > (*raiz)->dir->info) {
                        rotacaoRr(raiz);
                    } else {
                        rotacaoRl(raiz);
                    }
                }
            }
        } else {
            return NULL; /* Valor duplicado, não insere */
        }
        atual->alt = maior(altNo(atual->esq), altNo(atual->dir)) + 1;
        return atual; //Alterei a função para que ela retorne o endereço do novo nó (ATUAL)
    }
}

/* Libera a memória de todos os nós recursivamente */
void liberaNo(struct No* no) {
    if(no == NULL) return;
    liberaNo(no->esq);
    liberaNo(no->dir);
    free(no);
    no = NULL;
}

/* Libera toda a árvore AVL */
void liberaAvl(arvAvl* raiz) {
    if(raiz == NULL) return;
    liberaNo(*raiz);
    free(raiz);
}

/* Consulta se um valor existe na árvore AVL */
int consultarValorAvl(arvAvl* raiz, int valor) {
    if(raiz == NULL) return 0;
    if(*raiz == NULL) return 0;

    struct No* atual = *raiz;
    while(atual != NULL) {
        if(atual->info == valor) {
            return 1;
        } else if(atual->info > valor) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }
    return 0;
}

/* Retorna a altura de um nó ou -1 se nulo */
int altNo(struct No* no) {
    if(no == NULL) return -1;
    else return no->alt;
}

/* Retorna o fator de balanceamento do nó (altura esq - altura dir) */
int fatorBalanceamentoNo(struct No* no) {
    if(no == NULL) return 0;
    return altNo(no->esq) - altNo(no->dir);
}

/* Retorna o maior valor entre dois inteiros */
int maior(int x, int y) {
    return (x > y) ? x : y;
}

/* Rotação simples à esquerda (LL) */
void rotacaoLl(arvAvl* raiz) {
    struct No* no = (*raiz)->esq;
    (*raiz)->esq = no->dir;
    no->dir = *raiz;

    (*raiz)->alt = maior(altNo((*raiz)->esq), altNo((*raiz)->dir)) + 1;
    no->alt = maior(altNo(no->esq), (*raiz)->alt) + 1;

    *raiz = no;
}

/* Rotação simples à direita (RR) */
void rotacaoRr(arvAvl* raiz) {
    struct No* no = (*raiz)->dir;
    (*raiz)->dir = no->esq;
    no->esq = *raiz;

    (*raiz)->alt = maior(altNo((*raiz)->esq), altNo((*raiz)->dir)) + 1;
    no->alt = maior(altNo(no->dir), (*raiz)->alt) + 1;

    *raiz = no;
}

/* Rotação dupla à esquerda-direita (LR) */
void rotacaoLr(arvAvl* raiz) {
    rotacaoRr(&(*raiz)->esq);
    rotacaoLl(raiz);
}

/* Rotação dupla à direita-esquerda (RL) */
void rotacaoRl(arvAvl* raiz) {
    rotacaoLl(&(*raiz)->dir);
    rotacaoRr(raiz);
}

/* Remove um valor da árvore AVL mantendo o balanceamento */
int remover(arvAvl* raiz, int valor) {
    int res;
    if(*raiz == NULL) { /* Valor não encontrado */
        printf("Valor %d nao encontrado na árvore!\n", valor);
        return 0;
    }

    if(valor < (*raiz)->info) {
        if((res = remover(&(*raiz)->esq, valor)) == 1) {
            if(fatorBalanceamentoNo(*raiz) > 1 || fatorBalanceamentoNo(*raiz) < -1) {
                if(altNo((*raiz)->dir->esq) <= altNo((*raiz)->dir->dir)) {
                    rotacaoRr(raiz);
                } else {
                    rotacaoRl(raiz);
                }
            }
        }
    } else if(valor > (*raiz)->info) {
        if((res = remover(&(*raiz)->dir, valor)) == 1) {
            if(fatorBalanceamentoNo(*raiz) > 1 || fatorBalanceamentoNo(*raiz) < -1) {
                if(altNo((*raiz)->esq->dir) <= altNo((*raiz)->esq->esq)) {
                    rotacaoLl(raiz);
                } else {
                    rotacaoLr(raiz);
                }
            }
        }
    } else { /* Nó encontrado */
        if(((*raiz)->esq == NULL) || ((*raiz)->dir == NULL)) { 
            /* Nó com um filho ou nenhum */
            struct No* noVelho = (*raiz);
            if((*raiz)->esq != NULL) *raiz = (*raiz)->esq;
            else *raiz = (*raiz)->dir;
            free(noVelho);
            return 1;
        } else {
            /* Nó com dois filhos: substituir pelo menor nó da subárvore direita */
            struct No* temp = buscarMenor((*raiz)->dir);
            (*raiz)->info = temp->info;
            remover(&(*raiz)->dir, (*raiz)->info);
            if(fatorBalanceamentoNo(*raiz) > 1 || fatorBalanceamentoNo(*raiz) < -1) {
                if(altNo((*raiz)->esq->dir) <= altNo((*raiz)->esq->esq)) {
                    rotacaoLl(raiz);
                } else {
                    rotacaoLr(raiz);
                }
            }
        }
    }
    (*raiz)->alt = maior(altNo((*raiz)->esq), altNo((*raiz)->dir)) + 1;
    return 1;
}

/* Busca o nó com menor valor da árvore (mais à esquerda) */
struct No* buscarMenor(struct No* atual) {
    while(atual->esq != NULL) {
        atual = atual->esq;
    }
    return atual;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

/*
Esse é um enum, ele atribui um valor númerico
em ordem para cada nome de tipos_jogada, então estamos trabalhando basicamente com inteiros. 
*/

typedef int tp_item;

typedef struct tp_no{
    struct tp_no *esq;
    tipos_jogada titulo;
    char nome_jogador[MAX_NOME];
    tp_item info;
    struct tp_no *dir;
}tp_no;
/*
A ideia desse struct é que armazenemos o valor (info) como o número da ordem da tipos_jogada
(ordenando em que momento ela aconteceu) e o titulo (baseado no enum).
*/

typedef tp_no * tp_arvore;

tp_arvore inicializa_arvore(){
    return NULL;
}

int arvore_vazia(tp_arvore raiz){
    if(raiz == NULL) return 1;
    else return 0;
}

tp_no * aloca_no(){
    tp_no *no;
    no = (tp_no*)malloc(sizeof(tp_no));
    return no;
}

tp_arvore insere_no(tp_arvore *raiz, char* nome, tp_item e, tipos_jogada titulo){
    tp_no *pai = NULL, *novo, *p=*raiz;
    novo = aloca_no();
    if(!novo) return NULL;
    novo->info = e;
    novo->titulo = titulo; //além de armazenar o info, armazena o titulo
    strcpy(novo->nome_jogador, nome); //armazena também o nome do jogador
    novo->esq = NULL;
    novo->dir = NULL;
    while(p != NULL){
        pai = p;
        if(e < p->info){
            p=p->esq;
        }
        else{
            p=p->dir;
        }
    }
    if(pai != NULL){
        if(e < pai->info){
            pai->esq = novo;
        }
        else{
            pai->dir = novo;
        }
    }
    else{
        *raiz = novo;
    }
    return novo; //Alterei a função para que ela retorne o endereço do novo nó
} 
//segue a lógica de ordenação das árvores de busca binária

void pre_ordem(tp_no *p){
    if(p != NULL){
        printf("\n%d\n", p->info);
        pre_ordem(p->esq);
        pre_ordem(p->dir);
    }
}
//faz a impressão dos valores dos nós de acordo com a ordenação pré-ordem

void em_ordem(tp_no *p){
    if(p != NULL){
        em_ordem(p->esq);
        printf("\n%d\n", p->info);
        em_ordem(p->dir);
    }
}
//faz a impressão dos valores dos nós de acordo com a ordenação em-ordem

void pos_ordem(tp_no *p){
    if(p != NULL){
        pos_ordem(p->esq);
        pos_ordem(p->dir);
        printf("\n%d\n", p->info);
    }
}
//faz a impressão dos valores dos nós de acordo com a ordenação pós-ordem

tp_no *busca_no(tp_no *p, tp_item e){
    while(p != NULL){
        if(e < p->info){
            p = p->esq;
        }
        else{
            if(e > p->info) p = p->dir;
            else return (p);
        }
    }
    return NULL;
}

tp_arvore busca_no_rec(tp_arvore p, tipos_jogada e){
    if(p != NULL){
        if(e < p->info)
            return busca_no_rec(p->esq, e);
        else
            if(e > p->info)
                return busca_no_rec(p->dir, e);
            else
                return (p);
            
        }
    else{
        return (NULL);
    }
}
//faz a busca do nó por meio da recursividade

int qtde_niveis_arvore(tp_arvore raiz){
    if(raiz == NULL) return 0;
    int alt_esq = qtde_niveis_arvore(raiz->esq);
    int alt_dir = qtde_niveis_arvore(raiz->dir);
    if(alt_esq > alt_dir){
        return alt_esq + 1;
    }
    else{
        return alt_dir + 1;
    }
}

int qtde_nos(tp_arvore raiz){
    if(raiz == NULL) return 0;
    int tot_esq = qtde_nos(raiz->esq);
    int tot_dir = qtde_nos(raiz->dir);
    return (tot_esq + tot_dir + 1);
}

int remove_no (tp_arvore *raiz, tipos_jogada e ) {
	tp_no  *p=*raiz, *ant=NULL, *sub, *pai, *filho;
	while (p!=NULL && p->info!=e) {
		ant=p;
		if (e < p->info)
			p = p->esq;
		else
			p = p->dir;
	}
	if (p==NULL) /* não encontrou */
		return  0;
	/* nó tem no máximo um filho */
	if (p->esq==NULL)
		sub=p->dir;
	else
		if (p->dir==NULL)
			sub=p->esq;
		else {
		/* nó tem dois filhos */
			pai=p; sub=p->dir; filho=sub->esq;
			while (filho!=NULL) {
				pai=sub;  sub=filho;  filho=sub->esq;
			}
/* neste ponto, sub é o sucessor em ordem de p */
			if (pai!=p) {
				/*p não é o pai de sub e sub==pai->esq */
				pai->esq=sub->dir;
				/* remove o nó apontado por sub de sua atual 
				 posição e substitui pelo filho direito de rp */
				/* sub ocupa o lugar de p */
				sub->dir=p->dir;
			}
		/*define filho esq de sub para que sub ocupe o lugar de p */
			sub->esq=p->esq;
		}
		/* insere sub na posição ocupada por p */
		if (ant==NULL)
			*raiz=sub; /* p era raiz */
		else
			if (p==ant->esq)
				ant->esq=sub;
			else
				ant->dir=sub;
		free(p);
	return 1;
}
//a estrutura dessa função é evidentemente mais complicado 
//porque precisamos lidar com o que acontece, com os nós restantes 
//quando removemos um nó que é raiz em relação a eles

void destroi_no(tp_no *no){
    if(no == NULL) return;
    destroi_no(no->esq);
    destroi_no(no->dir);
    //printf("\n %d ", no->info);
    free(no);
    no = NULL;
    return;
}

void destroi_arvore(tp_arvore *raiz){
    if(*raiz == NULL) return;
    destroi_no(*raiz);
    // free(*raiz);
   *raiz=NULL;
}
//essas duas funções destroém a árvore, nó por nó

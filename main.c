#include <stdio.h>
#include <stdlib.h>
#include "estrutura_jogador.h"
#include "pilhaLE.h"
#include "cartas.h"
#include "mesa.h"


int main() {
    Mesa poker;
    inicializar_mesa(&poker);

    printf("Bem-vindo ao Poker 2025.1!");


    while(1){
        int entrada;
        printf("\nDigite:\n 1 - Criar Jogador\n 2 - Embaralhar e Preparar Mesa\n 3 - Sair do Jogo\n> ");
        scanf("%d", &entrada);

        if(entrada == 1){
            //limita o numero de jogadores a 8
            if (poker.num_jogadores >= MAX_JOGADORES) {
                printf("Limite de jogadores atingido!\n");
                continue;
            }

            jogador *novo = (jogador*)malloc(sizeof(jogador));
            char nome[MAXIMO_NOME];
            int fichas, posicaoMesa;
            printf("Digite o nome do jogador (serao salvos apenas 20 caracteres): ");
            scanf("%19s", nome);
            printf("Digite o numero de fichas que o jogador tera para comecar: ");
            scanf("%d", &fichas);
            posicaoMesa = poker.num_jogadores + 1;

            criarJogador(novo, nome, fichas, posicaoMesa);
            adicionar_jogador_a_mesa(&poker, novo);
        }else if (entrada == 2){
            //reinicializa o baralho
            Baralho b;
            inicializaBaralho(&b);
            embaralha(&b, &poker.baralho);
            imprimirBaralho(b); //(funcao para verificar a randomizacao do baralho, desative sempre)

        }else if(entrada == 3){
            printf("Jogo encerrado!");
            break;
        }
        else{
            printf("Entrada incorreta, encerrando jogo!");
            break;
        }
        
    }
    
    return 0;
}

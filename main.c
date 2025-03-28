#include <stdio.h>
#include "estrutura_jogador.h"


int main() {
    printf("Bem-vindo ao Poker 2025.1!");
    while(1){
        int entrada;
        printf("Digite:\n 1 - Criar Jogador\n 2 - Sair do jogo.\n> ");
        scanf("%d", &entrada);
        if(entrada == 1){
            jogador nUm;
            char nome[MAXIMO_NOME];
            int fichas;
            int posicaoMesa;
            printf("Digite o nome do jogador (serao salvos apenas 20 caracteres): ");
            scanf("%19s", nome);
            printf("Digite o numero de fichas que o jogador tera para comecar: ");
            scanf("%d", &fichas);
            printf("Informe a posição do jogador na mesa: ");
            scanf("%d", &posicaoMesa);
            criarJogador(&nUm, nome, fichas, posicaoMesa);
        }else if(entrada == 2){
            printf("Jogo encerrado!");
            break;
        }else{
            printf("Entrada incorreta, encerrando jogo!");
            break;
        }
        
    }
    return 0;
}
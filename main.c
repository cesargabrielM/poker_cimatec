#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "estrutura_jogador.h"
#include "pilhaLE.h"
#include "cartas.h"
#include "mesa.h"

/**
 * Limpa o buffer de entrada para evitar problemas com scanf
 */
void limpar_buffer() {
    while (getchar() != '\n');
}

/**
 * Função principal do programa
 * @return Código de saída
 */
int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252");
    
    Mesa poker; // Cria a mesa principal
    inicializar_mesa(&poker); // Inicializa o jogo

    printf("=== POKER BY CÉSAR E AMIGUINHOS ===\n");

    printf("\nBem-vindo ao jogo de Poker da turma de 2025.1 de AED produzido pela equipe César e amiguinhos! ");
    printf("Esperamos que se divirta!\n");

    // Menu principal
    while (1) {
        printf("\nMenu Principal:\n");
        printf("1. Adicionar Jogador\n");
        printf("2. Jogar Uma Rodada\n");
        printf("3. Sair\n> ");

        int opcao;
        if (scanf("%d", &opcao) != 1) { // Lê a opção do usuário
            printf("Entrada inválida!\n");
            limpar_buffer();
            continue;
        }

        switch (opcao) {
            case 1: { // Adicionar jogador
                if (poker.num_jogadores >= MAX_JOGADORES) {
                    printf("Máximo de jogadores atingido!\n");
                    break;
                }

                char nome[MAXIMO_NOME];
                int fichas;

                printf("Nome: ");
                scanf(" %19[^\n]", nome); // Lê o nome do jogador
                printf("Fichas: ");
                scanf("%d", &fichas); // Lê as fichas iniciais
                limpar_buffer(); // Limpa o buffer após leitura

                // Cria e adiciona o novo jogador
                jogador *novo = (jogador*)malloc(sizeof(jogador));
                criarJogador(novo, nome, fichas, poker.num_jogadores+1);
                adicionar_jogador_a_mesa(&poker, novo);
                break;
            }

            case 2: { // Jogar uma rodada
                if (poker.num_jogadores < 2) {
                    printf("Mínimo 2 jogadores!\n");
                    break;
                }

                printf("\n=== INÍCIO DA RODADA ===\n");

                // 1. Embaralha e distribui cartas
                Baralho novo;
                inicializaBaralho(&novo);
                embaralha(&novo, &poker.baralho);
                distribuir_cartas_jogadores(&poker);

                // 2. Apostas iniciais (simplificado)
                poker.pote = 0;
                for (int i = 0; i < poker.num_jogadores; i++) {
                    if (poker.jogadores[i]->atual == ativo) {
                        poker.jogadores[i]->aposta = 10; // Small blind fixo
                        poker.jogadores[i]->fichas -= 10;
                        poker.pote += 10;
                        printf("%s apostou 10 fichas\n", poker.jogadores[i]->nome);
                    }
                }

                // 3. Executa uma única rodada
                realizar_rodada(&poker);

                // 4. Limpa as mãos dos jogadores para próxima rodada
                for (int i = 0; i < poker.num_jogadores; i++) {
                    destroi_listad(poker.jogadores[i]->mao);
                    poker.jogadores[i]->mao = inicializa_listad();
                }

                // 5. Sai do loop após uma rodada
                printf("\nRodada concluída. O jogo será encerrado.\n");
                goto liberar_memoria;
            }

            case 3: // Sair do jogo
                printf("Espero que tenha gostado! ");
                printf("Saindo...\n");
                goto liberar_memoria;

            default:
                printf("Opção inválida!\n");
        }
    }

liberar_memoria:
    // Libera toda a memória alocada
    for (int i = 0; i < poker.num_jogadores; i++) {
        free(poker.jogadores[i]);
    }

    return 0;
}

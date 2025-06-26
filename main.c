#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "estrutura_jogador.h" //
#include "pilhaLE.h" //
#include "cartas.h" //
#include "mesa.h" // Inclui mesa.h, que agora inclui jogadas.h e arvAvl_dict.h

// DEFINIR contPrint aqui, em APENAS UM arquivo .c
int contPrint = 0;

/**
 * Limpa o buffer de entrada para evitar problemas com scanf
 */
void limpar_buffer() {
    while (getchar() != '\n'); //
}

/**
 * Função principal do programa
 * @return Código de saída
 */
int main() {
    setlocale(LC_ALL, "Portuguese_Brazil.1252"); //
    
    Mesa poker; // Cria a mesa principal
    inicializar_mesa(&poker); // Inicializa o jogo

    printf("=== POKER BY CÉSAR E AMIGUINHOS ===\n"); //

    printf("\nBem-vindo ao jogo de Poker da turma de 2025.1 de AED produzido pela equipe César e amiguinhos! "); //
    printf("Esperamos que se divirta!\n"); //

    // Inicializa o arquivo de log de jogadas
    if (!inicializaArquivo()) { //
        printf("Erro ao inicializar o arquivo de jogadas!\n"); //
        return 1; //
    }

    arvAvl *arvore_jogadas = criaAVL(); // Cria a árvore AVL para armazenar as jogadas
    if (arvore_jogadas == NULL) { //
        printf("Erro ao criar a árvore AVL de jogadas!\n"); //
        return 1; //
    }

    int contador_jogadas = 0; // Contador para a ordem das jogadas

    // Menu principal
    while (1) {
        printf("\nMenu Principal:\n"); //
        printf("1. Adicionar Jogador\n"); //
        printf("2. Jogar Uma Rodada\n"); //
        printf("3. Sair\n> "); //

        int opcao; //
        if (scanf("%d", &opcao) != 1) { // Lê a opção do usuário
            printf("Entrada inválida!\n"); //
            limpar_buffer(); //
            continue; //
        }

        switch (opcao) { //
            case 1: { // Adicionar jogador
                if (poker.num_jogadores >= MAX_JOGADORES) { //
                    printf("Máximo de jogadores atingido!\n"); //
                    break; //
                }

                char nome[MAXIMO_NOME]; //
                int fichas; //

                printf("Nome: "); //
                scanf(" %19[^\n]", nome); // Lê o nome do jogador
                printf("Fichas: "); //
                scanf("%d", &fichas); // Lê as fichas iniciais
                limpar_buffer(); // Limpa o buffer após leitura

                // Cria e adiciona o novo jogador
                jogador *novo = (jogador*)malloc(sizeof(jogador)); //
                criarJogador(novo, nome, fichas, poker.num_jogadores+1); //
                adicionar_jogador_a_mesa(&poker, novo); //
                break; //
            }

            case 2: { // Jogar uma rodada
                if (poker.num_jogadores < 2) { //
                    printf("Mínimo 2 jogadores!\n"); //
                    break; //
                }

                printf("\n=== INÍCIO DA RODADA ===\n"); //

                // 1. Embaralha e distribui cartas
                Baralho novo; //
                inicializaBaralho(&novo); //
                embaralha(&novo, &poker.baralho); //
                distribuir_cartas_jogadores(&poker); //

                // 2. Apostas iniciais (simplificado)
                poker.pote = 0; //
                for (int i = 0; i < poker.num_jogadores; i++) { //
                    if (poker.jogadores[i]->atual == ativo) { //
                        poker.jogadores[i]->aposta = 10; // Small blind fixo
                        poker.jogadores[i]->fichas -= 10; //
                        poker.pote += 10; //
                        printf("%s apostou 10 fichas\n", poker.jogadores[i]->nome); //
                    }
                }

                // 3. Executa uma única rodada, passando a árvore e o contador de jogadas
                realizar_rodada(&poker, arvore_jogadas, &contador_jogadas); //

                // 4. Limpa as mãos dos jogadores para próxima rodada
                for (int i = 0; i < poker.num_jogadores; i++) { //
                    destroi_listad(poker.jogadores[i]->mao); //
                    poker.jogadores[i]->mao = inicializa_listad(); //
                }

                // 5. Sai do loop após uma rodada
                printf("\nRodada concluída. O jogo será encerrado.\n"); //
                goto liberar_memoria; //
            }

            case 3: // Sair do jogo
                printf("Espero que tenha gostado! "); //
                printf("Saindo...\n"); //
                goto liberar_memoria; //

            default:
                printf("Opção inválida!\n"); //
        }
    }

liberar_memoria:
    printf("\n=== JOGADAS REGISTRADAS EM MEMÓRIA (ÁRVORE AVL) ===\n");
    if (!estahVaziaAvl(arvore_jogadas)) { //
        contPrint = 0; // Reinicia o contador para impressão
        // Para uma apresentação mais detalhada, você pode consultar cada nó e exibir as informações completas.
        // Já que a ordem das jogadas (info) é sequencial, podemos iterar de 1 até o contador_jogadas.
        for (int i = 1; i <= contador_jogadas; i++) {
            struct No* jogada = consultarValorAvl(arvore_jogadas, i); //
            if (jogada != NULL) { //
                const char* nome_jogada_str; //
                switch (jogada->titulo) { // Usando o enum tipos_jogada
                    case CARTA_ALTA: nome_jogada_str = "Carta Alta"; break; //
                    case PAR: nome_jogada_str = "Par"; break; //
                    case DOIS_PARES: nome_jogada_str = "Dois Pares"; break; //
                    case TRINCA: nome_jogada_str = "Trinca"; break; //
                    case STRAIGHT: nome_jogada_str = "Straight (Sequência)"; break; //
                    case FLUSH: nome_jogada_str = "Flush"; break; //
                    case FULL_HOUSE: nome_jogada_str = "Full House"; break; //
                    case QUADRA: nome_jogada_str = "Quadra"; break; //
                    case STRAIGHT_FLUSH: nome_jogada_str = "Straight Flush"; break; //
                    case ROYAL_FLUSH: nome_jogada_str = "Royal Flush"; break; //
                    default: nome_jogada_str = "Desconhecida"; break; //
                }
                printf("Jogada %d: Jogador: %s, Tipo: %s\n", jogada->info, jogada->nome_jogador, nome_jogada_str); //
            }
        }
    } else {
        printf("Nenhuma jogada foi registrada nesta partida.\n"); //
    }

    // Libera a memória alocada para a árvore AVL
    liberaAvl(arvore_jogadas); //

    // Libera toda a memória alocada para os jogadores
    for (int i = 0; i < poker.num_jogadores; i++) { //
        free(poker.jogadores[i]); //
    }
    // Libera as cartas comunitárias
    if (poker.cartas_comunitarias != NULL) { //
        destroi_listad(poker.cartas_comunitarias); //
    }
    // Não precisa destruir o baralho da mesa, pois ele é uma pilha e o pop já libera os nós.

    return 0; //
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

// Adicionado para configurar o console do Windows
#ifdef _WIN32
#include <windows.h> 
#endif

#include "pilhaLE.h"
#include "cartas.h"
#include "listade.h"
#include "estrutura_jogador.h"
#include "arvAvl_dict.h"
#include "jogadas.h"
#include "mesa.h"


void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    // --- MODIFICAÇÃO INÍCIO: Configuração do terminal ---
    // Define a localização para permitir caracteres portugueses
    setlocale(LC_ALL, "Portuguese_Brazilian");

    // Se estiver no Windows, define a página de código do console para UTF-8
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
    // --- MODIFICAÇÃO FIM ---


    srand((unsigned int)time(NULL));

    Mesa poker;
    poker.baralho.topo = NULL;
    inicializar_mesa(&poker);

    arvAvl* arvore_jogadas = criarAvl();
    int contador_jogadas = 0;
    inicializaArquivo();

    while(1) {
        limpar_tela(); //Limpa a tela a cada volta do menu
        mostrarMenu();
        
        //Mostra o status dos jogadores no menu
        if (poker.num_jogadores > 0) {
            printf("\n--- SALDO DOS JOGADORES ---\n");
            for(int i = 0; i < poker.num_jogadores; i++){
                printf("  - %s: %d fichas\n", poker.jogadores[i]->nome, poker.jogadores[i]->fichas);
            }
        } else {
            printf("\nNenhum jogador na mesa.\n");
        }
        
        printf("\n---------------------------------------------------\n");
        printf("MENU PRINCIPAL:\n");
        printf("1. Adicionar Jogador\n");
        printf("2. Iniciar Nova Rodada\n");
        printf("3. Sair do Jogo\n");
        printf("---------------------------------------------------\n");
        printf("> ");

        int opcao;
        if(scanf("%d", &opcao) != 1) {
            printf("Entrada inválida!\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        switch(opcao) {
            case 1: {
                if(poker.num_jogadores >= MAX_JOGADORES) {
                    printf("Máximo de jogadores atingido!\n");
                } else {
                    char nome[MAXIMO_NOME];
                    int fichas;

                    printf("Nome do Jogador: ");
                    scanf("%19s", nome);
                    limpar_buffer();

                    printf("Quantidade de Fichas Iniciais: ");
                    scanf("%d", &fichas);
                    limpar_buffer();

                    jogador *novo = (jogador*)malloc(sizeof(jogador));
                    if(novo == NULL) {
                        printf("Erro de alocação de memória!\n");
                        break;
                    }
                    criarJogador(novo, nome, fichas, poker.num_jogadores);
                    adicionar_jogador_a_mesa(&poker, novo);
                }
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;
            }

            case 2: {
                if(poker.num_jogadores < 2) {
                    printf("É necessário ter no mínimo 2 jogadores para começar!\n");
                } else {
                    realizar_rodada_completa(&poker, arvore_jogadas, &contador_jogadas);
                }
                printf("\nRodada finalizada. Pressione ENTER para voltar ao menu...");
                getchar();
                break;
            }

            case 3:
                limpar_tela();
                printf("Saindo e liberando memória... Obrigado por jogar!\n");
                for(int i = 0; i < poker.num_jogadores; i++) {
                    if (poker.jogadores[i]->mao != NULL) {
                        destroi_listad(poker.jogadores[i]->mao);
                    }
                    free(poker.jogadores[i]);
                }
                if(arvore_jogadas) liberaAvl(arvore_jogadas);
                if(poker.cartas_comunitarias) destroi_listad(poker.cartas_comunitarias);
                
                tp_item lixo;
                while(!pilhaVazia(&poker.baralho)){
                    pop(&poker.baralho, &lixo);
                }
                return 0;

            default:
                printf("Opção inválida!\n");
        }
    }

    return 0;
}

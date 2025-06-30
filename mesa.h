#ifndef MESA_H
#define MESA_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "estrutura_jogador.h"
#include "pilhaLE.h"
#include "cartas.h"
#include "listade.h"
#include "jogadas.h"
#include "arvAvl_dict.h"
#include "interface.h"

// Constantes do jogo
#define MAX_JOGADORES 8       // NC:mero mC!ximo de jogadores na mesa
#define CARTAS_COMUNITARIAS 5 // NC:mero de cartas comunitC!rias (Flop+Turn+River)
#define SMALL_BLIND_VALOR 5
#define BIG_BLIND_VALOR 10
#define APOSTA_MINIMA BIG_BLIND_VALOR // A aposta mínima é geralmente o BB

/**
 * Estrutura que representa a mesa de poker
 * ContC)m todas as informaC'C5es do estado atual do jogo
 */
typedef struct {
	tp_pilha baralho;                   // Baralho de cartas (pilha)
	jogador *jogadores[MAX_JOGADORES];  // Array de ponteiros para jogadores
	tp_listad *cartas_comunitarias;     // Lista de cartas comunitC!rias
	int num_jogadores;                  // NC:mero atual de jogadores
	int dealer_pos;                     // PosiC'C#o do dealer na mesa
	int pote;                           // Valor total do pote atual
} Mesa; // DefiniC'C#o da struct Mesa. Movemos para cima, se necessC!rio.

// ProtC3tipos das funC'C5es pC:blicas
static void inicializar_mesa(Mesa *mesa);
static void adicionar_jogador_a_mesa(Mesa *mesa, jogador *j);
static void distribuir_cartas_jogadores(Mesa *mesa);
static tipos_jogada avaliar_mao(jogador *j, tp_listad *comunitarias);   // Alteramos o retorno de avaliar_mao para 'tipos_jogada'

// FunC'C5es auxiliares (visibilidade restrita a este arquivo)
static int converter_valor_para_indice(const char *valor);
static int contar_ocorrencias_valores(tp_listad *cartas, int contagem[13]);
static int contar_ocorrencias_naipes(tp_listad *cartas, int naipes[4]);
static int verificar_par(tp_listad *cartas);
static int verificar_dois_pares(tp_listad *cartas);
static int verificar_trinca(tp_listad *cartas);
static int verificar_sequencia(tp_listad *cartas);
static int verificar_flush(tp_listad *cartas);
static int verificar_full_house(tp_listad *cartas);
static int verificar_quadra(tp_listad *cartas);             // Adicionar funC'C#o para QUADRA
static int verificar_straight_flush(tp_listad *cartas);     // Adicionar funC'C#o para STRAIGHT_FLUSH
static int verificar_royal_flush(tp_listad *cartas);        // Adicionar funC'C#o para ROYAL_FLUSH

void limpar_tela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/**
 * Inicializa o estado da mesa
 * @param mesa Ponteiro para a estrutura Mesa a ser inicializada
 */
static void inicializar_mesa(Mesa *mesa) {
	Baralho b;
	inicializaBaralho(&b);          // Inicializa um novo baralho
	embaralha(&b, &mesa->baralho);  // Embaralha as cartas

	mesa->num_jogadores = 0;        // Sem jogadores inicialmente
	mesa->dealer_pos = -1;          
	mesa->cartas_comunitarias = inicializa_listad(); // Inicializa lista de cartas comunitC!rias

	// Queima a primeira carta (regra do poker)
	Carta queimada;
	pop(&mesa->baralho, &queimada);
}

/*
 * Adiciona um jogador C  mesa
 * @param mesa Ponteiro para a estrutura Mesa
 * @param j Ponteiro para o jogador a ser adicionado
 */
static void adicionar_jogador_a_mesa(Mesa *mesa, jogador *j) {
	if (mesa->num_jogadores < MAX_JOGADORES) {
		mesa->jogadores[mesa->num_jogadores] = j;
		mesa->num_jogadores++;
	}
}

/**
 * Distribui cartas para todos os jogadores (2 cartas cada)
 * @param mesa Ponteiro para a estrutura Mesa
 */
static void distribuir_cartas_jogadores(Mesa *mesa) {
	// Distribui duas cartas para cada jogador
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < mesa->num_jogadores; j++) {
			Carta c;
			pop(&mesa->baralho, &c);
			insere_listad_no_fim(mesa->jogadores[j]->mao, c);
		}
	}
}

/**
 * Realiza uma rodada completa do jogo
 * @param mesa Ponteiro para a estrutura Mesa
 * @param arvore_jogadas Ponteiro para a C!rvore AVL onde as jogadas serC#o armazenadas
 * @param contador_jogadas Ponteiro para o contador de ordem das jogadas
 */
static void realizar_showdown(Mesa *mesa, arvAvl *arvore_jogadas, int *contador_jogadas) {
	jogador *vencedores[MAX_JOGADORES];
	int num_vencedores = 0;

    // Primeiro, verifica quantos jogadores ainda estão ativos.
    for (int i = 0; i < mesa->num_jogadores; i++) {
        if (mesa->jogadores[i]->atual == ativo) {
            vencedores[num_vencedores++] = mesa->jogadores[i];
        }
    }

    // Se só há um jogador ativo, ele ganha o pote automaticamente, sem mostrar as cartas.
    if (num_vencedores == 1) {
        printf("\n%s venceu a rodada, pois todos os outros desistiram!\n", vencedores[0]->nome);
        vencedores[0]->fichas += mesa->pote;
    } else if (num_vencedores > 1) {
        // Se há mais de um jogador, realiza a comparação de mãos (showdown).
        printf("\n=== SHOWDOWN ===\n");
        tipos_jogada melhores_jogadas_enum[MAX_JOGADORES];
        tipos_jogada melhor_combinacao = CARTA_ALTA;

        // Avalia a mão de cada jogador que ainda está na rodada
        for (int i = 0; i < num_vencedores; i++) {
            char titulo_mao[50];
            sprintf(titulo_mao, "Mão de %s: ", vencedores[i]->nome);
            desenhar_cartas(vencedores[i]->mao, titulo_mao);
            
            melhores_jogadas_enum[i] = avaliar_mao(vencedores[i], mesa->cartas_comunitarias);
            
            // Lógica para encontrar a melhor combinação
            if (melhores_jogadas_enum[i] > melhor_combinacao) {
                melhor_combinacao = melhores_jogadas_enum[i];
            }
        }
        
        // Encontra todos os jogadores que têm a melhor combinação (para casos de empate)
        jogador *vencedores_finais[MAX_JOGADORES];
        int num_vencedores_finais = 0;
        for(int i = 0; i < num_vencedores; i++) {
            if (melhores_jogadas_enum[i] == melhor_combinacao) {
                vencedores_finais[num_vencedores_finais++] = vencedores[i];
            }
        }
        
        // Exibe o resultado e distribui o pote
        // (A lógica de exibição da sua 'realizar_rodada' antiga é perfeita aqui)
        printf("\n=== RESULTADO DA RODADA ===\n");
        char nomeJogada[50];
        switch (melhor_combinacao) {
            case CARTA_ALTA: strcpy(nomeJogada, "Carta Alta"); break;
            case PAR: strcpy(nomeJogada, "um Par"); break;
            case DOIS_PARES: strcpy(nomeJogada, "Dois Pares"); break;
            case TRINCA: strcpy(nomeJogada, "uma Trinca"); break;
            case STRAIGHT: strcpy(nomeJogada, "um Straight"); break;
            case FLUSH: strcpy(nomeJogada, "um Flush"); break;
            case FULL_HOUSE: strcpy(nomeJogada, "um Full House"); break;
            case QUADRA: strcpy(nomeJogada, "uma Quadra"); break;
            case STRAIGHT_FLUSH: strcpy(nomeJogada, "um Straight Flush"); break;
            case ROYAL_FLUSH: strcpy(nomeJogada, "UM ROYAL FLUSH!"); break;
            default: strcpy(nomeJogada, "uma jogada desconhecida"); break;
        }

        printf("A mão vencedora é %s.\n", nomeJogada);

        if (num_vencedores_finais > 1) printf("Empate entre: ");
        else printf("Vencedor: ");

        for (int i = 0; i < num_vencedores_finais; i++) printf("%s ", vencedores_finais[i]->nome);
        
        int premio = mesa->pote / num_vencedores_finais;
        printf("\nPrêmio por jogador: %d fichas\n", premio);

        for (int i = 0; i < num_vencedores_finais; i++) {
            vencedores_finais[i]->fichas += premio;

            // *** MODIFICAÇÃO: ATIVANDO O LOG AQUI ***
            insereJogada(arvore_jogadas, vencedores_finais[i]->nome, *contador_jogadas, melhor_combinacao); // Insere a jogada vencedora na árvore de histórico
            salvaEmDisco(arvore_jogadas, *contador_jogadas); // Salva a jogada em disco no ficheiro jogadas.txt
            (*contador_jogadas)++; // Incrementa o contador para a próxima jogada
        }
    }
}

/**
 * Gerencia uma única rodada de apostas de forma robusta e posicional.
 * @param mesa Ponteiro para a Mesa
 * @param pos_inicial Posição do primeiro jogador a agir
 * @param aposta_inicial O valor da aposta que precisa ser paga (e.g., o Big Blind)
 * @param jogadores_ativos_na_rodada Ponteiro para o contador de jogadores ativos
 */
static void rodada_de_apostas(Mesa *mesa, int pos_inicial, int aposta_inicial, int *jogadores_ativos_na_rodada, arvAvl *arvore_jogadas, int *contador_jogadas) {
    int aposta_minima_para_aumentar = APOSTA_MINIMA;
    int aposta_atual_na_rodada = aposta_inicial;
    int jogador_que_aumentou = -1; // Índice do último jogador que aumentou

    // Se já havia uma aposta inicial (Big Blind), define quem a fez
    if (aposta_inicial > 0) {
        for (int i = 0; i < mesa->num_jogadores; i++) {
            if (mesa->jogadores[i]->aposta_rodada == aposta_inicial) {
                jogador_que_aumentou = i;
                break;
            }
        }
    }
    
    // Zera a aposta da rodada apenas para quem não postou blind
    for(int i = 0; i < mesa->num_jogadores; i++) {
        if(mesa->jogadores[i]->aposta_rodada == 0) {
            // Não faz nada, pois já foi zerado no início da rodada completa
        }
    }
    
    if (*jogadores_ativos_na_rodada <= 1) return;

    int jogadores_que_agiram = 0;
    int acoes_concluidas = 0;

    // O loop continua enquanto as apostas não estiverem igualadas
    while (!acoes_concluidas) {
        int todos_agiram = 1;

        // Itera sobre todos os jogadores a partir da posição inicial
        for (int i = 0; i < mesa->num_jogadores; i++) {
            int indice_jogador = (pos_inicial + i) % mesa->num_jogadores;
            jogador *j = mesa->jogadores[indice_jogador];

            if (j->atual != ativo) continue;

            // Se a ação voltou para o último que aumentou e a aposta dele está paga, fim da rodada
            if (jogador_que_aumentou == indice_jogador) {
                acoes_concluidas = 1;
                break;
            }

            // O turno do jogador só termina com uma ação válida
            int acao_valida = 0;
            do {

                printf("\n--- Vez de %s (%d fichas) ---\n", j->nome, j->fichas);
                printf("Pote atual: %d. Aposta para pagar: %d.\n", mesa->pote, aposta_atual_na_rodada - j->aposta_rodada);

                char acoes[100] = "Ações: [F]old";
                if (j->aposta_rodada == aposta_atual_na_rodada) strcat(acoes, ", [C]heck");
                if (j->aposta_rodada < aposta_atual_na_rodada && j->fichas >= (aposta_atual_na_rodada - j->aposta_rodada)) strcat(acoes, ", C[a]ll");
                if (aposta_atual_na_rodada == 0) strcat(acoes, ", [B]et");
                else if (j->fichas > (aposta_atual_na_rodada - j->aposta_rodada)) strcat(acoes, ", [R]aise");
                
                printf("%s\n> ", acoes);

                char escolha;
                scanf(" %c", &escolha);
                while(getchar() != '\n');
                int valor_aposta = 0;

                switch (toupper(escolha)) {
                    case 'F':
                        j->atual = foldou;
                        (*jogadores_ativos_na_rodada)--;
                        printf("%s desiste.\n", j->nome);
                        // Log the fold action as a jogada
                        insereJogada(arvore_jogadas, j->nome, *contador_jogadas, (tipos_jogada)-1);
                        salvaEmDisco(arvore_jogadas, *contador_jogadas);
                        (*contador_jogadas)++;
                        acao_valida = 1;
                        limpar_tela();
                        break;
                    case 'C':
                        if (j->aposta_rodada == aposta_atual_na_rodada) {
                            printf("%s passa (Check).\n", j->nome);
                             acao_valida = 1;
                             limpar_tela();
                        } else { printf("Ação inválida! Precisa pagar ou aumentar.\n"); limpar_tela(); }
                        break;
                    case 'B':
                        if (aposta_atual_na_rodada == 0) {
                            printf("Quanto quer apostar? (Mínimo %d): ", aposta_minima_para_aumentar);
                            scanf("%d", &valor_aposta);
                            if (valor_aposta >= aposta_minima_para_aumentar && valor_aposta <= j->fichas) {
                                aposta_atual_na_rodada = valor_aposta;
                                j->fichas -= valor_aposta;
                                j->aposta_rodada += valor_aposta;
                                mesa->pote += valor_aposta;
                                jogador_que_aumentou = indice_jogador;
                                printf("%s aposta %d.\n", j->nome, valor_aposta);
                                acao_valida = 1;
                                limpar_tela();
                            } else { printf("Valor de aposta inválido!\n"); limpar_tela();  }
                        } else { printf("Ação inválida! Use Raise para aumentar.\n"); limpar_tela(); }
                        break;
                    case 'A':
                        valor_aposta = aposta_atual_na_rodada - j->aposta_rodada;
                        if (valor_aposta > 0 && j->fichas >= valor_aposta) {
                            j->fichas -= valor_aposta;
                            j->aposta_rodada += valor_aposta;
                            mesa->pote += valor_aposta;
                            printf("%s paga %d.\n", j->nome, valor_aposta);
                            acao_valida = 1;
                            limpar_tela();
                        } else { printf("Ação inválida ou fichas insuficientes!\n"); limpar_tela();}
                        break;
                    case 'R':
                        if (aposta_atual_na_rodada > 0) {
                            printf("Aumentar para quanto no total? (Aposta atual %d): ", aposta_atual_na_rodada);
                            scanf("%d", &valor_aposta);
                            int valor_para_pagar = aposta_atual_na_rodada - j->aposta_rodada;
                            int aumento = valor_aposta - aposta_atual_na_rodada;
                            if (valor_aposta > aposta_atual_na_rodada && aumento >= aposta_minima_para_aumentar && j->fichas >= (valor_para_pagar + aumento)) {
                                j->fichas -= (valor_para_pagar + aumento);
                                j->aposta_rodada += (valor_para_pagar + aumento);
                                mesa->pote += (valor_para_pagar + aumento);
                                aposta_atual_na_rodada = valor_aposta;
                                jogador_que_aumentou = indice_jogador;
                                printf("%s aumenta para %d.\n", j->nome, valor_aposta);
                                acao_valida = 1;
                                limpar_tela();
                            } else { printf("Valor de aumento inválido!\n"); limpar_tela(); }
                        } else { printf("Ação inválida! Use Bet para a primeira aposta.\n"); limpar_tela(); }
                        break;
                    default:
                        printf("Opção inválida! Tente novamente.\n"); limpar_tela();
                }
            } while (!acao_valida);

            // Se um jogador desistiu e só sobrou um, a rodada de apostas termina imediatamente
            if (*jogadores_ativos_na_rodada <= 1) {
                acoes_concluidas = 1;
                break;
            }
        }
        
        // Verifica se todos os jogadores ativos pagaram a aposta atual
        todos_agiram = 1;
        for(int k = 0; k < mesa->num_jogadores; k++) {
            if(mesa->jogadores[k]->atual == ativo && mesa->jogadores[k]->aposta_rodada != aposta_atual_na_rodada) {
                todos_agiram = 0; // Ainda há apostas a serem pagas
                break;
            }
        }
        if(todos_agiram) acoes_concluidas = 1;
        
        // Se deu uma volta completa e não houve aumento, a rodada acaba
        if (jogador_que_aumentou == -1 && jogadores_que_agiram >= *jogadores_ativos_na_rodada) {
             acoes_concluidas = 1;
        }
    }
}

/**
 * Orquestra uma rodada completa de Texas Hold'em, com todas as
 * melhorias de interface, rotação do botão, blinds e fases de apostas.
 */
static void realizar_rodada_completa(Mesa *mesa, arvAvl *arvore_jogadas, int *contador_jogadas) {
    // --- ETAPA 1: PREPARAÇÃO DA RODADA ---
    limpar_tela(); // MODIFICAÇÃO: Limpa a tela no início da rodada.
    printf("\n\n--- INICIANDO NOVA RODADA ---\n");
    mesa->pote = 0;
    if(mesa->cartas_comunitarias) destroi_listad(mesa->cartas_comunitarias);
    mesa->cartas_comunitarias = inicializa_listad();

    // Conta quantos jogadores têm fichas para continuar
    int jogadores_com_fichas = 0;
    for(int i=0; i<mesa->num_jogadores; i++) {
        if(mesa->jogadores[i]->fichas > 0) {
            jogadores_com_fichas++;
        }
    }
    
    if (jogadores_com_fichas < 2) {
        printf("Não há jogadores suficientes com fichas para continuar.\n");
        return;
    }

    // Rotaciona o botão do dealer para a próximo jogador ativo
    do {
        mesa->dealer_pos = (mesa->dealer_pos + 1) % mesa->num_jogadores;
    } while (mesa->jogadores[mesa->dealer_pos]->fichas <= 0);
    
    printf("O botão do Dealer está com: %s (Posição %d)\n", mesa->jogadores[mesa->dealer_pos]->nome, mesa->dealer_pos);

    // Reseta o estado dos jogadores para a nova mão
    for(int i = 0; i < mesa->num_jogadores; i++){
        mesa->jogadores[i]->aposta_rodada = 0;
        if(mesa->jogadores[i]->fichas > 0) {
            mesa->jogadores[i]->atual = ativo;
        } else {
            mesa->jogadores[i]->atual = fora;
        }
        if(mesa->jogadores[i]->mao) destroi_listad(mesa->jogadores[i]->mao);
        mesa->jogadores[i]->mao = inicializa_listad();
    }

	// --- ETAPA 2: POSTAR OS BLINDS ---
    int small_blind_valor = SMALL_BLIND_VALOR;
    int big_blind_valor = BIG_BLIND_VALOR;
    
    // Encontra a posição do Small Blind
    int sb_pos = mesa->dealer_pos;
    do {
        sb_pos = (sb_pos + 1) % mesa->num_jogadores;
    } while (mesa->jogadores[sb_pos]->atual == fora);
    
    // Encontra a posição do Big Blind
    int bb_pos = sb_pos;
    do {
        bb_pos = (bb_pos + 1) % mesa->num_jogadores;
    } while (mesa->jogadores[bb_pos]->atual == fora);

    // Postar Small Blind
    jogador *sb_player = mesa->jogadores[sb_pos];
    sb_player->fichas -= small_blind_valor;
    sb_player->aposta_rodada = small_blind_valor;
    mesa->pote += small_blind_valor;
    printf("%s posta o Small Blind de %d.\n", sb_player->nome, small_blind_valor);

    // Postar Big Blind
    jogador *bb_player = mesa->jogadores[bb_pos];
    bb_player->fichas -= big_blind_valor;
    bb_player->aposta_rodada = big_blind_valor;
    mesa->pote += big_blind_valor;
    printf("%s posta o Big Blind de %d.\n", bb_player->nome, big_blind_valor);

    // --- ETAPA 3: DISTRIBUIÇÃO E FASES DE APOSTAS ---
	Baralho baralho_rodada;
	inicializaBaralho(&baralho_rodada);
	embaralha(&baralho_rodada, &mesa->baralho);
	distribuir_cartas_jogadores(mesa);

    // MODIFICAÇÃO: Mostra a mão de cada jogador usando a nova interface
	for (int i = 0; i < mesa->num_jogadores; i++) {
		if(mesa->jogadores[i]->atual == ativo) {
            char titulo_mao[50];
            sprintf(titulo_mao, "Mão de %s", mesa->jogadores[i]->nome);
			desenhar_cartas(mesa->jogadores[i]->mao, titulo_mao);
		}
	}
    
    int pos_inicial_preflop = (bb_pos + 1) % mesa->num_jogadores;
    int pos_inicial_posflop = (mesa->dealer_pos + 1) % mesa->num_jogadores;
    int jogadores_ativos = jogadores_com_fichas;

    // PRÉ-FLOP
    if (jogadores_ativos > 1) {
        printf("\n=== FASE DE APOSTAS: PRÉ-FLOP ===\n");
        rodada_de_apostas(mesa, pos_inicial_preflop, big_blind_valor, &jogadores_ativos, arvore_jogadas, contador_jogadas);
    }

    // FLOP
    if (jogadores_ativos > 1) {
        printf("\n=== FLOP ===\n");
        Carta queimada, c;
        pop(&mesa->baralho, &queimada);
        for(int i=0; i<3; i++) { pop(&mesa->baralho, &c); insere_listad_no_fim(mesa->cartas_comunitarias, c); }
        // MODIFICAÇÃO: Desenha as cartas da mesa
        mostraMesa(mesa->cartas_comunitarias, "Cartas da Mesa");
        rodada_de_apostas(mesa, pos_inicial_posflop, 0, &jogadores_ativos, arvore_jogadas, contador_jogadas);
    }

    // TURN
    if (jogadores_ativos > 1) {
        printf("\n=== TURN ===\n");
        Carta queimada, c;
        pop(&mesa->baralho, &queimada);
        pop(&mesa->baralho, &c);
        insere_listad_no_fim(mesa->cartas_comunitarias, c);
        // MODIFICAÇÃO: Desenha as cartas da mesa
        mostraMesa(mesa->cartas_comunitarias, "Cartas da Mesa");
        rodada_de_apostas(mesa, pos_inicial_posflop, 0, &jogadores_ativos, arvore_jogadas, contador_jogadas);
    }
    
    // RIVER
    if (jogadores_ativos > 1) {
        printf("\n=== RIVER ===\n");
        Carta queimada, c;
        pop(&mesa->baralho, &queimada);
        pop(&mesa->baralho, &c);
        insere_listad_no_fim(mesa->cartas_comunitarias, c);
        // MODIFICAÇÃO: Desenha as cartas da mesa
        mostraMesa(mesa->cartas_comunitarias, "Cartas da Mesa");
        rodada_de_apostas(mesa, pos_inicial_posflop, 0, &jogadores_ativos, arvore_jogadas, contador_jogadas);
    }

    // --- ETAPA 4: SHOWDOWN ---
    realizar_showdown(mesa, arvore_jogadas, contador_jogadas);
}

/**
 * Converte o valor da carta para um C-ndice numC)rico
 * @param valor String representando o valor da carta ("2" a "A")
 * @return C
ndice numC)rico (0 a 12) ou -1 se invC!lido
 */
static int converter_valor_para_indice(const char *valor) {
	const char *valores[] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
	for (int i = 0; i < 13; i++) {
		if (strcmp(valor, valores[i]) == 0) {
			return i;
		}
	}
	return -1; // Valor invC!lido
}

/**
 * Conta a ocorrC*ncia de cada valor de carta
 * @param cartas Lista de cartas a serem analisadas
 * @param contagem Array para armazenar a contagem (deve ter tamanho 13)
 * @return 1 se sucesso, 0 se erro
 */
static int contar_ocorrencias_valores(tp_listad *cartas, int contagem[13]) {
	if (!cartas || !contagem) return 0;

	// Inicializa contagem
	for (int i = 0; i < 13; i++) {
		contagem[i] = 0;
	}

	// Conta cada valor de carta
	tp_no_lista *atu = cartas->ini;
	while (atu != NULL) {
		int indice = converter_valor_para_indice(atu->info.valor);
		if (indice != -1) {
			contagem[indice]++;
		}
		atu = atu->prox;
	}
	return 1;
}

/**
 * Conta a ocorrC*ncia de cada naipe
 * @param cartas Lista de cartas a serem analisadas
 * @param naipes Array para armazenar a contagem (0=Copas, 1=Ouros, 2=Espadas, 3=Paus)
 * @return 1 se sucesso, 0 se erro
 */
static int contar_ocorrencias_naipes(tp_listad *cartas, int naipes[4]) {
	if (!cartas || !naipes) return 0;

	// Inicializa contagem
	for (int i = 0; i < 4; i++) {
		naipes[i] = 0;
	}

	// Conta cada naipe
	tp_no_lista *atu = cartas->ini;
	while (atu != NULL) {
		if (strcmp(atu->info.naipe, "Copas") == 0) naipes[0]++;
		else if (strcmp(atu->info.naipe, "Ouros") == 0) naipes[1]++;
		else if (strcmp(atu->info.naipe, "Espadas") == 0) naipes[2]++;
		else if (strcmp(atu->info.naipe, "Paus") == 0) naipes[3]++;
		atu = atu->prox;
	}
	return 1;
}

/**
 * Verifica se hC! pelo menos um par na mC#o
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver par, 0 caso contrC!rio
 */
static int verificar_par(tp_listad *cartas) {
	int contagem[13];
	if (!contar_ocorrencias_valores(cartas, contagem)) return 0;

	for (int i = 0; i < 13; i++) {
		if (contagem[i] >= 2) {
			return 1;
		}
	}
	return 0;
}

/**
 * Verifica se hC! dois pares na mC#o
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver dois pares, 0 caso contrC!rio
 */
static int verificar_dois_pares(tp_listad *cartas) {
	int contagem[13];
	if (!contar_ocorrencias_valores(cartas, contagem)) return 0;

	int pares = 0;
	for (int i = 0; i < 13; i++) {
		if (contagem[i] >= 2) {
			pares++;
		}
	}
	return (pares >= 2);
}

/**
 * Verifica se hC! uma trinca na mC#o
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver trinca, 0 caso contrC!rio
 */
static int verificar_trinca(tp_listad *cartas) {
	int contagem[13];
	if (!contar_ocorrencias_valores(cartas, contagem)) return 0;

	for (int i = 0; i < 13; i++) {
		if (contagem[i] >= 3) {
			return 1;
		}
	}
	return 0;
}

/**
 * Verifica se hC! uma sequC*ncia de 5 cartas
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver sequC*ncia, 0 caso contrC!rio
 */
static int verificar_sequencia(tp_listad *cartas) {
	int contagem[13] = {0};
	if (!contar_ocorrencias_valores(cartas, contagem)) return 0;

	// Converte a lista de cartas para um array de inteiros para facilitar a ordenaC'C#o e verificaC'C#o
	int valores_numericos[10]; // MC!ximo de 7 cartas (2 na mC#o + 5 comunitC!rias)
	int count = 0;
	tp_no_lista *atu = cartas->ini;
	while (atu != NULL) {
		valores_numericos[count++] = converter_valor_para_indice(atu->info.valor);
		atu = atu->prox;
	}

	// Ordena os valores (bubble sort simples para poucos elementos)
	for (int i = 0; i < count - 1; i++) {
		for (int j = 0; j < count - 1 - i; j++) {
			if (valores_numericos[j] > valores_numericos[j+1]) {
				int temp = valores_numericos[j];
				valores_numericos[j] = valores_numericos[j+1];
				valores_numericos[j+1] = temp;
			}
		}
	}

	// Remove duplicatas e verifica sequC*ncia
	int distinct_values[10];
	int distinct_count = 0;
	if (count > 0) {
		distinct_values[distinct_count++] = valores_numericos[0];
		for (int i = 1; i < count; i++) {
			if (valores_numericos[i] != valores_numericos[i-1]) {
				distinct_values[distinct_count++] = valores_numericos[i];
			}
		}
	}

	// Verifica sequC*ncia normal
	for (int i = 0; i <= distinct_count - 5; i++) {
		if (distinct_values[i+4] == distinct_values[i] + 4) {
			return 1;
		}
	}

	// Verifica sequC*ncia A-2-3-4-5 (Ace-low straight)
	// Se tiver A (indice 12), 2 (indice 0), 3 (indice 1), 4 (indice 2), 5 (indice 3)
	int has_ace = 0, has_2 = 0, has_3 = 0, has_4 = 0, has_5 = 0;
	for (int i = 0; i < distinct_count; i++) {
		if (distinct_values[i] == 12) has_ace = 1;
		if (distinct_values[i] == 0) has_2 = 1;
		if (distinct_values[i] == 1) has_3 = 1;
		if (distinct_values[i] == 2) has_4 = 1;
		if (distinct_values[i] == 3) has_5 = 1;
	}
	if (has_ace && has_2 && has_3 && has_4 && has_5) {
		return 1;
	}

	return 0;
}


/**
 * Verifica se hC! um flush (5 cartas do mesmo naipe)
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver flush, 0 caso contrC!rio
 */
static int verificar_flush(tp_listad *cartas) {
	int naipes[4];
	if (!contar_ocorrencias_naipes(cartas, naipes)) return 0;

	for (int i = 0; i < 4; i++) {
		if (naipes[i] >= 5) {
			return 1;
		}
	}
	return 0;
}

/**
 * Verifica se hC! um full house (trinca + par)
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver full house, 0 caso contrC!rio
 */
static int verificar_full_house(tp_listad *cartas) {
	int contagem[13];
	if (!contar_ocorrencias_valores(cartas, contagem)) return 0;

	int tem_trinca = 0;
	int pares = 0; // Pode haver mC:ltiplos pares
	for (int i = 0; i < 13; i++) {
		if (contagem[i] >= 3) {
			tem_trinca = 1;
		}
		if (contagem[i] >= 2) {
			pares++;
		}
	}
	// Para um full house, precisamos de uma trinca e de pelo menos um par adicional.
	// Se a trinca for a C:nica ocorrC*ncia de 2 ou mais cartas (ex: AAA 2 3), nC#o C) FH.
	// Se temos uma trinca e um par, ou uma trinca e outra trinca (ex: AAA KKK), tambC)m C) FH.
	return (tem_trinca && pares >= 2);
}

/**
 * Verifica se hC! uma quadra (quatro cartas do mesmo valor)
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver quadra, 0 caso contrC!rio
 */
static int verificar_quadra(tp_listad *cartas) {
	int contagem[13];
	if (!contar_ocorrencias_valores(cartas, contagem)) return 0;

	for (int i = 0; i < 13; i++) {
		if (contagem[i] >= 4) {
			return 1;
		}
	}
	return 0;
}

/**
 * Verifica se hC! um Straight Flush (sequC*ncia de 5 cartas do mesmo naipe)
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver Straight Flush, 0 caso contrC!rio
 */
static int verificar_straight_flush(tp_listad *cartas) {
	// Primeiro, verifica se hC! um flush
	if (!verificar_flush(cartas)) {
		return 0;
	}

	// Se houver flush, agrupa as cartas por naipe e verifica a sequC*ncia para cada naipe
	const char *naipes_str[] = {"Copas", "Ouros", "Espadas", "Paus"};
	for (int n = 0; n < 4; n++) {
		tp_listad *cartas_do_naipe = inicializa_listad();
		tp_no_lista *atu = cartas->ini;
		while (atu != NULL) {
			if (strcmp(atu->info.naipe, naipes_str[n]) == 0) {
				insere_listad_no_fim(cartas_do_naipe, atu->info);
			}
			atu = atu->prox;
		}

		// Se houver 5 ou mais cartas do mesmo naipe, verifica se formam uma sequC*ncia
		if (verificar_sequencia(cartas_do_naipe)) {
			destroi_listad(cartas_do_naipe);
			return 1;
		}
		destroi_listad(cartas_do_naipe);
	}
	return 0;
}

/**
 * Verifica se hC! um Royal Flush (10, J, Q, K, A do mesmo naipe)
 * @param cartas Lista de cartas a serem verificadas
 * @return 1 se houver Royal Flush, 0 caso contrC!rio
 */
static int verificar_royal_flush(tp_listad *cartas) {
	// Primeiro, verifica se hC! um flush
	if (!verificar_flush(cartas)) {
		return 0;
	}

	// Se houver flush, agrupa as cartas por naipe e verifica o Royal Flush
	const char *naipes_str[] = {"Copas", "Ouros", "Espadas", "Paus"};
	for (int n = 0; n < 4; n++) {
		int tem_10 = 0, tem_J = 0, tem_Q = 0, tem_K = 0, tem_A = 0;
		tp_no_lista *atu = cartas->ini;
		while (atu != NULL) {
			if (strcmp(atu->info.naipe, naipes_str[n]) == 0) {
				if (strcmp(atu->info.valor, "10") == 0) tem_10 = 1;
				else if (strcmp(atu->info.valor, "J") == 0) tem_J = 1;
				else if (strcmp(atu->info.valor, "Q") == 0) tem_Q = 1;
				else if (strcmp(atu->info.valor, "K") == 0) tem_K = 1;
				else if (strcmp(atu->info.valor, "A") == 0) tem_A = 1;
			}
			atu = atu->prox;
		}
		if (tem_10 && tem_J && tem_Q && tem_K && tem_A) {
			return 1;
		}
	}
	return 0;
}


/**
 * Avalia a melhor combinaC'C#o possC-vel para um jogador
 * @param j Ponteiro para o jogador
 * @param comunitarias Lista de cartas comunitC!rias
 * @return A melhor combinaC'C#o encontrada (do enum tipos_jogada)
 */
static tipos_jogada avaliar_mao(jogador *j, tp_listad *comunitarias) {
	// Combina as cartas do jogador com as comunitC!rias
	tp_listad *todas_cartas = inicializa_listad();

	// Adiciona cartas da mC#o do jogador
	tp_no_lista *atu = j->mao->ini;
	while (atu != NULL) {
		insere_listad_no_fim(todas_cartas, atu->info);
		atu = atu->prox;
	}

	// Adiciona cartas comunitC!rias
	atu = comunitarias->ini;
	while (atu != NULL) {
		insere_listad_no_fim(todas_cartas, atu->info);
		atu = atu->prox;
	}

	// Verifica combinaC'C5es da melhor para a pior, usando tipos_jogada
	if (verificar_royal_flush(todas_cartas)) { // Novo
		destroi_listad(todas_cartas);
		return ROYAL_FLUSH;
	}
	if (verificar_straight_flush(todas_cartas)) { // Novo
		destroi_listad(todas_cartas);
		return STRAIGHT_FLUSH;
	}
	if (verificar_quadra(todas_cartas)) { // Novo
		destroi_listad(todas_cartas);
		return QUADRA;
	}
	if (verificar_full_house(todas_cartas)) {
		destroi_listad(todas_cartas);
		return FULL_HOUSE;
	}
	if (verificar_flush(todas_cartas)) {
		destroi_listad(todas_cartas);
		return FLUSH;
	}
	if (verificar_sequencia(todas_cartas)) {
		destroi_listad(todas_cartas);
		return STRAIGHT; // Mapeia SEQUENCIA para STRAIGHT
	}
	if (verificar_trinca(todas_cartas)) {
		destroi_listad(todas_cartas);
		return TRINCA;
	}
	if (verificar_dois_pares(todas_cartas)) {
		destroi_listad(todas_cartas);
		return DOIS_PARES;
	}
	if (verificar_par(todas_cartas)) {
		destroi_listad(todas_cartas);
		return PAR;
	}

	// Se nC#o encontrou nenhuma combinaC'C#o vC!lida
	destroi_listad(todas_cartas);
	return CARTA_ALTA;
}

#endif
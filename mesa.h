#ifndef MESA_H
#define MESA_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "estrutura_jogador.h"
#include "pilhaLE.h"
#include "cartas.h"
#include "listade.h"
#include "jogadas.h"


// Constantes do jogo
#define MAX_JOGADORES 8       // NC:mero mC!ximo de jogadores na mesa
#define CARTAS_COMUNITARIAS 5 // NC:mero de cartas comunitC!rias (Flop+Turn+River)

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
void inicializar_mesa(Mesa *mesa);
void preparar_cartas_mesa(Mesa *mesa);
void adicionar_jogador_a_mesa(Mesa *mesa, jogador *j);
void distribuir_cartas_jogadores(Mesa *mesa);
// Alteramos o retorno de avaliar_mao para 'tipos_jogada'
tipos_jogada avaliar_mao(jogador *j, tp_listad *comunitarias);
void realizar_rodada(Mesa *mesa, arvAvl *arvore_jogadas, int *contador_jogadas);

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
static int verificar_quadra(tp_listad *cartas); // Adicionar funC'C#o para QUADRA
static int verificar_straight_flush(tp_listad *cartas); // Adicionar funC'C#o para STRAIGHT_FLUSH
static int verificar_royal_flush(tp_listad *cartas); // Adicionar funC'C#o para ROYAL_FLUSH


/**
 * Inicializa o estado da mesa
 * @param mesa Ponteiro para a estrutura Mesa a ser inicializada
 */
void inicializar_mesa(Mesa *mesa) {
	Baralho b;
	inicializaBaralho(&b);          // Inicializa um novo baralho
	embaralha(&b, &mesa->baralho);  // Embaralha as cartas

	mesa->num_jogadores = 0;        // Sem jogadores inicialmente
	mesa->dealer_pos = 0;           // Dealer comeC'a na posiC'C#o 0
	mesa->cartas_comunitarias = inicializa_listad(); // Inicializa lista de cartas comunitC!rias

	// Queima a primeira carta (regra do poker)
	Carta queimada;
	pop(&mesa->baralho, &queimada);
}

/**
 * Prepara as cartas comunitC!rias (Flop, Turn, River)
 * @param mesa Ponteiro para a estrutura Mesa
 */
void preparar_cartas_mesa(Mesa *mesa) {
	// Limpa cartas comunitC!rias existentes (se houver)
	if (mesa->cartas_comunitarias->ini != NULL) {
		destroi_listad(mesa->cartas_comunitarias);
		mesa->cartas_comunitarias = inicializa_listad();
	}

	// Flop: distribui 3 cartas (com uma queima antes)
	Carta queimada;
	pop(&mesa->baralho, &queimada); // Queima uma carta antes do flop

	for (int i = 0; i < 3; i++) {
		Carta c;
		pop(&mesa->baralho, &c);
		insere_listad_no_fim(mesa->cartas_comunitarias, c);
	}

	// Turn: distribui 1 carta (com uma queima antes)
	pop(&mesa->baralho, &queimada); // Queima antes do turn

	Carta turn;
	pop(&mesa->baralho, &turn);
	insere_listad_no_fim(mesa->cartas_comunitarias, turn);

	// River: distribui 1 carta (com uma queima antes)
	pop(&mesa->baralho, &queimada); // Queima antes do river

	Carta river;
	pop(&mesa->baralho, &river);
	insere_listad_no_fim(mesa->cartas_comunitarias, river);
}

/**
 * Adiciona um jogador C  mesa
 * @param mesa Ponteiro para a estrutura Mesa
 * @param j Ponteiro para o jogador a ser adicionado
 */
void adicionar_jogador_a_mesa(Mesa *mesa, jogador *j) {
	if (mesa->num_jogadores < MAX_JOGADORES) {
		mesa->jogadores[mesa->num_jogadores] = j;
		mesa->num_jogadores++;
	}
}

/**
 * Distribui cartas para todos os jogadores (2 cartas cada)
 * @param mesa Ponteiro para a estrutura Mesa
 */
void distribuir_cartas_jogadores(Mesa *mesa) {
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
tipos_jogada avaliar_mao(jogador *j, tp_listad *comunitarias) {
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

/**
 * Realiza uma rodada completa do jogo
 * @param mesa Ponteiro para a estrutura Mesa
 * @param arvore_jogadas Ponteiro para a C!rvore AVL onde as jogadas serC#o armazenadas
 * @param contador_jogadas Ponteiro para o contador de ordem das jogadas
 */
void realizar_rodada(Mesa *mesa, arvAvl *arvore_jogadas, int *contador_jogadas) { // Adicionados os novos parC"metros
	// 1. Prepara as cartas comunitC!rias
	preparar_cartas_mesa(mesa);

	// 2. Mostra as cartas comunitC!rias
	printf("\nCartas ComunitC!rias:\n");
	imprime_listad(mesa->cartas_comunitarias, 1);

	// 3. Mostra as mC#os de todos os jogadores
	for (int i = 0; i < mesa->num_jogadores; i++) {
		printf("\nMC#o de %s:\n", mesa->jogadores[i]->nome);
		imprime_listad(mesa->jogadores[i]->mao, 1);
	}

	// 4. Avalia as mC#os de todos os jogadores

	tipos_jogada melhores_jogadas_enum[MAX_JOGADORES];

	for (int i = 0; i < mesa->num_jogadores; i++) {
		melhores_jogadas_enum[i] = avaliar_mao(mesa->jogadores[i], mesa->cartas_comunitarias);

		// Incrementa o contador de jogadas e insere na C!rvore AVL
		(*contador_jogadas)++;
		insereJogada(arvore_jogadas, mesa->jogadores[i]->nome, *contador_jogadas, melhores_jogadas_enum[i]);
		// Salva a jogada no disco imediatamente apC3s a inserC'C#o (opcional, pode ser feito no final do jogo)
		salvaEmDisco(arvore_jogadas, *contador_jogadas);
	}

	// 5. Determina o vencedor (melhor combinaC'C#o, baseada em tipos_jogada)
	// Tipos_jogada estC! ordenado da pior para a melhor (CARTA_ALTA=0, ROYAL_FLUSH=9).
	// EntC#o, a "melhor" combinaC'C#o terC! o MAIOR valor no enum.
	tipos_jogada melhor_combinacao = CARTA_ALTA; // ComeC'a com a pior
	int vencedores[MAX_JOGADORES] = {0};
	int num_vencedores = 0;

	for (int i = 0; i < mesa->num_jogadores; i++) {
		if (melhores_jogadas_enum[i] > melhor_combinacao) { // Mudei a comparaC'C#o para '>'
			melhor_combinacao = melhores_jogadas_enum[i];
			num_vencedores = 0;
			vencedores[num_vencedores++] = i;
		} else if (melhores_jogadas_enum[i] == melhor_combinacao) {
			vencedores[num_vencedores++] = i;
		}
	}

	// 6. Distribui o pote
	if (num_vencedores > 0) {
		int valor_por_vencedor = mesa->pote / num_vencedores;
		for (int i = 0; i < num_vencedores; i++) {
			mesa->jogadores[vencedores[i]]->fichas += valor_por_vencedor;
		}
	}

	// 7. Mostra os resultados
	printf("\n=== RESULTADO DA RODADA ===\n");
	printf("Melhor combinaC'C#o: ");
	switch (melhor_combinacao) {
	case CARTA_ALTA:
		printf("Carta Alta\n");
		break;
	case PAR:
		printf("Par\n");
		break;
	case DOIS_PARES:
		printf("Dois Pares\n");
		break;
	case TRINCA:
		printf("Trinca\n");
		break;
	case STRAIGHT:
		printf("SequC*ncia\n");
		break;
	case FLUSH:
		printf("Flush\n");
		break;
	case FULL_HOUSE:
		printf("Full House\n");
		break;
	case QUADRA:
		printf("Quadra\n");
		break;
	case STRAIGHT_FLUSH:
		printf("Straight Flush\n");
		break;
	case ROYAL_FLUSH:
		printf("Royal Flush\n");
		break;
	default:
		printf("Desconhecida\n"); // Caso inesperado
	}

	if(num_vencedores > 1) {
		printf("Empate entre: ");
	} else {
		printf("Vencedor: ");
	}

	for (int i = 0; i < num_vencedores; i++) {
		printf("%s ", mesa->jogadores[vencedores[i]]->nome);
	}
	printf("\nPrC*mio por jogador: %d fichas\n", mesa->pote / num_vencedores);
}

#endif

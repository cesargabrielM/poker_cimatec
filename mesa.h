#ifndef MESA_H
#define MESA_H

#include "estrutura_jogador.h"
#include "cartas.h"
#include "pilhaLE.h"
#include "listade.h"

#define MAX_JOGADORES 8
#define CARTAS_COMUNITARIAS 5

typedef struct {
    tp_pilha baralho;
    jogador *jogadores[MAX_JOGADORES]; // Agora armazena ponteiros
    tp_listad *cartas_comunitarias;
    int num_jogadores;
    int dealer_pos;
} Mesa;

// Inicializa apenas o estado da mesa
void inicializar_mesa(Mesa *mesa) {
    Baralho b;
    inicializaBaralho(&b);
    embaralha(&b, &mesa->baralho);
    
    mesa->num_jogadores = 0;
    mesa->dealer_pos = 0;
    mesa->cartas_comunitarias = inicializa_listad();
    
    // Queima inicial
    Carta queimada;
    pop(&mesa->baralho, &queimada);
}

// Prepara cartas comunitárias (Flop, Turn, River)
void preparar_cartas_mesa(Mesa *mesa) {
    // Limpa cartas existentes
    if (mesa->cartas_comunitarias->ini != NULL) {
        destroi_listad(mesa->cartas_comunitarias);
        mesa->cartas_comunitarias = inicializa_listad();
    }
    
    // Flop (3 cartas)
    for (int i = 0; i < 3; i++) {
        Carta c;
        pop(&mesa->baralho, &c);
        insere_listad_no_fim(mesa->cartas_comunitarias, c);
    }
    
    // Turn (1 carta)
    Carta queimada;
    pop(&mesa->baralho, &queimada);
    Carta turn;
    pop(&mesa->baralho, &turn);
    insere_listad_no_fim(mesa->cartas_comunitarias, turn);
    
    // River (1 carta)
    pop(&mesa->baralho, &queimada);
    Carta river;
    pop(&mesa->baralho, &river);
    insere_listad_no_fim(mesa->cartas_comunitarias, river);
}

// Adiciona um jogador já criado à mesa
void adicionar_jogador_a_mesa(Mesa *mesa, jogador *j) {
    if (mesa->num_jogadores < MAX_JOGADORES) {
        mesa->jogadores[mesa->num_jogadores] = j;
        mesa->num_jogadores++;
    }
}

// Distribui cartas para os jogadores já na mesa
void distribuir_cartas_jogadores(Mesa *mesa) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < mesa->num_jogadores; j++) {
            Carta c;
            pop(&mesa->baralho, &c);
            insere_listad_no_fim(mesa->jogadores[j]->mao, c);
        }
    }
}

#endif